# QOJ 3936 模板代码

本目录对应 [XXIV 赛前模板训练赛](https://qoj.ac/contest/3936) 的 75 道题。
每题目录中的 `template.hpp` 是从本仓库权威模板依赖展开得到的自包含代码；
存在 `main.cpp` 时，它来自 `library-checker-template-solutions` 中同题型、已验证的独立提交源码。

当前共有 46 道附同题型已验证实现，29 道仅提供算法模板。
`main.cpp` 的输入输出仍以对应 Library Checker 题为准；题目 README 标注了需要适配的差异。

生成命令：`python qoj模板代码/tools/generate.py`

| 题号 | 题目 | QOJ ID | 模板条目 | 内容 |
| --- | --- | ---: | --- | --- |
| 1 | [前缀函数 - KMP](1_%E5%89%8D%E7%BC%80%E5%87%BD%E6%95%B0%20-%20KMP/) | 464 | 4.1.002 | template.hpp |
| 2 | [Z 函数](2_Z%20%E5%87%BD%E6%95%B0/) | 786 | 4.1.003 | main.cpp + template.hpp |
| 3 | [基本子串字典](3_%E5%9F%BA%E6%9C%AC%E5%AD%90%E4%B8%B2%E5%AD%97%E5%85%B8/) | 897 | 4.1.002 | template.hpp |
| 4 | [后缀自动机](4_%E5%90%8E%E7%BC%80%E8%87%AA%E5%8A%A8%E6%9C%BA/) | 217 | 4.3.004 | template.hpp |
| 5 | [广义后缀自动机](5_%E5%B9%BF%E4%B9%89%E5%90%8E%E7%BC%80%E8%87%AA%E5%8A%A8%E6%9C%BA/) | 266 | 4.3.005 | template.hpp |
| 6 | [回文自动机](6_%E5%9B%9E%E6%96%87%E8%87%AA%E5%8A%A8%E6%9C%BA/) | 801 | 4.4.001 | main.cpp + template.hpp |
| 7 | [后缀排序](7_%E5%90%8E%E7%BC%80%E6%8E%92%E5%BA%8F/) | 956 | 4.3.002 | main.cpp + template.hpp |
| 8 | [Manacher 算法](8_Manacher%20%E7%AE%97%E6%B3%95/) | 787 | 4.1.004 | main.cpp + template.hpp |
| 9 | [区间本质不同子串](9_%E5%8C%BA%E9%97%B4%E6%9C%AC%E8%B4%A8%E4%B8%8D%E5%90%8C%E5%AD%90%E4%B8%B2/) | 13268 | 4.3.004 | template.hpp |
| A | [字符串匹配问题](A_%E5%AD%97%E7%AC%A6%E4%B8%B2%E5%8C%B9%E9%85%8D%E9%97%AE%E9%A2%98/) | 552 | 4.1.011 | main.cpp + template.hpp |
| B | [Runs](B_Runs/) | 21648 | 4.1.007 | main.cpp + template.hpp |
| C | [Lyndon Factorization](C_Lyndon%20Factorization/) | 498 | 4.1.006 | main.cpp + template.hpp |
| 10 | [最小生成树](10_%E6%9C%80%E5%B0%8F%E7%94%9F%E6%88%90%E6%A0%91/) | 21979 | 5.5.001 | main.cpp + template.hpp |
| 11 | [桥](11_%E6%A1%A5/) | 995 | 5.3.004 | template.hpp |
| 12 | [割点](12_%E5%89%B2%E7%82%B9/) | 996 | 5.3.005 | template.hpp |
| 13 | [强连通分量](13_%E5%BC%BA%E8%BF%9E%E9%80%9A%E5%88%86%E9%87%8F/) | 906 | 5.3.001 | main.cpp + template.hpp |
| 14 | [最大流 1](14_%E6%9C%80%E5%A4%A7%E6%B5%81%201/) | 456 | 5.8.001 | template.hpp |
| 14X | [最大流 2](14X_%E6%9C%80%E5%A4%A7%E6%B5%81%202/) | 457 | 5.8.010 | template.hpp |
| 15 | [最小费用最大流 - 随机数据](15_%E6%9C%80%E5%B0%8F%E8%B4%B9%E7%94%A8%E6%9C%80%E5%A4%A7%E6%B5%81%20-%20%E9%9A%8F%E6%9C%BA%E6%95%B0%E6%8D%AE/) | 602 | 5.8.004 | template.hpp |
| 16a | [最短路 1](16a_%E6%9C%80%E7%9F%AD%E8%B7%AF%201/) | 1445 | 5.2.001 | main.cpp + template.hpp |
| 16b | [最短路 2](16b_%E6%9C%80%E7%9F%AD%E8%B7%AF%202/) | 1446 | 5.2.002 | template.hpp |
| 17 | [支配树](17_%E6%94%AF%E9%85%8D%E6%A0%91/) | 788 | 5.10.003 | main.cpp + template.hpp |
| 18 | [二分图最大匹配](18_%E4%BA%8C%E5%88%86%E5%9B%BE%E6%9C%80%E5%A4%A7%E5%8C%B9%E9%85%8D/) | 898 | 5.6.003 | main.cpp + template.hpp |
| 19 | [一般图最大匹配](19_%E4%B8%80%E8%88%AC%E5%9B%BE%E6%9C%80%E5%A4%A7%E5%8C%B9%E9%85%8D/) | 899 | 5.7.001 | main.cpp + template.hpp |
| 1A | [一般图最大权匹配](1A_%E4%B8%80%E8%88%AC%E5%9B%BE%E6%9C%80%E5%A4%A7%E6%9D%83%E5%8C%B9%E9%85%8D/) | 900 | 5.7.006 | main.cpp + template.hpp |
| 1B | [最小树形图](1B_%E6%9C%80%E5%B0%8F%E6%A0%91%E5%BD%A2%E5%9B%BE/) | 904 | 5.5.008 | main.cpp + template.hpp |
| 1C | [2-SAT 问题](1C_2-SAT%20%E9%97%AE%E9%A2%98/) | 997 | 5.4.001 | main.cpp + template.hpp |
| 1D | [正则二分图匹配](1D_%E6%AD%A3%E5%88%99%E4%BA%8C%E5%88%86%E5%9B%BE%E5%8C%B9%E9%85%8D/) | 265 | 5.6.003 | template.hpp |
| 1E | [边双连通分量](1E_%E8%BE%B9%E5%8F%8C%E8%BF%9E%E9%80%9A%E5%88%86%E9%87%8F/) | 999 | 5.3.004 | main.cpp + template.hpp |
| 1F | [边三连通分量](1F_%E8%BE%B9%E4%B8%89%E8%BF%9E%E9%80%9A%E5%88%86%E9%87%8F/) | 1000 | 5.3.013 | main.cpp + template.hpp |
| 1G | [三元环枚举](1G_%E4%B8%89%E5%85%83%E7%8E%AF%E6%9E%9A%E4%B8%BE/) | 905 | 5.9.006 | main.cpp + template.hpp |
| 1H | [有源汇有上下界最小费用最大流 - 随机数据](1H_%E6%9C%89%E6%BA%90%E6%B1%87%E6%9C%89%E4%B8%8A%E4%B8%8B%E7%95%8C%E6%9C%80%E5%B0%8F%E8%B4%B9%E7%94%A8%E6%9C%80%E5%A4%A7%E6%B5%81%20-%20%E9%9A%8F%E6%9C%BA%E6%95%B0%E6%8D%AE/) | 2021 | 5.8.011 | template.hpp |
| 1I | [弦图判定](1I_%E5%BC%A6%E5%9B%BE%E5%88%A4%E5%AE%9A/) | 21755 | 5.10.001 | main.cpp + template.hpp |
| 20 | [多项式乘法](20_%E5%A4%9A%E9%A1%B9%E5%BC%8F%E4%B9%98%E6%B3%95/) | 618 | 10.1.003 | main.cpp + template.hpp |
| 21 | [多项式求逆](21_%E5%A4%9A%E9%A1%B9%E5%BC%8F%E6%B1%82%E9%80%86/) | 619 | 10.2.003 | main.cpp + template.hpp |
| 22 | [多项式对数函数](22_%E5%A4%9A%E9%A1%B9%E5%BC%8F%E5%AF%B9%E6%95%B0%E5%87%BD%E6%95%B0/) | 620 | 10.2.005 | main.cpp + template.hpp |
| 23 | [多项式指数函数](23_%E5%A4%9A%E9%A1%B9%E5%BC%8F%E6%8C%87%E6%95%B0%E5%87%BD%E6%95%B0/) | 621 | 10.2.005 | main.cpp + template.hpp |
| 24 | [多项式多点求值](24_%E5%A4%9A%E9%A1%B9%E5%BC%8F%E5%A4%9A%E7%82%B9%E6%B1%82%E5%80%BC/) | 622 | 10.2.007 | main.cpp + template.hpp |
| 25 | [多项式复合](25_%E5%A4%9A%E9%A1%B9%E5%BC%8F%E5%A4%8D%E5%90%88/) | 1097 | 10.2.012 | main.cpp + template.hpp |
| 26 | [多项式复合逆](26_%E5%A4%9A%E9%A1%B9%E5%BC%8F%E5%A4%8D%E5%90%88%E9%80%86/) | 1098 | 10.3.005 | main.cpp + template.hpp |
| 27 | [常系数齐次线性递推](27_%E5%B8%B8%E7%B3%BB%E6%95%B0%E9%BD%90%E6%AC%A1%E7%BA%BF%E6%80%A7%E9%80%92%E6%8E%A8/) | 146 | 10.4.005 | main.cpp + template.hpp |
| 40a | [Primality Test](40a_Primality%20Test/) | 249 | 7.2.003 | main.cpp + template.hpp |
| 40b | [Prime Factorization](40b_Prime%20Factorization/) | 458 | 7.2.004 | main.cpp + template.hpp |
| 41 | [类欧几里得算法](41_%E7%B1%BB%E6%AC%A7%E5%87%A0%E9%87%8C%E5%BE%97%E7%AE%97%E6%B3%95/) | 21978 | 7.5.003 | template.hpp |
| 42 | [BM 算法](42_BM%20%E7%AE%97%E6%B3%95/) | 547 | 10.4.001 | main.cpp + template.hpp |
| 43 | [特征多项式](43_%E7%89%B9%E5%BE%81%E5%A4%9A%E9%A1%B9%E5%BC%8F/) | 603 | 9.1.007 | main.cpp + template.hpp |
| 44 | [Tutte 多项式](44_Tutte%20%E5%A4%9A%E9%A1%B9%E5%BC%8F/) | 45 | 5.10.010 | template.hpp |
| 45 | [Nim Product](45_Nim%20Product/) | 56 | 13.1.009 | main.cpp + template.hpp |
| 46 | [Online Multiplicative Inverse Query](46_Online%20Multiplicative%20Inverse%20Query/) | 5 | 7.1.001 | template.hpp |
| 48 | [Primitive root - 原根](48_Primitive%20root%20-%20%E5%8E%9F%E6%A0%B9/) | 144 | 7.4.003 | main.cpp + template.hpp |
| 49 | [Min_25 筛](49_Min_25%20%E7%AD%9B/) | 21405 | 7.3.007 | template.hpp |
| 4A | [Determinant of A+Bz](4A_Determinant%20of%20A%2BBz/) | 59 | 9.1.007 | template.hpp |
| 4B | [行列式](4B_%E8%A1%8C%E5%88%97%E5%BC%8F/) | 21608 | 9.1.006 | main.cpp + template.hpp |
| 4C | [Equation Mod 2](4C_Equation%20Mod%202/) | 428 | 9.2.001 | main.cpp + template.hpp |
| 4D | [Equation Mod 3](4D_Equation%20Mod%203/) | 695 | 9.2.001 | template.hpp |
| 51 | [四维偏序](51_%E5%9B%9B%E7%BB%B4%E5%81%8F%E5%BA%8F/) | 21566 | 2.2.005 | template.hpp |
| 52 | [平面最近点对](52_%E5%B9%B3%E9%9D%A2%E6%9C%80%E8%BF%91%E7%82%B9%E5%AF%B9/) | 142 | 11.5.001 | main.cpp + template.hpp |
| 60 | [动态树 - 简单版](60_%E5%8A%A8%E6%80%81%E6%A0%91%20-%20%E7%AE%80%E5%8D%95%E7%89%88/) | 21529 | 6.4.001 | template.hpp |
| 61 | [点分治](61_%E7%82%B9%E5%88%86%E6%B2%BB/) | 21959 | 6.2.001 | template.hpp |
| 62 | [文艺平衡树](62_%E6%96%87%E8%89%BA%E5%B9%B3%E8%A1%A1%E6%A0%91/) | 21403 | 3.5.004 | main.cpp + template.hpp |
| 63 | [李超树](63_%E6%9D%8E%E8%B6%85%E6%A0%91/) | 601 | 3.7.010 | main.cpp + template.hpp |
| 64 | [Historical Maximum Value](64_Historical%20Maximum%20Value/) | 785 | 3.3.003 | template.hpp |
| 80 | [Convex Checker](80_Convex%20Checker/) | 7730 | 11.3.001 | template.hpp |
| 81 | [极角排序](81_%E6%9E%81%E8%A7%92%E6%8E%92%E5%BA%8F/) | 10259 | 11.1.004 | main.cpp + template.hpp |
| 82 | [二维凸包](82_%E4%BA%8C%E7%BB%B4%E5%87%B8%E5%8C%85/) | 218 | 11.3.001 | main.cpp + template.hpp |
| 83 | [旋转卡壳](83_%E6%97%8B%E8%BD%AC%E5%8D%A1%E5%A3%B3/) | 784 | 11.3.006 | main.cpp + template.hpp |
| 84 | [点到线段距离](84_%E7%82%B9%E5%88%B0%E7%BA%BF%E6%AE%B5%E8%B7%9D%E7%A6%BB/) | 15283 | 11.1.008 | template.hpp |
| 85 | [线段到线段距离](85_%E7%BA%BF%E6%AE%B5%E5%88%B0%E7%BA%BF%E6%AE%B5%E8%B7%9D%E7%A6%BB/) | 15284 | 11.1.009 | template.hpp |
| 90 | [快速 OR 卷积](90_%E5%BF%AB%E9%80%9F%20OR%20%E5%8D%B7%E7%A7%AF/) | 1002 | 10.1.006 | template.hpp |
| 91 | [快速 AND 卷积](91_%E5%BF%AB%E9%80%9F%20AND%20%E5%8D%B7%E7%A7%AF/) | 1003 | 10.1.006 | main.cpp + template.hpp |
| 92 | [快速 XOR 卷积](92_%E5%BF%AB%E9%80%9F%20XOR%20%E5%8D%B7%E7%A7%AF/) | 1004 | 10.1.006 | main.cpp + template.hpp |
| 93 | [子集卷积](93_%E5%AD%90%E9%9B%86%E5%8D%B7%E7%A7%AF/) | 1005 | 10.1.011 | main.cpp + template.hpp |
| 94 | [稀疏 XOR 卷积](94_%E7%A8%80%E7%96%8F%20XOR%20%E5%8D%B7%E7%A7%AF/) | 85 | 10.1.006 | template.hpp |
| A0 | [树 Hash](A0_%E6%A0%91%20Hash/) | 499 | 6.1.008 | main.cpp + template.hpp |
| EX1 | [Kth 1](EX1_Kth%201/) | 15001 | 7.1.001 | template.hpp |
