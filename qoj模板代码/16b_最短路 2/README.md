# 16b. 最短路 2

- QOJ: https://qoj.ac/contest/3936/problem/1446
- 状态：候选 `template.hpp` 已提供，`main.cpp` 已完全展开并按 QOJ 题面适配，等待提交验证后再写回正式模板
- 模板条目：5.2.002
- 算法：维护可更新的最短路树；发现树上回边时标记负环及其所有可达后继。
- 接口：`shortestPathWithNegInf(n, edges, source)`，`NegEdge` 端点为 0 下标。
- 返回：`dist` 的空值表示不可达，`negInf[v]` 表示答案为负无穷。
- 复杂度：参考实现为队列式增量松弛，实际数据较快但最坏复杂度不作多项式保证；需要严格上界时使用正式模板的 Bellman-Ford。
