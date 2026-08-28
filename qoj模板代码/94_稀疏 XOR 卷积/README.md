# 94. 稀疏 XOR 卷积

- QOJ: https://qoj.ac/contest/3936/problem/85
- 状态：候选 `template.hpp` 已提供，`main.cpp` 已完全展开并按 QOJ 题面适配，等待提交验证后再写回正式模板
- 模板条目：10.1.006
- 备注：普通 FWT 是基础；QOJ 约束需要利用每个因子的稀疏支持。
- 候选新增：`sparseXorConvolution(n, factors)`，按 QOJ 稀疏因子格式返回长度 `2^n` 的结果。
- 算法：局部 Walsh 变换 + Cipolla 平方根分解，将乘法/除法和零值编码进 `SparseXorInfo`，再做全局 XOR 逆变换。
- 模数固定为 `1000000000000125953`；乘积使用 `unsigned __int128`。
- 复杂度：设最大因子维数为 `k`，为 `O(2^n n + sum 2^d d)` 次域运算，额外空间 `O(2^n + sum 2^d)`。
