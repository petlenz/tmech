#!/usr/bin/env python3
"""Generate markdown reports and plots from benchmark JSON results.

Reads Google Benchmark JSON files produced by run_benchmarks.py and
generates flag-comparison tables, regression tables, and grouped bar
chart plots comparing libraries per operation.
"""

import argparse
import json
import logging
import re
import sys
from collections import defaultdict
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, List, Optional, Tuple

logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s [%(levelname)s] %(message)s",
    datefmt="%H:%M:%S",
)
log = logging.getLogger(__name__)

# ---------------------------------------------------------------------------
# Data classes & constants
# ---------------------------------------------------------------------------

@dataclass
class BenchmarkResult:
    name: str
    cpu_time_ns: float
    real_time_ns: float


# Config names matching run_benchmarks.py output filenames
CONFIG_NAMES = ["scalar", "xsimd"]

# Ordered longest-first to avoid partial matches (e.g. eigen_voigt before eigen)
LIBRARY_SUFFIXES = ["eigen_voigt", "tmech", "eigen", "fastor", "simd", "old"]

LIBRARY_COLORS = {
    "tmech": "#1f77b4",        # blue
    "eigen": "#ff7f0e",        # orange
    "fastor": "#2ca02c",       # green
    "eigen_voigt": "#d62728",  # red
    "simd": "#9467bd",         # purple
    "old": "#8c564b",          # brown
}

# Maps operation name -> tmech formula for plot titles.
# Tensor orders: rank-2 = A_{ij}, rank-4 = A_{ijkl}, rank-8 = A_{ijklmnop}
OPERATION_FORMULAS = {
    "otimes":                           r"$a\, \mathbf{A} \otimes \mathbf{A} + b\,(\mathbf{A} \otimes \mathbf{B} + \ldots)$  (rank 2$\to$4)",
    "otimes_8":                         r"$a\, \mathbb{A} \otimes \mathbb{A} + b\,(\mathbb{A} \otimes \mathbb{B} + \ldots)$  (rank 4$\to$8)",
    "addition":                         r"$\mathbf{C} = \mathbf{A} + \mathbf{B}$  (rank 2)",
    "addition_4":                       r"$\mathbb{C} = \mathbb{A} + \mathbb{B}$  (rank 4)",
    "addition_8":                       r"$\mathbb{C} = \mathbb{A} + \mathbb{B}$  (rank 8)",
    "single_contraction":               r"$\mathbf{C} = \mathbf{A} \cdot \mathbf{B}$  (rank 2)",
    "single_contraction_times3":        r"$\mathbf{C} = \mathbf{A} \cdot \mathbf{B} \cdot \mathbf{A}$  (rank 2)",
    "single_contraction_vector":        r"$C_i = A_{ij}\, B_j$  (vector, rank 2)",
    "more_complex":                     r"$\mathbf{C} = \mathbf{A} \cdot \mathbf{B} + \mathbf{A} + \mathbf{B}$  (rank 2)",
    "double_contraction_4_2":           r"$\mathbf{C} = \mathbb{A} : \mathbf{B}$  (rank 4:2$\to$2)",
    "more_complex_4_2":                 r"$\mathbf{C} = \mathbb{A} : \mathbf{B} + 2\mathbf{B}$  (rank 4:2$\to$2)",
    "double_contraction_4_4":           r"$\mathbb{C} = \mathbb{A} : \mathbb{B}$  (rank 4:4$\to$4)",
    "double_contraction_times3_4_4":    r"$\mathbb{C} = \mathbb{A} : \mathbb{B} : \mathbb{A}$  (rank 4)",
    "complex_double_contraction_4_4":   r"$C_{iMjN} = A_{KiLj}\, B_{MKNL}$  (non-std contraction)",
    "more_complex_4_4":                 r"$\mathbb{C} = \mathbb{A} : \mathbb{B} + 2\mathbb{B} + \mathbb{B}$  (rank 4)",
    "double_contraction_8_8":           r"$\mathbb{C} = \mathbb{A} :: \mathbb{B}$  (rank 8::8$\to$8)",
    "inverse":                          r"$\mathbf{A} = \mathbf{B}^{-1}$  (rank 2)",
}

# ---------------------------------------------------------------------------
# Library extraction for plot grouping
# ---------------------------------------------------------------------------

def extract_library_and_operation(
    bench_name: str,
) -> Tuple[str, str, str]:
    """Extract (operation, library, template_params) from a benchmark name.

    Examples:
        "double_contraction_4_4_eigen_voigt<double, 3ul>"
          -> ("double_contraction_4_4", "eigen_voigt", "<double, 3ul>")
        "BM_single_contraction_tmech_vector<double, 3ul>"
          -> ("single_contraction_vector", "tmech", "<double, 3ul>")
    """
    # Split template params
    if "<" in bench_name:
        base, params = bench_name.split("<", 1)
        params = "<" + params
    else:
        base = bench_name
        params = ""

    # Strip BM_ prefix
    stripped = base
    if stripped.startswith("BM_"):
        stripped = stripped[3:]

    # Try each library suffix (longest first)
    for lib in LIBRARY_SUFFIXES:
        pattern = f"_{lib}"
        idx = stripped.rfind(pattern)
        if idx == -1:
            continue
        # Check it's a real suffix boundary (end of string or followed by _)
        after = idx + len(pattern)
        if after == len(stripped) or stripped[after] == "_":
            # Operation is everything before the library suffix
            op = stripped[:idx]
            # Append any trailing part after the library (e.g. _vector)
            if after < len(stripped):
                op = op + stripped[after:]
            return (op, lib, params)

    # No known library found — treat whole base as operation
    return (stripped, "unknown", params)

# ---------------------------------------------------------------------------
# JSON parsing
# ---------------------------------------------------------------------------

def parse_benchmark_json(json_path: Path) -> List[BenchmarkResult]:
    """Parse Google Benchmark JSON, keeping only aggregate mean entries."""
    try:
        with open(json_path) as f:
            data = json.load(f)
    except (json.JSONDecodeError, OSError) as exc:
        log.warning("Failed to parse %s: %s", json_path, exc)
        return []

    results: List[BenchmarkResult] = []
    for bm in data.get("benchmarks", []):
        if bm.get("aggregate_name") != "mean":
            continue

        # Normalise to nanoseconds
        time_unit = bm.get("time_unit", "ns")
        multiplier = {"ns": 1.0, "us": 1e3, "ms": 1e6, "s": 1e9}.get(
            time_unit, 1.0
        )
        cpu_time = bm.get("cpu_time", 0.0) * multiplier
        real_time = bm.get("real_time", 0.0) * multiplier

        # Strip the aggregate suffix from the name (e.g. "_mean")
        name = bm.get("run_name", bm.get("name", ""))

        results.append(BenchmarkResult(name=name, cpu_time_ns=cpu_time, real_time_ns=real_time))

    return results


def load_results(input_dir: Path) -> Dict[str, List[BenchmarkResult]]:
    """Load all config JSON files from a directory."""
    config_results: Dict[str, List[BenchmarkResult]] = {}
    for cfg_name in CONFIG_NAMES:
        json_path = input_dir / f"{cfg_name}.json"
        if not json_path.exists():
            continue
        results = parse_benchmark_json(json_path)
        if results:
            config_results[cfg_name] = results
            log.info("Parsed %d benchmark results for %s", len(results), cfg_name)
        else:
            log.warning("No results parsed from %s", json_path)
    return config_results

# ---------------------------------------------------------------------------
# Report generators
# ---------------------------------------------------------------------------

def _fmt(ns: Optional[float]) -> str:
    if ns is None:
        return "N/A"
    if ns >= 1e6:
        return f"{ns / 1e6:.2f} ms"
    if ns >= 1e3:
        return f"{ns / 1e3:.2f} \u00b5s"
    return f"{ns:.2f}"


def generate_flag_comparison_report(
    config_results: Dict[str, List[BenchmarkResult]],
) -> str:
    """Markdown table comparing scalar / xsimd."""
    # Collect all benchmark names (ordered by first appearance in scalar)
    all_names: List[str] = []
    seen = set()
    for cfg_name in CONFIG_NAMES:
        for r in config_results.get(cfg_name, []):
            if r.name not in seen:
                all_names.append(r.name)
                seen.add(r.name)

    # Build lookup: config_name -> {bench_name: cpu_time_ns}
    lookup: Dict[str, Dict[str, float]] = {}
    for cfg_name, results in config_results.items():
        lookup[cfg_name] = {r.name: r.cpu_time_ns for r in results}

    # Build header dynamically from CONFIG_NAMES
    display_names = {"scalar": "Scalar", "xsimd": "xsimd"}
    header_cells = " | ".join(f"{display_names.get(c, c)} (ns)" for c in CONFIG_NAMES)
    sep_cells = " | ".join("----------" for _ in CONFIG_NAMES)
    lines = [
        "## Build Configuration Comparison\n",
        f"| Benchmark | {header_cells} | Best |",
        f"|-----------|{sep_cells}|------|",
    ]

    for name in all_names:
        times = {}
        cells = []
        for cfg_name in CONFIG_NAMES:
            t = lookup.get(cfg_name, {}).get(name)
            times[cfg_name] = t
            cells.append(_fmt(t))

        # Determine best
        valid = {k: v for k, v in times.items() if v is not None}
        if valid:
            best_key = min(valid, key=valid.get)
            best = display_names.get(best_key, best_key)
        else:
            best = "N/A"

        cells_str = " | ".join(cells)
        lines.append(f"| `{name}` | {cells_str} | {best} |")

    return "\n".join(lines) + "\n"


def generate_regression_report(
    baseline_dir: Path,
    current_results: Dict[str, List[BenchmarkResult]],
) -> str:
    """Markdown tables comparing main vs PR for each config."""
    sections: List[str] = []
    sections.append("## PR Regression Report\n")

    for cfg_name in CONFIG_NAMES:
        baseline_json = baseline_dir / f"{cfg_name}.json"
        if not baseline_json.exists():
            continue
        baseline = parse_benchmark_json(baseline_json)
        if not baseline:
            continue

        current = current_results.get(cfg_name, [])
        if not current:
            continue

        baseline_lookup = {r.name: r.cpu_time_ns for r in baseline}
        current_lookup = {r.name: r.cpu_time_ns for r in current}

        all_names = list(dict.fromkeys(
            [r.name for r in baseline] + [r.name for r in current]
        ))

        lines = [
            f"### {cfg_name}\n",
            "| Benchmark | main (ns) | PR (ns) | Change | Status |",
            "|-----------|----------|---------|--------|--------|",
        ]

        for name in all_names:
            base_t = baseline_lookup.get(name)
            curr_t = current_lookup.get(name)

            if base_t is not None and curr_t is not None and base_t > 0:
                pct = ((curr_t - base_t) / base_t) * 100.0
                if pct > 5.0:
                    status = ":red_circle: regression"
                elif pct < -5.0:
                    status = ":green_circle: improvement"
                else:
                    status = ":white_circle: neutral"
                change_str = f"{pct:+.1f}%"
            else:
                change_str = "N/A"
                status = ":white_circle: neutral"

            lines.append(
                f"| `{name}` | {_fmt(base_t)} | {_fmt(curr_t)} | {change_str} | {status} |"
            )

        sections.append("\n".join(lines) + "\n")

    return "\n".join(sections)

# ---------------------------------------------------------------------------
# Plot generation
# ---------------------------------------------------------------------------

def generate_plots(
    config_results: Dict[str, List[BenchmarkResult]],
    output_dir: Path,
) -> Dict[str, List[Tuple[str, str]]]:
    """Generate per-operation bar charts for each build config.

    Returns a dict mapping ``"op params"`` group labels to a list of
    ``(config_name, relative_image_path)`` tuples so the caller can
    organise the plots by operation in the markdown report.
    """
    try:
        import matplotlib
        matplotlib.use("Agg")
        import matplotlib.pyplot as plt
    except ImportError:
        log.warning("matplotlib not available — skipping plot generation")
        return {}

    plots_dir = output_dir / "plots"
    plots_dir.mkdir(parents=True, exist_ok=True)

    # group_label -> [(cfg_name, rel_path), ...]
    plot_groups: Dict[str, List[Tuple[str, str]]] = defaultdict(list)

    for cfg_name, results in config_results.items():
        # Group benchmarks by (operation, params)
        groups: Dict[Tuple[str, str], Dict[str, float]] = defaultdict(dict)
        for r in results:
            op, lib, params = extract_library_and_operation(r.name)
            if lib == "unknown":
                continue
            groups[(op, params)][lib] = r.cpu_time_ns

        for (op, params), lib_times in groups.items():
            if len(lib_times) < 2:
                continue

            # Sort by time (fastest at top in horizontal bar chart)
            sorted_items = sorted(lib_times.items(), key=lambda x: x[1], reverse=True)
            libraries = [item[0] for item in sorted_items]
            times = [item[1] for item in sorted_items]
            colors = [LIBRARY_COLORS.get(lib, "#999999") for lib in libraries]

            fig, ax = plt.subplots(figsize=(10, max(2.5, 0.6 * len(libraries) + 1.5)))
            bars = ax.barh(libraries, times, color=colors)

            # Value labels on bars
            for bar, t in zip(bars, times):
                ax.text(
                    bar.get_width() + max(times) * 0.01,
                    bar.get_y() + bar.get_height() / 2,
                    _fmt(t),
                    va="center",
                    fontsize=9,
                )

            formula = OPERATION_FORMULAS.get(op)
            if formula:
                # Two-line title: formula on top, config/params below
                title = f"{formula}\n{op} {params} [{cfg_name}]"
                ax.set_title(title, fontsize=11)
            else:
                ax.set_title(f"{op} {params} [{cfg_name}]", fontsize=11)
            ax.set_xlabel("CPU time (ns)")
            ax.margins(x=0.15)
            fig.tight_layout()

            # Build filename: extract dimension from params for short name
            dim_match = re.search(r"(\d+)ul", params)
            dim_tag = f"_{dim_match.group(1)}d" if dim_match else ""
            safe_op = re.sub(r"[^a-zA-Z0-9_]", "_", op)
            filename = f"{cfg_name}_{safe_op}{dim_tag}.png"
            fig.savefig(plots_dir / filename, dpi=100)
            plt.close(fig)

            rel_path = f"plots/{filename}"
            group_label = f"{op} {params}".strip()
            plot_groups[group_label].append((cfg_name, rel_path))
            log.info("Saved plot: %s", rel_path)

    return dict(plot_groups)

# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main() -> int:
    parser = argparse.ArgumentParser(
        description="Generate markdown reports and plots from benchmark JSON results."
    )
    parser.add_argument(
        "--input-dir",
        required=True,
        help="Directory containing benchmark JSON files (scalar.json, xsimd.json, ...)",
    )
    parser.add_argument(
        "--output-dir",
        default="",
        help="Directory for report and plots (defaults to input-dir)",
    )
    parser.add_argument(
        "--mode",
        choices=["flag-compare", "regression", "both"],
        default="both",
        help="Report mode",
    )
    parser.add_argument(
        "--baseline-json-dir",
        default="",
        help="Path to main's JSON results (for regression mode)",
    )
    parser.add_argument(
        "--no-plots",
        action="store_true",
        default=False,
        help="Skip plot generation (useful when matplotlib is not available)",
    )

    args = parser.parse_args()

    input_dir = Path(args.input_dir).resolve()
    output_dir = Path(args.output_dir).resolve() if args.output_dir else input_dir
    output_dir.mkdir(parents=True, exist_ok=True)

    if not input_dir.exists():
        log.error("Input directory does not exist: %s", input_dir)
        return 1

    config_results = load_results(input_dir)

    # Generate reports
    report_parts: List[str] = []

    if args.mode in ("flag-compare", "both") and config_results:
        report_parts.append(generate_flag_comparison_report(config_results))

    if args.mode in ("regression", "both") and args.baseline_json_dir:
        baseline_dir = Path(args.baseline_json_dir).resolve()
        if baseline_dir.exists():
            report_parts.append(
                generate_regression_report(baseline_dir, config_results)
            )
        else:
            log.warning("Baseline directory %s not found, skipping regression report", baseline_dir)

    # Generate plots
    plot_groups: Dict[str, List[Tuple[str, str]]] = {}
    if not args.no_plots and config_results:
        plot_groups = generate_plots(config_results, output_dir)

    # Append plot section to report, grouped by operation
    if plot_groups:
        plot_lines = ["## Plots\n"]
        for group_label in sorted(plot_groups):
            plot_lines.append(f"### {group_label}\n")
            # Sort configs in canonical order
            cfg_order = {name: i for i, name in enumerate(CONFIG_NAMES)}
            entries = sorted(plot_groups[group_label], key=lambda x: cfg_order.get(x[0], 99))
            for cfg_name, img in entries:
                plot_lines.append(f"![{group_label} {cfg_name}]({img})\n")
            plot_lines.append("")
        report_parts.append("\n".join(plot_lines))

    if report_parts:
        report = "\n".join(report_parts)
        report_path = output_dir / "benchmark_report.md"
        report_path.write_text(report)
        log.info("Report written to %s", report_path)
    else:
        log.warning("No report generated (no results or mode mismatch)")
        # Write an empty report so downstream steps don't fail
        report_path = output_dir / "benchmark_report.md"
        report_path.write_text("## Benchmark Report\n\nNo results available.\n")

    return 0


if __name__ == "__main__":
    sys.exit(main())
