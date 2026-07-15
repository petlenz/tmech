#!/usr/bin/env python3
"""Generate the JOSS paper performance figure (paper/paper_benchmark.png).

Reads a Google Benchmark JSON result file (by default the xsimd/SIMD build) and
produces the grouped bar chart of execution time relative to tmech for a curated
set of representative operations, comparing tmech, Eigen and Fastor.

Usage:
    python3 scripts/make_paper_figure.py \
        --input scripts/benchmark_results/xsimd.json \
        --output paper/paper_benchmark.png
"""
import argparse
import json
from pathlib import Path

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

# (label, benchmark base name).  Each op is compared across the three libraries
# by substituting the library suffix into "<base>_<lib><double, 3ul>".
OPS = [
    ("add\n(2)",       "addition"),
    ("A·B\n(2)",  "single_contraction"),
    ("C:a\n(4,2)",     "double_contraction_4_2"),
    ("C:D\n(4,4)",     "double_contraction_4_4"),
    ("otimesu\n(2)",   "otimesu"),
    ("otimesl\n(2)",   "otimesl"),
    ("expr\n(2)",      "more_complex"),
    ("expr\n(4)",      "more_complex_4_4"),
]
LIBS = ["tmech", "eigen", "fastor"]
LEGEND = {"tmech": "tmech", "eigen": "Eigen", "fastor": "Fastor"}
COLORS = {"tmech": "#2b6d8f", "eigen": "#c9c9c9", "fastor": "#d08b2c"}
DIM = "<double, 3ul>"


def load_means(json_path):
    """Return {benchmark_name: mean real_time} for all mean-aggregate entries."""
    data = json.load(open(json_path))
    means = {}
    for b in data["benchmarks"]:
        if b.get("aggregate_name") == "mean":
            means[b["name"]] = b["real_time"]
    return means


def relative_times(means):
    """For each op, return {lib: time/tmech_time} (tmech == 1.0)."""
    rows = []
    for label, base in OPS:
        def t(lib):
            return means[f"{base}_{lib}{DIM}_mean"]
        base_t = t("tmech")
        rows.append((label, {lib: t(lib) / base_t for lib in LIBS}))
    return rows


def make_figure(rows, output):
    fig, ax = plt.subplots(figsize=(5.36, 2.6))
    n = len(rows)
    width = 0.26
    xs = range(n)
    for i, lib in enumerate(LIBS):
        offs = [x + (i - 1) * width for x in xs]
        vals = [r[1][lib] for r in rows]
        ax.bar(offs, vals, width, label=LEGEND[lib], color=COLORS[lib],
               edgecolor="none", zorder=3)
    ax.axhline(1.0, ls="--", lw=0.9, color="0.35", zorder=2)
    ax.set_xticks(list(xs))
    ax.set_xticklabels([r[0] for r in rows], fontsize=8)
    ax.set_ylabel("time relative\nto tmech", fontsize=9)
    ax.tick_params(axis="y", labelsize=8)
    ax.set_ylim(0, None)
    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)
    ax.legend(ncol=3, loc="lower center", bbox_to_anchor=(0.5, 1.0),
              frameon=False, fontsize=9, handlelength=1.2, columnspacing=1.5)
    fig.tight_layout()
    fig.savefig(output, dpi=200, bbox_inches="tight")
    plt.close(fig)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--input", type=Path,
                    default=Path("scripts/benchmark_results/xsimd.json"))
    ap.add_argument("--output", type=Path,
                    default=Path("paper/paper_benchmark.png"))
    args = ap.parse_args()
    rows = relative_times(load_means(args.input))
    for label, vals in rows:
        pretty = label.replace("\n", " ")
        print(f"{pretty:12} " + "  ".join(f"{k}={v:.2f}" for k, v in vals.items()))
    make_figure(rows, args.output)
    print(f"wrote {args.output}")


if __name__ == "__main__":
    main()
