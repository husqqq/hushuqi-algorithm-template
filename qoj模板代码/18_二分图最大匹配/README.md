# 18. 二分图最大匹配

- QOJ: https://qoj.ac/contest/3936/problem/898
- 状态：main.cpp 是标准 Hopcroft-Karp；random.cpp 是封装后的立即增广版本，随机数据下通常更快
- 模板条目：5.6.003
- 说明：random.cpp 只吸收 k.cpp 的多源 BFS 与立即增广思路，不保留其高速 I/O、全局定长数组和编译器属性；最坏复杂度不如 Hopcroft-Karp。
