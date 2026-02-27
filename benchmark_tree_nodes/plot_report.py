#!/usr/bin/env python3
"""
Benchmark report: Scalar vs xsimd (no unroll) vs xsimd (with unroll).

Usage:
    python plot_report.py results.json

Reads a single JSON file produced by the combined bench_tree_nodes binary.
Benchmark names contain a suffix (_scalar, _xsimd, _xsimd_unroll) that
identifies the configuration.

Produces PNG plots comparing the three configurations for all tmech benchmarks.
"""

import json
import sys
import re
from pathlib import Path

import matplotlib.pyplot as plt
import matplotlib
import numpy as np

matplotlib.rcParams.update({"font.size": 10, "figure.dpi": 150})

SUFFIXES = ["scalar", "xsimd", "xsimd_unroll"]
LABELS = ["Scalar", "xsimd (no unroll)", "xsimd + unroll"]
COLORS = ["#6c757d", "#0d6efd", "#198754"]


def load_benchmarks(path):
    """Load single JSON, split into 3 datasets keyed by base name."""
    with open(path) as f:
        data = json.load(f)

    datasets = {s: {} for s in SUFFIXES}
    for bm in data["benchmarks"]:
        name = bm["name"]
        # Only use _mean aggregates
        if not name.endswith("_mean"):
            continue
        # Strip _mean suffix
        name = name[: -len("_mean")]

        # Identify which config this belongs to (longest suffix match first)
        for suffix in sorted(SUFFIXES, key=len, reverse=True):
            # Pattern: base_name_SUFFIX<template args>
            pattern = f"_{suffix}<"
            idx = name.find(pattern)
            if idx >= 0:
                base = name[:idx] + name[idx + len(f"_{suffix}") :]
                datasets[suffix][base] = bm["real_time"]
                break

    return datasets


def shorten_name(name):
    """'otimes_iso<double, 3ul>' -> 'otimes_iso (3D)'"""
    m = re.match(r"(.+?)<(\w+),\s*(\d+)ul>$", name)
    if m:
        base = m.group(1)
        dim = m.group(3)
        return f"{base} ({dim}D)"
    return name


def group_benchmarks(names):
    """Group benchmarks by category for multi-panel plots."""
    groups = {}
    for name in names:
        short = shorten_name(name)
        if "otimes_nodes" in short:
            cat = "Tree-node sweep"
        elif "otimes" in short:
            cat = "Outer products"
        elif "addition" in short:
            cat = "Addition"
        elif "single_contraction" in short:
            cat = "Single contraction"
        elif "double_contraction" in short or "complex_double" in short:
            cat = "Double contraction"
        elif "more_complex" in short:
            cat = "Complex expressions"
        elif "inverse" in short:
            cat = "Inverse"
        else:
            cat = "Other"
        groups.setdefault(cat, []).append(name)
    return groups


def plot_group(ax, names, datasets, title):
    """Grouped bar chart — absolute times."""
    short_names = [shorten_name(n) for n in names]
    x = np.arange(len(names))
    width = 0.25

    for i, (suffix, label, color) in enumerate(zip(SUFFIXES, LABELS, COLORS)):
        vals = [datasets[suffix].get(n, 0) for n in names]
        bars = ax.bar(
            x + i * width,
            vals,
            width,
            label=label,
            color=color,
            edgecolor="white",
            linewidth=0.5,
        )
        for bar, v in zip(bars, vals):
            if v > 0:
                ax.text(
                    bar.get_x() + bar.get_width() / 2,
                    bar.get_height(),
                    f"{v:.0f}" if v >= 10 else f"{v:.1f}",
                    ha="center",
                    va="bottom",
                    fontsize=5.5,
                )

    vals_all = [
        datasets[s].get(n, 0) for s in SUFFIXES for n in names if datasets[s].get(n, 0) > 0
    ]
    if vals_all and max(vals_all) / min(vals_all) > 50:
        ax.set_yscale("log")

    ax.set_xticks(x + width)
    ax.set_xticklabels(short_names, rotation=35, ha="right", fontsize=8)
    ax.set_ylabel("Time (ns)")
    ax.set_title(title, fontweight="bold")
    ax.legend(fontsize=7, loc="upper left")
    ax.grid(axis="y", alpha=0.3)


def plot_speedup(ax, names, datasets, title):
    """Speedup bar chart relative to scalar baseline."""
    short_names = [shorten_name(n) for n in names]
    x = np.arange(len(names))
    width = 0.35

    scalar = datasets["scalar"]
    for i, (suffix, label, color) in enumerate(
        zip(SUFFIXES[1:], LABELS[1:], COLORS[1:])
    ):
        speedups = []
        for n in names:
            s = scalar.get(n, 0)
            v = datasets[suffix].get(n, 0)
            speedups.append(s / v if v > 0 else 0)
        bars = ax.bar(
            x + i * width,
            speedups,
            width,
            label=label,
            color=color,
            edgecolor="white",
            linewidth=0.5,
        )
        for bar, sp in zip(bars, speedups):
            ax.text(
                bar.get_x() + bar.get_width() / 2,
                bar.get_height(),
                f"{sp:.1f}x",
                ha="center",
                va="bottom",
                fontsize=6,
            )

    ax.axhline(y=1.0, color="black", linestyle="--", linewidth=0.8, alpha=0.5)
    ax.set_xticks(x + width / 2)
    ax.set_xticklabels(short_names, rotation=35, ha="right", fontsize=8)
    ax.set_ylabel("Speedup vs Scalar")
    ax.set_title(title, fontweight="bold")
    ax.legend(fontsize=7)
    ax.grid(axis="y", alpha=0.3)


def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} results.json")
        sys.exit(1)

    datasets = load_benchmarks(sys.argv[1])

    for suffix in SUFFIXES:
        print(f"  {suffix}: {len(datasets[suffix])} benchmarks")

    # Find common benchmarks across all 3 configs
    common = sorted(
        set(datasets["scalar"]) & set(datasets["xsimd"]) & set(datasets["xsimd_unroll"])
    )
    if not common:
        print("ERROR: no common benchmarks found across the 3 configs.")
        sys.exit(1)

    print(f"Found {len(common)} common benchmarks.")

    # Separate tree-node sweep from tmech benchmarks
    tmech_names = [n for n in common if "otimes_nodes" not in n]
    node_names = [n for n in common if "otimes_nodes" in n]

    groups = group_benchmarks(tmech_names)
    n_groups = len(groups)

    # --- Absolute times plot (tmech benchmarks only) ---
    fig, axes = plt.subplots(n_groups, 1, figsize=(14, 4.5 * n_groups))
    if n_groups == 1:
        axes = [axes]
    fig.suptitle(
        "Benchmark: Absolute Times (lower is better)", fontsize=14, fontweight="bold"
    )
    for ax, (cat, names) in zip(axes, sorted(groups.items())):
        plot_group(ax, names, datasets, cat)
    fig.tight_layout(rect=[0, 0, 1, 0.97])
    fig.savefig("bench_absolute.png", bbox_inches="tight")
    print("Saved bench_absolute.png")

    # --- Speedup plot (tmech benchmarks only) ---
    fig2, axes2 = plt.subplots(n_groups, 1, figsize=(14, 4.5 * n_groups))
    if n_groups == 1:
        axes2 = [axes2]
    fig2.suptitle(
        "Benchmark: Speedup vs Scalar Baseline (higher is better)",
        fontsize=14,
        fontweight="bold",
    )
    for ax, (cat, names) in zip(axes2, sorted(groups.items())):
        plot_speedup(ax, names, datasets, cat)
    fig2.tight_layout(rect=[0, 0, 1, 0.97])
    fig2.savefig("bench_speedup.png", bbox_inches="tight")
    print("Saved bench_speedup.png")

    # --- Tree-node sweep plot ---
    if node_names:
        fig3, (ax3a, ax3b) = plt.subplots(1, 2, figsize=(14, 5))
        fig3.suptitle(
            "Tree-Node Threshold Sweep (otimes chains, 3D)",
            fontsize=14,
            fontweight="bold",
        )

        # Extract node counts from names
        node_counts = []
        for n in node_names:
            m = re.search(r"otimes_nodes_(\d+)", n)
            if m:
                node_counts.append(int(m.group(1)))
            else:
                node_counts.append(0)

        for suffix, label, color in zip(SUFFIXES, LABELS, COLORS):
            vals = [datasets[suffix].get(n, 0) for n in node_names]
            ax3a.plot(node_counts, vals, "o-", label=label, color=color, linewidth=2)
        ax3a.axvline(x=16, color="red", linestyle="--", linewidth=1, alpha=0.7, label="Threshold (16)")
        ax3a.set_xlabel("Tree Nodes")
        ax3a.set_ylabel("Time (ns)")
        ax3a.set_title("Absolute Times")
        ax3a.legend(fontsize=8)
        ax3a.grid(alpha=0.3)

        # Speedup vs scalar
        scalar_vals = [datasets["scalar"].get(n, 0) for n in node_names]
        for suffix, label, color in zip(SUFFIXES[1:], LABELS[1:], COLORS[1:]):
            vals = [datasets[suffix].get(n, 0) for n in node_names]
            speedups = [s / v if v > 0 else 0 for s, v in zip(scalar_vals, vals)]
            ax3b.plot(node_counts, speedups, "o-", label=label, color=color, linewidth=2)
        ax3b.axhline(y=1.0, color="black", linestyle="--", linewidth=0.8, alpha=0.5)
        ax3b.axvline(x=16, color="red", linestyle="--", linewidth=1, alpha=0.7, label="Threshold (16)")
        ax3b.set_xlabel("Tree Nodes")
        ax3b.set_ylabel("Speedup vs Scalar")
        ax3b.set_title("Speedup")
        ax3b.legend(fontsize=8)
        ax3b.grid(alpha=0.3)

        fig3.tight_layout(rect=[0, 0, 1, 0.95])
        fig3.savefig("bench_tree_nodes.png", bbox_inches="tight")
        print("Saved bench_tree_nodes.png")

    # --- Summary table ---
    hdr = f"{'Benchmark':<45} {'Scalar':>9} {'xsimd':>9} {'+unroll':>9} {'Speedup':>9}"
    sep = "=" * len(hdr)
    print(f"\n{sep}\n{hdr}\n{sep}")
    for name in common:
        short = shorten_name(name)
        s = datasets["scalar"].get(name, 0)
        x = datasets["xsimd"].get(name, 0)
        u = datasets["xsimd_unroll"].get(name, 0)
        sp = f"{s/u:.2f}x" if u > 0 else "N/A"
        print(f"{short:<45} {s:>8.1f} {x:>8.1f} {u:>8.1f} {sp:>9}")
    print(sep)


if __name__ == "__main__":
    main()
