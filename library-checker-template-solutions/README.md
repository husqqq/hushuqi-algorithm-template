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
