# 不可完全置信清单

本目录只保存状态报告，不保存第二份题解源码。收到章节任务时仍必须先读取 `../LC_TEMPLATE_MAPPING.tsv`；这里用于快速定位映射、测试、候选或在线闭环尚未完成的项目，不能反向定义章节范围。

运行以下命令刷新：

```powershell
python library-checker-template-solutions/tools/refresh_untrusted_inventory.py
```

状态含义：

- `mapping_review_pending`：TSV 仅为 `classified`，尚无源码级映射审计。
- `source_missing/source_stale`：没有当前唯一 `main.cpp`，或清单哈希与磁盘不一致。
- `verification_missing`：当前源码既没有官方本地验证收据，也没有可认证的同哈希线上 AC。

`NO_CURRENT_ONLINE.tsv` 精确保存当前没有可认证线上 AC 的全部题；`ALL.tsv` 保存仍缺映射、源码或任一当前正确性证据的项目。`SUMMARY.md` 给出按章计数，`chapter-NN.md` 是收到第 NN 章任务时的未完成视图。全部项目可置信时，本目录会被自动删除；任何源码、manifest、meta、收据、映射或官方 commit 改变后都必须重新生成。
