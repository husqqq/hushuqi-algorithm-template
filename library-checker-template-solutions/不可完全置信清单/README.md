# 不可完全置信清单

本目录只保存状态报告，不保存第二份题解源码。收到章节任务时仍必须先读取 `../LC_TEMPLATE_MAPPING.tsv`；这里用于快速定位映射、测试、候选或在线闭环尚未完成的项目，不能反向定义章节范围。

运行以下命令刷新：

```powershell
python library-checker-template-solutions/tools/refresh_untrusted_inventory.py
```

状态含义：

- `mapping_review_pending`：TSV 仅为 `classified`，尚无源码级映射审计。
- `source_missing/source_stale`：没有当前唯一 `main.cpp`，或清单哈希与磁盘不一致。
- `generation_missing/generation_stale/generation_failed`：第 03 章缺生成状态、当前模板生成字节不同，或生成前置条件失效；即使候选已有 AC，仍列为未完成。
- `not_checked`：本工具尚未重生成该章，不能据此宣称模板同步完成。
- `verification_missing`：当前源码既没有官方本地验证收据，也没有可认证的同哈希线上 AC。

`NO_CURRENT_ONLINE.tsv` 精确保存当前没有候选同哈希线上 AC 的全部题；`ALL.tsv` 保存映射、候选哈希、第 03 章模板生成或正确性证据仍有缺口的项目。`SUMMARY.md` 给出按章计数，`chapter-NN.md` 是按章未完成视图。任何源码、模板、生成器、manifest、meta、收据、映射或官方 commit 改变后都必须重新生成。
