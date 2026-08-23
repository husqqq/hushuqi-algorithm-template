# Library Checker 参考实现

这里保留从现已删除的旧仓库中按提交 `bcd4dbc7d3d5ef7104b3cc7b19a951bb065a733f` 恢复的 251 道 Library Checker 独立 C++ 实现；恢复时该子树的 Git tree 为 `59deee7b3513774a94590dcb87f714d104292e1a`。它们只用于研究算法、核对 I/O 和比较性能。本目录不保存当前模板的适配、展开、验证或提交工具，也不能用这里的 AC 证明主模板通过。

每题目位于：

```text
<category>/<problem>/main.cpp
<category>/<problem>/meta.json
```

`meta.json` 记录实现来源、内容哈希及已有的在线评测结果，`manifest.json` 保留问题索引。公开的提交编号、状态、耗时和内存用于复核代码；账号、密码、Token、Cookie 和浏览器登录状态不会写入仓库。

当前模板生成的代码和完整提交流程统一位于并列的 [`../library-checker-template-solutions/`](../library-checker-template-solutions/README.md)。
