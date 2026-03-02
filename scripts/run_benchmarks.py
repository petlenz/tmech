#!/usr/bin/env python3
"""Build and run tmech benchmarks under multiple configurations.

Builds tmech under two configurations (scalar, xsimd),
runs Google Benchmark, and writes JSON result files to the output directory.

Report generation is handled separately by run_bench_report.py.
"""

import argparse
import logging
import os
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import List, Optional

logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s [%(levelname)s] %(message)s",
    datefmt="%H:%M:%S",
)
log = logging.getLogger(__name__)

# ---------------------------------------------------------------------------
# Data classes
# ---------------------------------------------------------------------------

@dataclass
class BuildConfig:
    name: str
    use_xsimd: bool

    def cmake_args(self) -> List[str]:
        return [
            "-DBUILD_BENCHMARK=ON",
            "-DBUILD_TESTS=OFF",
            "-DBUILD_EXAMPLES=OFF",
            "-DCMAKE_BUILD_TYPE=Release",
            f"-DTMECH_USE_XSIMD={'ON' if self.use_xsimd else 'OFF'}",
        ]


CONFIGS = [
    BuildConfig("scalar", use_xsimd=False),
    BuildConfig("xsimd", use_xsimd=True),
]

# ---------------------------------------------------------------------------
# Build helpers
# ---------------------------------------------------------------------------

def configure_and_build(
    source_dir: Path,
    build_dir: Path,
    config: BuildConfig,
    jobs: int,
) -> bool:
    """Run cmake configure + build.  Returns True on success."""
    build_dir.mkdir(parents=True, exist_ok=True)

    cmake_configure = [
        "cmake",
        "-B", str(build_dir),
        "-S", str(source_dir),
        *config.cmake_args(),
    ]
    log.info("Configuring %s: %s", config.name, " ".join(cmake_configure))
    try:
        subprocess.run(cmake_configure, check=True, timeout=600)
    except (subprocess.CalledProcessError, subprocess.TimeoutExpired) as exc:
        log.warning("Configure failed for %s: %s", config.name, exc)
        return False

    cmake_build = [
        "cmake", "--build", str(build_dir),
        "--target", "tmech_benchmark",
        "-j", str(jobs) if jobs > 0 else str(os.cpu_count() or 1),
    ]
    log.info("Building %s: %s", config.name, " ".join(cmake_build))
    try:
        subprocess.run(cmake_build, check=True, timeout=600)
    except (subprocess.CalledProcessError, subprocess.TimeoutExpired) as exc:
        log.warning("Build failed for %s: %s", config.name, exc)
        return False

    return True

# ---------------------------------------------------------------------------
# Benchmark runner
# ---------------------------------------------------------------------------

def find_benchmark_binary(build_dir: Path) -> Optional[Path]:
    """Locate the benchmark binary in the build tree."""
    for candidate in [
        build_dir / "benchmark" / "tmech_benchmark",
        build_dir / "tmech_benchmark",
    ]:
        if candidate.exists() and os.access(candidate, os.X_OK):
            return candidate
    return None


def run_benchmark(
    build_dir: Path,
    json_out: Path,
    repetitions: int,
    benchmark_filter: str,
) -> bool:
    """Run the benchmark binary.  Returns True on success."""
    binary = find_benchmark_binary(build_dir)
    if binary is None:
        log.warning("Benchmark binary not found in %s", build_dir)
        return False

    cmd = [
        str(binary),
        f"--benchmark_out={json_out}",
        "--benchmark_out_format=json",
        f"--benchmark_repetitions={repetitions}",
        "--benchmark_report_aggregates_only=true",
    ]
    if benchmark_filter:
        cmd.append(f"--benchmark_filter={benchmark_filter}")

    log.info("Running benchmarks: %s", " ".join(cmd))
    try:
        subprocess.run(cmd, check=True, timeout=600)
    except (subprocess.CalledProcessError, subprocess.TimeoutExpired) as exc:
        log.warning("Benchmark run failed: %s", exc)
        return False

    return True

# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main() -> int:
    parser = argparse.ArgumentParser(
        description="Build and run tmech benchmarks under multiple configurations."
    )
    parser.add_argument("--source-dir", default=".", help="tmech source root")
    parser.add_argument(
        "--build-dir-prefix",
        default="build_bench",
        help="Prefix for build directories (creates <prefix>_scalar, etc.)",
    )
    parser.add_argument(
        "--output-dir",
        default="benchmark_results",
        help="Directory for JSON output",
    )
    parser.add_argument(
        "--repetitions", type=int, default=5, help="--benchmark_repetitions value"
    )
    parser.add_argument(
        "--benchmark-filter", default="", help="Regex filter for benchmarks"
    )
    parser.add_argument(
        "--jobs", type=int, default=0, help="Parallel build jobs (0 = auto)"
    )

    args = parser.parse_args()

    source_dir = Path(args.source_dir).resolve()
    output_dir = Path(args.output_dir).resolve()
    output_dir.mkdir(parents=True, exist_ok=True)

    success_count = 0

    for cfg in CONFIGS:
        build_dir = Path(f"{args.build_dir_prefix}_{cfg.name}").resolve()
        json_out = output_dir / f"{cfg.name}.json"

        log.info("=== Configuration: %s ===", cfg.name)

        if not configure_and_build(source_dir, build_dir, cfg, args.jobs):
            log.warning("Skipping %s due to build failure", cfg.name)
            continue

        if not run_benchmark(build_dir, json_out, args.repetitions, args.benchmark_filter):
            log.warning("Skipping %s due to benchmark failure", cfg.name)
            continue

        log.info("Benchmark JSON written to %s", json_out)
        success_count += 1

    log.info("Completed %d/%d configurations", success_count, len(CONFIGS))
    return 0


if __name__ == "__main__":
    sys.exit(main())
