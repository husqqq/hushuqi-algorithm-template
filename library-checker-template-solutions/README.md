# Library Checker 当前模板解答

本目录保存由当前主模板生成的 251 道 Library Checker 自包含候选，以及它们的映射、元数据、验证和受控提交工具。题目全集由 [`LC_TEMPLATE_MAPPING.tsv`](LC_TEMPLATE_MAPPING.tsv) 冻结；候选索引与当前源码哈希由 [`manifest.json`](manifest.json) 记录。

每题只把以下文件作为公开的当前状态：

```text
<category>/<problem>/main.cpp
<category>/<problem>/meta.json
```

`main.cpp` 是该题唯一的验证与提交源码，只引用系统头文件。只有同一份源码通过本地模板测试、官方 checker，并取得线上最终 verdict 后，才能作为当前模板的验证证据。

## 入口

- [`docs/95-总览.md`](docs/95-总览.md)：目录职责、候选约束和文档导航。
- [`docs/90-模板复查与受控提交流程.md`](docs/90-模板复查与受控提交流程.md)：逐章复查、生成、验证和提交的强制流程。
- [`docs/91-Yosupo自动提交指南.md`](docs/91-Yosupo自动提交指南.md)：官方题库、登录、验证收据和单题提交命令。
- [`docs/96-全章节审计快照.md`](docs/96-全章节审计快照.md)：251 道题当前映射与证据快照。
- [`docs/97-多实现在线验证审计.md`](docs/97-多实现在线验证审计.md)：同题多种本地实现的生成、测试与在线验证记录。

## 本地状态

`.local/` 只保存在本机，包含：

- 固定提交版本的官方 `library-checker-problems` 工作树；
- `yosupo-profile/` 浏览器登录状态，其中可能包含 Cookie 和会话信息；
- 当前源码的本地验证收据、性能记录、提交批次和结果记录。

这些内容对继续验证和复用登录态有用，但体积较大且涉及本机状态或隐私，必须由顶层 `.gitignore` 排除，不能提交到 GitHub。题解源码不存放在 `.local/` 中。

## 第 03 章生成器

`generate_ch03_lc.py` 必须显式选择模式，使用 `--only` 指定题目：

```powershell
python library-checker-template-solutions/tools/generate_ch03_lc.py --only range_affine_range_sum --describe
python library-checker-template-solutions/tools/generate_ch03_lc.py --only range_affine_range_sum --write
python library-checker-template-solutions/tools/generate_ch03_lc.py --only range_affine_range_sum --check
```

`--describe` 只输出依赖与源码哈希；`--write` 同步候选、清单和来源元数据，兼容旧名 `--write-index`；`--check` 只核验生成字节、清单与来源元数据。无参数调用和只读模式均不写文件。模运算依赖第一章 `1.1.008` 的 `MInt<P>`，不再依赖第七章兼容入口。

同一源码已有的在线 AC 在刷新来源元数据时保留；源码改变后，当前验证失效，历史提交仍保留。生成、正确性、性能和在线状态分别记录，当前进度见[第 03 章审计](docs/03-数据结构审计.md)。
