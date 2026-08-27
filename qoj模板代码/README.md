# QOJ 3936 模板代码

本目录对应 [XXIV 赛前模板训练赛](https://qoj.ac/contest/3936) 的 75 道题。
每题目录中的 `template.hpp` 是从本仓库权威模板依赖展开得到的自包含代码；
存在 `main.cpp` 时，它来自 `library-checker-template-solutions` 中同题型、已验证的独立提交源码。

当前共有 46 道附同题型已验证实现，29 道仅提供算法模板。
`main.cpp` 的输入输出仍以对应 Library Checker 题为准；题目 README 标注了需要适配的差异。

生成命令：`python qoj模板代码/tools/generate.py`

| 题号 | 题目 | QOJ ID | 模板条目 | 内容 |
| --- | --- | ---: | --- | --- |
| 1 | [前缀函数 - KMP](1_前缀函数 - KMP/) | 464 | 4.1.002 | template.hpp |
| 2 | [Z 函数](2_Z 函数/) | 786 | 4.1.003 | main.cpp + template.hpp |
| 3 | [基本子串字典](3_基本子串字典/) | 897 | 4.1.002 | template.hpp |
| 4 | [后缀自动机](4_后缀自动机/) | 217 | 4.3.004 | template.hpp |
| 5 | [广义后缀自动机](5_广义后缀自动机/) | 266 | 4.3.005 | template.hpp |
| 6 | [回文自动机](6_回文自动机/) | 801 | 4.4.001 | main.cpp + template.hpp |
| 7 | [后缀排序](7_后缀排序/) | 956 | 4.3.002 | main.cpp + template.hpp |
| 8 | [Manacher 算法](8_Manacher 算法/) | 787 | 4.1.004 | main.cpp + template.hpp |
| 9 | [区间本质不同子串](9_区间本质不同子串/) | 13268 | 4.3.004 | template.hpp |
| A | [字符串匹配问题](A_字符串匹配问题/) | 552 | 4.1.011 | main.cpp + template.hpp |
| B | [Runs](B_Runs/) | 21648 | 4.1.007 | main.cpp + template.hpp |
| C | [Lyndon Factorization](C_Lyndon Factorization/) | 498 | 4.1.006 | main.cpp + template.hpp |
| 10 | [最小生成树](10_最小生成树/) | 21979 | 5.5.001 | main.cpp + template.hpp |
| 11 | [桥](11_桥/) | 995 | 5.3.004 | template.hpp |
| 12 | [割点](12_割点/) | 996 | 5.3.005 | template.hpp |
| 13 | [强连通分量](13_强连通分量/) | 906 | 5.3.001 | main.cpp + template.hpp |
| 14 | [最大流 1](14_最大流 1/) | 456 | 5.8.001 | template.hpp |
| 14X | [最大流 2](14X_最大流 2/) | 457 | 5.8.010 | template.hpp |
| 15 | [最小费用最大流 - 随机数据](15_最小费用最大流 - 随机数据/) | 602 | 5.8.004 | template.hpp |
| 16a | [最短路 1](16a_最短路 1/) | 1445 | 5.2.001 | main.cpp + template.hpp |
| 16b | [最短路 2](16b_最短路 2/) | 1446 | 5.2.002 | template.hpp |
| 17 | [支配树](17_支配树/) | 788 | 5.10.003 | main.cpp + template.hpp |
| 18 | [二分图最大匹配](18_二分图最大匹配/) | 898 | 5.6.003 | main.cpp + template.hpp |
| 19 | [一般图最大匹配](19_一般图最大匹配/) | 899 | 5.7.001 | main.cpp + template.hpp |
| 1A | [一般图最大权匹配](1A_一般图最大权匹配/) | 900 | 5.7.006 | main.cpp + template.hpp |
| 1B | [最小树形图](1B_最小树形图/) | 904 | 5.5.008 | main.cpp + template.hpp |
| 1C | [2-SAT 问题](1C_2-SAT 问题/) | 997 | 5.4.001 | main.cpp + template.hpp |
| 1D | [正则二分图匹配](1D_正则二分图匹配/) | 265 | 5.6.003 | template.hpp |
| 1E | [边双连通分量](1E_边双连通分量/) | 999 | 5.3.004 | main.cpp + template.hpp |
| 1F | [边三连通分量](1F_边三连通分量/) | 1000 | 5.3.013 | main.cpp + template.hpp |
| 1G | [三元环枚举](1G_三元环枚举/) | 905 | 5.9.006 | main.cpp + template.hpp |
| 1H | [有源汇有上下界最小费用最大流 - 随机数据](1H_有源汇有上下界最小费用最大流 - 随机数据/) | 2021 | 5.8.011 | template.hpp |
| 1I | [弦图判定](1I_弦图判定/) | 21755 | 5.10.001 | main.cpp + template.hpp |
| 20 | [多项式乘法](20_多项式乘法/) | 618 | 10.1.003 | main.cpp + template.hpp |
| 21 | [多项式求逆](21_多项式求逆/) | 619 | 10.2.003 | main.cpp + template.hpp |
| 22 | [多项式对数函数](22_多项式对数函数/) | 620 | 10.2.005 | main.cpp + template.hpp |
| 23 | [多项式指数函数](23_多项式指数函数/) | 621 | 10.2.005 | main.cpp + template.hpp |
| 24 | [多项式多点求值](24_多项式多点求值/) | 622 | 10.2.007 | main.cpp + template.hpp |
| 25 | [多项式复合](25_多项式复合/) | 1097 | 10.2.012 | main.cpp + template.hpp |
| 26 | [多项式复合逆](26_多项式复合逆/) | 1098 | 10.3.005 | main.cpp + template.hpp |
| 27 | [常系数齐次线性递推](27_常系数齐次线性递推/) | 146 | 10.4.005 | main.cpp + template.hpp |
| 40a | [Primality Test](40a_Primality Test/) | 249 | 7.2.003 | main.cpp + template.hpp |
| 40b | [Prime Factorization](40b_Prime Factorization/) | 458 | 7.2.004 | main.cpp + template.hpp |
| 41 | [类欧几里得算法](41_类欧几里得算法/) | 21978 | 7.5.003 | template.hpp |
| 42 | [BM 算法](42_BM 算法/) | 547 | 10.4.001 | main.cpp + template.hpp |
| 43 | [特征多项式](43_特征多项式/) | 603 | 9.1.007 | main.cpp + template.hpp |
| 44 | [Tutte 多项式](44_Tutte 多项式/) | 45 | 5.10.010 | template.hpp |
| 45 | [Nim Product](45_Nim Product/) | 56 | 13.1.009 | main.cpp + template.hpp |
| 46 | [Online Multiplicative Inverse Query](46_Online Multiplicative Inverse Query/) | 5 | 7.1.001 | template.hpp |
| 48 | [Primitive root - 原根](48_Primitive root - 原根/) | 144 | 7.4.003 | main.cpp + template.hpp |
| 49 | [Min_25 筛](49_Min_25 筛/) | 21405 | 7.3.007 | template.hpp |
| 4A | [Determinant of A+Bz](4A_Determinant of A+Bz/) | 59 | 9.1.007 | template.hpp |
| 4B | [行列式](4B_行列式/) | 21608 | 9.1.006 | main.cpp + template.hpp |
| 4C | [Equation Mod 2](4C_Equation Mod 2/) | 428 | 9.2.001 | main.cpp + template.hpp |
| 4D | [Equation Mod 3](4D_Equation Mod 3/) | 695 | 9.2.001 | template.hpp |
| 51 | [四维偏序](51_四维偏序/) | 21566 | 2.2.005 | template.hpp |
| 52 | [平面最近点对](52_平面最近点对/) | 142 | 11.5.001 | main.cpp + template.hpp |
| 60 | [动态树 - 简单版](60_动态树 - 简单版/) | 21529 | 6.4.001 | template.hpp |
| 61 | [点分治](61_点分治/) | 21959 | 6.2.001 | template.hpp |
| 62 | [文艺平衡树](62_文艺平衡树/) | 21403 | 3.5.004 | main.cpp + template.hpp |
| 63 | [李超树](63_李超树/) | 601 | 3.7.010 | main.cpp + template.hpp |
| 64 | [Historical Maximum Value](64_Historical Maximum Value/) | 785 | 3.3.003 | template.hpp |
| 80 | [Convex Checker](80_Convex Checker/) | 7730 | 11.3.001 | template.hpp |
| 81 | [极角排序](81_极角排序/) | 10259 | 11.1.004 | main.cpp + template.hpp |
| 82 | [二维凸包](82_二维凸包/) | 218 | 11.3.001 | main.cpp + template.hpp |
| 83 | [旋转卡壳](83_旋转卡壳/) | 784 | 11.3.006 | main.cpp + template.hpp |
| 84 | [点到线段距离](84_点到线段距离/) | 15283 | 11.1.008 | template.hpp |
| 85 | [线段到线段距离](85_线段到线段距离/) | 15284 | 11.1.009 | template.hpp |
| 90 | [快速 OR 卷积](90_快速 OR 卷积/) | 1002 | 10.1.006 | template.hpp |
| 91 | [快速 AND 卷积](91_快速 AND 卷积/) | 1003 | 10.1.006 | main.cpp + template.hpp |
| 92 | [快速 XOR 卷积](92_快速 XOR 卷积/) | 1004 | 10.1.006 | main.cpp + template.hpp |
| 93 | [子集卷积](93_子集卷积/) | 1005 | 10.1.011 | main.cpp + template.hpp |
| 94 | [稀疏 XOR 卷积](94_稀疏 XOR 卷积/) | 85 | 10.1.006 | template.hpp |
| A0 | [树 Hash](A0_树 Hash/) | 499 | 6.1.008 | main.cpp + template.hpp |
| EX1 | [Kth 1](EX1_Kth 1/) | 15001 | 7.1.001 | template.hpp |
