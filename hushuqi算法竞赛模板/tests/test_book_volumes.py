"""分册按章划分、超页重试与失败保留文件的回归测试。"""
from itertools import product
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch
from pypdf import PdfWriter

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "book"))
import build_volumes as volumes
import build


class VolumeTests(unittest.TestCase):
    def test_partitions_are_complete_ordered_and_optimal(self):
        for weights in product((1, 3, 8), repeat=5):
            candidates = volumes.candidate_partitions(weights)
            self.assertEqual(len(candidates), 6)
            self.assertEqual(len(set(candidates)), 6)
            scores = []
            for bounds in candidates:
                self.assertEqual((bounds[0], bounds[-1]), (0, 5))
                self.assertTrue(all(a < b for a, b in zip(bounds, bounds[1:])))
                sizes = [sum(weights[a:b]) for a, b in zip(bounds, bounds[1:])]
                scores.append((max(sizes), max(sizes) - min(sizes), bounds))
            self.assertEqual(scores, sorted(scores))

    def run_build(self, always_fail=False):
        with tempfile.TemporaryDirectory(prefix="volume-regression-") as directory:
            root = Path(directory)
            output = root / "volumes"
            output.mkdir()
            for i in range(4):
                (root / f"{i:02d}-chapter.md").write_text(f"# {i}. chapter\n", encoding="utf-8")
            for name in ("上", "中", "下"):
                (output / f"hushuqi算法竞赛模板({name}).pdf").write_bytes(b"previous")

            def render(name, files, tmp, out):
                # 第一候选的末册超限；备用边界将末册拆开。
                pages = 601 if always_fail or len(files) > 1 else 100
                if name == "上" and not always_fail:
                    pages = 100
                out.write_bytes((name + ":" + ",".join(p.stem for p in files)).encode())
                return pages

            with patch.object(volumes, "ROOT", root), patch.object(volumes, "VOLUME_DIR", output), patch.object(volumes, "chapter_pages", return_value=[1] * 4), patch.object(volumes, "build_one", side_effect=render):
                if always_fail:
                    with self.assertRaises(ValueError):
                        volumes.build_volumes(root / "full.pdf")
                    self.assertTrue(all(p.read_bytes() == b"previous" for p in output.glob("*.pdf")))
                else:
                    volumes.build_volumes(root / "full.pdf")
                    texts = [(output / f"hushuqi算法竞赛模板({name}).pdf").read_text() for name in ("上", "中", "下")]
                    self.assertEqual(texts, ["上:00-chapter,01-chapter", "中:02-chapter", "下:03-chapter"])

    def test_actual_page_limit_rebalances(self):
        self.run_build()

    def test_failed_build_preserves_all_volumes(self):
        self.run_build(always_fail=True)

    def test_invalid_chapter_weights(self):
        for weights in ([], [1, 2], [1, 0, 2]):
            with self.assertRaises(ValueError):
                volumes.candidate_partitions(weights)

    def test_chapter_pages_require_matching_bookmarks(self):
        with tempfile.TemporaryDirectory(prefix="volume-outline-") as directory:
            root = Path(directory)
            files = []
            writer = PdfWriter()
            for _ in range(8):
                writer.add_blank_page(width=595, height=842)
            for i, start in enumerate((1, 3, 6)):
                source = root / f"{i:02d}-chapter.md"
                source.write_text(f"# {i}. 测试章\n", encoding="utf-8")
                files.append(source)
                parent = writer.add_outline_item(f"{i}. 测试章", start)
                writer.add_outline_item(f"{i}.1 测试小节", start, parent=parent)
            pdf = root / "full.pdf"
            writer.write(pdf)
            self.assertEqual(volumes.chapter_pages(files, pdf), [2, 3, 2])
            files[0].write_text("# 0. 新章名\n", encoding="utf-8")
            with self.assertRaisesRegex(ValueError, "书签"):
                volumes.chapter_pages(files, pdf)

    def test_compile_failure_preserves_all_volumes(self):
        with tempfile.TemporaryDirectory(prefix="volume-failed-compile-") as directory:
            root = Path(directory)
            for i in range(3):
                (root / f"{i:02d}-chapter.md").touch()
            output = root / "output"
            output.mkdir()
            previous = output / "hushuqi算法竞赛模板(上).pdf"
            previous.write_bytes(b"previous")
            with patch.object(volumes, "ROOT", root), patch.object(volumes, "VOLUME_DIR", output), patch.object(volumes, "chapter_pages", return_value=[1] * 3), patch.object(volumes, "build_one", side_effect=RuntimeError("XeLaTeX failed")):
                with self.assertRaisesRegex(RuntimeError, "XeLaTeX"):
                    volumes.build_volumes(root / "full.pdf")
            self.assertEqual(previous.read_bytes(), b"previous")
            self.assertEqual(len(list(output.iterdir())), 1)

    def test_standalone_rebuilds_full_before_partition(self):
        calls = []
        with patch.object(build, "build_full", side_effect=lambda: calls.append("full") or Path("fresh.pdf")), patch.object(volumes, "build_volumes", side_effect=lambda path: calls.append(path)):
            volumes.main()
        self.assertEqual(calls, ["full", Path("fresh.pdf")])


if __name__ == "__main__":
    unittest.main()
