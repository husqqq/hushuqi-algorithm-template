"""Recompute Chapter 03 source equality without trusting metadata or AC flags."""

from __future__ import annotations

import importlib.util
import sys
from pathlib import Path


def chapter03_states(root: Path, entries: dict[str, dict]) -> dict[str, str]:
    """root 是题解目录；返回每道第 03 章题的当前生成状态，不写入文件。"""
    path = Path(__file__).resolve().parent / "generate_ch03_lc.py"
    spec = importlib.util.spec_from_file_location("inventory_ch03_generator", path)
    if spec is None or spec.loader is None:
        raise RuntimeError(f"无法加载生成器：{path}")
    generator = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = generator
    spec.loader.exec_module(generator)
    result = {}
    for item in generator.SPECS:
        problem = f"data_structure/{item.name}"
        entry = entries.get(problem)
        if entry is None:
            result[problem] = "generation_missing"
            continue
        try:
            generated, _ = generator.generate_twice(root.parent, item)
            target = root / entry["source"]
            result[problem] = (
                "generation_current" if target.is_file() and target.read_bytes() == generated
                else "generation_stale"
            )
        except (OSError, ValueError, KeyError):
            result[problem] = "generation_failed"
    return result
