# 64. Historical Maximum Value

- QOJ: https://qoj.ac/contest/3936/problem/785
- 状态：性能版 `main.cpp` 已保留用于 QOJ；打印版候选 `template.hpp` 待批准写入正式模板
- 模板条目：3.3.003
- 实现：`SegmentTree` 使用可组合懒标记同时维护当前值与历史峰值，区间接口采用 0 下标左闭右开。
