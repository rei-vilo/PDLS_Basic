#!/usr/bin/env python3
"""Generate Coverage.md from arduino/compile-sketches JSON reports."""

import json
import os
from datetime import datetime, timezone
from pathlib import Path

REPO_ROOT = Path(os.environ.get("GITHUB_WORKSPACE", "."))
REPORTS_DIR = Path(os.environ.get("SKETCHES_REPORTS_PATH", "sketches-reports"))
OUTPUT_FILE = REPO_ROOT / "Coverage.md"

BOARD_NOTES = [
    ("General examples", "rp2040:rp2040:rpipico"),
    ("EXT4 examples", "SiliconLabs:silabs:nano_matter"),
    ("File examples", "teensy:avr:teensy36"),
    ("Font16 examples", "rp2040:rp2040:rpipico2"),
]

REPORT_FILE_ORDER = [
    "rp2040-rp2040-rpipico.json",
    "SiliconLabs-silabs-nano_matter.json",
]


def read_version() -> str:
    properties_file = REPO_ROOT / "library.properties"
    for line in properties_file.read_text(encoding="utf-8").splitlines():
        if line.startswith("version="):
            return line.split("=", 1)[1].strip()
    return "unknown"


def ordered_report_files() -> list[Path]:
    report_files: list[Path] = []
    for filename in REPORT_FILE_ORDER:
        report_path = REPORTS_DIR / filename
        if report_path.exists():
            report_files.append(report_path)

    for report_path in sorted(REPORTS_DIR.glob("*.json")):
        if report_path not in report_files:
            report_files.append(report_path)

    return report_files


def load_sketch_rows() -> list[tuple[str, str, str]]:
    rows: list[tuple[str, str, str]] = []

    for report_path in ordered_report_files():
        with report_path.open(encoding="utf-8") as report_file:
            report_data = json.load(report_file)

        for board_entry in report_data.get("boards", []):
            board = board_entry.get("board", "")
            for sketch in board_entry.get("sketches", []):
                example_name = Path(sketch["name"]).name
                result = "OK" if sketch.get("compilation_success", False) else "FAILED"
                rows.append((example_name, board, result))

    return rows


def generate_report() -> None:
    start_time = datetime.now(timezone.utc).strftime("%Y-%m-%d %H:%M:%S")
    version = read_version()
    rows = load_sketch_rows()

    lines = [
        "",
        "# Coverage report for PDLS_Basic",
        "",
        "Test of the examples",
        "",
    ]

    for label, board in BOARD_NOTES:
        lines.append(f"*{label} target {board}*")
        lines.append("")

    lines.extend(
        [
            f"Report generated {start_time}",
            "",
            f"Version: {version}",
            "",
            "Example |Board | Result",
            "--- | --- | ---",
        ]
    )

    for example_name, board, result in rows:
        lines.append(f"{example_name} | {board} | {result}")

    end_time = datetime.now(timezone.utc).strftime("%Y-%m-%d %H:%M:%S")
    lines.extend(["", "---", "", f"Report completed {end_time}", ""])

    OUTPUT_FILE.write_text("\n".join(lines), encoding="utf-8")


if __name__ == "__main__":
    generate_report()
