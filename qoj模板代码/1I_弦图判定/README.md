# 1I. 弦图判定

- QOJ: https://qoj.ac/contest/3936/problem/21755
- 最新提交：https://qoj.ac/submission/2829433
- 状态：性能版 `main.cpp` 已通过 QOJ；打印版候选 `template.hpp` 待批准写入正式模板
- 模板条目：5.10.001
- 实现：`chordalCert(n,edges)` 使用权值桶 MCS、CSR 邻接、压平子结点和线性 PEO 验证，失败时只做一次 BFS 构造诱导环。
- 验证：5000 张随机图逐一检查 PEO 或诱导环证书；`n=20万,m约40万` 时约 `0.18s`，旧有序集合候选约 `0.41s`。
- 待回写：用线性候选更新 5.10.001 的大图证书接口，教学用朴素接口可继续保留。
