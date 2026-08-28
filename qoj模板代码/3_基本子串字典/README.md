# 3. 基本子串字典

- QOJ: https://qoj.ac/contest/3936/problem/897
- 状态：性能版 `main.cpp` 已保留用于 QOJ；打印版候选 `template.hpp` 待批准写入正式模板
- 模板条目：4.3.011
- 实现：`basicDict(s, queryList)` 批量查询 0 下标闭区间的真 border 统计；内部采用 `k.cpp` 的离线后缀数组倍增与等差数列交集。
