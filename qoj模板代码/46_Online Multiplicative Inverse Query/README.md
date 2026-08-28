# 46. Online Multiplicative Inverse Query

- QOJ: https://qoj.ac/contest/3936/problem/5
- 状态：函数接口题：按题面实现 init(p) 与 inv(x)，不提供 main.cpp；暂无提交记录
- 模板条目：7.1.001
- 备注：交互题只允许实现 init(p) 与 inv(x)，不要提交 main。
- 候选新增：固定模数 `998244353` 的分数树索引与线性递推逆元表，`inv(x)` 为摊还 O(1)。
- `init(p)` 只接受题面固定模数；候选实现保留 16/32 位表项以控制约 20 MiB 常驻表空间。
