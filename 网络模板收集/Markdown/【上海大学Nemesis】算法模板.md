# 【上海大学Nemesis】算法模板

> 由 `【上海大学Nemesis】算法模板.pdf` 的内置文本层直接提取；未使用 OCR。

<!-- page: 1 -->

                            ICPC World Finals 2024
                            Standard Code Library


Nemesis



 Coach                             教练
 Yong Yu                            俞勇
 Siyu Sun                         孙司宇
 Boren Tan                        谭博仁
 Shangfei Yang                    杨尚霏


 Contestant                        队员
 Zonghan Yang                     杨宗翰
 Mingchi Zhang                    张明驰
 Jianjun Zhang                    张建军


Last Commit: Sep 02, 2024 (8a779a: change main.pdf)

---

<!-- page: 2 -->

Contents                                                                          4.7    SAMSA & 后缀树 . . . . . . . . . . . . . . . . . . . . . . .          13
                                                                                  4.8    后缀数组 . . . . . . . . . . . . . . . . . . . . . . . . . . . .       13
                                                                                  4.9    Suffix Balanced Tree 后缀平衡树 . . . . . . . . . . . . . . .           13
1 Geometry                                                                 2      4.10   广义在线 SAM . . . . . . . . . . . . . . . . . . . . . . . . .         13
  1.1    二维几何基础操作 . . . . . . . . . . . . . . . . . . . . . . .            2      4.11   回文树 . . . . . . . . . . . . . . . . . . . . . . . . . . . . .      14
  1.2    整数半平面交 . . . . . . . . . . . . . . . . . . . . . . . . . .        2      4.12   Runs . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .   14
  1.3    凸包询问：凸包内、切点、交点、最近点 . . . . . . . . . . . .                        3      4.13   字符串 Hash . . . . . . . . . . . . . . . . . . . . . . . . . .       14
  1.4    点、线段在简单多边形内 . . . . . . . . . . . . . . . . . . . .               3      4.14   String Conclusions . . . . . . . . . . . . . . . . . . . . . .     14
  1.5    𝑂 (𝑛 2 ) Ear Clipping 三角剖分 . . . . . . . . . . . . . . . . .      3
  1.6
  1.7
         单插入动态凸包 . . . . . . . . . . . . . . . . . . . . . . . . .
         圆 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
                                                                           3
                                                                           4    5 Math 数学                                                                   15
  1.8    圆反演，阿波罗尼茨圆 . . . . . . . . . . . . . . . . . . . . .              4      5.1    Long Long 𝑂 (1) 乘, Barrett . . . . . . . . . . . . . . . . . .     15
  1.9    圆并 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .    4      5.2    exgcd, 逆元 . . . . . . . . . . . . . . . . . . . . . . . . . .      15
  1.10   多边形与圆交 . . . . . . . . . . . . . . . . . . . . . . . . . .        4      5.3    CRT 中国剩余定理 . . . . . . . . . . . . . . . . . . . . . . .           15
  1.11   球面基础, 经纬度球面距离 . . . . . . . . . . . . . . . . . . .               5      5.4    Miller Rabin, Pollard Rho . . . . . . . . . . . . . . . . . . .    15
  1.12   圆上整点 . . . . . . . . . . . . . . . . . . . . . . . . . . . .      5      5.5    扩展卢卡斯 . . . . . . . . . . . . . . . . . . . . . . . . . . .        15
  1.13   三相之力 . . . . . . . . . . . . . . . . . . . . . . . . . . . .      5      5.6    阶乘取模 . . . . . . . . . . . . . . . . . . . . . . . . . . . .       15
  1.14   相关公式 . . . . . . . . . . . . . . . . . . . . . . . . . . . .      5      5.7    类欧几里得直线下格点统计 . . . . . . . . . . . . . . . . . . .                 15
              Heron’s Formula . . . . . . . . . . . . . . . . . .
         1.14.1                                                            5      5.8    万能欧几里德 . . . . . . . . . . . . . . . . . . . . . . . . . .         15
              四面体内接球球心 . . . . . . . . . . . . . . . . . .
         1.14.2                                                            5      5.9    平方剩余 . . . . . . . . . . . . . . . . . . . . . . . . . . . .       16
              三角形内心 . . . . . . . . . . . . . . . . . . . . . .
         1.14.3                                                            5      5.10   线性同余不等式 . . . . . . . . . . . . . . . . . . . . . . . . .          16
              三角形外心 . . . . . . . . . . . . . . . . . . . . . .
         1.14.4                                                            5      5.11   原根 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .     16
              三角形垂心 . . . . . . . . . . . . . . . . . . . . . .
         1.14.5                                                            5      5.12   FFT . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .    16
              三角形偏心 . . . . . . . . . . . . . . . . . . . . . .
         1.14.6                                                            5      5.13   NTT . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .    16
              三角形内接外接圆半径 . . . . . . . . . . . . . . . .
         1.14.7                                                            5      5.14   MTT 任意模数卷积 . . . . . . . . . . . . . . . . . . . . . . .           16
              Pick’s Theorem 格点多边形面积 . . . . . . . . . . .
         1.14.8                                                            5      5.15   多项式运算 . . . . . . . . . . . . . . . . . . . . . . . . . . .        17
              Euler’s Formula 多面体与平面图的点、边、面 . . . .
         1.14.9                                                            5             5.15.1   多项式求逆 开根 ln exp . . . . . . . . . . . . . . . .           17
              三角公式 . . . . . . . . . . . . . . . . . . . . . . .
         1.14.10                                                           5             5.15.2   多项式除法 取模 . . . . . . . . . . . . . . . . . . .            17
              超球坐标系 . . . . . . . . . . . . . . . . . . . . . .
         1.14.11                                                           5             5.15.3   多点求值 . . . . . . . . . . . . . . . . . . . . . . .        17
              三维旋转公式 . . . . . . . . . . . . . . . . . . . . .
         1.14.12                                                           5             5.15.4   插值 . . . . . . . . . . . . . . . . . . . . . . . . .      17
              立体角公式 . . . . . . . . . . . . . . . . . . . . . .
         1.14.13                                                           5      5.16   线性递推 . . . . . . . . . . . . . . . . . . . . . . . . . . . .       17
              常用体积公式 . . . . . . . . . . . . . . . . . . . . .
         1.14.14                                                           5      5.17   Berlekamp-Massey 最小多项式 . . . . . . . . . . . . . . . .             18
              扇形与圆弧重心 . . . . . . . . . . . . . . . . . . .
         1.14.15                                                           5      5.18   FWT . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .    19
              高维球体积 . . . . . . . . . . . . . . . . . . . . . .
         1.14.16                                                           6      5.19   K 进制 FWT . . . . . . . . . . . . . . . . . . . . . . . . . .       19
  1.15   三维几何基础操作 . . . . . . . . . . . . . . . . . . . . . . .            6      5.20   Simplex 单纯形 . . . . . . . . . . . . . . . . . . . . . . . .        19
  1.16   三维凸包 . . . . . . . . . . . . . . . . . . . . . . . . . . . .      6      5.21   高斯消元最小范数解 . . . . . . . . . . . . . . . . . . . . . .              19
  1.17   最小覆盖球 . . . . . . . . . . . . . . . . . . . . . . . . . . .       6      5.22   Pell 方程 . . . . . . . . . . . . . . . . . . . . . . . . . . . .    19
                                                                                  5.23   解一元三次方程 . . . . . . . . . . . . . . . . . . . . . . . . .          19
                                                                                  5.24   自适应 Simpson . . . . . . . . . . . . . . . . . . . . . . . .        20
2 Tree & Graph             √
                                                                           6
         Hopcroft-Karp 𝑂 ( 𝑉 𝐸) . . . . . . . . . . . . . . . . . . .
  2.1
  2.2    Hungarian 𝑂 (𝑉 𝐸/𝑤) . . . . . . . . . . . . . . . . . . . . .
                                                                            6
                                                                            7
                                                                                6 Appendix                                                                  20
                                                                                  6.1    Formulas 公式表 . . . . . . . . . . . . . . . . . . . . . . . .       20
  2.3    Shuffle 一般图最大匹配 𝑂 (𝑉 𝐸) . . . . . . . . . . . . . . . .            7            6.1.1    Mobius Inversion . . . . . . . . . . . . . . . . . .      20
  2.4    极大团计数 . . . . . . . . . . . . . . . . . . . . . . . . . . .        7            6.1.2    杜教筛 . . . . . . . . . . . . . . . . . . . . . . . .       20
  2.5    KM 最大权匹配 𝑂 (𝑉 3 ) . . . . . . . . . . . . . . . . . . . .          7            6.1.3    降幂公式 . . . . . . . . . . . . . . . . . . . . . . .        20
  2.6    欧拉回路 . . . . . . . . . . . . . . . . . . . . . . . . . . . .       7            6.1.4    其他常用公式 . . . . . . . . . . . . . . . . . . . . .          20
  2.7    2-SAT, 强连通分量 / Bitset Kosaraju . . . . . . . . . . . . . .         7            6.1.5    单位根反演 . . . . . . . . . . . . . . . . . . . . . .         20
  2.8    Tarjan 点双，边双 . . . . . . . . . . . . . . . . . . . . . . .         8            6.1.6    Arithmetic Function . . . . . . . . . . . . . . . . .     20
  2.9    Dominator Tree 支配树 . . . . . . . . . . . . . . . . . . . .         8            6.1.7    Binomial Coefficients . . . . . . . . . . . . . . . .     20
  2.10   Dinic 最大流 . . . . . . . . . . . . . . . . . . . . . . . . . .      8            6.1.8    Fibonacci Numbers, Lucas Numbers . . . . . . . .          20
  2.11   原始对偶费用流 . . . . . . . . . . . . . . . . . . . . . . . . .          8            6.1.9    Sum of Powers . . . . . . . . . . . . . . . . . . .       21
  2.12   虚树 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .     9            6.1.10   Catalan Numbers 1, 1, 2, 5, 14, 42, 132, 429, 1430… .     21
  2.13   网络流总结 . . . . . . . . . . . . . . . . . . . . . . . . . . .        9            6.1.11   Motzkin Numbers 1, 1, 2, 4, 9, 21, 51, 127, 323, 835…     21
                                                                                         6.1.12   Derangement 错排数 0, 1, 2, 9, 44, 265, 1854, 14833…         21
  2.14   Gomory-Hu 无向图最小割树 𝑂 (𝑉 3 𝐸) . . . . . . . . . . . . .              9            6.1.13   Bell Numbers 1, 1, 2, 5, 15, 52, 203, 877, 4140 … . . .   21
  2.15   Stoer-Wagner 无向图最小割 𝑂 (𝑉 𝐸 + 𝑉 2 log 𝑉 ) . . . . . . . .           9            6.1.14   Stirling Numbers . . . . . . . . . . . . . . . . . .      21
  2.16   弦图 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .     9            6.1.15   Eulerian Numbers . . . . . . . . . . . . . . . . . .      21
  2.17   Minimum Mean Cycle 最小平均值环 𝑂 (𝑛 2 ) . . . . . . . . . .            10            6.1.16   Harmonic Numbers, 1, 3/2, 11/6, 25/12, 137/60… . .        21
  2.18   一般图最大匹配 - Blossom . . . . . . . . . . . . . . . . . . .           10            6.1.17   卡迈克尔函数 . . . . . . . . . . . . . . . . . . . . .          21
  2.19   图论结论 . . . . . . . . . . . . . . . . . . . . . . . . . . . .      10            6.1.18   求拆分数 . . . . . . . . . . . . . . . . . . . . . . .        21
         2.19.1   最小乘积问题原理 . . . . . . . . . . . . . . . . . .             10            6.1.19   Bernoulli Numbers 1, 1/2, 1/6, 0, -1/30, 0, 1/42 … . .    21
         2.19.2   最小环 . . . . . . . . . . . . . . . . . . . . . . . .      10            6.1.20   kMAX-MIN反演 . . . . . . . . . . . . . . . . . . .          21
         2.19.3   度序列的可图性 . . . . . . . . . . . . . . . . . . .            10            6.1.21   伍德伯里矩阵不等式 . . . . . . . . . . . . . . . . .               21
         2.19.4   切比雪夫距离与曼哈顿距离转化 . . . . . . . . . . . .                   10            6.1.22   Sum of Squares . . . . . . . . . . . . . . . . . . .      21
         2.19.5   树链的交 . . . . . . . . . . . . . . . . . . . . . . .       10            6.1.23   枚举勾股数 Pythagorean Triple . . . . . . . . . . .            22
         2.19.6   带修改MST . . . . . . . . . . . . . . . . . . . . . .       10            6.1.24   四面体体积 Tetrahedron Volume . . . . . . . . . .              22
         2.19.7   差分约束 . . . . . . . . . . . . . . . . . . . . . . .       10            6.1.25   杨氏矩阵与钩子公式 . . . . . . . . . . . . . . . . .               22
         2.19.8   Segment Tree Beats . . . . . . . . . . . . . . . . .     10            6.1.26   常见博弈游戏 . . . . . . . . . . . . . . . . . . . . .          22
         2.19.9   二分图 . . . . . . . . . . . . . . . . . . . . . . . .      10            6.1.27   概率相关 . . . . . . . . . . . . . . . . . . . . . . .        22
         2.19.10 稳定婚姻问题 . . . . . . . . . . . . . . . . . . . . .          10            6.1.28   邻接矩阵行列式的意义 . . . . . . . . . . . . . . . .                22
         2.19.11 竞赛图 Landau’s Theorem . . . . . . . . . . . . . .          11            6.1.29   Others (某些近似数值公式在这里) . . . . . . . . . .                  22
         2.19.12 Ramsey Theorem R(3,3)=6, R(4,4)=18 . . . . . . . .        11     6.2    Calculus, Integration Table 导数积分表 . . . . . . . . . . . .          22
         2.19.13 树的计数 Prufer序列 . . . . . . . . . . . . . . . . .           11     6.3    Python Hint . . . . . . . . . . . . . . . . . . . . . . . . . .    23
         2.19.14 有根树计数 1,1,2,4,9,20,48,115,286,719,1842,4766 . .           11
         2.19.15 无根树计数 . . . . . . . . . . . . . . . . . . . . . .         11
         2.19.16 生成树计数 Kirchhof’s Matrix-Tree Thoerem . . . .             11   7 Miscellany                                                                23
         2.19.17 有向图欧拉回路计数 BEST Thoerem . . . . . . . . .                  11     7.1    Zeller 日期公式 . . . . . . . . . . . . . . . . . . . . . . . .        23
         2.19.18 Tutte Matrix . . . . . . . . . . . . . . . . . . . . .    11     7.2    有理数二分: Stern-Brocot 树, Farey 序列 . . . . . . . . . . . .            23
         2.19.19 Edmonds Matrix . . . . . . . . . . . . . . . . . . .      11     7.3    黄金三分 . . . . . . . . . . . . . . . . . . . . . . . . . . . .       23
         2.19.20 有向图无环定向, 色多项式 . . . . . . . . . . . . . .                 11     7.4    DP 优化 . . . . . . . . . . . . . . . . . . . . . . . . . . . .      23
         2.19.21 拟阵交问题 . . . . . . . . . . . . . . . . . . . . . .         11            7.4.1    四边形不等式 . . . . . . . . . . . . . . . . . . . . .          23
         2.19.22 双极定向 . . . . . . . . . . . . . . . . . . . . . . .        11            7.4.2    树形背包优化 . . . . . . . . . . . . . . . . . . . . .          23
         2.19.23 图中的环 . . . . . . . . . . . . . . . . . . . . . . .        11            7.4.3   𝑂 (𝑛 · max 𝑎𝑖 ) Subset Sum . . . . . . . . . . . . .       23
                                                                                  7.5    Hash Table . . . . . . . . . . . . . . . . . . . . . . . . . .     24
                                                                                  7.6    基数排序 . . . . . . . . . . . . . . . . . . . . . . . . . . . .       24
3 Data Structure                                                           11     7.7    Hacks: O3, 读入优化, Bitset, builtin . . . . . . . . . . . . . .       24
  3.1    非递归线段树 . . . . . . . . . . . . . . . . . . . . . . . . . .        11     7.8    试机赛与纪律文件 . . . . . . . . . . . . . . . . . . . . . . .             24
         3.1.1 区间加，区间求最大值 . . . . . . . . . . . . . . . .                  11     7.9    Constant Table 常数表 . . . . . . . . . . . . . . . . . . . . .       24
  3.2    点分治 . . . . . . . . . . . . . . . . . . . . . . . . . . . . .     12
  3.3    LCT 动态树 . . . . . . . . . . . . . . . . . . . . . . . . . .       12
  3.4    可持久化 Treap . . . . . . . . . . . . . . . . . . . . . . . . .      12

4 String                                                                   12
  4.1    最小表示法 . . . . . . . . . . . . . . . . . . . . . . . . . . .       12
  4.2    Manacher . . . . . . . . . . . . . . . . . . . . . . . . . . .    12
  4.3    KMP, exKMP . . . . . . . . . . . . . . . . . . . . . . . . .      13
  4.4    AC 自动机 . . . . . . . . . . . . . . . . . . . . . . . . . . .      13
  4.5    Lydon Word Decomposition . . . . . . . . . . . . . . . . .        13
  4.6    后缀自动机 . . . . . . . . . . . . . . . . . . . . . . . . . . .       13

---

<!-- page: 3 -->

     上海交通大学 Shanghai Jiao Tong University                         Nemesis                                                       Page 2

     1. Geometry                                                    40    && sgn(dot(a ‐ b.s, b.t ‐ b.s)) >= 0; }
                                                                    41 bool ray_inter_judge(line a, line b) { // 射线判交
     1.1 二维几何基础操作                                                   42    LD s1, s2; // can be LL
 1 #define cp const point &                                         43    s1 = det(a.t ‐ a.s, b.s ‐ a.s);
 2 int turn (cp a, cp b, cp c) { return sgn(det(b‐a, c‐a)); }       44    s2 = det(a.t ‐ a.s, b.t ‐ a.s);
 3 vector <point> convex_hull (vector <point> a) {                  45    if (sgn(s1) == 0 && sgn(s2) == 0) {
 4    sort (a.begin(), a.end()); // 小于号 (y, x) 字典序                  46       return sgn(dot(a.t ‐ a.s, b.s ‐ a.s)) >= 0
 5    a.erase(unique(a.begin(), a.end()), a.end());//必要时去重          47           || sgn(dot(b.t ‐ b.s, a.s ‐ b.s)) >= 0; }
 6    int n = (int) a.size (), cnt = 0;                             48    if (!sgn(s1 ‐ s2) || sgn(s1) == sgn(s2 ‐ s1)) return 0;
 7    vector <point> ret;                                           49    swap(a, b);
 8    for (int i = 0; i < n; i++) {                                 50    s1 = det(a.t ‐ a.s, b.s ‐ a.s);
 9       while (cnt > 1                                             51    s2 = det(a.t ‐ a.s, b.t ‐ a.s);
10       && turn (ret[cnt ‐ 2], ret[cnt ‐ 1], a[i]) <= 0)           52    return sgn(s1) != sgn(s2 ‐ s1); }
11          ‐‐cnt, ret.pop_back (); // 保留边界:        < 0
12       ++cnt, ret.push_back (a[i]); }                              1   int half(cp a){return a.y > 0||(a.y == 0 && a.x > 0)?1:0;}
13    for (int i = n ‐ 2, fixed = cnt; i >= 0; i‐‐) {                2   bool turn_left(cl a, cl b, cl c) {
14       while (cnt > fixed                                          3     return turn(a.s, a.t, line_inter(b, c)) > 0; }
15       && turn (ret[cnt ‐ 2], ret[cnt ‐ 1], a[i]) <= 0)            4   bool is_para(cl a, cl b){return!sgn(det(a.t‐a.s,b.t‐b.s));}
16          ‐‐cnt, ret.pop_back (); // 所有点共线边界会保留两次                  5   bool cmp(cl a, cl b) {
17       ++cnt, ret.push_back (a[i]); }                              6     int sign = half(a.t ‐ a.s) ‐ half(b.t ‐ b.s);
18    if (n > 1) ret.pop_back (); // n <= 2 吗？                       7     int dir = sgn(det(a.t ‐ a.s, b.t ‐ b.s));
19    return ret; } // 小于号为 (y, x) 时边 [0, 2pi) 逆时针                   8     if (!dir && !sign) return turn(a.s, a.t, b.t) < 0;
                                                                     9     else return sign ? sign > 0 : dir > 0; }
                                                                    10   vector <point> hpi(vector <line> h) { // 半平面交
 1 vector <point> add (vector <point> a, vector <point> b) {
                                                                    11     sort(h.begin(), h.end(), cmp);
 2 // size > 0, rotate(begin, min, end), 无重, 小于号 (y, x)
                                                                    12     vector <line> q(h.size()); int l = 0, r = ‐1;
 3    if (a.size() == 1 || b.size() == 1) {
                                                                    13     for(auto &i : h) {
 4       vector <point> ret;
                                                                    14      while (l < r && !turn_left(i, q[r ‐ 1], q[r])) ‐‐r;
 5       for (auto i : a) for (auto j : b) ret.push_back(i+j);
                                                                    15      while (l < r && !turn_left(i, q[l], q[l + 1])) ++l;
 6       return ret; }
                                                                    16      if (l <= r && is_para(i, q[r])) continue;
 7    vector <point> x, y;
                                                                    17      q[++r] = i; }
 8    for (int i = 0; i < a.size(); i++)
                                                                    18     while (r ‐ l > 1 && !turn_left(q[l], q[r ‐ 1], q[r])) ‐‐r;
 9       x.push_back(a[(i + 1) % a.size()] ‐ a[i]);
                                                                    19     while (r ‐ l > 1 && !turn_left(q[r], q[l], q[l + 1])) ++l;
10    for (int i = 0; i < b.size(); i++)
                                                                    20     if(r ‐ l < 2) return {};
11       y.push_back(b[(i + 1) % b.size()] ‐ b[i]);
                                                                    21     vector <point> ret(r ‐ l + 1);
12    vector <point> ret (x.size() + y.size());
                                                                    22     for(int i = l; i <= r; i++)
13    merge(x.begin(), x.end(), y.begin(), y.end(),
                                                                    23       ret[i ‐ l] = line_inter(q[i], q[i == r ? l : i + 1]);
14         ret.begin(), [](cp u, cp v) {
                                                                    24     return ret; }
15       return half(u)‐half(v) ? half(u) : det(u, v) > 0;});
                                                                    25   // 空集会在队列里留下一个开区域；开区域会被判定为空集。
16    point cur = a[0] + b[0];
                                                                    26   // 为了保证正确性，一定要加足够大的框，尽可能避免零面积区域。
17    for (auto &i : ret) swap(i, cur), cur = cur + i;
                                                                    27   // 实在需要零面积区域边缘，需要仔细考虑 turn_left 的实现。
18    return ret; } // ret 可能共线，但没有重点

                                                                         1.2   整数半平面交
 1   struct point {                                                  1   struct line : point {
 2      point rot(LD t) const { // 逆时针                               2      LD z; // ax + by + c >= 0
 3         return {x*cos(t) ‐ y*sin(t), x*sin(t) + y*cos(t)};}       3      line () {}
 4      point rot90() const { return {‐y, x}; }};                    4      line (LD a, LD b, LD c): point(a, b), z(c) {}
 5   bool two_side(cp a, cp b, cl c) {                               5      line (cp a, cp b): point((b‐a).rot90()), z(det(a, b)){}
 6      return turn(c.s, c.t, a) * turn(c.s, c.t, b) < 0; }          6      LD operator () (cp a) const{return dot(a, *this) + z;}};
 7   point line_inter(cl a, cl b) { // 直线交点                          7   point line_inter (cl u, cl v) {
 8      LD s1 = det(a.t ‐ a.s, b.s ‐ a.s);                           8   return point(det({u.z, u.y}, {v.z, v.y}),
 9      LD s2 = det(a.t ‐ a.s, b.t ‐ a.s);                           9                det({u.x, u.z}, {v.x, v.z}) ) / ‐det(u, v); }
10      return (b.s * s2 ‐ b.t * s1) / (s2 ‐ s1); }                 10   LD dis (cl l, cp x = {0, 0}) { return l(x) / l.len(); }
11   vector <point> cut (const vector<point> &c, line l) {          11   bool is_para(cl x, cl y) { return !sgn(det(x, y)); }
12      vector <point> ret; // 线切凸包                                 12   LD det(cl a, cl b, cl c) {
13      int n = (int) c.size(); if (!n) return ret;                 13      return det(a,b)*c.z + det(b,c)*a.z + det(c,a)*b.z;}
14      for (int i = 0; i < n; i++) {                               14   int check(cl a, cl b, cl c) { // sgn left(a, inter(b, c))
15         int j = (i + 1) % n;                                     15      return sgn(det(b, c, a)) * sgn(det(b, c)); }
16         if (turn (l.s, l.t, c[i]) >= 0) ret.push_back(c[i]);     16   bool turn_left(cl a, cl b, cl c){return check(a, b, c) >0;}
17         if (two_side (c[i], c[j], l))                            17   bool cmp (cl a, cl b) {
18            ret.push_back(line_inter(l, {c[i], c[j]})); }         18   if (is_para(a, b) && dot(a, b) > 0) return dis(a) < dis(b);
19      return ret; }                                               19   return half(a) == half(b) ? sgn(det(a,b))>0 : half(b)>0;}
20   bool pos (cp a,cl b){ // point_on_segment 点在线段上                20   // 用以上函数替换 HPI 函数，需要 half(point)
21      return turn(b.s, b.t, a) == 0 // 在直线上                       21   line perp(cl l) { return {l.y, ‐l.x, 0}; } // 垂直
22      && sgn(dot(b.s ‐ a, b.t ‐ a)) <= 0;}                        22   line para(cl l, cp o) { // 过一点平行
23   bool inter_judge(cl a,cl b) { // 线段判非严格交                       23       return {l.x, l.y, l.z ‐ l(o)}; }
24      if (pos (b.s, a) || pos (b.t, a)) return true;              24   point proj(cp x, cl l) {return x ‐ l * (l(x)/l.len2());}
25      if (pos (a.s, b) || pos (a.t, b)) return true;              25   point refl(cp x, cl l) {return x ‐ l * (l(x)/l.len2())*2;}
26      return two_side (a.s, a.t, b)                               26   bool is_perp(cl x, cl y) { return !sgn(dot(x, y)); }
27          && two_side (b.s, b.t, a); }                            27   LD area(cl a, cl b, cl c) { // 0 when error
28   point proj_to_line (cp a, cl b) { // 点在直线投影                    28      LD d = det(a, b, c);
29      point st = b.t ‐ b.s;                                       29      return d * d / (det(a, b) * det(b, c) * det(c, a)); }
30      return b.s + st * (dot(a ‐ b.s, st) / dot(st, st));}        30   vector <line> cut (const vector <line>& o, line l){
31   LD p2l (cp a, cl b) { // point_to_line                         31      vector <line> ret; int n = (int) o.size();
32      return abs(det(b.t‐b.s, a‐b.s)) / dis(b.s, b.t); }          32      for (int i = 0; i < n; i++) {
33   LD p2s (cp a, cl b) { // point_to_segment 注意退化                 33         cl u = o[i], v = o[(i+1) % n], w = o[(i + 2) % n];
34      if ( sgn(dot(b.s ‐ a, b.t ‐ b.s))                           34         int va = check(l, u, v), vb = check(l, v, w);
35         * sgn(dot(b.t ‐ a, b.t ‐ b.s)) >= 0)                     35         if (va > 0 || vb > 0 || (va == 0 && vb == 0))
36         return min (dis (a, b.s), dis (a, b.t));                 36            ret.push_back(v);
37      return p2l (a, b); }                                        37         if (va >= 0 && vb < 0) ret.push_back(l);
38   bool point_on_ray (cp a, cl b) { // 点在射线上                      38      } return ret; }
39      return turn (b.s, b.t, a) == 0

---

<!-- page: 4 -->

     上海交通大学 Shanghai Jiao Tong University                      Nemesis                                                          Page 3
     1.3 凸包询问：凸包内、切点、交点、最近点                                         29 LD get_far (int uid, int vid) {
 1   int n; vector <point> a; // 可以封装成一个 struct                     30    // u ‐> v in polygon, check the ray u ‐> polygon
 2   bool inside (cp u) { // 点在凸包内                                  31    cp u = p[uid], v = p[vid];
 3      int l = 1, r = n ‐ 2;                                       32    LD far = 1e9;
 4      while (l < r) {                                             33    for (int i = 0; i < n; i++) {
 5         int mid = (l + r + 1) / 2;                               34       int j = (i + 1) % n, k = (i + 2) % n;
 6         if (turn(a[0], a[mid], u) >= 0) l = mid;                 35       cp ii = p[i], jj = p[j], kk = p[k];
 7         else r = mid ‐ 1; }                                      36       if (two_side (ii, jj, {u, v})) {
 8      return turn (a[0], a[l], u) >= 0                            37          LD s1 = det(jj ‐ ii, u ‐ ii);
 9          && turn (a[l], a[l + 1], u) >= 0                        38          LD s2 = det(jj ‐ ii, v ‐ ii);
10          && turn (a[l + 1], a[0], u) >= 0; }                     39          if (sgn(s1 ‐ s2) && sgn(s1) != sgn(s2 ‐ s1))
11   int search (auto f) { // 凸包求极值，需要 C++17                        40             far = min(far,
12      int l = 0, r = n ‐ 1;                                       41                       dis(u, line_inter({ii,jj},{u,v})));}
13      int d = f(a[r], a[l]) ? (swap(l, r), ‐1) : 1;               42       if (j != uid && point_on_ray (jj, {u, v})) {
14      while (d * (r ‐ l) > 1) {                                   43          bool good = turn(ii, jj, kk) <= 0;
15         int mid = (l + r) / 2;                                   44          for (auto x : {u ‐ (jj ‐ u), jj + (jj ‐ u)})
16         if (f(a[mid], a[l]) && f(a[mid], a[mid ‐ d])) l = mid;   45             good &= !(   turn(ii, jj, x) > 0
17         else r = mid; } return l; }                              46                       && turn(jj, kk, x) > 0);
18   pair<int, int> get_tan(cp u) { // 求切线                          47          if (!good) far = min(far, dis(u, jj));
19    return // 严格在凸包外；需要边界上时，特判 a[n‐1] ‐> a[0]                     48       } } return far; }
                                                                    49 void work() {
20    {search([&](cp x, cp y){return turn(u, y, x) > 0;}),
21     search([&](cp x, cp y){return turn(u, x, y) > 0;})};}        50     for (int i = 0; i < n; i++)
22   point at (int i) { return a[i % n]; }                          51       for (int j = 0; j < n; j++) if (i != j) {
23   int inter (cp u, cp v, int l, int r) {                         52          if (!in_polygon(p[i], p[j])) continue;
24      int sl = turn(u, v, at(l));                                 53          LD ret = get_far(i,j)+get_far(j,i)‐dis(p[i],p[j]);
25      while (l + 1 < r) {                                         54          ans = max(ans, ret); } }
26         int m = (l + r) / 2;
27
28
           if (sl == turn(u, v, at(m))) l = m;
           else r = m;   } return l % n; }
                                                                         1.5 𝑂 (𝑛 2 ) Ear Clipping 三角剖分
29   bool get_inter(cp u, cp v, int &i, int &j) { // 求直线交点           1 vector <array <int, 3>> tri;
30      int p0 = search([&](cp x, cp y){                             2 void solve () {
31       return det(v ‐ u, x ‐ u) < det(v ‐ u, y ‐ u);}),            3    list <int> l;
32      p1 = search([&](cp x, cp y) {                                4    for (int k = 0; k < n; k++) l.push_back(k);
33       return det(v ‐ u, x ‐ u) > det(v ‐ u, y ‐ u);});            5    auto check = [&](auto u, auto v, auto w) {
34      if (turn(u, v, a[p0]) * turn(u, v, a[p1]) < 0) {             6       if (turn(u, v, w) <= 0) return false;
35         if (p0 > p1) swap(p0, p1);                                7       for (auto i : l)
36         i = inter (u, v, p0, p1);                                 8          if (turn(u, v, p[i]) == 1 &&
37         j = inter (u, v, p1, p0 + n);                             9              turn(v, w, p[i]) == 1 &&
38         return true; } else return false; }                      10              turn(w, u, p[i]) == 1) return false;
39   LD near (cp u, int l, int r) {                                 11       return true; };
40      if (l > r) r += n;                                          12    for (auto it = l.begin(); l.size() > 3; ) {
41      int sl = sgn (dot(u ‐ at(l), at(l + 1) ‐ at(l)));           13       auto u = (it == l.begin() ? prev(l.end()) : prev(it));
42      LD ret = p2s (u, {at(l), at(l + 1)});                       14       auto v = (next(it) == l.end() ? l.begin() : next(it));
43      while (l + 1 < r) {                                         15       if (!check(p[*u], p[*it], p[*v])) it = v;
44         int m = (l + r) / 2;                                     16       else {
45         if (sl == sgn (dot(u ‐ at(m), at(m + 1) ‐ at(m))))       17          tri.push_back({*u, *it, *v});
46            l = m; else r = m; }                                  18          l.erase(it);
47      return min(ret, p2s(u, {at(l), at(l + 1)})); }              19          it = u; } }
48   LD get_near (cp u) { // 求凸包外点到凸包最近点                            20    tri.push_back({l.front(), *next(l.begin()), l.back()}); }
49      if (inside(u)) return 0;
50      auto [x, y] = get_tan(u);
51      return min(near(u, x, y), near(u, y, x)); }
                                                                         1.6   单插入动态凸包
                                                                     1   struct hull { // upper hull, left to right
     1.4 点、线段在简单多边形内                                                 2   set <point> a; LL tot;
                                                                     3   hull () {tot = 0;}
 1 bool point_in_polygon (cp u, const vector <point> & p) {
                                                                     4   LL calc(auto it) {
 2    int n = (int) p.size (), cnt = 0;                              5      auto u = it == a.begin() ? a.end() : prev(it);
 3    for (int i = 0; i < n; ++i) {                                  6      auto v = next(it);
 4       point a = p[i], b = p[(i + 1) % n];                         7      LL ret = 0;
 5       if (pos (u, {a, b})) return true;                           8      if (u != a.end()) ret += det(*u, *it);
 6       int x = turn (a, u, b);                                     9      if (v != a.end()) ret += det(*it, *v);
 7       int y = sgn (a.y ‐ u.y);                                   10      if (u != a.end() && v != a.end()) ret ‐= det(*u, *v);
 8       int z = sgn (b.y ‐ u.y);                                   11      return ret; }
 9       if (x > 0 && y <= 0 && z > 0) ++cnt;                       12   void insert (point p) {
10       if (x < 0 && z <= 0 && y > 0) ‐‐cnt; }                     13      if (!a.size()) { a.insert (p); return; }
11    return cnt != 0; } // < 0 在逆时针多边形内; > 0 顺时针                   14      auto it = a.lower_bound (p);
12 bool in_polygon (cp u, cp v) {
                                                                    15      bool out;
13    // u, v in polygon; p contain those u, v on border            16      if (it == a.begin()) out = (p < *it); // special case
14    for (int i = 0; i < n; i++) {                                 17      else if (it == a.end()) out = true;
15       int j = (i + 1) % n, k = (i + 2) % n;                      18      else out = turn(*prev(it), *it, p) > 0;
16       cp ii = p[i], jj = p[j], kk = p[k];                        19      if (!out) return;
17       if (inter_judge_strict({u, v}, {ii, jj})) return 0;        20      while (it != a.begin()) {
18       if (point_on_segment (jj, {u, v})) {                       21         auto o = prev(it);
19          bool good = true, left = turn (ii, jj, kk) >= 0;        22         if (o == a.begin() || turn(*prev(o), *o, p) < 0)
20          for (auto x : {u, v})                                                ↩→ break;
21             if (left)                                            23         else erase(o); }
22                good &=    turn(ii, jj, x) >= 0                   24      while (it != a.end()) {
23                        && turn(jj, kk, x) >= 0;                  25         auto o = next(it);
24             else                                                 26         if (o == a.end() || turn(p, *it, *o) < 0) break;
25                good &= !(   turn(jj, x, kk) > 0                  27         else erase(it), it = o; }
26                          && turn(jj, ii, x) > 0);                28      tot += calc(a.insert(p).first); }
27          if (!good) return 0;                                    29   void erase(auto it) { tot ‐= calc(it); a.erase(it); } };
28       } } return 1; }

---

<!-- page: 5 -->

     上海交通大学 Shanghai Jiao Tong University                      Nemesis                                                                                  Page 4
     1.7 圆                                                               圆幂定理: 过 𝑃 的直线交圆在 𝐴 和 𝐵 两点, 则 𝑃𝐴 · 𝑃𝐵 = |ℎ|
                                                                         根轴: 到两圆等幂点的轨迹是一条垂直于连心线的直线
                                                                         反演: 已知一圆 𝐶 , 圆心为 𝑂 , 半径为 𝑟 , 如果 𝑃 与 𝑃 0 在过圆心 𝑂 的直线上, 且
 1   struct circle { point c; LD r;};
 2   bool in_circle(cp a, const circle &b) {
                                                                         𝑂𝑃 · 𝑂𝑃 0 = 𝑟 2 , 则称 𝑃 与 𝑃 0 关于 𝑂 互为反演. 一般 𝐶 取单位圆.
 3      return sgn(b.r ‐ dis(b.c, a)) >= 0; }                            反演的性质:
 4   circle make_circle(cp a, cp b) { // 以 a b 为直径的圆                     不过反演中心的直线反形是过反演中心的圆, 反之亦然.
 5      point p = (a + b) / 2;                                           不过反演中心的圆, 它的反形是一个不过反演中心的圆.
 6      return {p, dis(a, p)}; }                                         两条直线在交点 𝐴 的夹角, 等于它们的反形在相应点 𝐴0 的夹角, 但方向相反.
 7   circle make_circle(cp a, cp b, cp c) { // 三点共线 inf / nan            两个相交圆周在交点 𝐴 的夹角等于它们的反形在相应点 𝐴0 的夹角, 但方向相反.
                                                                         直线和圆周在交点 𝐴 的夹角等于它们的反演图形在相应点 𝐴0 的夹角, 但方向相反.
 8      point bc = c ‐ b, ca = a ‐ c, ab = b ‐ a;
                                                                         正交圆反形也正交. 相切圆反形也相切, 当切点为反演中心时, 反形为两条平行线. 两
 9      point o = (b + c ‐ bc.rot90()*dot(ca,ab)/det(ca,ab)) / 2;        两相切的圆 r1, r2, r3, 求与他们都相切的圆 r4. 分母取负号, 答案再取绝对值, 为外
                                                                                                                                    𝑟 1𝑟 2𝑟 3
10      return {o, dis(o, a)}; } // 检查上一行正负号                             切圆半径. 分母取正号为内切圆半径. 𝑟 4± =                                     √
11   circle min_circle (vector <point> p) { // 最小覆盖圆                                                         𝑟 1𝑟 2 +𝑟 1𝑟 3 +𝑟 2𝑟 3 ±2 𝑟 1𝑟 2𝑟 3 (𝑟 1 +𝑟 2 +𝑟 3 )
12    circle ret({0, 0}, 0);
13    shuffle (p.begin (), p.end (), rng);                           1    circle inv_c2c(point O, LD R, circle A) {
14    for (int i = 0; i < (int) p.size (); i++)                      2       LD OA = dis(A.c, O);
15     if (!in_circle(p[i], ret)) {                                  3       LD RB = 0.5 * R * R * (1 / (OA ‐ A.r) ‐ 1 / (OA + A.r));
16      ret = circle (p[i], 0);                                      4       LD OB = OA * RB / A.r;
17      for (int j = 0; j < i; j++) if (!in_circle(p[j], ret)) {     5       point B = O + (A.c ‐ O) * (OB / OA);
18       ret = make_circle (p[i], p[j]);                             6       return {B, RB};
19       for (int k = 0; k < j; k++) if (!in_circle(p[k], ret))      7    } // 点 O 在圆 A 外，求圆 A 的反演圆 B，R 是反演半径
20        ret = make_circle (p[i], p[j], p[k]);                      8    circle inv_l2c(point O, LD R, line l) {
21     } } return ret; }                                             9       point P = proj_to_line(O, l);
22   pair <point, point> line_circle_inter (cl a, circle c) {       10       LD d = dis(O, P);
23      LD d = p2l (c.c, a);                                        11       LD RB = R * R / (2 * d);
24      // 需要的话返回 vector <point>                                    12       point VB = (P ‐ O) / d * RB;
25      /* if (sgn (d ‐ R) >= 0) return {}; */                      13       return {O + VB, RB};
26      LD x = sqrt (sqr(c.r) ‐ sqr(d)); // sqrt(max(0., ...))      14    } // 不过 O 点的直线反演为过 O 点的圆，R 是反演半径
27      return {                                                    15    line inv_c2l (point O, LD R, circle A) {
28         proj_to_line (c.c, a) + (a.s ‐ a.t).unit() * x,          16       LD t = R * R / (2 * A.r);
29         proj_to_line (c.c, a) ‐ (a.s ‐ a.t).unit() * x }; }      17       point p = O + (A.c ‐ O).unit() * t;
30   LD circle_inter_area (circle a, circle b) { // 圆面积交            18       return {p, p + (O ‐ p).rot90()};
31      LD d = dis (a.c, b.c);                                      19    } // 过 O 点的圆反演为不过 O 点的直线，R 是反演半径
32      if (sgn (d ‐ (a.r + b.r)) >= 0) return 0;
33      if (sgn (d ‐ abs(a.r ‐ b.r)) <= 0) {                             1.9     圆并
34         LD r = min (a.r, b.r);
35         return r * r * PI; }                                      1    int C; circle c[MAXN]; LD area[MAXN];
36      LD x = (d * d + a.r * a.r ‐ b.r * b.r) / (2 * d),            2    struct event { // 如果需要边界而非面积，那么仔细考虑事件顺序
37            t1 = acos (min (1., max (‐1., x / a.r))),              3     point p; LD ang; int delta;
38            t2 = acos (min (1., max (‐1., (d ‐ x) / b.r)));        4     bool operator <(const event &a){return ang < a.ang;}};
39      return sqr(a.r)*t1 + sqr(b.r)*t2 ‐ d*a.r*sin(t1);}           5    void addevent(cc a, cc b, vector<event> &e, int &cnt) {
40   vector <point> circle_inter (circle a, circle b) { // 圆交点       6     LD d2=dis2(a.c, b.c),dw=((a.r‐b.r)*(a.r+b.r)/d2+1)/2,pw=
41      if (a.c == b.c || sgn (dis (a.c, b.c) ‐ a.r ‐ b.r) > 0       7    sqrt(max(0.,‐(d2‐sqr(a.r‐b.r)*(d2‐sqr(a.r+b.r))/sqr(2*d2));
42         || sgn (dis (a.c, b.c) ‐ abs (a.r ‐ b.r)) < 0)            8     point d = b.c ‐ a.c, p = d.rot(PI / 2),
43         return {};                                                9      q0 = a.c + d * dw + p * pw,
44      point r = (b.c ‐ a.c).unit();                               10      q1 = a.c + d * dw ‐ p * pw;
45      LD d = dis (a.c, b.c);                                      11     LD ang0 = atan2((q0 ‐ a.c).y, (q0 ‐ a.c).x),
46      LD x = ((sqr (a.r) ‐ sqr (b.r)) / d + d) / 2;               12        ang1 = atan2((q1 ‐ a.c).y, (q1 ‐ a.c).x);
47      LD h = sqrt (sqr (a.r) ‐ sqr (x));                          13     e.push_back({q1,ang1,1}); e.push_back({q0,ang0,‐1});
48      if (sgn (h) == 0) return {a.c + r * x};                     14     cnt += ang1 > ang0; }
49      return {a.c + r * x + r.rot90 () * h,                       15    bool issame(cc a, cc b) {
50              a.c + r * x ‐ r.rot90 () * h}; }                    16     return sgn(dis(a.c,b.c)) == 0 && sgn(a.r‐b.r) == 0; }
51   // 返回按照顺时针方向                                                   17    bool overlap(cc a, cc b) {
52   vector <point> tangent (cp a, circle b) {                      18     return sgn(a.r ‐ b.r ‐ dis(a.c, b.c)) >= 0; }
53      return circle_inter (make_circle (a, b.c), b); }            19    bool intersect(cc a, cc b) {
54   vector <line> extangent (circle a, circle b) {                 20     return sgn(dis(a.c, b.c) ‐ a.r ‐ b.r) < 0; }
55      vector <line> ret; // 未考虑两圆内切的一条外切线                         21    void solve() {
56      if (sgn(dis(a.c, b.c)‐abs(a.r ‐ b.r))<=0) return ret;       22     fill(area, area + C + 2, 0);
57      if (sgn(a.r ‐ b.r) == 0) {                                  23     for(int i = 0; i < C; ++i) { int cnt = 1;
58         point dir = b.c ‐ a.c;                                   24      vector<event> e;
59         dir = (dir * a.r / dir.len()).rot90();                   25      for(int j=0; j<i; ++j) if(issame(c[i],c[j])) ++cnt;
60         ret.push_back({a.c + dir, b.c + dir});                   26      for(int j = 0; j < C; ++j)
61         ret.push_back({a.c ‐ dir, b.c ‐ dir});                   27       if(j != i && !issame(c[i], c[j]) && overlap(c[j], c[i]))
                                                                               ↩→ ++cnt;
62      } else {
63         point p = (b.c * a.r ‐ a.c * b.r) / (a.r ‐ b.r);         28      for(int j = 0; j < C; ++j)
64         auto u = tangent(p, a), v = tangent(p, b);               29       if(j != i && !overlap(c[j], c[i]) && !overlap(c[i], c[j])
                                                                               ↩→ && intersect(c[i], c[j]))
65         if (u.size() == 2 && v.size() == 2) {
66            if (sgn(a.r‐b.r) < 0)                                 30        addevent(c[i], c[j], e, cnt);
67               swap(u[0], u[1]), swap(v[0], v[1]);                31      if(e.empty()) area[cnt] += PI * c[i].r * c[i].r;
68            ret.push_back({u[0], v[0]});                          32      else {
69            ret.push_back({u[1], v[1]}); } }                      33       sort(e.begin(), e.end());
70      return ret; }                                               34       e.push_back(e.front());
71   vector <line> intangent(circle a, circle b) {                  35       for(int j = 0; j + 1 <(int)e.size(); ++j) {
72      vector <line> ret; // 未考虑两圆外切的一条内切线                         36        cnt += e[j].delta;
73      point p = (b.c * a.r + a.c * b.r) / (a.r + b.r);            37        area[cnt] += det(e[j].p,e[j + 1].p) / 2;
74      vector u = tangent(p, a), v = tangent(p, b);                38        LD ang = e[j + 1].ang ‐ e[j].ang;
75      if (u.size() == 2 && v.size() == 2) {                       39        if(ang < 0) ang += PI * 2;
76         ret.push_back({u[0], v[0]});                             40        area[cnt] += ang * c[i].r * c[i].r / 2 ‐ sin(ang) *
                                                                                 ↩→ c[i].r * c[i].r / 2; } } } }
77         ret.push_back({u[1], v[1]}); } return ret; }

     1.8 圆反演，阿波罗尼茨圆                                                      1.10 多边形与圆交
     所有关于两点 𝐴, 𝐵 满足 𝑃𝐴/𝑃𝐵 = 𝑘 且不等于 1 的点 𝑃 的轨迹是一个圆.                  1 LD angle (cp u, cp v) {
     圆幂: 半径为 𝑅 的圆 𝑂 , 任意一点 𝑃 到 𝑂 的幂为 ℎ = 𝑂𝑃 2 − 𝑅 2                 2    return 2 * asin(dis(u.unit(), v.unit()) / 2); }

---

<!-- page: 6 -->

     上海交通大学 Shanghai Jiao Tong University                            Nemesis                                                                                           Page 5
 3 LD area(cp s, cp t, LD r) { // 2 * area                                 1.14 相关公式
 4    LD theta = angle(s, t);
 5    LD dis = p2s ({0, 0}, {s, t});                                       1.14.1 Heron’s Formula                                   1.14.4 三角形外心
 6    if (sgn(dis ‐ r) >= 0) return theta * r * r;                         𝑝 = 𝑎+𝑏+𝑐 , 𝑎 ≥ 𝑏 ≥ 𝑐,                                                             −→ −→ −
                                                                               p2
                                                                                                                                                   𝐴® + 𝐵® − −𝐵𝐶 ·𝐶𝐴 →𝑇
                                                                                                                                                              → −→ 𝐴𝐵
 7    auto [u, v] = line_circle_inter({s, t}, {{0, 0}, r});
                                                                           𝑆 = 𝑝 (𝑝 − 𝑎)(𝑝 − 𝑏)(𝑝 − 𝑐) ,                                                    𝐴𝐵× 𝐵𝐶
 8    point lo = sgn(det(s, u)) >= 0 ? u : s;                                    r                                                     𝑂® =
 9    point hi = sgn(det(v, t)) >= 0 ? v : t;                                                                      2                            2
                                                                           𝑆 = 21    𝑎 2𝑐 2 −      𝑎 2 +𝑐 2 −𝑏 2       .
10    return det(lo, hi) + (theta ‐ angle(lo, hi)) * r * r; }                                            2
                                                                                                                                    1.14.5 三角形垂心
11 LD solve(vector<point> &p, cc c) {                                      1.14.2 四面体内接球球心
12    LD ret = 0;                                                          假设 𝑠𝑖 是第 𝑖 个顶点相对面的面积，则                                           𝐻® = 3𝐺® − 2𝑂®
13    for (int i = 0; i < (int) p.size (); ++i) {                          有                                                        1.14.6 三角形偏心
14       auto u = p[i] ‐ c.c;
                                                                                 𝑠 1𝑥 1 + 𝑠 2𝑥 2 + 𝑠 3𝑥 3 + 𝑠 4𝑥 4
15       auto v = p[(i + 1) % p.size()] ‐ c.c;                              
                                                                            
                                                                             𝑥 =                                                                  −𝑎𝐴® + 𝑏 𝐵® + 𝑐𝐶®
                                                                            
                                                                                        𝑠1 + 𝑠2 + 𝑠3 + 𝑠4
16       int s = sgn(det(u, v));                                            
                                                                            
                                                                                 𝑠 1𝑦1 + 𝑠 2𝑦2 + 𝑠 3𝑦3 + 𝑠 4𝑦4                                      −𝑎 + 𝑏 + 𝑐
17       if      (s > 0) ret += area (u, v, c.r);                             𝑦 =                                                   内角的平分线和对边的两个外角平分线
18       else if (s < 0) ret ‐= area (v, u, c.r);                           
                                                                                        𝑠1 + 𝑠2 + 𝑠3 + 𝑠4                          交点, 外切圆圆心. 剩余两点的同理.
                                                                            
                                                                                  𝑠 1𝑧 1 + 𝑠 2𝑧 2 + 𝑠 3𝑧 3 + 𝑠 4𝑧 4
19    } return abs (ret) / 2; } //ret在p逆时针时为正                               
                                                                                                                                   1.14.7 三角形内接外接圆半径
                                                                            𝑧 =
                                                                                        𝑠1 + 𝑠2 + 𝑠3 + 𝑠4                                      2𝑆       𝑎𝑏𝑐
                                                                                                                                          𝑟=         ,𝑅=
                                                                           体积可以使用 1/6 混合积求, 内接球半                                             𝑎 +𝑏 +𝑐
     1.11 球面基础, 经纬度球面距离                                                    径为
                                                                                                                                                          4𝑆
     球面距离: 连接球面两点的大圆劣弧 (所有曲线中最短)                                                                                                    1.14.8 Pick’s Theorem 格点多边
     球面角: 球面两个大圆弧所在半平面形成的二面角                                                                     3𝑉                                             形面积
                                                                                    𝑟 =
     球面凸多边形: 把一个球面多边形任意一边向两方无限延长成大圆, 其余边都在此大
     圆的同旁.
                                                                                          𝑠1 + 𝑠2 + 𝑠3 + 𝑠4                         𝑆 = 𝐼 + 𝐵2 − 1. 𝐼 内部点, 𝐵 边界点。
     球面角盈 𝐸 : 球面凸n边形的内角和与 (𝑛 − 2)𝜋 的差                                      1.14.3 三角形内心                                             1.14.9 Euler’s Formula 多面体
     离北极夹角 𝜃 , 距离 ℎ 的球冠: 𝑆 = 2𝜋𝑅ℎ = 2𝜋𝑅 2 (1 − cos 𝜃 ) , 𝑉 = 𝜋ℎ
                                                                2
                                                                                                                                             与平面图的点、边、面
                                                              3 (3𝑅 − ℎ)                   𝑎𝐴® + 𝑏 𝐵® + 𝑐𝐶®                         Convex polyhedron: 𝑉 − 𝐸 + 𝐹 = 2.
     球面凸n边形面积: 𝑆 = 𝐸𝑅 2                                                               𝐼® =
                                                                                             𝑎 +𝑏 +𝑐                                Planar graph: |𝐹 | = |𝐸| − |𝑉 | + 𝑛 + 1,
                                                                                                                                    𝑛 : #(connected components).
1 // lontitude 经度范围: ±𝜋 , latitude 纬度范围: ±𝜋/2
2 LD sphereDis(LD lon1, LD lat1, LD lon2, LD lat2, LD R) {
                                                                           1.14.10 三角公式
3    return R * acos(cos(lat1) * cos(lat2) * cos(lon1 ‐ lon2)              sin(𝑎) + sin(𝑏) = 2 sin( 𝑎+𝑏        𝑎−𝑏
                                                                                                     2 ) cos( 2 )
           ↩→ +   sin(lat1) * sin(lat2)); }                                                         𝑎+𝑏
                                                                           sin(𝑎) − sin(𝑏) = 2 cos( 2 ) sin( 𝑎−𝑏  2 )
                                                                           cos(𝑎) + cos(𝑏) = 2 cos( 𝑎+𝑏
                                                                                                      2   ) cos( 𝑎−𝑏 )
                                                                                                                   2
     1.12 圆上整点                                                             cos(𝑎) − cos(𝑏) = −2 sin( 𝑎+𝑏           𝑎−𝑏
                                                                                                         2 ) sin( 2 )
 1 vector <LL> solve(LL r) {                                               sin(𝑎 ± 𝑏) = sin 𝑎 cos 𝑏 ± cos 𝑎 sin 𝑏
 2    vector <LL> ret; // non‐negative Y pos                               cos(𝑎 ± 𝑏) = cos 𝑎 cos 𝑏 ∓ sin 𝑎 sin 𝑏
                                                                                         tan(𝑎)±tan(𝑏 )
 3    ret.push_back(0);                                                    tan(𝑎 ± 𝑏) = 1∓tan(𝑎) tan(𝑏 )
 4    LL l = 2 * r, s = sqrt(l);                                                                         sin(𝑎±𝑏 )
 5    for (LL d=1; d<=s; d++) if (l%d==0) {                                tan(𝑎) ± tan(𝑏) = cos(𝑎) cos(𝑏 )
                                                                                                                              
 6       LL lim=LL(sqrt(l/(2*d)));                                         sin(𝑛𝑎) = 𝑛 cos𝑛−1 𝑎 sin 𝑎 − 𝑛3 cos𝑛−3 𝑎 sin3 𝑎 + 𝑛5 cos𝑛−5 𝑎 sin5 𝑎 − . . .
                                                                                                                     
                                                                           cos(𝑛𝑎) = cos𝑛 𝑎 − 𝑛2 cos𝑛−2 𝑎 sin2 𝑎 + 𝑛4 cos𝑛−4 𝑎 sin4 𝑎 − . . .
 7       for (LL a = 1; a <= lim; a++) {
 8          LL b = sqrt(l/d‐a*a);
 9          if (a*a+b*b==l/d && __gcd(a,b)==1 && a!=b)                     1.14.11 超球坐标系
10             ret.push_back(d*a*b);
11       } if (d*d==l) break;                                                                            𝑥1        =       𝑟 cos(𝜙 1 )
12       lim = sqrt(d/2);                                                                                𝑥2        =       𝑟 sin(𝜙 1 ) cos(𝜙 2 )
13       for (LL a=1; a<=lim; a++) {                                                                    ···
14          LL b = sqrt(d ‐ a * a);                                                             𝑥𝑛−1               =       𝑟 sin(𝜙 1 ) · · · sin(𝜙𝑛−2 ) cos(𝜙𝑛−1 )
15          if (a*a+b*b==d && __gcd(a,b)==1 && a!=b)                                               𝑥𝑛              =       𝑟 sin(𝜙 1 ) · · · sin(𝜙𝑛−2 ) sin(𝜙𝑛−1 )
16             ret.push_back(l/d*a*b);
                                                                                                𝜙𝑛−1               ∈       [0, 2𝜋]
17    } } return ret; }
                                                                                      ∀𝑖 = 1..𝑛 − 1𝜙𝑖              ∈       [0, 𝜋]

     1.13 三相之力                                                             1.14.12     三维旋转公式
                                                                                                                                  " 0#
1 point incenter (cp a, cp b, cp c) {                                                                                              𝑥     h𝑥 i
                                                                           绕着 (0, 0, 0) − (𝑢𝑥, 𝑢𝑦, 𝑢𝑧) 旋转 𝜃 , (𝑢𝑥, 𝑢𝑦, 𝑢𝑧) 是单位向量. 𝑦 0 = 𝑅 𝑦
2    double p = dis(a, b) + dis(b, c) + dis(c, a);
                                                                                                                                   𝑧0     𝑧
3    return ( a*dis(b, c) + b*dis(c, a) + c*dis(a, b) ) / p;}
                                                                                                                                                                          !
4 point circumcenter (cp a, cp b, cp c) {                                           cos 𝜃 +𝑢𝑥2 (1−cos 𝜃 ) 𝑢𝑥 𝑢 𝑦 (1−cos 𝜃 ) −𝑢𝑧 sin 𝜃 𝑢𝑥 𝑢𝑧 (1−cos 𝜃 )+𝑢 𝑦 sin 𝜃
5    point p = b ‐ a, q = c ‐ a, s (dot(p,p)/2, dot(q,q)/2);                𝑅=      𝑢 𝑦 𝑢𝑥 (1−cos 𝜃 )+𝑢𝑧 sin 𝜃 cos 𝜃 +𝑢 𝑦2 (1−cos 𝜃 ) 𝑢 𝑦 𝑢𝑧 (1−cos 𝜃 ) −𝑢𝑥 sin 𝜃             .
6    double d = det(p, q); return a + point(det(s, {p.y,                            𝑢𝑧 𝑢𝑥 (1−cos 𝜃 ) −𝑢 𝑦 sin 𝜃 𝑢𝑧 𝑢 𝑦 (1−cos 𝜃 )+𝑢𝑥 sin 𝜃 cos 𝜃 +𝑢𝑧2 (1−cos 𝜃 )
           ↩→ q.y}),   det({p.x, q.x}, s)) / d; }
 7 point orthocenter (cp a, cp b, cp c) {                                  1.14.13     立体角公式
 8    return a + b + c ‐ circumcenter (a, b, c) * 2.0; }                                                           𝜙 : 二面角
 9 point fermat_point (cp a, cp b, cp c) {
                                                                                                        Ω = (𝜙𝑎𝑏 + 𝜙𝑏𝑐 + 𝜙𝑎𝑐 ) rad − 𝜋 sr
10    if (a == b) return a; if (b == c) return b;
11    if (c == a) return c;
12    double ab = dis(a, b), bc = dis(b, c), ca = dis(c, a);                                                              𝑎® 𝑏® 𝑐®
                                                                                                  1
13    double cosa = dot(b ‐ a, c ‐ a) / ab / ca;                                        tan         Ω/rad =                                     
14    double cosb = dot(a ‐ b, c ‐ b) / ab / bc;
                                                                                                  2         𝑎𝑏𝑐 + 𝑎® · 𝑏® 𝑐 + (𝑎® · 𝑐®) 𝑏 + 𝑏® · 𝑐® 𝑎
15    double cosc = dot(b ‐ c, a ‐ c) / ca / bc;
16    double sq3 = PI / 3.0; point mid;                                                                                       𝜃 𝑎 + 𝜃𝑏 + 𝜃 𝑐
17    if (sgn (cosa + 0.5) < 0) mid = a;                                                                               𝜃𝑠 =
                                                                                                                                     2
18    else if (sgn (cosb + 0.5) < 0) mid = b;
19    else if (sgn (cosc + 0.5) < 0) mid = c;
20    else if (sgn (det(b ‐ a, c ‐ a)) < 0)                                1.14.14   常用体积公式                                               • 椭球 Ellipsoid 𝑉 = 34 𝜋𝑎𝑏𝑐 .
21         mid = line_inter ({a, b + (c ‐ b).rot (sq3)}, {b, c
                                                                                 • 棱锥 Pyramid 𝑉 = 13 𝑆ℎ .
                 ↩→ + (a ‐ c).rot (sq3)});
                                                                                                                                    1.14.15 扇形与圆弧重心
22       else mid = line_inter ({a, c + (b ‐ c).rot (sq3)}, {c, b                • 球 Sphere 𝑉 = 34 𝜋𝑅 3 .                                      4𝑟 sin(𝜃 /2)
           ↩→ + (a ‐ b).rot (sq3)});                                                                                                扇形重心与圆心距离为              ,
23       return mid; } // minimize(|A‐x|+|B‐x|+|C‐x|)                            • 棱台 Frustum √                                                     3𝜃
                                                                                                                                               4𝑟 sin3 (𝜃 /2)
                                                                                    𝑉 = 13 ℎ(𝑆 1 + 𝑆 1𝑆 2 + 𝑆 2 ) .                 圆弧重心与圆心距离为
                                                                                                                                               3(𝜃 −sin(𝜃 ) ) .

---

<!-- page: 7 -->

     上海交通大学 Shanghai Jiao Tong University                         Nemesis                                                         Page 6
     1.14.16   高维球体积                                                 20       if (mark[b][c] == stp) ins(c, b, v);
                                                                     21       if (mark[c][a] == stp) ins(a, c, v); } }
                  𝑉2 = 𝜋𝑅 2, 𝑆 2 = 2𝜋𝑅                               22 bool Find(int n) {
                       4                                             23    for (int i = 2; i < n; i++) {
                  𝑉3 = 𝜋𝑅 3, 𝑆 3 = 4𝜋𝑅 2                             24       p3 ndir = cross (p[0] ‐ p[i], p[1] ‐ p[i]);
                       3
                                                                     25       if (ndir == p3(0,0,0)) continue;
                       1 2 4
                  𝑉4 = 𝜋 𝑅 , 𝑆 4 = 2𝜋 2 𝑅 3                          26       swap(p[i], p[2]);
                       2                                             27       for (int j = i + 1; j < n; j++) {
                                    2𝜋                 2𝜋            28          if (sgn(volume(0, 1, 2, j)) != 0) {
                  Generally, 𝑉𝑛 =      𝑉𝑛−2, 𝑆𝑛−1 =        𝑆𝑛−3
                                     𝑛               𝑛−2             29             swap(p[j], p[3]);
                  Where, 𝑆 0 = 2, 𝑉1 = 2, 𝑆 1 = 2𝜋, 𝑉2 = 𝜋           30             ins(0, 1, 2);
                                                                     31             ins(0, 2, 1);
     1.15 三维几何基础操作                                                   32             return 1;
                                                                     33    } } } return 0; }
 1   /* 右手系逆时针绕轴旋转, (𝑥, 𝑦, 𝑧)𝐴 = (𝑥 new, 𝑦 new, 𝑧 new )              34 mt19937 rng;
 2   new[i] += old[j] * A[j][i] */                                   35 bool solve() {
 3   void calc(p3 n, LD cosw) {                                      36    face.clear();
 4       LD sinw = sqrt(1 ‐ cosw * cosw);                            37    int n = (int) p.size();
 5       n = n.unit();                                               38    shuffle(p.begin(), p.end(), rng);
 6       for (int i = 0; i < 3; i++) {                               39    if (!Find(n)) return 0;
 7          int j = (i + 1) % 3, k = (j + 1) % 3;                    40    for (int i = 3; i < n; i++) add(i);
 8          LD x = n[i], y = n[j], z = n[k];                         41    return 1; }
 9          A[i][i] = (y * y + z * z) * cosw + x * x;
10          A[i][j] = x * y * (1 ‐ cosw) + z * sinw;
11          A[i][k] = x * z * (1 ‐ cosw) ‐ y * sinw; } }
12   p3 cross (p3 a, p3 b) { return p3(                                   1.17 最小覆盖球
13    a.y*b.z ‐ a.z*b.y, a.z*b.x ‐ a.x*b.z, a.x*b.y ‐ a.y*b.x);}     1 vector<p3> b; Circle calc() {
14   LD mix(p3 a, p3 b, p3 c) { return dot(cross(a, b), c); }        2 if(b.empty()) { return Circle(p3(0, 0, 0), 0);
15   struct l3 { p3 s, t; };                                         3 }else if(1 == b.size()) {return Circle(b[0], 0);
16   struct plane { // nor 为单位法向量, 离原点距离 m                           4 }else if(2 == b.size()) {
17       p3 nor; LD m;                                               5   return Circle((b[0] + b[1]) / 2, (b[0] ‐ b[1]).len() / 2);
18       plane(p3 r, p3 a) : nor(r.unit()), m(dot(nor, a)) {} };     6 }else if(3 == b.size()) {
19   // 除法注意除零；点到直线投影: 与二维一致                                         7   LD r = dis(b[0], b[1]) * dis(b[1], b[2]) * dis(b[2], b[0])
20   p3 project_to_plane(p3 a, plane b) { // 点到平面投影                            ↩→ / 2 / cross(b[0] ‐ b[2], b[1] ‐ b[2]).len();
21       return a + b.nor * (b.m ‐ dot(a, b.nor)); }                 8      return Circle(intersect({b[1] ‐ b[0], (b[1] + b[0]) / 2},
22   pair<p3, p3> l3_closest(l3 x, l3 y) { // 两直线最近点                           ↩→ {b[2] ‐ b[1], (b[2] + b[1]) / 2}, {cross(b[1] ‐ b[0],
23       LD a = dot(x.t ‐ x.s, x.t ‐ x.s);                                     ↩→ b[2] ‐ b[0]), b[0]}), r);
24       LD b = dot(x.t ‐ x.s, y.t ‐ y.s);                           9     }else { p3 o(intersect({b[1] ‐ b[0], (b[1] + b[0]) / 2},
25       LD e = dot(y.t ‐ y.s, y.t ‐ y.s);                                   ↩→ {b[2] ‐ b[0], (b[2] + b[0]) / 2}, {b[3] ‐ b[0], (b[3] +
26       LD d = a*e ‐ b*b; p3 r = x.s ‐ y.s;                                 ↩→ b[0]) / 2})); return Circle(o, (o ‐ b[0]).len()); } }
27       LD c = dot(x.t ‐ x.s, r), f = dot(y.t ‐ y.s, r);            10   Circle miniBall(int n) {
28       LD s = (b*f ‐ c*e) / d, t = (a*f ‐ c*b) / d;                11    Circle res(calc());
29       return {x.s + (x.t ‐ x.s)*s, y.s + (y.t ‐ y.s)*t}; }        12    for(int i = 0; i < n; i++) if(!in_circle(a[i], res)) {
30   p3 intersect(plane a, l3 b) { // 直线与平面交点                        13     b.push_back(a[i]); res = miniBall(i); b.pop_back();
31       LD t = dot(a.nor, a.nor*a.m ‐ b.s)/dot(a.nor, b.t ‐ b.s);   14     if (i) { p3 tmp = a[i]; memmove(a + 1, a, sizeof(p3) * i);
32       return b.s + (b.t ‐ b.s) * t; }                                       ↩→ a[0] = tmp; } }
33   // 平面与平面求交线                                                     15    return res; }
34   l3 intersect(plane a, plane b) {
35       p3 d = cross(a.nor, b.nor), d2 = cross(b.nor, d);
36       LD t = dot(d2, a.nor);
37
38
         p3 s = d2 * (a.m ‐ dot(b.nor*b.m, a.nor))/t + b.nor*b.m;
         return {s, s + d}; }
                                                                          2. Tree & Graph
                                                                                       √
39   // 三个平面求交点                                                           2.1   Hopcroft-Karp 𝑂 ( 𝑉 𝐸)
40   p3 intersect(plane a, plane b, plane c) {
                                                                      1   vector <int> E[N];
41   return intersect(a, intersect(b, c));
                                                                      2   vector <int> ml, mr, a, p;
42   p3 c1 (a.nor.x, b.nor.x, c.nor.x);
                                                                      3   void match (int nl, int nr) { // 1‐based
43   p3 c2 (a.nor.y, b.nor.y, c.nor.y);
                                                                      4      ml.assign(nl + 1, 0);
44   p3 c3 (a.nor.z, b.nor.z, c.nor.z);
                                                                      5      mr.assign(nr + 1, 0); // nr
45   p3 c4 (a.m, b.m, c.m);
                                                                      6      while (true) {
46   return 1 / mix(c1, c2, c3) * p3(mix(c4, c2, c3), mix(c1, c4,
                                                                      7         bool ok = 0;
       ↩→ c3), mix(c1, c2, c4)); }
                                                                      8         a.assign(nl + 1, 0);
                                                                      9         p.assign(nl + 1, 0); // nl
     1.16 三维凸包                                                       10         static queue <int> q;
                                                                     11         for (int i = 1; i <= nl; i++)
 1   vector <p3> p;                                                  12            if (!ml[i]) a[i] = p[i] = i, q.push(i);
 2   int mark[N][N], stp;                                            13         while (!q.empty()) {
 3   typedef array <int, 3> Face;                                    14            int x = q.front(); q.pop();
 4   vector <Face> face;                                             15            if (ml[a[x]]) continue;
 5   LD volume (int a, int b, int c, int d) {                        16            for (auto y : E[x]) {
 6      return mix (p[b] ‐ p[a], p[c] ‐ p[a], p[d] ‐ p[a]); }        17               if (!mr[y]) {
 7   void ins(int a, int b, int c) {face.push_back({a, b, c});}      18                  for (ok = 1; y; x = p[x])
 8   void add(int v) {                                               19                     mr[y] = x, swap(ml[x], y);
 9      vector <Face> tmp; int a, b, c; stp++;                       20                  break;
10      for (auto f : face) {                                        21               } else if (!p[mr[y]])
11         if (sgn(volume(v, f[0], f[1], f[2])) < 0) {               22                  q.push(y = mr[y]), p[y] = x, a[y] = a[x];
12            for (auto i : f) for (auto j : f)                      23            } } // while (!q.empty())
13               mark[i][j] = stp; }                                 24         if (!ok) break; } }
14         else {                                                    25   array<vector<int>, 2> min_edge_cover(int nl, int nr) {
15            tmp.push_back(f);}                                     26    match(nl, nr); vector <int> l, r;
16      } face = tmp;                                                27    for (int i = 1; i <= nl; i++) if (!a[i]) l.push_back(i);
17      for (int i = 0; i < (int) tmp.size(); i++) {                 28    for (int i = 1; i <= nr; i++) if (a[mr[i]]) r.push_back(i);
18         a = tmp[i][0], b = tmp[i][1], c = tmp[i][2];              29    return {l, r}; }
19         if (mark[a][b] == stp) ins(b, a, v);

---

<!-- page: 8 -->

     上海交通大学 Shanghai Jiao Tong University                        Nemesis                                                          Page 7
     2.2 Hungarian 𝑂 (𝑉 𝐸/𝑤)                                         33    fill(mx, mx + n, ‐1); fill(my, my + n, ‐1);
                                                                     34    fill(ly, ly + n, 0);
 1 using B = bitset<N>; B edge[N];
                                                                     35    for (int i = 0; i < n; i++)
 2 bool dfs(int x, B& unvis, vector<int>& match) {
                                                                     36       lx[i] = *max_element(g[i], g[i] + n);
 3    for(B z = edge[x];;) {
                                                                     37    for (int i = 0; i < n; i++) {
 4       z &= unvis;
                                                                     38       fill(slack, slack + n, INF);
 5       int y = z._Find_first();
                                                                     39       fill(visx, visx + n, 0); fill(visy, visy + n, 0);
 6       if(y == N) return 0;
                                                                     40       bfs(i); }
 7       unvis.reset(y);
                                                                     41    for (int i = 0; i < n; i++)
 8       if(!match[y] || dfs(match[y], unvis, match))
                                                                     42       if (g[i][mx[i]] == 0) mx[i] = ‐1;
 9          return match[y] = x, 1; } }
                                                                     43 } } km;
10 vector<int> match(int nl, int nr) {
11    B unvis; unvis.set();
12    vector<int> match(nr + 1), ret(nl + 1);                             2.6   欧拉回路
13    for(int i = 1;i <= nl;++i)
14       if(dfs(i, unvis, match)) unvis.set();                        1   /* comment : directed */
15    for(int i = 1;i <= nr;++i) ret[match[i]] = i;                   2   int e, cur[N]/*, deg[N]*/;
16    return ret[0] = 0, ret; }                                       3   vector<int>E[N];
                                                                      4   int id[M]; bool vis[M];
                                                                      5   stack<int>stk;
     2.3 Shuffle 一般图最大匹配 𝑂 (𝑉 𝐸)                                      6   void dfs(int u) {
 1 int n, m, mat[N], vis[N]; vector<int> E[N];
                                                                      7      for (cur[u]; cur[u] < E[u].size(); cur[u]++) {
 2 bool dfs(int tim, int x) {
                                                                      8         int i = cur[u];
 3    shuffle(E[x].begin(), E[x].end(), rng);                         9         if (vis[abs(E[u][i])]) continue;
 4    vis[x] = tim;                                                  10         int v = id[abs(E[u][i])] ^ u;
 5    for (auto y : E[x]) {                                          11         vis[abs(E[u][i])] = 1; dfs(v);
 6       int z = mat[y]; if (vis[z] == tim) continue;                12         stk.push(E[u][i]); }
 7       mat[x] = y, mat[y] = x, mat[z] = 0;                         13   }// dfs for all when disconnect
 8       if (!z || dfs(tim, z)) return true;                         14   void add(int u, int v) {
 9       mat[x] = 0, mat[y] = z, mat[z] = y; }                       15      id[++e] = u ^ v; // s = u
10    return false; }                                                16      E[u].push_back(e); E[v].push_back(‐e);
11 int main() { // 暗含二分图性质跑一次即可
                                                                     17   /*    E[u].push_back(e); deg[v]++; */
12    for (int _ = 0; _ < 10; _++) {                                 18   } bool valid() {
13       fill(vis + 1, vis + n + 1, 0);                              19      for (int i = 1; i <= n; i++)
14       for (int i = 1; i <= n; ++i) if (!mat[i]) dfs(i, i);}}      20         if (E[i].size() & 1) return 0;
                                                                     21   /*      if (E[i].size() != deg[i]) return 0;*/
                                                                     22      return 1;}
     2.4 极大团计数
 1   // 0下标, 需删除自环 (即确保 𝐸𝑖𝑖 = 0, 补图要特别注意)                                 2.7   2-SAT, 强连通分量 / Bitset Kosaraju
 2   // 极大团计数, 最坏情况O(3^(n/3))
 3   ll ans; ull E[64]; #define bit(i) (1ULL << (i))                  1   int stp, sccs, top; // N 开 **两倍**
 4   void dfs(ull P, ull X, ull R) { // 不要方案时可去掉R                     2   int dfn[N], low[N], scc[N], stk[N], ans[N];
 5      if (!P && !X) { ++ans; sol.pb(R); return; }                   3   void add(int x, int a, int y, int b) { // 注意连边对称
 6      ull Q = P & ~E[__builtin_ctzll(P | X)];                       4      E[x << 1 | a].push_back(y << 1 | b); }
 7      for (int i; i = __builtin_ctzll(Q), Q; Q &= ~bit(i)) {        5   void tarjan(int x) {
 8         dfs(P & E[i], X & E[i], R | bit(i));                       6      dfn[x] = low[x] = ++stp;
 9         P &= ~bit(i), X |= bit(i); }}                              7      stk[top++] = x;
10   ans = 0; dfs(n == 64 ? ~0ULL : bit(n) ‐ 1, 0, 0);                8      for (auto y : E[x]) {
                                                                      9         if (!dfn[y])
                                                                     10            tarjan(y), low[x] = min(low[x], low[y]);
     2.5 KM 最大权匹配 𝑂 (𝑉 3 )                                           11         else if (!scc[y])
                                                                     12            low[x] = min(low[x], dfn[y]); }
 1   struct KM {
                                                                     13      if (low[x] == dfn[x]) {
 2   int n, nl, nr;
                                                                     14         sccs++;
 3   T g[N][N], lx[N], ly[N], slack[N];
                                                                     15         do scc[stk[‐‐top]] = sccs;
 4   int mx[N], my[N], visx[N], visy[N], pre[N], q[N], ql, qr;
                                                                     16         while (stk[top] != x); } }
 5   int check(int i) {
                                                                     17   bool solve() {
 6      visy[i] = 1;
                                                                     18      int cnt = n + n; stp = top = sccs = 0;
 7      if (~my[i]) {
                                                                     19      fill(dfn, dfn + cnt + 1, 0); fill(scc, scc + cnt + 1, 0);
 8          q[qr++] = my[i], visx[my[i]] = 1;
                                                                     20      for (int i = 0; i < cnt; ++i) if (!dfn[i]) tarjan(i);
 9          return 0; }
                                                                     21      for (int i = 0; i < n; ++i) {
10      while (~i) my[i] = pre[i], swap(i, mx[pre[i]]);
                                                                     22         if (scc[i << 1] == scc[i << 1 | 1]) return false;
11      return 1; }
                                                                     23         ans[i] = (scc[i << 1 | 1] < scc[i << 1]); }
12   void bfs(int s) {
                                                                     24      return true; }
13      ql = 0, qr = 1;
14      q[ql] = s, visx[s] = 1;
15      for (T d; ; ) {                                               1 bitset<N> e[N], re[N], vis; vector<int> sta;
16          while (ql < qr)                                           2 void dfs0(int x, bitset<N> e[]) {
17          for (int v = 0, u = q[ql++]; v < n; v++)                  3    vis.reset(x);
18          if (!visy[v] && slack[v] >= (d=lx[u]+ly[v]‐g[u][v])) {    4    while (true) {
19             pre[v] = u;                                            5       int go = (e[x] & vis)._Find_first();
20             if (d) slack[v] = d; else if (check(v)) return;        6       if(go == N) break;
21          } d = INF;                                                7       dfs0(go, e); }
22          for (int i = 0; i < n; i++)                               8    sta.push_back(x); }
23             if (!visy[i]) d = min(d, slack[i]);                    9 vector<vector<int>> solve() { // re 需要连好反向边
24          for (int i = 0; i < n; i++) {                            10    vis.set();
25             if (visy[i]) ly[i] += d; else slack[i] ‐= d;          11    for(int i = 1;i <= n;++i) if(vis.test(i)) dfs0(i, e);
26             if (visx[i]) lx[i] ‐= d; }                            12    vis.set();
27          for (int i = 0; i < n; i++)                              13    auto s = sta;
28             if (!visy[i] && !slack[i] && check(i)) return;        14    vector<vector<int>> ret;
29      } }                                                          15    for(int i = n ‐ 1;i >= 0;‐‐i) if(vis.test(s[i])) {
30   void solve() {                                                  16       sta.clear(), dfs0(s[i], re), ret.push_back(sta); }
31      n = max(nl, nr); // always compute a full matching           17    return ret; }
32      fill(pre, pre + n, ‐1);

---

<!-- page: 9 -->

     上海交通大学 Shanghai Jiao Tong University                      Nemesis                                                         Page 8
     2.8 Tarjan 点双，边双
 1   /** 求割边 **/                                                    1   struct edge { int v, nxt; value f; } e[M * 2];
 2   // 注意！建立边双树或者圆方树后【边表大小】是否开够                                    2   int ecnt = 1, head[N], cur[N];
 3   // 求边双：无视掉 bri 后搜出每个连通块，记得多测                                   3   void add(int u, int v, value f) {
 4   int DFN[N], low[N], dfscnt; // clear DFN low bri dfscnt        4      e[++ecnt] = {v, head[u],         f}; head[u] = ecnt;
 5   bool bri[M << 1]; // 注意此处是边数                                   5      e[++ecnt] = {u, head[v], (value)0}; head[v] = ecnt; }
 6   void tarjan(int x, int last) { // last 是边                      6   int n, S, T, q[N], tag[N];
 7      DFN[x] = low[x] = ++dfscnt;                                 7   bool bfs() {
 8      for (int i = head[x], d; i; i = h[i].next) {                8      for (int i = S; i <= T; i++) tag[i] = 0; // S‐T?
 9         d = h[i].node;                                           9      int he = 0, ta = 1; q[0] = T; tag[T] = 1;
10         if (!DFN[d]) {                                          10      while (he < ta) {
11            tarjan(d, i);                                        11         int x = q[he++]; if (x == S) return true;
12            low[x] = min(low[x], low[d]);                        12         for (int o = head[x]; o; o = e[o].nxt)
13            if (low[d] > DFN[x]) bri[i] = bri[i ^ 1] = 1;        13            if (e[o ^ 1].f && !tag[e[o].v])
14         } else if (DFN[d] < DFN[x] && ((i ^ 1) != last))        14                tag[e[o].v] = tag[x] + 1, q[ta++] = e[o].v;
15            low[x] = min(low[x], DFN[d]); } }                    15      } return false; }
16   /** 建立圆方树+求割点 **/                                             16   value dfs(int x, value flow) {
17   int is_cut[N], DFN[N], low[N], dfscnt, pcnt;                  17      if (x == T) return flow;
18   int stk[N], dep; // clear DFN low is_cut dfscnt, let pcnt=n   18      value used = 0;
19   void tarjan(int x, int fa) {                                  19      for (int &o = cur[x]; o; o = e[o].nxt) {
20      int child = 0;                                             20         if (e[o].f && tag[x] ‐ 1 == tag[e[o].v]) {
21      DFN[x] = low[x] = ++dfscnt; stk[++dep] = x;                21            value ret = dfs(e[o].v, min(flow ‐ used, e[o].f));
22      #define head org // org 是原图表头，tr 是圆方树表头                    22            if (ret) {
23      for (int i = head[x], d; i; i = h[i].next) {               23                e[o].f ‐= ret; e[o ^ 1].f += ret;
24         d = h[i].node;                                          24                used += ret;
25         if (!DFN[d]) {                                          25                if (used == flow) return flow;
26            ++child; tarjan(d, x);                               26      } } } return used; }
27            low[x] = min(low[x], low[d]);                        27   value dinic() {
28            if (low[d] >= DFN[x]) {                              28      value ans = 0;
29               is_cut[x] = true;                                 29      while (bfs()) {
30               ++pcnt; // square node index                      30         for (int i = S; i <= T; i++) cur[i] = head[i]; // S‐T?
31               int j = 0, sz = 1;                                31         ans += dfs(S, INF);
32               do {                                              32      } return ans; }
33                  j = stk[dep‐‐];
34                  addedge(pcnt, j, tr);
35                  ++sz;
                                                                        2.11 原始对偶费用流
36               } while (j != d);                                1     bool bfs() {
37               addedge(pcnt, x, tr); }                          2       for (int i = S; i <= T; i++) cur[i] = head[i]; // S‐T?
38         } else if (DFN[d] < low[x]) low[x] = DFN[d]; }         3       for (int i = S; i <= T; i++) dep[i] = 0; // S‐T?
39      #undef head                                               4       dep[S] = 1; queue<int> q; q.push(S);
40      if (!fa && child == 1) is_cut[x] = false; }               5       while (!q.empty()) {
                                                                  6         int x = q.front(); q.pop();
                                                                  7         for (int i = head[x]; i; i = e[i].nxt) {
   2.9 Dominator Tree 支配树                                         8           auto [nxt, v, w, f] = e[i];
 1 vector<int> G[MAXN], R[MAXN], son[MAXN];                       9           if (f && h[v] == h[x] + w && !dep[v]) {
 2 int ufs[MAXN]; // R 是反图，son 存的是 sdom 树上的儿子                    10             dep[v] = dep[x] + 1, q.push(v);
 3 int idom[MAXN], sdom[MAXN], anc[MAXN];                        11       } } } return !!dep[T]; }
 4 // anc: sdom的dfn最小的祖先                                         12     int dfs(int x, int flow) {
 5 int pr[MAXN], dfn[MAXN], id[MAXN], stamp;                     13       if (x == T) return flow;
 6 int findufs(int x) { if (ufs[x] == x) return x;               14       int used = 0, ret;
 7    int t = ufs[x]; ufs[x] = findufs(ufs[x]);                  15       for (int &i = cur[x]; i; i = e[i].nxt) {
 8    if (dfn[sdom[anc[x]]] > dfn[sdom[anc[t]]])                 16         auto [nxt, v, w, f] = e[o];
 9        anc[x] = anc[t];                                       17         if (dep[v] == dep[x] + 1 && h[v] == h[x] + w
10    return ufs[x]; }                                           18         && f && (ret = dfs(d, min(flow ‐ used, f))) ) {
11 void dfs(int x) {                                             19           e[i].f ‐= ret; e[i ^ 1].f += f; used += ret;
12    dfn[x] = ++stamp; id[stamp] = x; sdom[x] = x;              20           if (flow == used) break;
13    for (int y : G[x]) if (!dfn[y]) { pr[y] = x; dfs(y); } } 21         } } return used; }
14 void get_dominator(int n) {                                   22     typedef pair <value, int> pii; // Unusual!
15    for (int i = 1; i <= n; i++) ufs[i] = anc[i] = i;          23     pii solve() { // return {cost, flow}
16    dfs(1);                                                    24       value cost = 0; int flow = 0;
17    for (int i = n; i > 1; i‐‐) { int x = id[i];               25       for (int i = S; i <= T; i++) h[i] = 0; // S‐T?
18        for (int y : R[x]) if (dfn[y]) { findufs(y);           26       for (bool first = true; ; ) {
19            if (dfn[sdom[x]] > dfn[sdom[anc[y]]])              27         priority_queue<pii, vector<pii>, greater<pii>> q;
20                 sdom[x] = sdom[anc[y]]; }                     28         for (int i = S; i <= T; i++) dis[i] = INF_value; // S‐T?
21        son[sdom[x]].push_back(x); ufs[x] = pr[x];             29         dis[S] = 0;
22        for (int u : son[pr[x]]) { findufs(u);                 30         if (first) {
23            idom[u] = (sdom[u] == sdom[anc[u]] ?               31           // TODO: SSSP, Bellman‐Ford / DP on DAG
24                 pr[x] : anc[u]); }                            32           first = false;
25        son[pr[x]].clear(); }                                  33         } else { q.push({0, S});
26    for (int i = 2; i <= n; i++) { int x = id[i];              34           while (!q.empty()) {
27        if (idom[x] != sdom[x]) idom[x] = idom[idom[x]];       35             auto [d, x] = q.top(); q.pop();
28        son[idom[x]].push_back(x); } }                         36             if (dis[x] != d) continue;
                                                                 37             for (int o = head[x]; o; o = e[o].nxt) {
                                                                 38               value w = d + e[o].w + h[x] ‐ h[e[o].v];
   2.10 Dinic 最大流                                                39               if (e[o].f > 0 && dis[e[o].v] > w) {
   复杂度证明思路 假设 dist 为残量网络上的距离. Dinic 一轮增广会找到一个极 40                                    dis[e[o].v] = w; q.push({w, e[o].v});
   大的长度为 dist(𝑠, 𝑡) 的增广路集合 blocking flow, 增广后 dist(𝑠, 𝑡) 将会增大. 因 41         } } } } if (dis[T] >= INF_value) break;
   此只有 𝑂 (𝑉 ) 轮; 如果一轮增广是 𝑂 (𝑉 𝐸) 的, 总复杂度是 𝑂 (𝑉 2 𝐸) .            42         // 所有点必须可达, 否则加 h[i] += min(dis[i], dis[T])
   单位流量网络 在 0-1 流量图上 Dinic 有更好的性质.                               43         for (int i = S; i <= T; i++) h[i] += dis[i]; // S‐T?
                                                                 44         int f = 0; while (bfs()) f += dfs(S, INF_int);
       • 复杂度为 𝑂 (min{𝑉 2/3, 𝐸 1/2 }𝐸) .
                                                                 45         cost += f * h[T]; flow += f;
       • dist(𝑠, 𝑡) = 𝑑 , 残量网络上至多还存在 𝐸/𝑑 的流.
                                                                 46       } return {cost, flow}; }
       • 每个点只有一个入/出度时复杂度 𝑂 (𝑉 1/2 𝐸) , 例如 Hopcroft-Karp.

---

<!-- page: 10 -->

     上海交通大学 Shanghai Jiao Tong University                                Nemesis                                                                     Page 9
     2.12 虚树                                                                      整数线性规划转费用流
 1    bool has_root = false; for (int i : q) has_root |= (i == 1);                首先将约束关系转化为所有变量下界为 0, 上界没有要求, 并满足一些等式,
 2    if (!has_root) q.push_back(1);                                                   Í
                                                                                  每个变量在均在等式左边且出现恰好两次                 , 系数为 +1 和 −1, 优化目标为
 3    sort(q.begin(), q.end(), [](int u, int v) {                                 max 𝑣𝑖 𝑥𝑖 的形式. 将等式看做点, 等式i右边的值 𝑏𝑖 若为正, 则 𝑆 向 𝑖
 4       return dfn[u] < dfn[v]; });                                              连边 (𝑏𝑖 , 0) , 否则i向T连边 (−𝑏𝑖 , 0) . 将变量看做边, 记变量 𝑥𝑖 的上界为
 5    int top = 0;                                                                𝑚𝑖 (无上界则 𝑚𝑖 = 𝑖𝑛𝑓 ), 将 𝑥𝑖 系数为 +1 的那个等式 𝑢 向系数为 −1 的等
 6    for (int x : q) {                                                           式 𝑣 连边 (𝑚𝑖 , 𝑣 𝑖 ) .
 7       used.push_back(x);                                                       2.14 Gomory-Hu 无向图最小割树 𝑂 (𝑉 3 𝐸)
 8       if (top == 0) stk[++top] = x;
                                                                                  每次随便找两个点 𝑠, 𝑡 求在原图的最小割, 在最小割树上连 (𝑠, 𝑡, 𝑤 cut ) , 递
 9       else { int lca = LCA(stk[top], x); used.push_back(lca);
                                                                                  归对由割集隔开的部分继续做. 在得到的树上, 两点最小割即为树上瓶颈路.
10          while (top > 1 && dep[lca] < dep[stk[top ‐ 1]]) {
                                                                                  实现时, 由于是随意找点, 可以写为分治的形式.
11             h[stk[top ‐ 1]].push_back(stk[top]); ‐‐top; }
12          if (dep[lca] < dep[stk[top]])                                         2.15 Stoer-Wagner 无向图最小割 𝑂 (𝑉 𝐸 + 𝑉 2 log 𝑉 )
13             h[lca].push_back(stk[top‐‐]);                                  1   const int N = 601;
14          if (stk[top] != lca) stk[++top] = lca;                            2   int f[N], siz[N], G[N][N];
15          stk[++top] = x; } }                                               3   int getf(int x) {return f[x] == x ? x : f[x] = getf(f[x]);}
16    while (‐‐top) // assert (top)                                           4   int dis[N], vis[N], bin[N];
17       h[stk[top]].push_back(stk[top + 1]);                                 5   int n, m;
18    LL ans = solve(1, 0);                                                   6   int contract(int &s, int &t) { // Find s,t
19    for (auto i : used) h[i].clear();                                       7      memset(vis, 0, sizeof(vis));
                                                                              8      memset(dis, 0, sizeof(dis));
     2.13 网络流总结                                                               9      int i, j, k, mincut, maxc;
                                                                             10      for (i = 1; i <= n; i++) {
     最小割集, 最小割必须边以及可行边
                                                                             11         k = ‐1; maxc = ‐1;
     最小割集 从 𝑆 出发, 在残余网络中BFS所有权值非 0 的边 (包括反向边), 12                                       for (j = 1; j <= n; j++)
     得到点集 {𝑆 } , 另一集为 {𝑉 } − {𝑆 } .                                          13            if (!bin[j] && !vis[j] && dis[j] > maxc) {
     最小割集必须点 残余网络中S直接连向的点必在S的割集中, 直接连向T的 14                                                    k = j;
     点必在T的割集中; 若这些点的并集为全集, 则最小割方案唯一.                                         15                maxc = dis[j]; }
     最小割可行边 在残余网络中求强联通分量, 将强联通分量缩点后, 剩余的 16                                             if (k == ‐1) return mincut;
     边即为最小割可行边, 同时这些边也必然满流.                                                  17         s = t; t = k; mincut = maxc; vis[k] = true;
     最小割必须边 在残余网络中求强联通分量, 若S出发可到u, T出发可到v, 18                                           for (j = 1; j <= n; j++)
     等价于 scc𝑆 = scc𝑢 且 scc𝑇 = scc𝑣 , 则该边为必须边.                                19            if (!bin[j] && !vis[j]) dis[j] += G[k][j];
     常见问题                                                                    20      } return mincut; }
     最大权闭合子图 点权, 限制条件形如: 选择A则必须选择B, 选择B则必须 21                                     const int inf = 0x3f3f3f3f;
     选择C, D. 建图方式: B向A连边, CD向B连边. 求解: S向正权点连边, 负权点 22                             int solve() {
     向T连边, 其余边容量 ∞, 求最小割, 答案为S所在最小割集.                                        23      int mincut, i, j, s, t, ans;
     二次布尔型 (文理分科) 𝑛 个点分为两类, 𝑖 号点有 𝑙𝑖 或 𝑟 𝑖 的代价Í                     , 𝑖, 𝑗 同 24      for (mincut = inf, i = 1; i < n; i++) {
     属一侧分别获得 𝑙𝑖 𝑗 或 𝑟 𝑖 𝑗 的代价, 问最小代价. 𝐿 → 𝑖 : (𝑙𝑖 + 1/2 𝑗 𝑙𝑖 𝑗 ) , 25                   ans = contract(s, t);
                            Í                                                           bin[t] = true;
     𝑖 → 𝑅 : (𝑟𝑖 + 1/2 𝑗 𝑟𝑖 𝑗 ) , 𝑖 ↔ 𝑗 : 1/2(𝑙𝑖 𝑗 + 𝑟𝑖 𝑗 ) . 实现时边权乘 2 为整 26
                                                                             27         if (mincut > ans) mincut = ans;
     数, 求解后答案除 2 为整数. 图拆点可以看作二分图.                                            28         if (mincut == 0) return 0;
     如果是二元限制是分类不同时有一个代价 𝑑𝑖 𝑗 ，建图可以简化为 𝐿 → 𝑖 : 𝑙𝑖 , 29                                   for (j = 1; j <= n; j++)
     𝑖 → 𝑅 : 𝑟𝑖 , 𝑖 ↔ 𝑗 : 𝑑𝑖 𝑗 . 经典例子: xor 最小值, 按位拆开建图.                      30            if (!bin[j]) G[s][j] = (G[j][s] += G[j][t]);
     混合图欧拉回路 把无向边随便定向, 计算每个点的入度和出度, 如果有 31                                           } return mincut; }
     某个点出入度之差 deg𝑖 = in𝑖 − out𝑖 为奇数, 肯定不存在欧拉回路. 对 32                              int main() {
     于 deg𝑖 > 0 的点, 连接边 (𝑖,𝑇 , deg𝑖 /2) ; 对于 deg𝑖 < 0 的点, 连接边 33                     cin >> n >> m;
                                                                             34      for (int i = 1; i <= n; ++i) f[i] = i, siz[i] = 1;
     (𝑆, 𝑖, −deg𝑖 /2) . 最后检查是否满流即可.
                                                                             35      for (int i = 1, u, v, w; i <= m; ++i) {
     二物流 水源 𝑆 1 , 水汇 𝑇1 , 油源 𝑆 2 , 油汇 𝑇2 , 每根管道流量共用. 求流 36                              cin >> u >> v >> w;
     量和最大. 建超级源 𝑆𝑆 1 汇 𝑇𝑇1 , 连边 𝑆𝑆 1 → 𝑆 1 ,𝑆𝑆 1 → 𝑆 2 ,𝑇1 → 37                         int fu = getf(u), fv = getf(v);
     𝑇𝑇1 ,𝑇2 → 𝑇𝑇1 , 设最大流为 𝑥 1 . 建超级源 𝑆𝑆 2 汇 𝑇𝑇2 , 连边 𝑆𝑆 2 → 𝑆 1 , 38                   if (fu != fv) {
     𝑆𝑆 2 → 𝑇2 ,𝑇1 → 𝑇𝑇2 , 𝑆 2 → 𝑇𝑇2 , 设最大流为 𝑥 2 . 则最大流中水流量 39                             if (siz[fu] > siz[fv]) swap(fu, fv);
      𝑥 1 +𝑥 2      𝑥 1 −𝑥 2
          2 , 油流量       2 .                                                  40            f[fu] = fv, siz[fv] += siz[fu]; }
     无源汇有上下界可行流 每条边 (𝑢, 𝑣) 有一个上界容量 𝐶𝑢,𝑣 和下界容 41                                         G[u][v] += w, G[v][u] += w; }
     量 𝐵𝑢,𝑣 , 我们让下界变为 0, 上界变为 𝐶𝑢,𝑣 − 𝐵𝑢,𝑣 , 但这样做流量不守恒. 42                            cout << (siz[getf(1)] != n ? 0 : solve()); }
     建立超级源点
          Í        Í 和超级汇点 𝑇𝑇 , 用 𝑑𝑢𝑖 来记录每个节点的流量情况,
                  𝑆𝑆
     𝑑𝑢𝑖 = 𝐵 𝑗,𝑖 − 𝐵𝑖,𝑗 , 添加一些附加弧. 当 𝑑𝑢𝑖 > 0 时, 连边 (𝑆𝑆, 𝑖, 𝑑𝑢𝑖 ) ;
     当 𝑑𝑢𝑖 < 0 时, 连边 (𝑖,𝑇𝑇 , −𝑑𝑢𝑖 ) . 最后对 (𝑆𝑆,𝑇𝑇 ) 求一次最大流即可,                      2.16 弦图
     当所有附加边全部满流时 (即 maxflow == 𝑑𝑢𝑖 > 0) 时有可行解.                                    弦图的定义 连接环中不相邻的两个点的边称为弦. 一个无向图称为弦图,
     有源汇有上下界最大可行流 建立超级源点 𝑆𝑆 和超级汇点 𝑇𝑇 , 首先判断                                       当图中任意长度都大于 3 的环都至少有一个弦.
     是否存在可行流, 用无源汇有上下界可行流的方法判断. 增设一条从 𝑇 到 𝑆                                       单纯点 一个点称为单纯点当 {𝑣 } ∪ 𝐴(𝑣) 的导出子图为一个团. 任何一个
     没有下界容量为无穷的边, 那么原图就变成了一个无源汇有上下界可行流问                                           弦图都至少有一个单纯点, 不是完全图的弦图至少有两个不相邻的单纯点.
     题. 同样地建图后, 对 (𝑆𝑆,𝑇𝑇 ) 进行一次最大流, 判断是否有可行解. 如果
                                                                                  完美消除序列 一个序列 𝑣 1, 𝑣 2, ..., 𝑣 𝑛 满足 𝑣 𝑖 在 𝑣 𝑖 , · · · , 𝑣 𝑛 的诱导子图中
     有可行解, 删除超级源点 𝑆𝑆 和超级汇点 𝑇𝑇 , 并删去 𝑇 到 𝑆 的这条边, 再对
                                                                                  为一个单纯点. 一个无向图是弦图当且仅当它有一个完美消除序列.
     (𝑆,𝑇 ) 进行一次最大流, 此时得到的 maxflow 即为有源汇有上下界最大可
     行流.                                                                          最大势算法 从 𝑛 到 1 的顺序依次给点标号. 设 label𝑖 表示第 𝑖 个点与多
     有源汇有上下界最小可行流 建立超级源点 𝑆𝑆 和超级汇点 𝑇𝑇 , 和无源                                        少个已标号的点相邻, 每次选择 label 最大的未标号的点进行标号. 用桶维
     汇有上下界可行流一样新增一些边, 然后从SS到TT跑最大流. 接着加上边                                         护优先队列可以做到 𝑂 (𝑛 + 𝑚) .
     (𝑇 , 𝑆, ∞) , 再从 𝑆𝑆 到 𝑇𝑇 跑一遍最大流. 如果所有新增边都是满的, 则存在                             弦图的判定 判定最大势算法输出是否合法即可. 如果依次判断是否构成
     可行流, 此时 𝑇 到 𝑆 这条边的流量即为最小可行流.                                                 团, 时间复杂度为 𝑂 (𝑛𝑚) . 考虑优化, 设 𝑣 𝑖+1, · · · , 𝑣 𝑛 中所有与 𝑣 𝑖 相邻的
     有上下界费用流 如果求无源汇有上下界最小费用可行流或有源汇有上下                                             点依次为 𝑁 (𝑣 𝑖 ) = {𝑣 𝑗1, · · · , 𝑣 𝑗𝑘 } . 只需判断 𝑣 𝑗1 是否与 𝑣 𝑗2, · · · , 𝑣 𝑗𝑘 相
     界最小费用最大可行流, 用1.6.3.1/1.6.3.2 的构图方法, 给边加上费用即可.                                邻即可. 时间复杂度 𝑂 (𝑛 + 𝑚) .
     求有源汇有上下界最小费用最小可行流, 要先用1.6.3.3的方法建图, 先求出
     一个保证必要边满流情况下的最小费用. 如果费用全部非负, 那么这时的费                                          弦图的染色        完美消除序列从后往前染色, 染上出度的 mex.
     用就是答案. 如果费用有负数, 那么流多了可能更好, 继续做从 𝑆 到 𝑇 的流                                     最大独立集        完美消除序列从前往后能选就选.
     量任意的最小费用流, 加上原来的费用就是答案.
     费用流消负环 新建超级源SS汇TT, 对于所有流量非空的负权边e, 先流满                                        团数     最大团的点数. 一般图团数 ≤ 色数, 弦图团数 = 色数.
     (ans+=e.f*e.c, e.rev.f+=e.f, e.f=0), 再连边SS→e.to, e.from→TT,                  极大团      弦图的极大团一定为 {𝑥 } ∪ 𝑁 (𝑥) .
     流量均为e.f(>0), 费用均为0. 再连边T→S流量 ∞ 费用0. 此时没有负环了.
                                                                                  最小团覆盖        用最少的团覆盖所有的点. 设最大独立集为 {𝑝 1, . . . , 𝑝𝑡 } , 则
     做一遍SS到TT的最小费用最大流, 将费用累加ans, 拆掉T→S的那条边 (此
     边的流量为残量网络中S→T的流量). 此时负环已消, 再继续跑最小费用最
                                                                                  {𝑝 1 ∪ 𝑁 (𝑝 1 ), . . . , 𝑝𝑡 ∪ 𝑁 (𝑝𝑡 )} 为最小团覆盖.
                                                                                                          Î
     大流.                                                                          弦图 𝑘 染色计数                  𝑣 ∈𝑉 𝑘 − 𝑁 (𝑣) + 1.

---

<!-- page: 11 -->

     上海交通大学 Shanghai Jiao Tong University                         Nemesis                                                               Page 10
     区间图 每个顶点代表一个区间, 有边当且仅当区间有交. 区间图是弦图, 一 17                               l = r = 1, q[1] = rt, col[rt] = 2;
     个完美消除序列是右端点排序.                        18                               while (l <= r) {
                                                                    19         u = q[l++];
 1   vector <int> L[N];                                             20         for (int v : G[u]) if (!col[v]) {
 2   int seq[N], lab[N], col[N], id[N], vis[N];                     21            col[v] = 1, col[h[v]] = 2, p[v] = u, q[++r] = h[v];
 3   void mcs() {                                                   22            if (!h[v]) {
 4      for (int i = 0; i < n; i++) L[i].clear();                   23                while (u) u = h[p[v]], h[v] = p[v], h[p[v]] = v,
 5      fill(lab + 1, lab + n + 1, 0);                                                  ↩→ v = u;
 6      fill(id + 1, id + n + 1, 0);                                24                return true;
 7      for (int i = 1; i <= n; i++) L[0].push_back(i);             25            }
 8      int top = 0;                                                26         } else if (col[v] == 2) {
 9      for (int k = n; k; k‐‐) {                                   27            int t = lca(u, v); blossom(u, v, t), blossom(v, u,
10          int x = ‐1;                                                             ↩→ t);
11          for ( ; ; ) {                                           28         }
12             if (L[top].empty()) top ‐‐;                          29      } return false;
13             else {                                               30   } // for(int i=1;i<=n;i++)if(!h[i]&&match(i))++res;
14                x = L[top].back(), L[top].pop_back();
15                if (lab[x] == top) break;                              2.19 图论结论
16             }                                                         2.19.1 最小乘积问题原理
17          }
                                                                         每个元素有两个权值 {𝑥𝑖 } 和 {𝑦𝑖 } , 要求在某个限制下 (例如生成树, 二
18          seq[k] = x; id[x] = k;
19          for (auto v : E[x]) {                                        分 图 匹 配) 使 得 Σ𝑥 Σ𝑦 最 小. 对 于 任 意 一 种 符 合 限 制 的 选 取 方 法, 记
20             if (!id[v]) {                                             𝑋 = Σ𝑥𝑖 , 𝑌 = Σ𝑦𝑖 , 可看做平面内一点 (𝑋, 𝑌 ) . 答案必在下凸壳上, 找
21                L[++lab[v]].push_back(v);                              出该下凸壳所有点, 即可枚举获得最优答案. 可以递归求出此下凸壳所有点,
22                top = max(top, lab[v]);                                分别找出距 𝑥, 𝑦 轴最近的两点 𝐴, 𝐵 , 分别对应于 Σ𝑦𝑖 , Σ𝑥𝑖 最小. 找出距离
23             } } } }                                                   线段最远的点 𝐶 , 则 𝐶 也在下凸壳上, 𝐶 点满足 𝐴𝐵 × 𝐴𝐶 最小, 也即
24   bool check() {                                                           (𝑋𝐵 − 𝑋𝐴 )𝑌𝐶 + (𝑌𝐴 − 𝑌𝐵 )𝑋𝐶 − (𝑋𝐵 − 𝑋𝐴 )𝑌𝐴 − (𝑌𝐵 − 𝑌𝐴 )𝑋𝐴
25      fill(vis + 1, vis + n + 1, 0);
26      for (int i = n; i; i‐‐) {                                        最小, 后两项均为常数, 因此将所以权值改成 (𝑋 𝐵 − 𝑋𝐴 )𝑦𝑖 + (𝑌𝐵 − 𝑌𝐴 )𝑥𝑖 ,
27          int x = seq[i];                                              求同样问题 (例如最小生成树, 最小权匹配) 即可. 求出 𝐶 点以后, 递归 𝐴𝐶 ,
28          vector <int> to;                                             𝐵𝐶 .
29          for (auto v : E[x])                                          2.19.2 最小环
30             if (id[v] > i) to.push_back(v);                           无向图最小环: 每次floyd到 𝑘 时, 判断 1 到 𝑘 − 1 的每一个 𝑖, 𝑗 :
31          if (to.empty()) continue;
32          int w = to.front();                                                       ans = min{ans, 𝑑 (𝑖, 𝑗) + 𝐺 (𝑖, 𝑘) + 𝐺 (𝑘, 𝑗)}.
33          for (auto v : to) if (id[v] < id[w]) w = v;
34          for (auto v : E[w]) vis[v] = i;                              有向图最小环: 做完floyd后, 𝑑 (𝑖, 𝑖) 即为经过 𝑖 的最小环.
35          for (auto v : to)                                            2.19.3 度序列的可图性
36             if (v != w && vis[v] != i) return false;                  判断一个度序列是否可转化为简单图, 除了一种贪心构造的方法外, 下列
37      } return true; }                                                 方法更快速. EG定理: 将度序列从大到小排序得到 {𝑑𝑖 } , 此序列可转化
38   void color() {                                                      为简单图当且仅当 Σ𝑑𝑖 为偶数, 且对于任意的 1 ≤ 𝑘 ≤ 𝑛 − 1 满足
39      fill(vis + 1, vis + n + 1, 0);                                   Í𝑘                       Í𝑛
40      for (int i = n; i; i‐‐) {                                                        𝑖=𝑘+1 min(𝑘, 𝑑𝑖 ) .
                                                                           𝑖=1 𝑑𝑖 ≤ 𝑘 (𝑘 − 1) +
41          int x = seq[i];                                              2.19.4 切比雪夫距离与曼哈顿距离转化
42          for (auto v : E[x]) vis[col[v]] = x;                         曼哈顿转切比雪夫: (𝑥 + 𝑦, 𝑥 − 𝑦) , 适用于一些每次只能向四联通的格子走
                                                                                                𝑥+𝑦 𝑥 −𝑦
43          for (int c = 1; !col[x]; c++)                                一格的问题. 切比雪夫转曼哈顿: ( 2 , 2 ) , 适用于统计距离.
44             if (vis[c] != x) col[x] = c;                              2.19.5 树链的交
45      } }
                                                                     1 bool cmp(int a,int b){return dep[a]<dep[b];}
                                                                     2 path merge(path u, path v){
     2.17   Minimum Mean Cycle 最小平均值环 𝑂 (𝑛 2 )                       3    int d[4], c[2];
 1   // 点标号为 1, 2, · · · , 𝑛 , 0为虚拟源点向其他点连权值为0的单向边.                  4    if (!u.x||!v.x) return path(0, 0);
 2   // f[i][v] : 从 0 到 v 恰好经过 i 条路的最短路                              5    d[0]=lca(u.x,v.x); d[1]=lca(u.x,v.y);
 3   ll f[N][N] = {Inf}; int u[M], v[M], w[M]; f[0][0] = 0;          6    d[2]=lca(u.y,v.x); d[3]=lca(u.y,v.y);
 4   for(int i = 1; i <= n + 1; i ++)                                7    c[0]=lca(u.x,u.y); c[1]=lca(v.x,v.y);
 5      for(int j = 0; j < m; j ++)                                  8    sort(d,d+4,cmp); sort(c,c+2,cmp);
 6         f[i][v[j]] = min(f[i][v[j]], f[i ‐ 1][u[j]] + w[j]);      9    if (dep[c[0]] <= dep[d[0]] && dep[c[1]] <= dep[d[2]])
 7   double ans = Inf;                                              10       return path(d[2],d[3]);
 8   for(int i = 1; i <= n; i ++) {                                 11    else return path(0, 0); }
 9      double t = ‐Inf;
10      for(int j = 1; j <= n; j ++)                                     2.19.6 带修改MST
11         t = max(t, (f[n][i] ‐ f[j][i]) / (double)(n ‐ j));            维护少量修改的 MST (银川 21: 求有 16 个 ‘e1 or e2’ 的限制条件的MST)
12      ans = min(t, ans); }                                             找出必须边将修改边标 −∞, 在MST上的其余边为必须边, 以此缩点.
                                                                         找出无用边将修改边标 ∞, 不在MST上的其余边为无用边, 删除之.
                                                                         假设修改边数为 𝑘 , 操作后图中最多剩下 𝑘 + 1 个点和 2𝑘 条边.
     2.18 一般图最大匹配 - Blossom
                                                                         2.19.7 差分约束
1 int n, m, l, r, u, v, res, q[N], p[N], h[N], fa[N], col[N];            𝑥𝑟 − 𝑥𝑙 ≤ 𝑐 :add(l, r, c) 𝑥𝑟 − 𝑥𝑙 ≥ 𝑐 :add(r, l, ‐c)
2 vector<int> G[N];                                                      2.19.8 Segment Tree Beats
3 inline int find(int x) {return fa[x] == x ? x : fa[x] =
                                                                         区间 min, 区间求和. 维护最大值 𝑚 , 严格次大值 𝑠 以及最大值个数 𝑡 . 现在
       ↩→ find(fa[x]);}
                                                                         假设我们要让区间 [𝐿, 𝑅] 对 𝑥 取 min, 先在线段树中定位若干个节点, 对
 4 int lca(int u, int v) {
                                                                         于每个节点分三种情况讨论: 1, 当 𝑚 ≤ 𝑥 时直接退出; 2, 当 𝑠𝑒 < 𝑥 < 𝑚𝑎
 5    static int fl[N], tim; ++tim;                                      时, 只会影响到所有最大值, 所以把 𝑛𝑢𝑚 加上 𝑡 ∗ (𝑥 − 𝑚𝑎) , 把 𝑚𝑎 更新为
 6    while (fl[u] != tim) {
 7       if (u) fl[u] = tim, u = find(p[h[u]]);
                                                                         𝑥 , 打上标记退出; 3, 当 𝑠𝑒 ≥ 𝑥 时递归. 均摊 𝑂 (log2 𝑛) .
 8       swap(u, v);                                                     2.19.9 二分图
 9    } return u; }                                                      最 小 点 覆 盖 =最 大 匹 配 数 . 独 立 集 与 覆 盖 集 互 补 . 最 小 点 覆 盖 构 造 方
10 void blossom(int u, int v, int t) {                                   法: 对二分图流图求割集, 跨过的边指示最小点覆盖. Hall定理 𝐺 =
11    while (find(u) != t) {                                             (𝑋, 𝑌 , 𝐸), |𝑀 | = |𝑋 | ⇔ ∀𝑆 ⊆ 𝑋, |𝑆 | ≤ |𝐴(𝑆)| .
12       p[u] = v, v = h[u], fa[u] = fa[v] = t;                          2.19.10 稳定婚姻问题
13       if (col[v] == 1) col[v] = 2, q[++r] = v;                        男士按自己喜欢程度从高到底依次向每位女士求婚, 女士遇到更喜欢的男士
14       u = p[v]; } }                                                   时就接受他, 并抛弃以前的配偶. 被抛弃的男士继续按照列表向剩下的女士
15 bool match(int rt) {                                                  依次求婚, 直到所有人都有配偶. 算法一定能得到一个匹配, 而且这个匹配一
16    for (int i = 1; i <= n; i++) fa[i] = i, col[i] = 0;                定是稳定的. 时间复杂度 𝑂 (𝑛 2 ) .

---

<!-- page: 12 -->

上海交通大学 Shanghai Jiao Tong University                                       Nemesis                                                      Page 11
2.19.11   竞赛图 Landau’s Theorem                                                    𝐵 = {𝑥𝑖 |𝑆 ∪ {𝑥𝑖 } ∈ F2 }. 如果 𝑇 中某个元素 𝑥𝑖 ∉ 𝐴, 说明 𝑥𝑖 加进 𝑆 中形
                                                                                  成了某个” 环”, 从而不满足 F1 的限制. 考虑这个” 环” 上每个元素 𝑦 𝑗 , 满足
𝑛 个点竞赛图点按出度按升序排序, 前 𝑖 个点的出度之和不小于 𝑖 (𝑖 2−1) , 度
            𝑛 (𝑛−1)                                                               𝑆\{𝑦 𝑗 } ∪ {𝑥𝑖 } ∈ F1 , 将 𝑦 𝑗 向每个 𝑥𝑖 连边. 如果 𝑇 中某个元素 𝑥𝑖 ∉ 𝐵 ,
数总和等于           2   . 否则可以用优先队列构造出方案.                                             同理找出 𝑆 中每一个元素 𝑦 𝑗 使得 𝑆\{𝑦 𝑗 } ∪ {𝑥𝑖 } ∈ F2 , 将 𝑥𝑖 向 𝑦 𝑗 连边.
2.19.12   Ramsey Theorem R(3,3)=6, R(4,4)=18                                      现在求出从 𝐴 到 𝐵 的多源多汇最短路, 权值在点上, 若点属于 𝑇 则权值为
6 个人中存在 3 人相互认识或者相互不认识.                                                           正, 否则属于 𝑆 , 权值为负. 最短路上每个 𝑇 中的点放进 𝑆 , 𝑆 中的点放进 𝑇 ,
2.19.13 树的计数 Prufer序列                                                             则完成了一次增广. 由于每次增广路的起点和终点都在 𝑇 中, 所以每次增广
prufer编码长度为 𝑛 − 2, 且度数为 𝑑𝑖 的点在prufer编码中出现 𝑑𝑖 − 1 次.                               都会使得 |𝑆 | 增加1.
由树得到序列: 总共需要 𝑛 − 2 步, 第 𝑖 步在当前的树中寻找具有最小标号             最大拟阵交问题可以去掉权值直接求增广路.
的叶子节点, 将与其相连的点的标号设为Prufer序列的第 𝑖 个元素 𝑝𝑖 , 并将           2.19.22 双极定向
此叶子节点从树中删除, 直到最后得到一个长度为 𝑛 − 2 的Prufer 序列和一 1 //双极定向: 给定无向图和两个极点s,t, 要求将每条边定向后成为DAG,
个只有两个节点的树.                                               ↩→ 使得s可达所有点, 所有点均可达t
由序列得到树: 先将所有点的度赋初值为 1, 然后加上它的编号在Prufer序 2 //topo为定向后DAG的拓扑序, 边 (u,v) 定向为u‐>v当且仅当拓扑序
列中出现的次数, 得到每个点的度; 执行 𝑛 − 2 步, 第 𝑖 步选取具有最小标号              ↩→ 中u在v的前面.
的度为 1 的点 𝑢 与 𝑣 = 𝑝𝑖 相连, 得到树中的一条边, 并将 𝑢 和 𝑣 的度减一. 3 int n, dfn[N], low[N], stamp, p[N], preorder[N], topo[N];
最后再把剩下的两个度为 1 的点连边, 加入到树中.                          4 bool fucked = 0, sign[N]; vector<int> G[N];
相关结论: 𝑛 个点完全图, 每个点度数依次为 𝑑 1 ,𝑑 2 ,…,𝑑𝑛 , 这样生成树的棵树 5 void dfs(int x, int fa, int s, int t){
            (𝑛−2)!
为: (𝑑 −1)!(𝑑 −1)!...(𝑑 −1)! .                       6      dfn[x] = low[x] = ++stamp;
      1      2        𝑛
                                                                    𝑛 −1      7      preorder[stamp] = x, p[x] = fa;
左边有 𝑛 1 个点, 右边有 𝑛 2 个点的完全二分图的生成树棵树为 𝑛 1 2                                  × 8       if (x == s) dfs(t, x, s, t);
𝑛𝑛2 1 −1 .                                  9                                        for (int y : G[x]){
𝑚Í个连通块Î    , 每个连通块有 𝑐𝑖 个点, 把他们全部连通的生成树方案数: 10                                           if (x == s && y == t) continue;
( 𝑐𝑖 )𝑚−2 𝑐𝑖                               11                                           if (!dfn[y]){
                                                                             12            if (x == s) fucked = true;
2.19.14   有根树计数 1,1,2,4,9,20,48,115,286,719,1842,4766
                    Í𝑛      Í                                                13            dfs(y, x, s, t);
无标号 𝑎𝑛+1 = 1/𝑛 𝑘=1 ( 𝑑 |𝑘 𝑑 · 𝑎(𝑑)) · 𝑎(𝑛 − 𝑘 + 1)                           14            low[x] = min(low[x], low[y]); }
2.19.15 无根树计数                                                                15         else if (dfn[y] < dfn[x] && y != fa)
                      Í
𝑛 是奇数时, 有 𝑎𝑛 − 𝑛/2          𝑎   𝑎
                               𝑖 𝑛−𝑖 种不同的无根树          .
                                                                             16            low[x] = min(low[x], dfn[y]); } }
                      Í𝑖𝑛/2                                                  17   bool bipolar_orientation(int s, int t){
𝑛 时偶数时, 有 𝑎𝑛 − 𝑖 𝑎𝑖 𝑎𝑛−𝑖 + 21 𝑎𝑛/2 (𝑎𝑛/2 + 1) 种不同的无根树. 18                            G[s].push_back(t), G[t].push_back(s);
2.19.16 生成树计数 Kirchhoff’s Matrix-Tree Thoerem                                19      stamp = fucked = 0, dfs(s, s, s, t);
                                                                                     for (int i = 1; i <= n; i++)
Kirchhoff Matrix 𝑇 = 𝐷𝑒𝑔 − 𝐴, 𝐷𝑒𝑔 是度数对角阵, 𝐴 是邻接矩阵. 无向 20
                                                                             21         if (i != s && (!dfn[i] || low[i] >= dfn[i]))
图度数矩阵是每个点度数; 有向图度数矩阵是每个点入度.
                                                                             22            fucked = true;
邻接矩阵 𝐴[𝑢] [𝑣] 表示 𝑢 → 𝑣 边个数, 重边按照边数计算, 自环不计入度
                                                                             23      if (fucked) return false;
数.
无向图生成树计数: 𝑐 = |𝐾 的任意1个 𝑛 − 1 阶主子式 |                                          24      sign[s] = 0;//memset sign[] is not necessary
有向图外向树计数: 𝑐 = | 去掉根所在的那阶得到的主子式 |                                             25      int pre[n + 5], suf[n + 5]; // list
                                                                             26      suf[0] = s; pre[s] = 0, suf[s] = t;
2.19.17 有向图欧拉回路计数 BEST Thoerem                                               27      pre[t] = s, suf[t] = n + 1; pre[n + 1] = t;
                                        Ö                                    28      for (int i = 3; i <= n; i++){
                   ec(𝐺) = 𝑡 𝑤 (𝐺)           (deg(𝑣) − 1)!                   29         int v = preorder[i];
                                        𝑣 ∈𝑉                                 30         if (!sign[preorder[low[v]]]){ // insert before p[v]
其中 deg 为入度 (欧拉图中等于出度), 𝑡 𝑤 (𝐺) 为以 𝑤 为根的外向树的个数. 31                                          int P = pre[p[v]];
相关计算参考生成树计数.                                                                 32            pre[v] = P, suf[v] = p[v];
欧拉连通图中任意两点外向树个数相同: tv (𝐺) = tw (𝐺) .                                         33            suf[P] = pre[p[v]] = v; }
2.19.18 Tutte Matrix                                                         34         else{ // insert after p[v]
Tutte matrix 𝐴 of a graph 𝐺 = (𝑉 , 𝐸) :                                      35            int S = suf[p[v]];
                                                                             36            pre[v] = p[v], suf[x] = S;
                        
                        𝑥 𝑖 𝑗
                        
                                    if (𝑖, 𝑗) ∈ 𝐸 and 𝑖 < 𝑗                 37
                                                                             38
                                                                                           suf[p[v]] = pre[S] = v; }
                                                                                        sign[p[x]] = !sign[preorder[low[x]]]; }
                 𝐴𝑖 𝑗 = − 𝑥𝑖 𝑗 if (𝑖, 𝑗) ∈ 𝐸 and 𝑖 > 𝑗
                        
                        0                                                   39      for (int x = s, cnt = 0; x != n + 1; x = suf[x])
                                                 otherwise                  40         topo[++cnt] = x;
                                                                                     return true; }
where 𝑥𝑖 𝑗 are indeterminates. The determinant of this skew-symmetric 41
matrix is then a polynomial (in the variables 𝑥𝑖 𝑗 , 𝑖 < 𝑗 ): this coincides
with the square of the pfaffian of the matrix 𝐴 and is non-zero (as a             2.19.23
                                                                                       图中的环
polynomial) if and only if a perfect matching exists.                             没有奇环的图是二分图, 没有偶环的图是仙人掌. 判定没有奇环仅用深度奇
2.19.19 Edmonds Matrix                                                            偶性判即可; 判定没有偶环的图需要记录覆盖次数判定是否存在奇环有交.
Edmonds matrix 𝐴 of a balanced ( |𝑈 | = |𝑉 | ) bipartite graph 𝐺 =
(𝑈 , 𝑉 , 𝐸) :                 
                                                                                  3. Data Structure
                                  𝑥𝑖 𝑗
                                    (𝑢𝑖 , 𝑣 𝑗 ) ∈ 𝐸        3.1 非递归线段树
                         𝐴𝑖 𝑗 =                            3.1.1 区间加，区间求最大值
                                  0 (𝑢𝑖 , 𝑣 𝑗 ) ∉ 𝐸
                                                         1 void update(int l, int r, int d) {
where the 𝑥𝑖 𝑗 are indeterminates. 𝐺 有完美匹配当且仅当关于 𝑥𝑖 𝑗 的多
                                                         2      for (l += M‐1, r += M+1; l^r^1; l >>= 1, r >>= 1) {
项式 𝑑𝑒𝑡 (𝐴𝑖 𝑗 ) 不恒为 0. 完美匹配的个数等于多项式中单项式的个数.               3         if (l < M) {
     有向图无环定向, 色多项式
2.19.20                                                            4          t[l] = max(t[l*2], t[l*2+1]) + mark[l];
图的色多项式 𝑃𝐺 (𝑞) 对图 𝐺 的 𝑞 -染色计数.                                      5          t[r] = max(t[r*2], t[r*2+1]) + mark[r]; }
Triangle 𝐾3 : 𝑥 (𝑥 − 1)(𝑥 − 2)                                     6       if (~l & 1) { t[l ^ 1] += d; mark[l ^ 1] += d; }
Complete graph 𝐾𝑛 : 𝑥 (𝑥 − 1)(𝑥 − 2) · · · (𝑥 − (𝑛 − 1))           7       if (r & 1) { t[r ^ 1] += d; mark[r ^ 1] += d; } }
Tree with 𝑛 vertices : 𝑥 (𝑥 − 1) 𝑛−1                               8    for (; l; l >>= 1, r >>= 1)
Cycle 𝐶𝑛 : (𝑥 − 1)𝑛 + (−1)𝑛 (𝑥 − 1)                                9       if (l < M) t[l] = max(t[l*2], t[l*2+1]) + mark[l],
# acyclic orientations of an 𝑛 -vertex graph 𝐺 is (−1)𝑛 𝑃𝐺 (−1) .
                                                                  10                  t[r] = max(t[r*2], t[r*2+1]) + mark[r]; }
                                                                  11 int query(int l, int r) {
2.19.21 拟阵交问题                                                     12    int maxl = ‐INF, maxr = ‐INF;
拟阵定义: , 𝑆,𝑇 是独立集, 则 𝑆 子集是, 若 |𝑆 | > |𝑇 | , 则 𝑆 能扩充 𝑇 . 最大 13            for (l += M‐1, r += M+1; l^r^1; l >>= 1, r >>= 1) {
带权拟阵交问题: 全集 𝑈 中每个元素都有权值 𝑤 𝑖 . 设同一个全集 𝑈 上有两 14                              maxl += mark[l]; maxr += mark[r];
个满足拟阵性质的集族 F1 , F2 . 对于 𝑘 = 1..|𝑈 | , 分别求出一个集合 𝑆 , 满 15                    if (~l & 1) maxl = max(maxl, t[l ^ 1]);
足 𝑆 ∈ F1 ∩ F2 且 |𝑆 | 恰好为 𝑘 的前提下, 𝑆 中元素权值和最小.                      16       if ( r & 1) maxr = max(maxr, t[r ^ 1]); }
设集合大小为 𝑘 时已经求出了答案 𝑆 . 现在希望求出集合大小为 𝑘 + 1 17                              while (l) { maxl += mark[l]; maxr += mark[r];
的 答 案. 𝑈 中 所 有 元 素 分 为 两 个 集 合: 当 前 答 案 集 合 𝑆 , 和 剩 余 集 合 18               l >>= 1; r >>= 1; }
𝑇 = 𝑈 \𝑆 . 考虑 𝑇 中的某个元素 𝑥𝑖 . 记 𝐴 = {𝑥𝑖 |𝑆 ∪ {𝑥𝑖 } ∈ F1 }, 19             return max(maxl, maxr); }

---

<!-- page: 13 -->

     上海交通大学 Shanghai Jiao Tong University                      Nemesis                                                      Page 12
     3.2 点分治                                                       38    access(x), splay(x);
 1   vector<pair<int, int> > G[maxn];                              39    while (lch(x)) pushdown(x), x = lch(x);
 2   int sz[maxn], son[maxn], q[maxn];                             40    return splay(x), x;    }
                                                                   41 void link(int x, int y) {
 3   int pr[maxn], depth[maxn], rt[maxn][19], d[maxn][19];
 4   int cnt_all[maxn],sum_all[maxn],cnt[maxn][],sum[maxn][];      42    makeroot(x);
 5   bool vis[maxn], col[maxn];                                    43    if (findroot(y) != x) fa[x] = y; }
                                                                   44 void cut(int x, int y) {
 6   int getcenter(int o, int s) {
 7      int head = 0, tail = 0; q[tail++] = o;                     45    makeroot(x);
 8      while (head != tail) {                                     46    if (findroot(y) != x || fa[y] != x || lch(y)) return;
 9         int x = q[head++]; sz[x] = 1; son[x] = 0;               47    rch(x) = fa[y] = 0, pushup(x); }
10         for (auto [y, _] : G[x]) if (!vis[y] && y != pr[x]) {
11            pr[y] = x; q[tail++] = y; } }                             3.4   可持久化 Treap
12      for (int i = tail ‐ 1; i; i‐‐) {
13         int x = q[i]; sz[pr[x]] += sz[x];                        1   /* 不可持久化：把 copy(a, b) 换成 a = b, 并且去除新建结点 */
14         if (sz[x] > sz[son[pr[x]]]) son[pr[x]] = x; }            2   const int MX = (2e5 + 233) * 18 * 8;
15      int x = q[0];                                               3   int vcnt;
16      while (son[x] && sz[son[x]] * 2 >= s) x = son[x];           4   struct node {
17      return x; }                                                 5     int sz, ch[2], pri;
18   void getdis(int o, int k) {                                    6     int rev; LL sum; int val;
19      int head = 0, tail = 0; q[tail++] = o;                      7   } tr[MX];
20      while (head != tail) {                                      8   int newnode(int v) {
21         int x = q[head++]; sz[x] = 1; rt[x][k] = o;              9     static mt19937 rng(114514);
22         for (auto [y, w] : G[x]) if (!vis[y] && y != pr[x]) {   10     int x = ++vcnt;
23            pr[y]=x; d[y][k] = d[x][k] + w; q[tail++]=y; } }     11     tr[x].sz = 1;
24      for (int i = tail ‐ 1; i; i‐‐)sz[pr[q[i]]] += sz[q[i]];}   12     lch = rch = 0;
25   void build(int o, int k, int s, int fa) {                     13     tr[x].pri = rng();
26      int x = getcenter(o, s);                                   14     tr[x].sum = tr[x].val = v;
27      vis[x] = true; depth[x] = k; pr[x] = fa;                   15     tr[x].rev = false;
28      for (auto [y, w] : G[x]) if (!vis[y]) {                    16     return x; }
29         d[y][k] = w; pr[y] = x; getdis(y, k); }                 17   void copy(int x, int y) { tr[x] = tr[y]; }
30      for (auto [y, w] : G[x]) if (!vis[y])                      18   int merge(int x, int y) {
31         build(y, k + 1, sz[y], x); }                            19     if (!x || !y) return x + y;
32   void modify(int x) {                                          20     int z = ++vcnt;
33      int t = col[x] ? ‐1 : 1; cnt_all[x] += t;                  21     if (tr[x].pri < tr[y].pri) {
34      for (int u = pr[x], k = depth[x] ‐ 1; u; u = pr[u],k‐‐){   22       pushdown(x); copy(z, x);
35         sum_all[u] += t * d[x][k]; cnt_all[u] += t;             23       tr[z].ch[1] = merge(tr[z].ch[1], y);
36         sum[rt[x][k]][k] += t*d[x][k]; cnt[rt[x][k]][k] += t;   24     } else {
37      } col[x] ^= true; }                                        25       pushdown(y); copy(z, y);
38   int query(int x) { int ans = sum_all[x];                      26       tr[z].ch[0] = merge(x, tr[z].ch[0]); }
39      for (int u = pr[x], k = depth[x] ‐ 1; u; u = pr[u], k‐‐)   27     pushup(z); return z; }
40         ans += sum_all[u] ‐ sum[rt[x][k]][k]                    28   void split(int x, int dsz, int &r1, int &r2) {
41            + d[x][k] * (cnt_all[u] ‐ cnt[rt[x][k]][k]);         29     if (!x) {
42      return ans; }                                              30       r1 = r2 = 0;
                                                                   31     } else {
                                                                   32       pushdown(x);
     3.3 LCT 动态树                                                   33       if (tr[lch].sz + 1 <= dsz) {
 1   // 记得初始化 mn; 维护虚子树：access link cut pushup                     34         r1 = ++vcnt; copy(r1, x);
 2   int fa[MX], ch[MX][2], w[MX], mn[MX], mark[MX];               35         split(tr[r1].ch[1], dsz ‐ 1 ‐ tr[lch].sz
 3   int get(int x) {return x == ch[fa[x]][1];}                    36           , tr[r1].ch[1], r2);
 4   int nrt(int x) {return get(x) || x == ch[fa[x]][0];}          37         pushup(r1);
 5   void pushup(int x) {                                          38       } else {
 6      mn[x] = w[x];                                              39         r2 = ++vcnt; copy(r2, x);
 7      if (lch(x)) mn[x] = min(mn[x], mn[lch(x)]);                40         split(tr[r2].ch[0], dsz, r1, tr[r2].ch[0]);
 8      if (rch(x)) mn[x] = min(mn[x], mn[rch(x)]); }              41         pushup(r2); } } }
 9   void rev(int x) {mark[x] ^= 1, swap(lch(x), rch(x));}
10   void pushdown(int x) {
11
12
        if (mark[x]) {
           if (lch(x)) rev(lch(x));
                                                                        4. String
13         if (rch(x)) rev(rch(x));                                  4.1 最小表示法
14         mark[x] = false; } }                                    1 int min_pos(vector<int> a) { // 0‐based
15   void rot(int x) {                                             2    int n = a.size(), i = 0, j = 1, k = 0;
16      int f = fa[x], gf = fa[f];                                 3    while (i < n && j < n && k < n) {
17      int which = get(x), W = ch[x][!which];                     4       auto u = a[(i + k) % n]; auto v = a[(j + k) % n];
18      if (nrt(f)) ch[gf][ch[gf][1] == f] = x;                    5       int t = u > v ? 1 : (u < v ? ‐1 : 0);
19      ch[x][!which] = f, ch[f][which] = W;                       6       if (t == 0) k++; else {
20      if (W) fa[W] = f;                                          7          if (t > 0) i += k + 1; else j += k + 1;
21      fa[f] = x, fa[x] = gf;                                     8          if (i == j) j++;
22      pushup(f); }                                               9          k = 0; } } return min(i, j); }
23   void splay(int x) {
24      static int stk[MX];
25      int f = x, dep = 0; stk[++dep] = f;                          4.2 Manacher
26      while (nrt(f)) stk[++dep] = f = fa[f];                     1 // n为串长, 回文半径输出到p数组中，数组要开串长的两倍
27      while (dep) pushdown(stk[dep‐‐]);                          2 void manacher(const char *t, int n) {
28      while (nrt(x)) {                                           3    static char s[MAXN * 2];
29         if (nrt(f = fa[x])) rot(get(x) == get(f) ? f : x);      4    for (int i = n; i; i‐‐) s[i * 2] = t[i];
30         rot(x);                                                 5    for (int i = 0; i <= n; i++) s[i * 2 + 1] = '#';
31      } pushup(x); }                                             6    s[0] = '$'; s[(n + 1) * 2] = '\0'; n = n * 2 + 1;
32   void access(int x) {                                          7    int mx = 0, j = 0;
33      for(int y = 0 ; x ; x = fa[y = x])                         8    for (int i = 1; i <= n; i++) {
34         splay(x), rch(x) = y, pushup(x); }                      9       p[i] = (mx > i ? min(p[j * 2 ‐ i], mx ‐ i) : 1);
35   void makeroot(int x) {access(x), splay(x), rev(x);}          10       while (s[i ‐ p[i]] == s[i + p[i]]) p[i]++;
36   void split(int x, int y) {makeroot(x), access(y), splay(y);} 11       if (i + p[i] > mx) { mx = i + p[i]; j = i; } } }
37   int findroot(int x) {

---

<!-- page: 14 -->

     上海交通大学 Shanghai Jiao Tong University                     Nemesis                                                       Page 13
     4.3 KMP, exKMP                                                5       sa[stamp] = id[x]; last = x; }
 1 void kmp(char *s, int n) { // 1‐based
                                                                   6     for (int c = 0; c < 26; c++)
 2    fail[0] = fail[1] = 0;                                       7       if (ch[x][c]) dfs(ch[x][c]);
 3    for (int i = 1; i < n; i++) { int j = fail[i];               8    last = par[x]; }
                                                                   9 int main() { last = ++cnt; scanf("%s", s + 1);
 4       while (j && s[i + 1] != s[j + 1]) j = fail[j];
 5       if (s[i + 1] == s[j + 1]) fail[i + 1] = j + 1;           10     int n = strlen(s + 1); for (int i = n; i; i‐‐) {
 6       else fail[i + 1] = 0; } }                                11       expand(s[i] ‐ 'a'); id[last] = i; }
 7 void exkmp(char *s, int *a, int n) { // 1‐based
                                                                  12    vis[1] = true; for (int i = 1; i <= cnt; i++) if (id[i])
 8    int l = 0, r = 0; a[1] = n;                                 13          for (int x = i,pos = n; x && !vis[x]; x = par[x]){
 9    for (int i = 2; i <= n; i++) {                              14             vis[x] = true; pos ‐= val[x] ‐ val[par[x]];
10       a[i] = i > r ? 0 : min(r ‐ i + 1, a[i ‐ l + 1]);         15             ch[par[x]][s[pos + 1] ‐ 'a'] = x; }
11       while (i+a[i] <= n && s[1+a[i]] == s[i+a[i]]) a[i]++;    16    dfs(1); for (int i = 1; i <= n; i++)
12       if (i + a[i] ‐ 1 > r) {l = i; r = i + a[i] ‐ 1;}}}       17       printf("%d%c", sa[i], i < n ? ' ' : '\n');
                                                                  18    for (int i = 1; i < n; i++) printf("%d%c", height[i],
                                                                  19         i < n ? ' ' : '\n'); return 0; }
     4.4 AC 自动机
     注意代码是以 0 为根的，如果要 1-base 的话要改一下没有儿子时的逻辑。
                                                                       4.8   后缀数组
 1 int ch[MAXN][26], fail[MAXN], q[MAXN], cnt = 0;                 1   // height[i] = lcp(sa[i], sa[i ‐ 1])
 2 int insert(const char *c) { int x = 0; while (*c) {             2   // 如果有多组数据，全部都清空到 max(n, m)
 3    if (!ch[x][*c ‐ 'a']) ch[x][*c ‐ 'a'] = ++cnt;               3   constexpr int MAXN = 1000005;
 4    x = ch[x][*c++ ‐ 'a']; } return x; }                         4   void get_sa(char *s, int n, int *sa,
 5 void getfail() { int x, head = 0, tail = 0;                     5         int *rnk, int *height) { // 1‐based
 6    for (int c = 0; c < 26; c++) if (ch[0][c])                   6      static int buc[MAXN], id[MAXN], p[MAXN], t[MAXN];
 7          q[tail++] = ch[0][c];                                  7      int m = 300;
 8    while (head != tail) { x = q[head++];                        8      for (int i = 1; i <= n; i++) buc[rnk[i] = s[i]]++;
 9       for (int c = 0; c < 26; c++) { if (ch[x][c]) {            9      for (int i = 1; i <= m; i++) buc[i] += buc[i ‐ 1];
10             fail[ch[x][c]] = ch[fail[x]][c];                   10      for (int i = n; i; i‐‐) sa[buc[rnk[i]]‐‐] = i;
11             q[tail++] = ch[x][c];                              11      memset(buc, 0, sizeof(int) * (m + 1));
12          } else ch[x][c] = ch[fail[x]][c]; } } }               12      for (int k = 1, cnt = 0; cnt != n; k *= 2, m = cnt) {
                                                                  13         cnt = 0;
     4.5 Lydon Word Decomposition                                 14         for (int i = n; i > n ‐ k; i‐‐) id[++cnt] = i;
                                                                  15         for (int i = 1; i <= n; i++)
1 //满足s的最小后缀等于s本身的串s称为Lyndon串.                                    16            if (sa[i] > k) id[++cnt] = sa[i] ‐ k;
2 //等价于: s是它自己的所有循环移位中唯一最小的一个.                                    17         for (int i = 1; i <= n; i++) buc[p[i]=rnk[id[i]]]++;
3 //任意字符串s可以分解为 𝑠 = 𝑠 1𝑠 2𝑠𝑘 , 其中 𝑠𝑖 是Lyndon串,                    18         for (int i = 1; i <= m; i++) buc[i] += buc[i ‐ 1];
     ↩→ 𝑠𝑖 ≥ 𝑠𝑖+1 . 且这种分解方法是唯一的.
                                                                  19         for (int i = n; i; i‐‐) sa[buc[p[i]]‐‐] = id[i];
 4 //后缀排序后，排名的所有前缀最小值构成了 Ly 分解的左端点。                               20         memset(buc, 0, sizeof(int) * (m + 1));
 5 void mnsuf(char *s, int *mn, int n){ // 每个前缀的最小后缀              21         memcpy(t, rnk, sizeof(int) * (n + 1));
 6     //1 ‐ base ，求 Lyndon 分解去掉 mn 即可                            22         t[n + 1] = 0; // 记得清空 n + 1
 7      for(int i = 1; i <= n;)                                   23         cnt = 0; for (int i = 1; i <= n; i++) {
 8          int j = i + 1, k = i; mn[i] = i;                      24            if (t[sa[i]] != t[sa[i ‐ 1]] ||
 9          for(; j <= n && s[k] <= s[j]; j++){                   25               t[sa[i] + k] != t[sa[i ‐ 1] + k]) cnt++;
10                if(s[k] < s[j]) k = mn[j] = i;                  26            rnk[sa[i]] = cnt; } }
11                    else mn[j] = mn[k] + j ‐ k, k++;            27      for (int i = 1; i <= n; i++) sa[rnk[i]] = i;
12          for(; i <= k; i += j ‐ k) {} } } //                   28      for (int i = 1, k = 0; i <= n; i++) { if (k) k‐‐;
              ↩→ lyn+=s[i..i+k‐j‐1]
                                                                  29         if (rnk[i] > 1) while (sa[rnk[i] ‐ 1] + k <= n &&
13 void mxsuf(char *s, int *mx, int n){ // 每个前缀的最大后缀              30               s[i + k] == s[sa[rnk[i] ‐ 1] + k]) k++;
14     fill(mx + 1, mx + n + 1, 0); // 1 ‐ base                   31         height[rnk[i]] = k; } } // 两个都要判，否则会左/右越界
15     for(int i = 1; i <= n;){                                   32   char s[MAXN]; int sa[MAXN], rnk[MAXN], height[MAXN];
16         int j = i + 1, k = i; !mx[i] ? mx[i] = i : 0;          33   int main() { scanf("%s", s + 1); int n = strlen(s + 1);
17         for(; j <= n && s[k] >= s[j]; j++){                    34      get_sa(s, n, sa, rnk, height); }
18             !mx[j] ? mx[j] = i : 0;
19             s[k] > s[j] ? k = i : k++; }
20         for(; i <= k; i += j ‐ k) {} } }                            4.9   Suffix Balanced Tree 后缀平衡树
                                                                  1 // 后缀平衡树每次在字符串开头添加或删除字符, 考虑在当前字符串 S
     4.6 后缀自动机                                                           ↩→ 前插入一个字符       c, 那么相当于在后缀平衡树中插入一个新的后缀
 1   int last, mxl[N * 2], par[N * 2], sam_cnt;                          ↩→ cS, 简单的话可以使用预处理哈希二分 LCP 判断两个后缀的大小作
                                                                         ↩→ cmp, 直接写 set, 时间复杂度 O(nlg^2n). 为了方便可以把字符
 2   array <int, 26> go[N * 2];
                                                                         ↩→ 反过来做
 3   void extend(int c) {
 4      int p = last, np = ++sam_cnt; mxl[np] = mxl[p] + 1;        2   // 例题 : 加一个字符或删一个字符, 同时询问不同子串个数
 5      while (p && !go[p][c]) { go[p][c] = np; p = par[p]; }      3   struct cmp{
 6      if (!p) par[np] = 1; else { int q = go[p][c];              4       bool operator()(int a,int b){
 7         if (mxl[q] == mxl[p] + 1) par[np] = q;                  5          int p=lcp(a,b);//注意这里是后面加，lcp是反过来的
 8         else { int nq = ++sam_cnt; mxl[nq] = mxl[p] + 1;        6          if(a==p)return 0;if(b==p)return 1;
 9            go[nq] = go[q];                                      7          return s[a‐p]<s[b‐p];}
10            par[nq] = par[q]; par[np] = par[q] = nq;             8   };set<int,cmp>S;set<int,cmp>::iterator il,ir;
11            while (p && go[p][c] == q) { go[p][c] = nq;          9   void del(){S.erase(L‐‐);}//在后面删字符
12               p = par[p]; } } } last = np; }                   10   void add(char ch){//在后面加字符
13   void init() { last = sam_cnt = 1; }                          11       s[++L]=ch;mx=0;il=ir=S.lower_bound(L);
14   int c[N], q[N * 2];                                          12       if(il!=S.begin())mx=max(mx,lcp(L,*‐‐il));
15   void solve() { // 跑完得到的q是一个合法的拓扑序, c 记得清空                    13       if(ir!=S.end())mx=max(mx,lcp(L,*ir));
16      for (int i = 1; i <= sam_cnt; i++) c[mxl[i] + 1]++;       14       an[L]=an[L‐1]+L‐mx;S.insert(L); }
17      for (int i = 1; i <= n; i++) c[i] += c[i ‐ 1]; // n: 串长   15   LL getan(){printf("%lld\n",an[L]);}//询问不同子串个数
18      for (int i = 1; i <= sam_cnt; i++) q[++c[mxl[i]]] = i;}
                                                                       4.10 广义在线 SAM
     4.7 SAMSA & 后缀树                                              1    struct SAM{
1 bool vis[MAXN * 2]; char s[MAXN];                               2    int tot,fail[MM],len[MM],t[MM][26];
2 int id[MAXN * 2], ch[MAXN * 2][26], height[MAXN], stamp = 0;    3    SAM(){tot=1;}
3 void dfs(int x) {                                               4    int insert(int c,int last){
4    if (id[x]) { height[stamp++] = val[last];                    5       if(t[last][c]){

---

<!-- page: 15 -->

     上海交通大学 Shanghai Jiao Tong University                       Nemesis                                                                           Page 14
 6       int p=last,q=t[p][c];                                       25         if(r!=n)rr+=gr(l,r+1);
 7       if(len[p]+1==len[q])return q;                               26         if(rr‐ll+1>=2*(r‐l+1))run.push_back({ll,rr,r‐l+1}); }
 8       else { int nq=++tot;                                                     ↩→ }
 9          fail[nq]=fail[q];fail[q]=nq;                             27   void solve(){
10          len[nq]=len[p]+1;memcpy(t[nq],t[q],sizeof(t[q]));        28      st[n + 1] = '\0'; run.clear();
11          for(;p && t[p][c]==q;p=fail[p])t[p][c]=nq;               29      init();//Hash 或者 SA 的启动
12          //可以直接复制下面的代码。                                           30      for(int op=0;op<=1;op++){//0正常字典序,1反序
13          return nq; } }                                           31         lyndon(op); getrun(); }
14    int p=last,np=++tot;                                           32      sort(run.begin(),run.end(),[](Runs x, Runs y){
15    len[np]=len[p]+1;                                              33           return x == y ? x.p < y.p : (x.l != y.l ? x.l < y.l
16    for(;p && !t[p][c];p=fail[p])t[p][c]=np;                                       ↩→ : x.r < y.r);});
17    if(!p)fail[np]=1;                                              34      run.erase(unique(run.begin(),run.end()),run.end()); }
18    else {
19       int q=t[p][c];
20       if(len[q]==len[p]+1)fail[np]=q;                                  4.13 字符串 Hash
21       else { int nq=++tot;                                             Random primes generated at Thu Sep 5 00:24:52 2024
                                                                          3e2 179 191 211 227 251 311 313 347 349 353 379 389 397 419
22          fail[nq]=fail[q];fail[q]=nq;                                  1e6 950569 959449 960703 961531 972623 1016681 1063619
23          len[nq]=len[p]+1;memcpy(t[nq],t[q],sizeof(t[q]));             1e12 992345236997 995678562787 1023452343671 1045674564469
24          for(;p && t[p][c]==q;p=fail[p])t[p][c]=nq;                    1e13 10123412340917 10123412346533 10234523455957 10567856781973
                                                                          2e13 19234523459539 19345634567977 20234523454021 20567856785261
25          fail[np]=nq; } }                                              1e15 995678567851157 1045674567457081 1045674567459773
26    return np; } }sam;                                                  1e17 101234123412348037 103456345634562587 105678567856789793
27 // scanf("%s",st+1);int slen=strlen(st+1);                             5e17 491234123412346679 493456345634561563 502345234523452883
                                                                          1e18 992345234523451717 994567456745676007 1045674567456745241
28 // int last=1;                                                         2e18 1956785678567854391 1956785678567855843 2056785678567853529
29 // for(int j=1;j<=slen;j++)last=sam.insert(st[j]‐'a',last);

                                                                     1 static constexpr u128 inv = []() {
     4.11 回文树                                                        2    u128 ret = P;
     0 的子树是长为偶数的串, 1 的子树是长为奇数的. 0 代表空串, 1 没有意义.                      3    for (int i = 0; i < 6; i++) ret *= 2 ‐ ret * P;
                                                                     4    return ret; }();
 1 int len[N], fail[N], go[N][26], last, pam_cnt; char s[N];         5 constexpr u128 chk = u128(‐1) / P;
 2 int dif[N], slink[N], g[N]; // 最小回文划分相关的 dp                       6 bool check(i128 a, i128 b) {
 3 void clgo(int x){memset(go[x], 0, sizeof(go[x]));}                7    if (a < b) swap(a, b);
 4 void init(){ fail[0] = pam_cnt = 1; len[1] = ‐1;                  8    return (a ‐ b) * inv <= chk; }
 5     last = 0; clgo(0); clgo(1);}
 6 int getfail(int n, int p){                                             4.14 String Conclusions
 7     while (s[n ‐ len[p] ‐ 1] != s[n]) p = fail[p];
                                                                          双回文串
 8     return p;}
 9 void extend(int n) {// 1 ‐ base                                        如果 𝑠 = 𝑥 1𝑥 2 = 𝑦 1𝑦 2 = 𝑧 1𝑧 2, |𝑥 1 | < |𝑦 1 | < |𝑧 1 |, 𝑥 2, 𝑦 1, 𝑦 2, 𝑧 1 是回文
10     int p = getfail(n, last), c = s[n] ‐ 'a';                          串, 则 𝑥 1 和 𝑧 2 也是回文串.
11     if (!go[p][c]) { int q = ++pam_cnt, now = p;                       Border 和周期
12        clgo(q);                                                        如果 𝑟 是 𝑆 的一个border, 则 |𝑆 | − 𝑟 是 𝑆 的一个周期.
13        len[q] = len[p] + 2;                                            如果 𝑝 和 𝑞 都是 𝑆 的周期, 且满足 𝑝 + 𝑞 ≤ |𝑆 | + 𝑔𝑐𝑑 (𝑝, 𝑞) , 则 𝑔𝑐𝑑 (𝑝, 𝑞)
14        p = getfail(n, fail[p]);                                        也是一个周期.
15        fail[q] = go[p][c]; last = go[now][c] = q;                      字符串匹配与Border
16        dif[q] = len[q] ‐ len[fail[q]];                                 若字符串 𝑆 , 𝑇 满足 2|𝑆 | ≥ |𝑇 | , 则 𝑆 在 𝑇 中所有匹配位置成等差数列.
17        slink[q] = dif[q] == dif[fail[q]] ?                             若 𝑆 的匹配次数大于2, 则等差数列的周期恰好等于 𝑆 的最小周期.
18        slink[fail[q]] : fail[q];} else last = go[p][c];}               Border 的结构
19 void solve(){
                                                                          字符串 𝑆 的所有不小于 |𝑆 |/2 的border长度组成一个等差数列.
20     for(int x = last; x > 1; x = slink[x]){
                                                                          字符串 𝑆 的所有 border 按长度排序后可分成 𝑂 (log |𝑆 |) 段, 每段是一个等
21        g[x] = dp[i ‐ len[slink[x]] ‐ dif[x]];
                                                                          差数列.
22        if(dif[x] == dif[fail[x]]) g[x] <‐ g[fail[x]];
                                                                          回文串Border
23        dp[i] <‐ g[x]; } }
24 //双端插入: 前缀后缀分别写一个last和getfail, 回文所以只需要维护                               回文串长度为 𝑡 的后缀是一个回文后缀, 等价于 𝑡 是该串的border. 因此回
     ↩→ 回文后缀, 当last=串长的时候两个last要等于整个串
                                                                          文后缀的长度也可以划分成 𝑂 (log |𝑆 |) 段.
25 //去均摊:quick[x][c] 表示x的最长真回文后缀能匹配上c, 初始                                 子串最小后缀
     ↩→ 化quick[0] 全为1                                                     设 𝑠 [𝑝..𝑛] 是 𝑠 [𝑖..𝑛] , (𝑙 ≤ 𝑖 ≤ 𝑟 ) 中最小者, 则minsuf(l, r) 等于 𝑠 [𝑝..𝑟 ]
                                                                          的最短非空 border. minsuf(l, r) = min{𝑠 [𝑝..𝑟 ] , minsuf(r − 2𝑘 + 1, r)},
     4.12 Runs                                                            (2𝑘 < 𝑟𝑙 + 1 ≤ 2𝑘+1 ) .
 1   struct Runs{                                                         子串最大后缀
 2       int l,r,p;                                                       从左往右扫, 用set维护后缀的字典序递减的单调队列, 并在对应时刻添加”
 3   };vector<Runs> run;                                                  小于事件” 点以便在之后修改队列; 查询直接在set里lower_bound.
 4   bool operator==(Runs x,Runs y){return x.l==y.l && x.r==y.r;}         ZJJ: SAM处理手法
 5   int gl(int x,int y); // 求 𝑆 [1, 𝑥], 𝑆 [1, 𝑦] 的最长公共后缀                 1. 基本子串结构: CLB 搞的那玩意。
 6   int gr(int x,int y); // 求 𝑆 [𝑥, 𝑛], 𝑆 [𝑦, 𝑛] 的最长公共前缀                 2. 正反串 SAM 的基本联系: 一个子串出现的位置将会在两个SAM中同时
 7   //上面两个可以用 二分 + Hash 或者后缀数组实现。                                        得到映照。
 8   bool getcmp(int x,int y){//S[x,n]<S[y,n]                             3. SAM 上转成数点问题。
 9      int len=gr(x,y);
                                                                          4. 线段树合并维护 endpos 集合。
10      return st[x+len]<st[y+len];}                                      5. 树剖保证到根的链上只涉及 log 次修改和查询。(区间 border)
11   int ly[N];                                                           6. LCT 保证到根的链只修改均摊 log 个不同的颜色段。(区间本质不同子串
12   void lyndon(bool type){//后缀排序法求 Lyndon                               数量)
13      stack<PII> stk;stk.push({n,n});ly[n]=n;                           ZJJ: 字符串常见错误
14      for(int i=n‐1;i>=1;i‐‐){                                          1. 字符串算法变式记得判匹配位置超出字符串的情况，例如多组数据下的
15          int now=i;                                                    双端插入回文串，后缀数组多组。2. 警惕 char 运算中 ‘a’ 和 ‘␣a’ 的区别。3.
16          while(!stk.empty() && getcmp(i,stk.top().first)!=type)        char kmp[]
17             now=stk.top().second,stk.pop();
18          ly[i]=now;
19          stk.push({i,now});
20      } }
21   void getrun(){
22      for(int l=1;l<=n;l++){
23          int r=ly[l],ll=l,rr=r;
24          if(l!=1)ll‐=gl(l‐1,r);

---

<!-- page: 16 -->

     上海交通大学 Shanghai Jiao Tong University                         Nemesis                                                         Page 15

     5. Math 数学                                                     34
                                                                    35
                                                                                   factor (a / f, d);
                                                                                   factor (f, d); } }
     5.1 Long Long 𝑂 (1) 乘, Barrett
 1   LL modmul(LL a, LL b, LL M) { // skip2004, M < 63bit                5.5     扩展卢卡斯
 2      LL ret = a * b ‐ M * LL(1.L * a / M * b + 0.5);              1 int l,a[33],p[33],P[33];
 3      return ret < 0 ? ret + M : ret; }                            2 U fac(int k,LL n){// 求 n! mod pk^tk, 返回值 U{ 不包含 pk 的
 4   ULL modmul(ULL a, ULL b, LL M) { // orz@CF, M in 63 bit                ↩→ 值 ,pk 出现的次数 }
 5      ULL c = (long double)a * b / M;                              3      if (!n)return U{1,0};LL x=n/p[k],y=n/P[k],ans=1;int i;
 6      LL ret = LL(a * b ‐ c * M) % LL(M); // must be signed        4      if(y){// 求出循环节的答案
 7      return ret < 0 ? ret + M : ret; }                            5          for(i=2;i<P[k];i++)if(i%p[k])ans=ans*i%P[k];
 8   // use int128 instead if M > 63 bit                             6          ans=Pw(ans,y,P[k]);
 9   struct DIV {                                                    7      }for(i=y*P[k];i<=n;i++) if(i%p[k])ans=ans*i%M;// 求零散部
10      ULL p, ip;                                                            ↩→ 分
11      void init (ULL _p) { p = _p; ip = ‐1llu / p; }               8      U z=fac(k,x);return U{ans*z.x%M,x+z.z};
12      int mod (ULL x) { // x < 2 ^ 64                              9   }LL get(int k,LL n,LL m){// 求 C(n,m) mod pk^tk
13         ULL q = ULL(((u128)ip * x) >> 64);                       10      U a=fac(k,n),b=fac(k,m),c=fac(k,n‐m);// 分三部分求解
14         ULL r = x ‐ q * p;                                       11      return Pw(p[k],a.z‐b.z‐c.z,P[k])*a.x%P[k]*
15         return int(r >= p ? r ‐ p : r);                                    ↩→ inv(b.x,P[k])%P[k]*inv(c.x,P[k])%P[k];
16   } }; // speedup only when mod is not const                     12   }LL CRT(){// CRT 合并答案
                                                                    13      LL d,w,y,x,ans=0;
     5.2 exgcd, 逆元                                                  14      fr(i,1,l)w=M/P[i],exgcd(w,P[i],x,y),
                                                                              ↩→ ans=(ans+w*x%M*a[i])%M;
     假设我们已经找到了一组解 (𝑝 0, 𝑞 0 ) 满足 𝑎𝑝 0 + 𝑏𝑞 0 = gcd(𝑎, 𝑏) , 那么其
                                                               15           return (ans+M)%M;
     他的解都满足
                                                                    16   }LL C(LL n,LL m){// 求 C(n,m)
                        𝑏                        𝑎                  17      fr(i,1,l)a[i]=get(i,n,m);
            𝑝 = 𝑝0 +           ×𝑡    𝑞 = 𝑞0 −           ×𝑡          18      return CRT();
                     gcd(𝑝, 𝑞)                gcd(𝑝, 𝑞)             19   }LL exLucas(LL n,LL m,int M){
     其中t为任意整数.                                                      20      int jj=M,i //求 C(n,m)mod M,M=prod(pi^ki), O(pi^kilg^2n)
                                                                    21      for(i=2;i*i<=jj;i++)if(jj%i==0)
1 LL exgcd(LL a, LL b, LL &x, LL &y) {                              22          for(p[++l]=i,P[l]=1;jj%i==0;P[l]*=p[l])jj/=i;
2    if (b == 0) return x = 1, y = 0, a;                            23      if(jj>1)l++,p[l]=P[l]=jj;
3    LL t = exgcd(b, a % b, y, x);                                  24      return C(n,m);}
4    y ‐= a / b * x; return t;}
5 LL inv(LL x, LL m) {                                                   5.6     阶乘取模
6    LL a, b; exgcd(x, m, a, b); return (a % m + m) % m; }
                                                                                                 2 log 𝑛
                                                                                                      2
                                                                     1 // n! mod p^q Time : 𝑂 (𝑝𝑞 log 𝑝 )
     递推逆元: inv(𝑖) ≡ (𝑃 − 𝑃/𝑖) · inv(𝑃 mod 𝑖)                         2 // Output : {a, b} means a*p^b
     5.3 CRT 中国剩余定理                                                  3 using Val=unsigned long long; //Val 需要 mod p^q 意义下 + *
                                                                     4 typedef vector<Val> poly;
1    bool crt_merge(LL a1, LL m1, LL a2, LL m2, LL &A, LL &M) {      5 poly polymul(const poly &a,const poly &b){
2    LL c = a2 ‐ a1, d = __gcd(m1, m2); //合并两个模方程                    6    int n = (int) a.size(); poly c (n, Val(0));
3    if(c % d) return 0; // gcd(m1, m2) | (a2 ‐ a1) 时才有解             7    for (int i = 0; i < n; ++ i) {
4    c = (c % m2 + m2) % m2; c /= d; m1 /= d; m2 /= d;               8       for (int j = 0; i + j < n; ++ j) {
5    c = c * inv(m1 % m2, m2) % m2; //0逆元可任意值                        9          c[i + j] = c[i + j] + a[i] * b[j]; } }
6    M = m1*m2*d; A = (c *m1 %M *d %M +a1) % M; return 1;}//有解      10    return c; } Val choo[70][70];
                                                                    11 poly polyshift(const poly &a, Val delta) {
     5.4 Miller Rabin, Pollard Rho                                  12    int n = (int) a.size(); poly res (n, Val(0));
                                                                    13    for (int i = 0; i < n; ++ i) { Val d = 1;
 1   mt19937 rng(123);                                              14       for (int j = 0; j <= i; ++ j) {
 2   #define rand() LL(rng() & LLONG_MAX)                           15          res[i ‐ j] = res[i ‐ j]+a[i]*choo[i][j]*d;
 3   const int BASE[] = {2, 7, 61};//int(7,3e9)                     16          d = d * delta; } } return res; }
 4   //{2,325,9375,28178,450775,9780504,1795265022}LL(37)           17 void prepare(int q) {
 5   struct miller_rabin {                                          18    for (int i = 0; i < q; ++ i) { choo[i][0] = Val(1);
 6   bool check (const LL &M, const LL &base) {                     19       for (int j = 1; j <= i; ++ j)
 7      LL a = M ‐ 1;                                               20          choo[i][j]=choo[i‐1][j‐1]+choo[i‐1][j]; } }
 8      while (~a & 1) a >>= 1;                                     21 pair<Val, LL> fact(LL n, LL p, LL q) { Val ans = 1;
 9      LL w = power (base, a, M); // power should use mul          22    for (int r = 1; r < p; ++ r) {
10      for (; a != M ‐ 1 && w != 1 && w != M ‐ 1; a <<= 1)         23       poly x (q, Val(0)), res (q, Val(0));
11         w = mul (w, w, M);                                       24       res[0] = 1; LL _res = 0; x[0] = r; LL _x = 0;
12      return w == M ‐ 1 || (a & 1) == 1; }                        25       if (q > 1) x[1] = p, _x = 1; LL m = (n ‐ r + p) / p;
13   bool solve (const LL &a) {//𝑂 ((3 or 7) · log 𝑛 · mul)         26       while (m) { if (m & 1) {
14      if (a < 4) return a > 1;                                    27             res=polymul(res,polyshift(x,_res)); _res+=_x; }
15      if (~a & 1) return false;                                   28          m >>= 1; x = polymul(x, polyshift(x, _x)); _x+=_x;
16      for (int i = 0; i < sizeof(BASE)/4 && BASE[i] < a; ++i)                        ↩→ }
17         if (!check (a, BASE[i])) return false;                   29            ans = ans * res[0]; }
18      return true; } };                                           30         LL cnt = n / p; if (n >= p) { auto tmp=fact(n / p, p, q);
19   miller_rabin is_prime;                                         31            ans = ans * tmp.first; cnt += tmp.second; }
20 LL get_factor (LL a, LL seed) {//𝑂 (𝑛
                                         1/4 · log 𝑛 · mul)
                                                                    32         return {ans, cnt}; }
21    LL x = rand () % (a ‐ 1) + 1, y = x;
22    for (int head = 1, tail = 2; ; ) {                                 5.7
                                                                           类欧几里得直线下格点统计
23       x = mul (x, x, a); x = (x + seed) % a;                           Í𝑛−1 𝑎+𝑏𝑖
24       if (x == y) return a;                                       1 //  𝑖=0 b 𝑚 c , 𝑛, 𝑚, 𝑎, 𝑏 > 0
25       LL ans = gcd (abs (x ‐ y), a);                              2 LL solve(LL n, LL a, LL b, LL m){
26       if (ans > 1 && ans < a) return ans;                         3    if (b == 0) return n * (a / m);
27       if (++head == tail) { y = x; tail <<= 1; } } }              4    if (a >= m) return n * (a / m) + solve(n, a % m, b, m);
28 void factor (LL a, vector<LL> &d) {                               5    if (b >= m) return (n‐1)*n/2*(b/m) + solve(n,a,b%m,m);
29    if (a <= 1) return;                                            6    return solve((a + b * n) / m, (a + b * n) % m, m, b); }
30    if (is_prime.solve (a)) d.push_back (a);
31    else {                                                             5.8     万能欧几里德
32       LL f = a;
33       for (; f >= a; f = get_factor (a, rand() % (a ‐ 1) +        1 Val work(LL P, LL R, LL Q, LL n, Val VU, Val VR) {
             ↩→ 1));                                                 2 //(Px+R)/Q, 1<=x<=i, 经过整点先U再R

---

<!-- page: 17 -->

     上海交通大学 Shanghai Jiao Tong University                         Nemesis                                                     Page 16
3       if(!(((i128)n * P + R) / Q)) return ksm(VR, n);                  𝑘 次剩余 给定方程 𝑥 𝑘 ≡ 𝑎 mod 𝑚 , 求所有解. 若 𝑘 与 𝜑 (𝑚) 互质,
4       if(P>=Q) return work(P%Q,R,Q,n, VU, ksm(VU, P/Q) * VR);          则可以直接求出 𝑘 对 𝜑 (𝑚) 的逆元. 否则, 将 𝑘 拆成两部分, 𝑘 = 𝑢𝑣 , 其
                                                                                                                              −1
                                                                         中 𝑢⊥𝜑 (𝑚) , 𝑣 |𝜑 (𝑚) , 先求 𝑥 𝑣 ≡ 𝑎 mod 𝑚 , 则 𝑎𝑛𝑠 = 𝑥 𝑢 . 下面讨论
5       Val res; swap(VU,VR);
6       res = ksm(VU, (Q‐R‐1)/P)*VR;
7       LL m = ((i128)n * P + R) / Q;
                                                                                                                            𝑠
                                                                         𝑘 |𝜑 (𝑚) 的问题. 任取一原根 𝑔, 对两侧取离散对数, 设 𝑥 = 𝑔 , 𝑎 = 𝑔𝑡 , 其中
8       res = res * work(Q, (Q‐R‐1)%P, P, m‐1, VU, VR);                  𝑡 可以用BSGS求出, 则问题转化为求出所有的 𝑠 满足 𝑘𝑠 ≡ 𝑡 mod 𝜑 (𝑚) ,
9       return res * ksm(VU, n ‐ ((i128)m*Q ‐ R ‐ 1) / P); }             exgcd 即可求解, 显然有解的条件是 𝑘 |𝛿𝑚 (𝑎) .
                                                                         5.12 FFT
     5.9 平方剩余                                                        1   using cp = complex<double>; const double PI = acos(‐1.0);
 1   // x^2=a (mod p),0 <=a<p, 返回 true or false 代表是否存在解              2   vector<cp> omega[25]; // 单位根
 2   // p必须是质数, 若是多个单次质数的乘积，可以分别求解再用CRT合并                            3   // n 是 DFT 的最大长度，例如如果最多有两个长为 m 的多项式相乘，
 3   // 复杂度为 O(log n)                                                4   // 或者求逆的长度为 m，那么 n 需要 >= 2m
 4   void multiply(ll &c, ll &d, ll a, ll b, ll w) {                 5   void fft_init(int n) { // n = 2^k
 5      int cc = (a * c + b * d % MOD * w) % MOD;                    6      for (int k = 2, d = 0; k <= n; k *= 2, d++) {
 6      int dd = (a * d + b * c) % MOD; c = cc, d = dd; }            7         omega[d].resize(k + 1);
 7   bool solve(int n, int &x) {                                     8         for (int i = 0; i <= k; i++) // polar 是用模和辐角求复
                                                                                 ↩→ 数
 8      if (n==0) return x=0,true; if (MOD==2) return x=1,true;
 9      if (power(n, MOD / 2, MOD) == MOD ‐ 1) return false;         9             omega[d][i] = polar(1.0, 2 * PI * i / k); } }
10      ll c = 1, d = 0, b = 1, a, w;                               10   void fft(cp* a, int n, int t) {
11      // finding a such that a^2 ‐ n is not a square              11      for (int i = 1, j = 0; i < n ‐ 1; i++) {
12      do { a = rand() % MOD; w = (a * a ‐ n + MOD) % MOD;         12         int k = n; do j ^= (k >>= 1); while (j < k);
13         if (w == 0) return x = a, true;                          13         if (i < j) swap(a[i], a[j]); }
14      } while (power(w, MOD / 2, MOD) != MOD ‐ 1);                14      for (int k = 1, d = 0; k < n; k *= 2, d++)
15      for (int times = (MOD + 1) / 2; times; times >>= 1) {       15         for (int i = 0; i < n; i += k * 2)
16         if (times & 1) multiply(c, d, a, b, w);                  16             for (int j = 0; j < k; j++) {
17         multiply(a, b, a, b, w); }                               17                cp w = omega[d][t > 0 ? j : k * 2 ‐ j];
18      // x = (a + sqrt(w)) ^ ((p + 1) / 2)                        18                cp u = a[i + j], v = w * a[i + j + k];
19      return x = c, true; }                                       19                a[i + j] = u + v; a[i + j + k] = u ‐ v; }
                                                                    20      if (t < 0) for (int i = 0; i < n; i++) a[i] /= n; }

     5.10 线性同余不等式
1 // Find the minimal non‐negtive solutions for
       ↩→   𝑙 ≤ 𝑑 · 𝑥 mod 𝑚 ≤ 𝑟                                          5.13 NTT
2 // 0 ≤ 𝑑, 𝑙, 𝑟 < 𝑚; 𝑙 ≤ 𝑟, 𝑂 (log 𝑛)                               1   vector<int> omega[25]; // 单位根
3 LL cal(LL m, LL d, LL l, LL r) {                                   2   // n 是 DFT 的最大长度，例如如果最多有两个长为 m 的多项式相乘，
4    if (l==0) return 0; if (d==0) return MXL; // 无解                 3   // 或者求逆的长度为 m，那么 n 需要 >= 2m
5    if (d * 2 > m) return cal(m, m ‐ d, m ‐ r, m ‐ l);              4   void ntt_init(int n) { // n = 2^k
6    if ((l ‐ 1) / d < r / d) return (l ‐ 1) / d + 1;                5      for (int k = 2, d = 0; k <= n; k *= 2, d++) {
7    LL k = cal(d, (‐m % d + d) % d, l % d, r % d);                  6         omega[d].resize(k + 1);
8    return k==MXL ? MXL : (k*m + l ‐ 1)/d+1;}// 无解 2                7         int wn = qpow(3, (p ‐ 1) / k), tmp = 1;
9 // return all x satisfying l1<=x<=r1 and l2<=(x*mul+add)           8         for (int i = 0; i <= k; i++) { omega[d][i] = tmp;
       ↩→ %LIM<=r2                                                   9            tmp = (LL)tmp * wn % p; } } }
10   // here LIM = 2^32 so we use UI instead of "%".                10   // 传入的数必须是 [0, p) 范围内，不能有负的
11   // 𝑂 (log 𝑝 + #𝑠𝑜𝑙𝑢𝑡𝑖𝑜𝑛𝑠)                                      11   // 否则把 d == 16 改成 d % 8 == 0 之类，多取几次模
12   struct Jump { UI val, step;                                    12   void ntt(int *c, int n, int tp) {
13       Jump(UI val, UI step) : val(val), step(step) { }           13      static ULL a[N];
14       Jump operator + (const Jump & b) const {                   14      for (int i = 0; i < n; i++) a[i] = c[i];
15           return Jump(val + b.val, step + b.step); }             15      for (int i = 1, j = 0; i < n ‐ 1; i++) {
16       Jump operator ‐ (const Jump & b) const {                   16         int k = n; do j ^= (k >>= 1); while (j < k);
17           return Jump(val ‐ b.val, step + b.step); }};           17         if (i < j) swap(a[i], a[j]); }
18   inline Jump operator * (UI x, const Jump & a) {                18      for (int k = 1, d = 0; k < n; k *= 2, d++) {
19       return Jump(x * a.val, x * a.step); }                      19         if (d == 16) for (int i = 0; i < n; i++) a[i] %= p;
20   vector<UI> solve(UI l1, UI r1, UI l2, UI r2, pair<UI,UI>       20         for (int i = 0; i < n; i += k * 2)
       ↩→ muladd) {                                                 21            for (int j = 0; j < k; j++) {
21       UI mul = muladd.first, add = muladd.second, w = r2 ‐ l2;   22               int w = omega[d][tp > 0 ? j : k * 2 ‐ j];
22       Jump up(mul, 1), dn(‐mul, 1); UI s(l1 * mul + add);        23               ULL u = a[i + j], v = w * a[i + j + k] % p;
23       Jump lo(r2 ‐ s, 0), hi(s ‐ l2, 0);                         24               a[i + j] = u + v;
24       function<void(Jump&, Jump&)> sub=[&](Jump& a, Jump& b){    25               a[i + j + k] = u ‐ v + p; } }
25           if (a.val > w) {                                       26      if (tp>0) {for (int i = 0; i < n; i++) c[i] = a[i] % p;}
26              UI t(((LL)a.val‐max(0LL, w+1LL‐b.val)) / b.val);    27      else { int inv = qpow(n, p ‐ 2);
27              a = a ‐ t * b; } };                                 28         for (int i = 0; i < n; i++) c[i] = a[i] * inv % p;}}
28       sub(lo, up), sub(hi, dn);
29       while (up.val > w || dn.val > w) {
30           sub(up, dn); sub(lo, up);
31           sub(dn, up); sub(hi, dn); }                                 5.14 MTT 任意模数卷积
32       assert(up.val + dn.val > w); vector<UI> res;    1 void dft(cp* a, cp* b, int n) { static cp c[MAXN];
33       Jump bg(s + mul * min(lo.step, hi.step), min(lo.step,
                                                         2    for (int i = 0; i < n; i++)
           ↩→ hi.step));                                 3       c[i] = cp(a[i].real(), b[i].real());
34       while (bg.step <= r1 ‐ l1) {                    4    fft(c, n, 1);
35           if (l2 <= bg.val && bg.val <= r2)           5    for (int i = 0; i < n; i++) { int j = (n ‐ i) & (n ‐ 1);
36              res.push_back(bg.step + l1);             6       a[i] = (c[i] + conj(c[j])) * 0.5;
37           if (l2 <= bg.val‐dn.val && bg.val‐dn.val <= r2) {
                                                         7       b[i] = (c[i] ‐ conj(c[j])) * ‐0.5i; } }
38              bg = bg ‐ dn;                            8 void idft(cp* a, cp* b, int n) { static cp c[MAXN];
39           } else bg = bg + up; }                      9    for (int i = 0; i < n; i++) c[i] = a[i] + 1i * b[i];
40       return res; }                                  10    fft(c, n, ‐1);
                                                        11    for (int i = 0; i < n; i++) {
   5.11 原根                                              12       a[i] = c[i].real(); b[i] = c[i].imag(); } }
               𝑥                               𝑠
   定义 使得 𝑎 mod 𝑚 = 1 的最小的x, 记作 𝛿𝑚 (𝑎) . 若 𝑎 ≡ 𝑔 mod 𝑚 , 13 vector<int> multiply(const vector<int>& u,
   其中 𝑔 为 𝑚 的一个原根. 则虽然 𝑠 随 𝑔 的不同取值有所不同, 但是必然满足 14                const vector<int>& v, int mod) { // 任意模数卷积
                                                        15    static cp a[2][MAXN], b[2][MAXN], c[3][MAXN];
   𝛿𝑚 (𝑎) = gcd(𝑠, 𝜑 (𝑚)) .
                                                        16    int base = ceil(sqrt(mod));
                       𝛿   (𝑎)
   性质 𝛿𝑚 (𝑎𝑘 ) = gcd(𝛿 (𝑎),𝑘 )
                        𝑚
                                                        17    int n = (int)u.size(), m = (int)v.size();
                           𝑚

---

<!-- page: 18 -->

     上海交通大学 Shanghai Jiao Tong University                        Nemesis                                                                                     Page 17
18      int fft_n = 1; while (fft_n < n + m ‐ 1) fft_n *= 2;            5.15.2 多项式除法 取模
19      for (int i = 0; i < 2; i++) {                                   需要抄求逆。
20         fill(a[i], a[i] + fft_n, 0);
21         fill(b[i], b[i] + fft_n, 0); }                           1   poly poly_auto_mul(poly a, poly b) { // 自动判断长度的乘法
22      for (int i = 0; i < 3; i++)                                 2      int res_len = (int)a.size() + (int)b.size() ‐ 1;
23         fill(c[i], c[i] + fft_n, 0);                             3      int ntt_n = 1; while (ntt_n < res_len) ntt_n *= 2;
24      for (int i = 0; i < n; i++) { // 一定要取模！                     4      a.resize(ntt_n); b.resize(ntt_n);
25         a[0][i] = (u[i] % mod) % base;                           5      ntt(a.data(), ntt_n, 1); ntt(b.data(), ntt_n, 1);
26         a[1][i] = (u[i] % mod) / base; }                         6      for (int i = 0; i < ntt_n; i++)
27      for (int i = 0; i < m; i++) { // 一定要取模！                     7         a[i] = (LL)a[i] * b[i] % p;
28         b[0][i] = (v[i] % mod) % base;                           8      ntt(a.data(), ntt_n, ‐1); a.resize(res_len); return a; }
29         b[1][i] = (v[i] % mod) / base; }                         9   // 多项式除法，a 和 b 长度可以任意
30      dft(a[0], a[1], fft_n); dft(b[0], b[1], fft_n);            10   // 商的长度是 n ‐ m + 1，余数的长度是 m ‐ 1
31      for (int i = 0; i < fft_n; i++) {                          11   poly poly_div(const poly& a, const poly& b) {
32         c[0][i] = a[0][i] * b[0][i];                            12      int n = (int)a.size(), m = (int)b.size();
33         c[1][i] = a[0][i] * b[1][i] + a[1][i] * b[0][i];        13      if (n < m) return {};
34         c[2][i] = a[1][i] * b[1][i]; }                          14      int ntt_n = 1; while (ntt_n < n ‐ m + 1) ntt_n *= 2;
35      fft(c[1], fft_n, ‐1); idft(c[0], c[2], fft_n);             15      poly f(ntt_n), g(ntt_n);
36      int base2 = base * base % mod;                             16      for (int i = 0; i < n ‐ m + 1; i++) f[i] = a[n ‐ i ‐ 1];
37      vector<int> ans(n + m ‐ 1);                                17      for (int i = 0; i < m && i < n ‐ m + 1; i++)
38      for (int i = 0; i < n + m ‐ 1; i++)                        18         g[i] = b[m ‐ i ‐ 1];
39         ans[i] = ((LL)(c[0][i].real() + 0.5) +                  19      auto g_inv = poly_inv(g);
40            (LL)(c[1][i].real() + 0.5) % mod * base +            20      fill(g_inv.begin() + n ‐ m + 1, g_inv.end(), 0);
41            (LL)(c[2][i].real() + 0.5) % mod * base2) % mod;     21      auto c = poly_mul(f, g_inv); c.resize(n ‐ m + 1);
42      return ans; }                                              22      reverse(c.begin(), c.end()); return c; }
                                                                   23   // 多项式取模，a 和 b 长度可以任意，返回 (余数，商)
                                                                   24   pair<poly, poly> poly_mod(const poly& a, const poly& b) {
                                                                   25      int n = (int)a.size(), m = (int)b.size();
     5.15 多项式运算                                                    26      if (n < m) return {a, {}};
                                                                   27      auto d = poly_div(a, b); auto c = poly_auto_mul(b, d);
     5.15.1 多项式求逆 开根 ln exp
                                                                   28      poly r(m ‐ 1);
 1   using poly = vector<int>; // 用到 poly 的部分补成 2 ^ k              29      for (int i = 0; i < m ‐ 1; i++)
 2   poly poly_calc(const poly& u, const poly& v, // 长度要相同         30         r[i] = (a[i] ‐ c[i] + p) % p;
 3      function<int(int, int)> op) { // 返回长度是两倍                   31      return {r, d}; }
 4      static int a[MAXN], b[MAXN], c[MAXN];
 5      int n = (int)u.size();
                                                                        5.15.3 多点求值
 6      memcpy(a, u.data(), sizeof(int) * n);
 7      fill(a + n, a + n * 2, 0);                                      需要抄取模。
 8      memcpy(b, v.data(), sizeof(int) * n);
 9      fill(b + n, b + n * 2, 0);                                  1 struct poly_eval { poly f; vector<int> x; // 函数和询问点
10      ntt(a, n * 2, 1); ntt(b, n * 2, 1);                         2    vector<poly> gs; vector<int> ans; // gs 是预处理数组
11      for (int i = 0; i < n * 2; i++) c[i] = op(a[i], b[i]);      3    poly_eval(poly f, vector<int> x) : f(f), x(x) {}
12      ntt(c, n * 2, ‐1); return poly(c, c + n * 2); }             4    void pretreat(int l, int r, int o) { poly& g = gs[o];
13   poly poly_mul(const poly& u, const poly& v) { // 乘法            5       if (l == r) { g = poly{p ‐ x[l], 1}; return; }
14      return poly_calc(u, v, [](int a, int b)                     6       int mid = (l + r) / 2; pretreat(l, mid, o * 2);
15         { return (LL)a * b % p; }); } // 返回长度是两倍                 7       pretreat(mid + 1, r, o * 2 + 1);
16   poly poly_inv(const poly& a) { // 求逆，返回长度不变                    8       if (o > 1)
17      poly c{qpow(a[0], p ‐ 2)}; // 常数项一般都是 1                     9          g = poly_auto_mul(gs[o * 2], gs[o * 2 + 1]); }
18      for (int k = 2; k <= (int)a.size(); k *= 2) {              10    void solve(int l, int r, int o, const poly& f) {
19         c.resize(k); poly b(a.begin(), a.begin() + k);          11       if (l == r) { ans[l] = f[0]; return; }
20         c = poly_calc(b, c, [](int bi, int ci) {                12       int mid = (l + r) / 2;
21            return ((2 ‐ (LL)bi * ci) % p + p) * ci % p; });     13       solve(l, mid, o * 2, poly_mod(f, gs[o * 2]).first);
22         memset(c.data() + k, 0, sizeof(int) * k); }             14       solve(mid + 1, r, o * 2 + 1,
23      c.resize(a.size()); return c; }                            15          poly_mod(f, gs[o * 2 + 1]).first); }
24   poly poly_sqrt(const poly& a) { // 开根，返回长度不变                  16    vector<int> operator() () { // 包装好的接口
25      poly c{1}; // 常数项不是 1 的话要写二次剩余                             17       int n = (int)f.size(), m = (int)x.size();
26      for (int k = 2; k <= (int)a.size(); k *= 2) {              18       if (m <= n) x.resize(m = n + 1);
27         c.resize(k); poly b(a.begin(), a.begin() + k);          19       else if (n < m ‐ 1) f.resize(n = m ‐ 1);
28         b = poly_mul(b, poly_inv(c));                           20       int bit_ceil = 1; while (bit_ceil < m) bit_ceil *= 2;
29         for (int i = 0; i < k; i++) // inv_2 是 2 的逆元            21       ntt_init(bit_ceil * 2); // 注意这里 ntt_init 过了
30            c[i] = (LL)(c[i] + b[i]) * inv_2 % p; }              22       gs.resize(2 * bit_ceil + 1); pretreat(0, m ‐ 1, 1);
31      c.resize(a.size()); return c; }                            23       ans.resize(m); solve(0, m ‐ 1, 1, f); return ans;} };
32   poly poly_derivative(const poly& a) { poly c(a.size());
33      for (int i = 1; i < (int)a.size(); i++) // 求导                   5.15.4 插值
34         c[i ‐ 1] = (LL)a[i] * i % p; return c; }                     牛顿插值     实现时可以用 𝑘 次差分替代右边的式子，也可以卷积。
35   poly poly_integrate(const poly& a) { poly c(a.size());
                                                                                         𝑘  
                                                                                         Õ                                   Õ
                                                                                                                             𝑖                   
36      for (int i = 1; i < (int)a.size(); i++) // 不定积分                                     𝑛                                             𝑖−𝑗    𝑖
37         c[i] = (LL)a[i ‐ 1] * inv[i] % p; return c; }                       𝑓 (𝑛) =              𝑟𝑖 ⇐⇒         𝑟𝑖 =             (−1)             𝑓 ( 𝑗)
38   poly poly_ln(const poly& a) { // ln, 常数项非0, 返回长度不变                                  𝑖=0
                                                                                                𝑖                            𝑗=0
                                                                                                                                                 𝑗
39      auto c = poly_mul(poly_derivative(a), poly_inv(a));
40      c.resize(a.size()); return poly_integrate(c); }                 拉格朗日插值                           Õ             Ö 𝑥 − 𝑥𝑗
41   // exp，常数项必须是 0，返回长度不变                                                                    𝑓 (𝑥) =       𝑓 (𝑥𝑖 )
42   // 常数很大并且总代码很长, 一般可以改用分治 FFT                                                                                            𝑥𝑖 − 𝑥 𝑗
                                          1 Í𝑖 −1                                                        𝑖             𝑗≠𝑖
43   // 依据: 设 𝐺 (𝑥) = exp 𝐹 (𝑥) , 则 𝑔𝑖 = 𝑖    𝑘=1 𝑔𝑖 −𝑘 𝑘 𝑓𝑘
44   poly poly_exp(const poly& a) { poly c{1};                          5.16 线性递推
45      for (int k = 2; k <= (int)a.size(); k *= 2) {                   𝑂 (𝑘 2 log 𝑛)
46         c.resize(k); auto b = poly_ln(c);
47         for (int i = 0; i < k; i++)                              1   // Complexity: init O(n^2log) query O(n^2logk)
48            b[i] = (a[i] ‐ b[i] + p) % p;                         2   // Requirement: const LOG const MOD
49         (++b[0]) %= p; c = poly_mul(b, c);                       3   // Example: In: {1, 3} {2, 1} an = 2an‐1 + an‐2
50         memset(c.data() + k, 0, sizeof(int) * k); }              4   //          Out: calc(3) = 7
51      c.resize(a.size()); return c; }                             5   typedef vector<int> poly;
                                                                    6   struct LinearRec {

---

<!-- page: 19 -->

  上海交通大学 Shanghai Jiao Tong University                            Nemesis                                                       Page 18
 7    int n; poly first, trans; vector<poly> bin;                     4      for (int i = 0; i < (int)a.size(); i++) { int tmp = 0;
 8 poly add(poly &a, poly &b) {                                       5         for (int j = 0; j < (int)v.size(); j++)
 9    poly res(n * 2 + 1, 0);                                         6            tmp = (tmp + (LL)a[i ‐ j ‐ 1] * v[j]) % p;
10    // 不要每次新开 vector, 可以使用矩阵乘法优化                                    7         if (a[i] == tmp) continue;
11    for (int i = 0; i <= n; ++i) {                                  8         if (k < 0) { k = i; delta = (a[i] ‐ tmp + p) % p;
12       for (int j = 0; j <= n; ++j) {                               9            v = vector<int>(i + 1); continue; }
13          (res[i+j]+=(LL)a[i] * b[j] % MOD) %= MOD;                10         vector<int> u = v;
14    for (int i = 2 * n; i > n; ‐‐i) {                              11         int val = (LL)(a[i] ‐ tmp + p) *
15       for (int j = 0; j < n; ++j) {                               12            qpow(delta, p ‐ 2) % p;
16          (res[i‐1‐j]+=(LL)res[i]*trans[j]%MOD) %=MOD;}            13         if (v.size() < last.size() + i ‐ k)
17       res[i] = 0; }                                               14            v.resize(last.size() + i ‐ k);
18    res.erase(res.begin() + n + 1, res.end());                     15         (v[i ‐ k ‐ 1] += val) %= p;
19    return res; }                                                  16         for (int j = 0; j < (int)last.size(); j++) {
20 LinearRec(poly &first, poly &trans): first(first),                17            v[i ‐ k + j] = (v[i ‐ k + j] ‐
     ↩→ trans(trans)   {                                             18               (LL)val * last[j]) % p;
21    n = first.size(); poly a(n + 1, 0); a[1] = 1;                  19            if (v[i ‐ k + j] < 0) v[i ‐ k + j] += p; }
22    bin.push_back(a); for (int i = 1; i < LOG; ++i)                20         if ((int)u.size() ‐ i < (int)last.size() ‐ k) {
23       bin.push_back(add(bin[i ‐ 1], bin[i ‐ 1])); }               21            last = u; k = i; delta = a[i] ‐ tmp;
24 int calc(int k) { poly a(n + 1, 0); a[0] = 1;                     22            if (delta < 0) delta += p; } }
25    for (int i = 0; i < LOG; ++i)                                  23      for (auto &x : v) x = (p ‐ x) % p;
26       if (k >> i & 1) a = add(a, bin[i]);                         24                        Í𝑚1); //一般是需要最小递推式的, 处理一下
                                                                             v.insert(v.begin(),
27    int ret = 0; for (int i = 0; i < n; ++i)                       25      return v; } // ∀𝑖, 𝑗=0 𝑎𝑖 − 𝑗 𝑣 𝑗 = 0
28       if ((ret += (LL)a[i + 1] * first[i] % MOD) >= MOD)
29          ret ‐= MOD;
30    return ret; }};                                                     如果要求向量序列的递推式，就把每位乘一个随机权值 (或者说是乘一个随
                                                                          机行向量 𝑣 𝑇 ) 变成求数列递推式即可。如果是矩阵序列的话就随机一个行向
  𝑂 (𝑘 log 𝑘 log 𝑛) - Bostan-Mori                                         量 𝑢𝑇 和列向量 𝑣 ，然后把矩阵变成 𝑢𝑇 𝐴𝑣 的数列。
 1 int bostan_mori(int k, poly a, poly b) {                               优 化 矩 阵 快 速 幂DP 假 设 𝑓𝑖 有 𝑛 维， 先 暴 力 求 出 𝑓0˜2𝑛−1 ， 然 后 跑
 2    int n = (int)a.size(); while (k) { poly c = b;                      Berlekamp-Massey，最后调用快速线性递推即可。
 3       for (int i = 1; i < n; i += 2) c[i] = (p ‐ c[i]) % p;            求矩阵最小多项式 矩阵 𝐴 的最小多项式是次数最小的并且 𝑓 (𝐴) = 0
 4       a = poly_mul(a, c); b = poly_mul(b, c);
                                                                          的多项式 𝑓 。实际上最小多项式就是 {𝐴𝑖 } 的最小递推式，所以直接调用
 5       for (int i = 0; i < n; i++) {
 6          a[i] = a[i * 2 + k % 2]; b[i] = b[i * 2]; }
                                                                          Berlekamp-Massey 就好了，显然它的次数不超过 𝑛 。
 7       a.resize(n); b.resize(n); k /= 2; }                              瓶颈在于求出 𝐴𝑖 ，实际上我们只要处理 𝐴𝑖 𝑣 就行了，每次对向量做递推。
 8         Í𝑚
      return (LL)a[0] * qpow(b[0], p ‐ 2) % p; }                          求稀疏矩阵的行列式 如果能求出特征多项式，则常数项乘上 (−1)𝑛 就是
 9 // 𝑎𝑛 =   𝑖=1 𝑓𝑖 𝑎𝑛−𝑖 (𝑓0 = 0) , f.size() = a.size() + 1               行列式，但是最小多项式不一定就是特征多项式。
10 int linear_recurrance(int n, poly f, poly a) {                         把 𝐴 乘上一个随机对角阵 𝐵 ，则 𝐴𝐵 的最小多项式有很大概率就是特征多
11    int m = (int)a.size(), ntt_n = 1;                                   项式，最后再除掉 det 𝐵 就行了。
12    while (ntt_n <= m) ntt_n *= 2; ntt_init(ntt_n * 2);
                                                                          求稀疏矩阵的秩 设 𝐴 是一个 𝑛 × 𝑚 的矩阵, 首先随机一个 𝑛 × 𝑛 的对角
13    f.resize(ntt_n); a.resize(ntt_n); f[0] = 1;
14    for (int i = 1; i <= m; i++) f[i] = (p ‐ f[i]) % p;                 阵 𝑃 和一个 𝑚 × 𝑚 的对角阵 𝑄 , 然后计算 𝑄𝐴𝑃𝐴𝑇 𝑄 的最小多项式即可。
15    a = poly_mul(a, f); a.resize(ntt_n);                                实际上不用计算这个矩阵，因为求最小多项式时要用它乘一个向量，我们依
16    fill(a.data() + m, a.data() + ntt_n, 0);                            次把这几个矩阵乘到向量里就行了。答案就是最小多项式除掉所有 𝑥 因子
17    return bostan_mori(n, a, f); }                                      后剩下的次数。
                                                                          解稀疏方程组 𝐴𝑥 = 𝑏 ，其中 𝐴 是一个 𝑛 × 𝑛 的满秩稀疏矩阵，𝑏 和 𝑥 是
                                                                          1 × 𝑛 的列向量，𝐴, 𝑏 已知，需要解出 𝑥 。
  𝑂 (𝑘 log 𝑘 log 𝑛) - 多项式取模
                                                                          做法: 显然 𝑥 = 𝐴 −1𝑏 . 如果我们能求出 {𝐴𝑖 𝑏} (𝑖 ≥ 0) 的最小递推式
  需要抄前面的多项式取模。预处理 𝑂 (𝑘 log 𝑘 log 𝑛) ，固定 𝑛 和系数只改变
                                                                          {𝑟 0...𝑚−1 }(𝑚 ≤ 𝑛 )，那么就有结论
  初始值的话，询问一次 𝑂 (𝑘) 。注意只询问一次的话不如 Bostan-Mori 快。
                                                                                                       1   Õ
                                                                                                           𝑚−2
 1 poly poly_power_mod(LL k, const poly& m) { // x^k mod m                                𝐴 −1𝑏 = −              𝐴𝑖 𝑏𝑟𝑚−2−𝑖
 2    poly ans{1}, a{0, 1}; while (k) { if (k & 1)                                                    𝑟𝑚−1 𝑖=0
 3          ans = poly_mod(poly_auto_mul(ans, a), m).first;
 4       a = poly_mod(poly_auto_mul(a, a), m).first; k /= 2; }            因为 𝐴 是稀疏矩阵，直接按定义递推出 𝑏 . . . 𝐴2𝑛−1𝑏 即可。
 5         Í𝑚
      return ans; }
 6 // 𝑎𝑛 =   𝑖=1 𝑐 𝑖 𝑎𝑛−𝑖 (𝑐 0 = 0)                                   1 vector<int> solve_sparse_equations(const vector<tuple<int,
 7 struct linear_recurrence { poly f; // f是预处理结果                            ↩→ int,int> > &A, const vector<int> &b) {
 8    linear_recurrence(const poly& c, LL n) {                        2      int n = (int)b.size(); // 0‐based
 9       assert(c[0] == 0); // c[0] 是没有用的                             3      vector<vector<int> > f({b});
10       int m = (int)c.size() ‐ 1;                                   4      for (int i = 1; i < 2 * n; i++) {
11       int ntt_n = 1; while (ntt_n < m * 2) ntt_n *= 2;             5         vector<int> v(n); auto &u = f.back();
12       ntt_init(ntt_n); // 图省事就直接 ntt_init(1 << 18)                 6         for (auto [x, y, z] : A) // [x, y, value]
13       poly t(m + 1); t[m] = 1;                                     7            v[x] = (v[x] + (long long)u[y] * z) % p;
14       for (int i = 0; i < m; i++)t[i] = (p ‐ c[m ‐ i]) % p;        8         f.push_back(v); }
15       f = poly_power_mod(n, t); }                                  9      vector<int> w(n); mt19937 gen;
16    int operator()(const vector<int>& a) { // 0~m‐1项初始值            10      for (auto &x : w)
17       assert(a.size() == f.size()); int ans = 0;                  11         x = uniform_int_distribution<int>(1, p ‐ 1)(gen);
18       for (int i = 0; i < (int)a.size(); i++)                     12      vector<int> a(2 * n);
19          ans = (ans + (LL)f[i] * a[i]) % p;                       13      for (int i = 0; i < 2 * n; i++)
20       return ans; } };                                            14         for (int j = 0; j < n; j++)
                                                                     15            a[i] = (a[i] + (long long)f[i][j] * w[j]) % p;
  5.17 Berlekamp-Massey 最小多项式                                        16      auto c = berlekamp_massey(a); int m = (int)c.size();
  如果要求出一个次数为 𝑘 的递推式，则输入的数列需要至少有 2𝑘 项。                                17      vector<int> ans(n);
               Í𝑚−1                                                  18      for (int i = 0; i < m ‐ 1; i++)
  返回的内容满足 𝑗=0 𝑎𝑖 − 𝑗 𝑐 𝑗 = 0，并且 𝑐 0 = 1。
                                                 Í𝑚−1                19         for (int j = 0; j < n; j++)
  如果不加最后的处理的话，代码返回的结果会变成 𝑎𝑖 =                      𝑗=0 𝑐 𝑗 −1𝑎𝑖 − 𝑗 ，20            ans[j] = (ans[j] +
  有时候这样会方便接着跑递推，需要的话就删掉最后的处理。                                        21               (long long)c[m ‐ 2 ‐ i] * f[i][j]) % p;
                                                                     22      int inv = qpow(p ‐ c[m ‐ 1], p ‐ 2);
                                                                     23      for (int i = 0; i < n; i++)
1 vector<int> berlekamp_massey(const vector<int> &a) {
                                                                     24         ans[i] = (long long)ans[i] * inv % p;
2    vector<int> v, last; // v is the answer, 0‐based                25      return ans; }
3    int k = ‐1, delta = 0;

---

<!-- page: 20 -->

     上海交通大学 Shanghai Jiao Tong University                                   Nemesis                                                     Page 19
     5.18 FWT                                                                 41 for (int i = 1; i <= n; i++) {LD x; scanf("%lf", &x); a[0]
                                                                                      ↩→ [i]   = x;}
1 /*
             1,1    1,−1        1,0    1,0         1,1     0.5,0.5      42 for (int i = 1; i <= m; i++) {LD x;
 2   And:    0,1      0,1   Or:   1,1     −1,1 Xor:   1,−1     0.5,−0.5       43    for (int j = 1; j <= n; j++) scanf("%lf", &x), a[i][j] =
 3   IFWT的矩阵时FWT的逆, 对于任意运算 ⊕ ，满足FWT的矩阵需要：                                                ↩→ ‐x;
 4   𝐶 [𝑖] [ 𝑗] × 𝐶 [𝑖] [𝑘] = 𝐶 [𝑖] [ 𝑗 ⊕ 𝑘]                                  44    scanf("%lf", &x); a[i][0] = x;}
 5   对于不存在FWT矩阵的运算: 通过映射01变成另外一个可行的运算。*/                                      45 if (solve()) { printf("%.9lf\n", (LD)ans()); if (K)
 6   const LL XOR[2][2] = {{1, 1}, {1, M‐1}};                                         ↩→ output();}}
 7   const LL i2 = (M+1)/2, iXOR[2][2] = {{i2, i2}, {i2, M‐i2}};
                                                                              46 // 标准型: maximize c x, subject to Ax ≤ b and x ≥ 0
                                                                                                   T
 8   void FWT(LL f[], const LL C[2][2], int n) {
                                                                              47 // 对偶型: minimize b y, subject to A x ≥ c and y ≥ 0
                                                                                                   T               T
 9     for (int t = 1; t < n; t <<= 1) {
10       for (int l = 0; l < n; l += t + t) {
11         for (int i = 0; i < t; i++) {                                           5.21 高斯消元最小范数解
12           LL x = f[l + i], y = f[l + t + i];
13           f[l + i]     = (C[0][0] * x + C[0][1] * y) % M;                   1 typedef vector <LD> vec; /* sum a[i][0..d] = 0 */
14           f[l + t + i] = (C[1][0] * x + C[1][1] * y) % M;                   2 pair<vec,vector<vec>> gauss(vector<vec> &a, int n, int d) {
15         } } } }                                                             3    vector <int> pivot(d, ‐1);
                                                                               4    for (int i = 0, o = 0; i < d; i++) {
                                                                               5       int j = o; while (j < n && abs(a[j][i]) < eps) j++;
     5.19 K 进制 FWT                                                             6       if (j == n) continue;
 1 // n : power of k, omega[i] : (primitive kth root) ^ i                      7       swap(a[j], a[o]); LD w = a[o][i];
 2 void fwt(int* a, int k, int type) {                                         8       for (int k = 0; k <= d; k++) a[o][k] /= w;
 3   static int tmp[K];                                                        9       for (int x = 0; x < n; x++)
 4   for (int i = 1; i < n; i *= k)                                           10          if (x != o && abs(a[x][i]) > eps) {
 5     for (int j = 0, len = i * k; j < n; j += len)                          11             w = a[x][i];
 6       for (int low = 0; low < i; low++) {                                  12             for (int k = 0; k <= d; k++)
 7         for (int t = 0; t < k; t++)                                        13                a[x][k] ‐= a[o][k] * w;   }
 8           tmp[t] = a[j + t * i + low];                                     14       pivot[i] = o++;
 9         for (int t = 0; t < k; t++){                                       15    } vec x0(d); vector <vec> t; int free = 0;
10           int x = j + t * i + low;                                         16    for (int i = 0; i < d; i++)
11           a[x] = 0;                                                        17       if (pivot[i] != ‐1) x0[i] = ‐a[pivot[i]][d];
12           for (int y = 0; y < k; y++)                                      18       else free ++;
13             a[x] = int(a[x] + 1ll * tmp[y] * omega[(k +                    19    for (int i = 0; i < d; i++) if (pivot[i] == ‐1) {
                       ↩→ type)   * t * y % k] % MOD);                        20       vec x(d); x[i] = ‐1;
14             }                                                              21       for (int j = 0; j < d; j++)
15         }                                                                  22          if (pivot[j] != ‐1) x[j] = a[pivot[j]][i];
16     if (type == ‐1)                                                        23       t.push_back(x);
17       for (int i = 0, invn = inv(n); i < n; i++)                           24    } if (t.size()) {
18         a[i] = int(1ll * a[i] * invn % MOD); }                             25       vector <vec> f;
                                                                              26       for (int u = 0; u < free; u++) {
                                                                              27          vec x(free + 1);
     5.20 Simplex 单纯形                                                         28          for (int i = 0; i < free; i++)
                                                                              29             for (int j = 0; j < d; j++)
 1 const LD eps = 1e‐9, INF = 1e9; const int N = 105;
                                                                              30                x[i] += t[u][j] * t[i][j];
 2 namespace Simplex {
                                                                              31          for (int j = 0; j < d; j++)
 3 int n, m, id[N], tp[N]; LD a[N][N];
                                                                              32             x[free] += t[u][j] * x0[j];
 4 void pivot(int r, int c) {
                                                                              33          f.push_back(x);
 5    swap(id[r + n], id[c]);
                                                                              34       }
 6    LD t = ‐a[r][c]; a[r][c] = ‐1;
                                                                              35       auto [k, tt] = gauss(f, free, free);
 7    for (int i = 0; i <= n; i++) a[r][i] /= t;
                                                                              36       assert (tt.size() == 0);
 8    for (int i = 0; i <= m; i++) if (a[i][c] && r != i) {
                                                                              37       for (int x = 0; x < free; x++)
 9        t = a[i][c]; a[i][c] = 0;
                                                                              38          for (int i = 0; i < d; i++)
10        for (int j = 0; j <= n; j++) a[i][j] += t*a[r][j];}}
                                                                              39             x0[i] += k[x] * t[x][i];
11 bool solve() {
                                                                              40    } return {x0, t}; }
12    for (int i = 1; i <= n; i++) id[i] = i;
13    for ( ; ; ) {
14        int i = 0, j = 0; LD w = ‐eps;                                           5.22 Pell 方程
15        for (int k = 1; k <= m; k++)
16           if (a[k][0] < w || (a[k][0] < ‐eps && rand() & 1))                1   // 𝑥 2 − 𝑛 ∗ 𝑦 2 = 1 最小正整数根, n 为完全平方数时无解
17              w = a[i = k][0];                                               2   // 𝑥𝑘+1 = 𝑥 0𝑥𝑘 + 𝑛𝑦 0𝑦𝑘
18        if (!i) break;                                                       3   // 𝑦𝑘+1 = 𝑥 0𝑦𝑘 + 𝑦 0𝑥𝑘
19        for (int k = 1; k <= n; k++)                                         4   pair<LL, LL> pell(LL n) {
20           if (a[i][k] > eps) {j = k; break;}                                5      static LL p[N], q[N], g[N], h[N], a[N];
21        if (!j) { printf("Infeasible"); return 0;}                           6      p[1] = q[0] = h[1] = 1; p[0] = q[1] = g[1] = 0;
22        pivot(i, j);}                                                        7      a[2] = (LL)(floor(sqrtl(n) + 1e‐7L));
23    for ( ; ; ) {                                                            8      for(int i = 2; ; i ++) {
24        int i = 0, j = 0; LD w = eps, t;                                     9          g[i] = ‐g[i ‐ 1] + a[i] * h[i ‐ 1];
25        for (int k = 1; k <= n; k++)                                        10          h[i] = (n ‐ g[i] * g[i]) / h[i ‐ 1];
26           if (a[0][k] > w) w = a[0][j = k];                                11          a[i + 1] = (g[i] + a[2]) / h[i];
27        if (!j) break;                                                      12          p[i] = a[i] * p[i ‐ 1] + p[i ‐ 2];
28        w = INF;                                                            13          q[i] = a[i] * q[i ‐ 1] + q[i ‐ 2];
29        for (int k = 1; k <= m; k++)                                        14          if(p[i] * p[i] ‐ n * q[i] * q[i] == 1)
30           if (a[k][j] < ‐eps && (t = ‐a[k][0]/a[k][j]) < w)                15             return {p[i], q[i]}; }}
31              w = t, i = k;
32        if (!i) { printf("Unbounded"); return 0;}
33        pivot(i, j);}                                                            5.23 解一元三次方程
34    return 1;}                                                               1 double a(p[3]), b(p[2]), c(p[1]), d(p[0]);
35 LD ans() {return a[0][0];}                                                  2 double k(b / a), m(c / a), n(d / a);
36 void output() {                                                             3 double p(‐k * k / 3. + m);
37    for (int i = n + 1; i <= n + m; i++) tp[id[i]] = i ‐ n;                  4 double q(2. * k * k * k / 27 ‐ k * m / 3. + n);
38    for (int i = 1; i <=n; i++) printf("%.9lf ", tp[i] ?                     5 Complex omega[3] = {Complex(1, 0), Complex(‐0.5, 0.5 *
        ↩→ a[tp[i]][0] : 0);}                                                      ↩→ sqrt(3)), Complex(‐0.5, ‐0.5 * sqrt(3))};
39 }using namespace Simplex;                                                   6 Complex r1, r2; double delta(q * q / 4 + p * p * p / 27);
40 int main() { int K; read(n); read(m); read(K);                              7 if (delta > 0) {

---

<!-- page: 21 -->

     上海交通大学 Shanghai Jiao Tong University                                                                                         Nemesis                                                                                                              Page 20
 8    r1 = cubrt(‐q / 2. + sqrt(delta));                                                                                              a coprime (𝑘 + 1) -tuple together with 𝑛 .
                                                                                                                                                                                             Õ
 9    r2 = cubrt(‐q / 2. ‐ sqrt(delta));                                                                                                                                                            𝐽𝑘 (𝛿 ) = 𝑛𝑘
10 } else {
                                                                                                                                                                                             𝛿 |𝑛
11    r1 = pow(‐q / 2. + pow(Complex(delta), 0.5), 1. / 3);                                                                                                              𝑛 Õ
                                                                                                                                                                         Õ 𝑛                                            Õ
                                                                                                                                                                                                                        𝑛
12    r2 = pow(‐q / 2. ‐ pow(Complex(delta), 0.5), 1. / 3); }                                                                                                                          [𝑔𝑐𝑑 (𝑖, 𝑗 ) = 1]𝑖 𝑗 =                 𝑖 2𝜑 (𝑖 )
13 for(int _(0); _ < 3; _++) {                                                                                                                                           𝑖=1 𝑗 =1                                       𝑖=1
14    Complex x = ‐k/3. + r1*omega[_] + r2*omega[_* 2 % 3]; }                                                                                                                   Õ              𝑛
                                                                                                                                                                               𝛿 𝑠 𝐽𝑟 (𝛿 ) 𝐽𝑠 ( ) = 𝐽𝑟 +𝑠 (𝑛)
                                                                                                                                                                                               𝛿
                                                                                                                                                                          𝛿 |𝑛
     5.24 自适应 Simpson                                                                                                                                             Õ             𝑛              Õ
                                                                                                                                                                       𝜑 (𝛿 )𝑑         = 𝜎 (𝑛),       |𝜇 (𝛿 ) | = 2𝜔 (𝑛)
1 // Adaptive Simpson's method : LD simpson::solve (LD (*f)                                                                                                                       𝛿
                                                                                                                                                                  𝛿 |𝑛                           𝛿 |𝑛
        ↩→ (LD),LD l, LD r, LD eps) : integrates f over (l, r)                                                                                                          Õ                        Õ
        ↩→ witherror eps.                                                                                                                                                   2𝜔 (𝛿 ) = 𝑑 (𝑛 2 ),       𝑑 (𝛿 2 ) = 𝑑 2 (𝑛)
                                                                                                                                                                           𝛿 |𝑛                              𝛿 |𝑛
 2   struct simpson {
 3   LD area (LD (*f) (LD), LD l, LD r) {                                                                                                                     Õ 𝑛                     Õ 𝜇 (𝛿 ) 𝜑 (𝑛)
                                                                                                                                                               𝑑     2𝜔 (𝛿 ) = 𝑑 2 (𝑛),         =
 4      LD m = l + (r ‐ l) / 2;                                                                                                                                  𝛿                          𝛿      𝑛
                                                                                                                                                              𝛿 |𝑛                                           𝛿 |𝑛
 5      return (f (l) + 4 * f (m) + f (r)) * (r ‐ l) / 6;
                                                                                                                                                                               Õ 𝜇 (𝛿 )                      Õ 𝜇 2 (𝛿 )                 𝑛
 6   }                                                                                                                                                                                          = 𝑑 (𝑛),                          =
 7   LD solve (LD (*f) (LD), LD l, LD r, LD eps, LD a) {                                                                                                                               𝜑 (𝛿 )                           𝜑 (𝛿 )        𝜑 (𝑛)
                                                                                                                                                                               𝛿 |𝑛                          𝛿 |𝑛
 8      LD m = l + (r ‐ l) / 2;
 9      LD left = area (f, l, m), right = area (f, m, r);                                                                                                                                     𝑛 |𝜑 (𝑎𝑛 − 1)
10      if (abs (left + right ‐ a) <= 15 * eps) // TLE: || eps <                                                                                              Õ                                                           Õ (𝜇 ∗ 𝑓 ) (𝑑 )
          ↩→ EPS ** 2                                                                                                                                                       𝑓 (gcd(𝑘 − 1, 𝑛) ) = 𝜑 (𝑛)
                                                                                                                                                                                                                                      𝜑 (𝑑 )
11          return left + right + (left + right ‐ a) / 15.0;                                                                                                 1≤𝑘 ≤𝑛                                                       𝑑 |𝑛
                                                                                                                                                           gcd(𝑘,𝑛)=1
12      return solve (f, l, m, eps / 2, left) + solve (f, m, r,
                                                                                                                                                                  𝜑 (lcm(𝑚, 𝑛) )𝜑 (gcd(𝑚, 𝑛) ) = 𝜑 (𝑚)𝜑 (𝑛)
          ↩→ eps / 2, right);                                                                                                                                             Õ              Õ
13   }                                                                                                                                                                       𝑑 3 (𝛿 ) = ( 𝑑 (𝛿 ) ) 2
14   LD solve (LD (*f) (LD), LD l, LD r, LD eps) {                                                                                                                                    𝛿 |𝑛                𝛿 |𝑛
                                                                                                                                                                                                Õ        𝑢      𝑣
15      return solve (f, l, r, eps, area (f, l, r));                                                                                                                     𝑑 (𝑢𝑣) =               𝜇 (𝛿 )𝑑 ( )𝑑 ( )
16   }};                                                                                                                                                                                                  𝛿     𝛿
                                                                                                                                                                                   𝛿 | gcd(𝑢,𝑣)
                                                                                                                                                                                               Õ               𝑢𝑣
                                                                                                                                                                         𝜎𝑘 (𝑢 )𝜎𝑘 (𝑣) =               𝛿 𝑘 𝜎𝑘 ( 2 )
                                                                                                                                                                                                               𝛿
     6. Appendix                                                                                                                                                                        Õ
                                                                                                                                                                                        𝑛
                                                                                                                                                                                                     𝛿 | gcd(𝑢,𝑣)
                                                                                                                                                                                                                                   𝑘
                                                                                                                                                                         𝜇 (𝑛) =              [gcd(𝑘, 𝑛) = 1] cos 2𝜋
     6.1 Formulas 公式表                                                                                                                                                                   𝑘=1
                                                                                                                                                                                                                                   𝑛
     6.1.1 Mobius Inversion                                                          6.1.5 单位根反演                                                                         Õ
                                                                                                                                                                         𝑛                                   Õ
                                                                                                                                                                                                             𝑛
                   Õ                                Õ                    𝑛                             j   k                                                                                                                                     𝑘
                                                                                                         𝑛                                              𝜑 (𝑛) =                 [gcd(𝑘, 𝑛) = 1] =                   gcd(𝑘, 𝑛) cos 2𝜋
      𝐹 (𝑛) =             𝑓 (𝑑 ) ⇒ 𝑓 (𝑛) =                   𝜇 (𝑑 )𝐹                                     Õ
                                                                                                         𝑘                                                                                                                                         𝑛
                                                    𝑑                                                                                                                    𝑘=1                                 𝑘=1
                   𝑑 |𝑛             𝑑 |𝑛                               𝐶𝑛𝑖𝑘                                                                                                   Õ𝑛
                   Õ                Õ 𝑑                                                                                                                            
                                                                                                                                                                     
                                                                   𝑖=0                                                                                               
                                                                                                                                                                      𝑆 (𝑛) =     (𝑓 ∗ 𝑔) (𝑘 )
      𝐹 (𝑛) =     𝑓 (𝑑 ) ⇒ 𝑓 (𝑛) =       𝜇        𝐹 (𝑑 ) 引理                                                                                                          
                                                                                                                                                                     
                                             𝑛                                                                                                                       
                                                                                                                                                                              𝑘=1
              𝑛|𝑑                   𝑛|𝑑
                                                            1 Õ 𝑖𝑛
                      Õ               Õ                       𝑘 −1                                                                                                                                                  𝑛
                                                                                                                                                                 
                                                                                                                                                                  Õ𝑛                  Õ𝑛         b𝑖c
                                                                                                                                                                                                  Õ
           [𝑥 = 1] =      𝜇 (𝑑 ), 𝑥 =      𝜇 (𝑑 )                  𝜔 = [𝑘 | 𝑛]                                                                                   
                                                                                                                                                                            𝑛
                                                            𝑘 𝑖=0 𝑘                                                                                              
                                                                                                                                                                      𝑆 ( b    c )  =     𝑓 (𝑖 )      (𝑔 ∗ 1) ( 𝑗 )
                                 𝑑 |𝑥                   𝑑 |𝑥                                                                                                                𝑘
                                                                                                                                                                  𝑘=1                 𝑖=1        𝑗 =1
     6.1.2 杜教筛                                                                       反演                                                                              Õ𝑛
                                                                                                                                                           
                                                                                                                                                           
                                                  Õ
                                                  𝑛            j𝑛 k                              Õ
                                                                                                   𝑛                                                       
                                                                                                                                                            𝑆 (𝑛) =      (𝑓 · 𝑔) (𝑘 ), 𝑔 completely multiplicative
                            𝑛 (𝑛 + 1)                                                   𝐴𝑛𝑠 =            𝐶𝑛𝑖 [𝑘 | 𝑖 ]                                      
                                                                                                                                                           
                                                                                                                                                           
           𝑆𝜑 (𝑛) =                   −                 𝑆𝜑                                                                                                            𝑘=1
                                2                                   𝑑                              𝑖=0                                                             j𝑛 k
                                                  𝑑=2                                                                                                      
                                                                                                                                                            Õ 𝑛                       Õ𝑛
                                                    j𝑛 k                                                                                                 
                                                                                                                                                                𝑆           𝑔 (𝑘  )       (𝑓 ∗ 1) (𝑘 )𝑔 (𝑘 )
                                         Õ
                                         𝑛                                                         Õ
                                                                                                   𝑛
                                                                                                                 1 Õ 𝑖𝑗
                                                                                                                     𝑘 −1                                  
                                                                                                                                                                                    =
                   𝑆 𝜇 (𝑛) = 1 −               𝑆𝜇                                              =         𝐶𝑛𝑖 (         𝜔 )                                  𝑘=1      𝑘
                                                                                                                                                                                       𝑘=1
                                                         𝑑                                                       𝑘 𝑗 =0 𝑘
                                         𝑑=2                                                       𝑖=0                                6.1.7      Binomial Coefficients
     6.1.3 降幂公式                                                                                                                                                                                                               𝑛  
                                                                                                                                                                                                                              Õ               
                                                                                                 1 Õ 𝑖 Õ 𝑖𝑗
                                                                                                    𝑛     𝑘 −1                         C    0 1    2   3      4      5     6      7      8    9 10
                                                                                                                                       0    1                                                                                   𝑛+1
                                                                                                                                                                                                                                 𝑘
           𝑎 ≡𝑎𝑘          𝑘 mod 𝜑 (𝑝 )+𝜑 (𝑝 )
                                                   , 𝑘 ≥ 𝜑 (𝑝 )                                =       𝐶𝑛      𝜔𝑘                      1    1 1                                                                                           =
                                                                                                 𝑘 𝑖=0                                 2    1 2     1
                                                                                                                                                                                                                         𝑚      𝑚+1
                                                                                                          𝑗 =0                                                                                                      𝑘=0
     6.1.4 其他常用公式                                                                                                                      3    1 3     3 1                                                                Õ∞                     
                                                                                                                                       4    1 4     6 4 1                                                  √                (−1) 𝑘 −1 2𝑘 − 2 𝑘
                                                                                                   1Õ Õ 𝑖
            Õ
            𝑛                                                                                       𝑘 −1         𝑛
                                                  𝜑 (𝑛) + 𝑒 (𝑛)                                                 𝑗                      5    1 5    10 10 5 1                                                 1+𝑧 = 1+                            𝑧
                   [ (𝑖, 𝑛) = 1] 𝑖 = 𝑛                                                         =         ( 𝐶 (𝜔 ) 𝑖 )                  6    1 6    15 20 15 6 1                                                            𝑘 × 22𝑘 −1 𝑘 − 1
                                                        2                                          𝑘 𝑗 =0 𝑖=0 𝑛 𝑘                      7    1 7    21 35 35 21 7 1                                                     𝑘=1
            𝑖=1
                                                                                                                                       8    1 8    28 56 70 56 28 8 1                                         Õ
                                                                                                                                                                                                              𝑟                           
             Õ𝑛 Õ
                𝑖                                          j𝑛 k                                                                      9    1 9    36 84 126 126 84 36 9 1
                                                                                                                                                                                                                  𝑟 −𝑘 𝑠 +𝑘          𝑟 +𝑠 +1
                                                                                                   1 Õ
                                                                                                     𝑘 −1                                                                                                                       =
                            [ (𝑖, 𝑗 ) = 𝑑 ] = 𝑆𝜑                                               =
                                                                                                                𝑗
                                                                                                          (1 + 𝜔𝑘 ) 𝑛
                                                                                                                                       10   1 10
                                                                                                                                               
                                                                                                                                                   45 120 210 252 210 120 45 10 1                                   𝑚      𝑛        𝑚 +𝑛 +1
                                                               𝑑                                                                                                                                             𝑘=0
          𝑖=1 𝑗 =1                                                                                 𝑘 𝑗 =0                                      𝑛                                                                                     
                                                                                                                                                  ≡ [𝑛&𝑘 = 𝑘 ]                 (mod 2)                               𝑛 +𝑚       𝑛 +𝑚
       𝑛 Õ
       Õ 𝑚                        Õ 𝑘  j𝑛 k j𝑚 k                                                                                             𝑘                                                            𝐶𝑛,𝑚 =           −            ,𝑛 ≥ 𝑚
                                                                                     另, 如果要求的是 [𝑛%𝑘 = 𝑡 ] , 其实就                                                                                                        𝑚        𝑚−1
                [ (𝑖, 𝑗 ) = 𝑑 ] =  𝜇
                                     𝑑 𝑘       𝑘                                     是 [𝑘 | (𝑛 − 𝑡 ) ] . 同理推式子即可.
       𝑖=1 𝑗 =1                            𝑑 |𝑘                                                                                                                                                       
                           𝑛
                          bÕ                                𝑛                                                                                            𝑛               𝑘 −𝑛 − 1 Õ 𝑟 +𝑘             𝑟 +𝑛 +1
         Õ
         𝑛                 𝑖 c             Õ
                                           𝑛                𝑖 c
                                                           bÕ                                                                                               = (−1) 𝑘                ,             =
               𝑓 (𝑖 )            𝑔(𝑗 ) =          𝑔 (𝑖 )            𝑓 (𝑗)                                                                                𝑘                   𝑘               𝑘          𝑛
                                                                                                                                                                                      𝑘 ≤𝑛
                                                                                                                                                                                    Õ              
         𝑖=1              𝑗 =1              𝑖=1              𝑗 =1                                                                                            𝑛 1 + · · · + 𝑛𝑝                  𝑛1      𝑛𝑝
                                                                                                                                                                                =                 ···
     6.1.6 Arithmetic Function                                                                                                                                      𝑚                          𝑘1      𝑘𝑝
                                                                                                                                                                                                    𝑘 1 +···+𝑘𝑝 =𝑚
                                       (𝑝 − 1)! ≡ −1(𝑚𝑜𝑑 𝑝 )
                                                                                                                                      6.1.8      Fibonacci Numbers, Lucas Numbers
                     𝑎 > 1, 𝑚, 𝑛 > 0, then gcd(𝑎𝑚 − 1, 𝑎𝑛 − 1) = 𝑎 gcd(𝑛,𝑚) − 1
                                                   Õ
                                         𝜇 2 (𝑛) =    𝜇 (𝑑 )                                                                                                                                                             gcd(𝑓𝑚 , 𝑓𝑛 ) = 𝑓gcd(𝑚,𝑛)
                                                                                                                                                           𝑧
                                                                        𝑑 2 |𝑛
                                                                                                                                                   𝐹 (𝑧 ) =                                                              𝑓𝑛2 + (−1) 𝑛 = 𝑓𝑛+1 𝑓𝑛−1
                                                                                                                                                      1 − 𝑧√ − 𝑧 2
        𝑎 > 𝑏, gcd(𝑎, 𝑏 ) = 1, then gcd(𝑎 − 𝑏 , 𝑎 − 𝑏 ) = 𝑎    𝑚
                                                                       −𝑏𝑚       𝑛      𝑛          gcd(𝑚,𝑛)            gcd(𝑚,𝑛)
                                                                                                                                                      1− 5                                                               𝑓𝑛+𝑘 = 𝑓𝑛 𝑓𝑘+1 + 𝑓𝑛−1 𝑓𝑘
                         Ö𝑚                                                                                                                     𝜙ˆ =                                                                                       2
                                                                                                                                                                                                                             𝑓2𝑛+1 = 𝑓𝑛2 + 𝑓𝑛+1
                                          −1   mod 𝑚, 𝑚 = 4, 𝑝 𝑞 , 2𝑝 𝑞                                                                                  2
                                   𝑘≡                                                                                                   Õ
                                                                                                                                        𝑛                Õ𝑛
                                                                                                                                                                                                                 (−1) 𝑘 𝑓𝑛−𝑘 = 𝑓𝑛 𝑓𝑘 −1 − 𝑓𝑛−1 𝑓𝑘
                  𝑘=1,𝑔𝑐𝑑 (𝑘,𝑚)=1
                                         1     mod 𝑚, otherwise                                                                           𝑓𝑘 = 𝑓𝑛+2 − 1,      𝑓𝑘2 = 𝑓𝑛 𝑓𝑛+1
                                                        Õ                (𝑛)
                                                                        𝜔Ö           (𝑎 +1)𝑘                                            𝑘=1                          𝑘=1                                   def fib(n): # F(n), F(n + 1)
                                                                                 𝑝𝑖 𝑖          −1                                       Õ
                                                                                                                                        𝑛
                                        𝜎𝑘 (𝑛) =               𝑑𝑘 =                                                                                    1            2                                          if not n: return (0, 1)
                                                                                      𝑝𝑖𝑘 − 1                                                 𝑓𝑘 𝑓𝑛−𝑘 = (𝑛 − 1) 𝑓𝑛 + 𝑛𝑓𝑛−1                                     a, b = fib(n >> 1)
                                                        𝑑 |𝑛              𝑖=1                                                                          5            5                                          c = a * (2 * b ‐ a)
                                                                        Ö              1
                                                                                                                                        𝑘=0
                                                                                                                                                                                                               d = a * a + b * b
                                                  𝐽𝑘 (𝑛) = 𝑛𝑘                  (1 −      )                                                         𝑓2𝑛                                                         if n & 1:
                                                                                      𝑝𝑘                                                                = 𝑓𝑛−1 + 𝑓𝑛+1                                              return (d, c + d)
                                                                        𝑝 |𝑛                                                                        𝑓𝑛                                                         else:
     𝐽𝑘 (𝑛) is the number of 𝑘 -tuples of positive integers all less than or equal to n that form                                      𝑓1 +2𝑓2 +3𝑓3 +· · ·+𝑛𝑓𝑛 = 𝑛𝑓𝑛+2 − 𝑓𝑛+3 +2                                   return (c, d)

---

<!-- page: 22 -->

上海交通大学 Shanghai Jiao Tong University                                                                   Nemesis                                                                                               Page 21
                                                                                                                                                                                          Õ 𝑛 
                                                                                                                                                                             𝑥𝑛 =         𝑥𝑘
                                                
                                                 𝑓𝑟 ,                    𝑚 mod 4 = 0;                             𝑛+1         𝑛      𝑛                                                     𝑘
                                                
                                                                                                                          =𝑘     +                                                        𝑘
                                                
                                                 (−1) 𝑟 +1 𝑓𝑛−𝑟 ,
                                                      𝑚 mod 4 = 1;                                                                                                                      Õ 𝑛 
                                                                                                                     𝑘         𝑘     𝑘−1
                 Modulo 𝑓𝑛 , 𝑓𝑚𝑛+𝑟 ≡    𝑛                                                                              Õ                                                           =        (−1) 𝑛−𝑘 𝑥 𝑘
                                 
                                  (−1)   𝑓𝑟 ,         𝑚 mod 4 = 2;                                                    𝑛         𝑚 𝑛                                                        𝑘
                                 
                                                                                                                 𝑚!                𝑘 (−1) 𝑚−𝑘                                             𝑘
                                       𝑟 +1+𝑛                                                                        𝑚
                                                                                                                           =
                                                                                                                                 𝑘
                                  (−1)        𝑓𝑛−𝑟 , 𝑚 mod 4 = 3.                                                                                                       For fixed 𝑘 , EGF and OGF:
                                                     √          √                                                             𝑘
                                                 1+ 5 𝑛     1− 5 𝑛                                                                                                           ∞   𝑛
                                                                                                                                                                             Õ                           𝑥   
         𝐿0 = 2, 𝐿1 = 1, 𝐿𝑛 = 𝐿𝑛−1 + 𝐿𝑛−2 = (          ) +(         )                                       n\k    0   1     2     3    4       5      6                        𝑛 𝑥                𝑥𝑘    𝑒 −1 𝑘
                                                    2          2                                             0     1                                                                           =
                                           2−𝑥                                                               1     0   1                                                    𝑛=0
                                                                                                                                                                                     𝑘 𝑛!          𝑘!       𝑥
                              𝐿 (𝑥 ) =                                                                       2     0   1    1
                                       1 − 𝑥 − 𝑥2                                                            3     0   1    3 1                                                Õ∞          Ö𝑘
除了 𝑛 = 0, 4, 8, 16, 𝐿𝑛 是素数, 则 𝑛 是素数.                                                                         4     0   1    7 6     1                                              𝑛 𝑛
                                     √             √                                                                                                                                  𝑥 = 𝑥𝑘     (1 − 𝑖𝑥 ) −1
                                                                                                             5     0   1    15 25 10 1                                             𝑘
                                1+ 5 ˆ 1− 5                                                                  6     0   1    31 90 65 15 1                                      𝑛=0           𝑖=1
                           𝜙=          , 𝜙=                                                                  7     0   1    63 301 350 140 21
                                    2             2
                              𝜙 𝑛 − 𝜙ˆ𝑛                                                                     6.1.15 Eulerian Numbers
                         𝐹𝑛 =     √     , 𝐿𝑛 = 𝜙 𝑛 + 𝜙ˆ𝑛
                                    5                                                                        n\k    0 1    2    3    4    5 6                             𝑛             𝑛−1                  𝑛−1
                                     √          √ 𝑛                                                         1     1                                                        = (𝑘 + 1)           + (𝑛 − 𝑘 )
                            𝐿𝑛 + 𝐹𝑛 5        1+ 5                                                             2     1 1                                                   𝑘               𝑘                  𝑘−1
                                                                                                                                                                                        Õ 𝑛  𝑥 + 𝑘 
                                        =
                                 2              2                                                             3     1 4    1
                                                                                                              4     1 11 11     1                                                𝑥𝑛 =
6.1.9 Sum of Powers                                                                                           5     1 26 66     26   1                                                        𝑘      𝑛
                                                                                                            6     1 57 302 302 57       1                                              𝑘
                   Õ
                   𝑛
                                 𝑛 (𝑛 + 1) (2𝑛 + 1) Õ 3
                                                      𝑛
                                                               𝑛 (𝑛 + 1) 2                                                                                                 Õ  𝑚         
                                                                                                              7     1 120 1191 2416 1191 120 1                            𝑛          𝑛+1
                          𝑖2 =                     ,     𝑖 =                                                                                                                 =               (𝑚 + 1 − 𝑘 ) 𝑛 (−1) 𝑘
                   𝑖=1
                                         6           𝑖=1
                                                                   2                                                                                                      𝑚            𝑘
                                                                                                                                                                                    𝑘=0
                          Õ
                          𝑛
                                    𝑛 (𝑛 + 1) (2𝑛 + 1) (3𝑛 2 + 3𝑛 − 1)
                              𝑖 =4                                                                          6.1.16 Harmonic    Numbers,                                  6.1.17 卡迈克尔函数
                          𝑖=1
                                                    30                                                             1, 3/2, 11/6, 25/12,                                  卡迈克尔函数表示模 𝑚 剩余系下最大的阶,
                              Õ𝑛                                                                                   137/60…                                               即 𝜆 (𝑚) = max𝑎⊥𝑚 𝛿𝑚 (𝑎) .
                                       𝑛 (𝑛 + 1) (2𝑛 2 + 2𝑛 − 1)
                                         2        2
                                                                                                                                                                         容易看出, 若 𝜆 (𝑚) = 𝜑 (𝑚) , 则 𝑚 存在原
                                  𝑖5 =                                                                              Õ 1 Õ
                                                                                                                    𝑛     𝑛
                              𝑖=1
                                                    12                                                                                                                   根.
                                                                                                            𝐻𝑛 =        ,   𝐻𝑘 = (𝑛 + 1)𝐻𝑛 − 𝑛                           该 函 数 可 由 下 述 方 法 计 算: 分 解 质 因 数
6.1.10 Catalan Numbers 1, 1, 2, 5, 14, 42, 132, 429, 1430…                                                            𝑘                                                             𝛼 𝛼
                                                                                                                    𝑘=1     𝑘=1                                          𝑚 = 𝑝 1 1 𝑝 2 2 ...𝑝𝑡𝛼𝑡 . 则 𝜆 (𝑚) =
                                                                      2𝑛                                Õ
                                                                                                             𝑛                                                                    𝛼        𝛼
                          Õ
                          𝑛−1
                                 4𝑛 − 2         2𝑛    2𝑛              𝑛
                                                                                                                      𝑛 (𝑛 + 1)       𝑛 (𝑛 − 1)                          lcm(𝜆 (𝑝 1 1 ), 𝑝 2 2 , ..., 𝑝𝑡𝛼𝑡 ) . 其中对奇质
          𝑐 0 = 1, 𝑐𝑛 =         𝑐𝑖 𝑐𝑛−1−𝑖 = 𝑐𝑛−1
                                        =     =    −                                                            𝑘𝐻𝑘 =           𝐻𝑛 −
                                  𝑛+1     𝑛+1   𝑛    𝑛−1                                                                  2               4                                         𝛼
                                                                                                                                                                         数 𝑝 , 𝜆 (𝑝 ) = (𝑝 − 1)𝑝 𝛼 −1 . 对2的
                          𝑖=0                                                                               𝑘=1
                                   √                                                                        𝑛  
                                                                                                            Õ                                                          幂, 𝜆 (2𝑘 ) = 2𝑘 −2 , 𝑠.𝑡 .𝑘 ≥ 3. 𝜆 (4) =
                               1 − 1 − 4𝑥                                                                       𝑘         𝑛+1               1                            𝜆 (2) = 2.
                      𝑐 (𝑥 ) =                                                                                    𝐻𝑘 =           (𝐻𝑛+1 −        )
                                    2𝑥                                                                          𝑚        𝑚+1              𝑚+1
                                                                                                            𝑘=1
Usage: 𝑛 对括号序列; 𝑛 个点满二叉树; 𝑛 × 𝑛 的方格左下到右上不过对角线方案数; 凸 𝑛 + 2
边形三角形分割数; 𝑛 个数的出栈方案数; 2𝑛 个顶点连接, 线段两两不交的方案数.
                                                                                                            6.1.18 求拆分数
类卡特兰数 从 (1, 1) 出发走到 (𝑛, 𝑚) , 只能向右或者向上走, 不能越过 𝑦 = 𝑥 这条线
                      𝑛        𝑛−1                                                                          def penta(k):                                                记 𝑝 (𝑛) 表示 𝑛 的拆分数, 𝑓 (𝑛, 𝑘 ) 表示将 𝑛
(即保证 𝑥 ≥ 𝑦 ), 合法方案数是 𝐶𝑛+𝑚−2 − 𝐶𝑛+𝑚−2 .                                                                        return k*(3*k‐1)//2                                        拆分且每种数字使用次数必须小于等于 𝑘 的
6.1.11 Motzkin Numbers 1, 1, 2, 4, 9, 21, 51, 127, 323, 835…
                                                           Í
                                                                                                            def compute_partition(goal):
                                                                                                              p = [1]
                                                                                                                                                                         拆分数. 则
                                                                                                𝑎
圆上 𝑛 点间画不相交弦的方案数. 选 𝑛 个数 𝑘 1 , 𝑘 2 , ..., 𝑘𝑛 ∈ { −1, 0, 1} , 保证                                 𝑖 𝑘𝑖 (1 ≤     for n in range(1,goal+1):
                                                                                                                                                                                            𝑘
                                                                                                                                                                             𝑃 (𝑥 )Φ(𝑥 ) = 1, 𝐹 (𝑥 )Φ(𝑥 ) = 1
𝑎 ≤ 𝑛) 非负且所有数总和为 0 的方案数.                                                                                        p.append(0)
                                                                                                                                                                         暴力拆开卷积, 可以得到将1,-1,2,-2… 带入五
                                                                                                                for k in range(1,n+1):
                      Õ
                      𝑛−1
                                      (2𝑛 + 3)𝑀𝑛 + 3𝑛𝑀𝑛−1                                                                                                                边形数 (−1) 𝑘 𝑥 𝑘 (3𝑘 −1) /2 中, 由于小于n的
          𝑀𝑛+1 = 𝑀𝑛 +     𝑀𝑖 𝑀𝑛−1−𝑖 =
                                                                                                                  c = (‐1)**(k+1)                                                  √                  √
                                              𝑛+3
                                                                                                                  for t in [penta(k), penta(‐k)]:                        五边形数只有 𝑛 个, 可以 𝑂 (𝑛 𝑛) 计算答
                       𝑖                                                                                            if (n‐t) >= 0:
                                                                                                                                                                         案:
                                                                                                                      p[n] = p[n] + c*p[n‐t]
                                                b 𝑛2 c                                                                                                                  𝑝 (𝑛) = 𝑝 (𝑛 − 1) + 𝑝 (𝑛 − 2) − 𝑝 (𝑛 −
                                       Õ 𝑛                                                                   return p
                                                                                                                       Î∞                                                5) − 𝑝 (𝑛 − 7) + · · · ,
                                         𝑀𝑛 =   Catlan(𝑘 )
                                            2𝑘                                                              Φ(𝑥 ) = 𝑛=1 (1 − 𝑥 𝑛 )                                       𝑓 (𝑛, 𝑘 ) = 𝑝 (𝑛) − 𝑝 (𝑛 − 𝑘 ) − 𝑝 (𝑛 −
                                       𝑖=0
                                               √                                                             Í∞                                                          2𝑘 ) + 𝑝 (𝑛 − 5𝑘 ) + 𝑝 (𝑛 − 7𝑘 ) − · · ·
                                                                                                            = 𝑛=−∞  (−1) 𝑘 𝑥 𝑘 (3𝑘 −1) /2
                                      1 − 𝑥 − 1 − 2𝑥 − 3𝑥 2
                            𝑀 (𝑋 ) =
                                                 2𝑥 2
6.1.12       Derangement 错排数 0, 1, 2, 9, 44, 265, 1854, 14833…                                              6.1.19 Bernoulli Numbers 1, 1/2, 1/6, 0, -1/30, 0, 1/42 …
                                                                                                                                                            Õ 𝐵𝑖 𝑥 𝑖             𝑥
                                         1     1      1   1         (−1) 𝑛                                                                     𝐵 (𝑥 ) =                    =
              𝐷 1 = 0, 𝐷 2 = 1, 𝐷𝑛 = 𝑛!( −        +     −   + ... +        )                                                                                        𝑖!         𝑒𝑥 − 1
                                         0! 1! 2! 3!                  𝑛!                                                                                    𝑖 ≥0
                              𝐷𝑛 = (𝑛 − 1) (𝐷𝑛−1 + 𝐷𝑛−2 )                                                                                            Õ
                                                                                                                                                     𝑛−1          𝑛 
                                                                                                                                                                  Õ        
                                                                                                                                                     𝑛     𝐵𝑖         𝑛+1
6.1.13 Bell Numbers 1, 1, 2, 5, 15, 52, 203, 877, 4140 …                                                                     𝐵𝑛 = [𝑛 = 0] −                     ,            𝐵𝑖 = 0
𝑛 个元素集合划分的方案数.                                                                                                                                  𝑖=0
                                                                                                                                                     𝑖 𝑛 − 𝑘 + 1 𝑖=0    𝑖
                                          𝑛  
                                          Õ                     𝑛  
                                                                Õ
                                             𝑛                     𝑛                                                                             Õ
                                                                                                                                                 𝑚−1      Õ𝑛  
                                 𝐵𝑛 =                , 𝐵𝑛+1 =             𝐵𝑘                                                                                  𝑛       𝑚𝑖+1
                                                 𝑘                    𝑘                                                                𝑆𝑛 (𝑚) =      𝑖𝑛 =        𝐵𝑛−𝑖
                                          𝑘=1                   𝑘=0                                                                              𝑖=0      𝑖=0
                                                                                                                                                              𝑖       𝑖 +1
                                 𝐵𝑝𝑚 +𝑛 ≡ 𝑚𝐵𝑛 + 𝐵𝑛+1 (mod 𝑝 )
                                                                                                            𝐵 0 = 1, 𝐵 1 = − 12 , 𝐵 4 = − 30
                                                                                                                                          1
                                                                                                                                             , 𝐵 6 = 42
                                                                                                                                                     1
                                                                                                                                                        , 𝐵 8 = − 30
                                                                                                                                                                  1
                                                                                                                                                                     , ....
                                            Õ∞
                                                𝐵𝑛 𝑛     𝑥
                                   𝐵 (𝑥 ) =        𝑥 = ee −1                                                (除了 𝐵 1 = − 12 以外, 伯努利数的奇数项都是 0.)
                                            𝑛=0
                                                𝑛!                                                          自然数幂次和关于次数的EGF:
                                                                                                                                                     ∞ Í𝑛
                                                                                                                                                     Õ        𝑘
6.1.14 Stirling Numbers                                                                                                                                 𝑖=0 𝑖
                                                                                                                                       𝐹 (𝑥 ) =                 𝑥𝑘
第一类 𝑛 个元素集合分作 𝑘 个非空轮换方案数.                                                                                                                                𝑘!
                                                                                                                                                     𝑘=0
                                                                                                                                                     Õ𝑛
                                                                                                                                                                                   𝑒 (𝑛+1)𝑥 −1
                                                                                                                                                 =         𝑒 𝑖𝑥 =
                                                                                                                                                                               𝑒𝑥 − 1
           𝑛+1          𝑛      𝑛                                                                                                                   𝑖=0
                          +                                      𝑛+1
             𝑘
                   =𝑛
                        𝑘    𝑘−1                                       = 𝑛!𝐻𝑛 (see 6.1.16)                  6.1.20 kMAX-MIN反演
                                                                  2                                                                                    Õ
                                                                                                                                                                 (−1) |𝑇 |−𝑘 𝐶 𝑘|𝑇−1
                                  𝑛                                      Õ 𝑛                                                    𝑘MAX(𝑆 ) =                                        |−1 MIN(𝑇 )
            𝑠 (𝑛, 𝑘 ) = (−1) 𝑛−𝑘                                    𝑥𝑛 =        (−1) 𝑛−𝑘 𝑥 𝑘                                                         𝑇 ⊂𝑆,𝑇 ≠∅
                                  𝑘                                          𝑘
                                                                          𝑘                                 代入 𝑘 = 1 即为MAX-MIN反演:
n\k   0      1     2       3         4     5     6                                                                                                              Õ
 0    1                                                                  Õ 𝑛                                                         MAX(𝑆 ) =                      (−1) |𝑇 |−1 MIN(𝑇 )
 1    0      1                                                      𝑥𝑛 =        𝑥𝑘
 2    0      1    1                                                          𝑘                                                                         𝑇 ⊂𝑆,𝑇 ≠∅
                                                                               𝑘
 3    0      2    3   1
 4    0      6   11   6    1                               For fixed 𝑘 , EGF:                               6.1.21 伍德伯里矩阵不等式
 5    0     24 50     35 10 1                                                                                               (𝐴 + 𝑈 𝐶𝑉 ) −1 = 𝐴 −1 − 𝐴 −1𝑈 (𝐶 −1 + 𝑉 𝐴 −1𝑈 ) −1𝑉 𝐴 −1
                                                             ∞   𝑛
                                                             Õ                                   
 6
 7
      0
      0
            120 274 225 85 15 1
            720 1764 1624 735 175 21
                                                                𝑛 𝑥    𝑥𝑘               ln(1 − 𝑥 ) 𝑘        该等式可以动态维护矩阵的逆, 令 𝐶 = [1] , 𝑈 , 𝑉 分别为 1 × 𝑛 和 𝑛 × 1 的向量, 这样可以构
                                                                     =
                                                            𝑛=0
                                                                𝑘 𝑛!   𝑘!                   𝑥               造出 𝑈 𝐶𝑉 为只有某行或者某列不为0的矩阵, 一次修改复杂度为 𝑂 (𝑛 2 ) .
                                                                                                            6.1.22 Sum of Squares
                                                                                                            𝑟𝑘 (𝑛) 表示用 𝑘 个平方数组成 𝑛 的方案数. 假设:
                                                                                                                                                                               𝑏
                                                                                                                                            𝑛 = 2𝑎0 𝑝 1 1 · · · 𝑝𝑟2𝑎𝑟 𝑞 1 1 · · · 𝑞𝑏𝑠 𝑠
                                                                                                                                                           2𝑎
第二类 把 𝑛 个元素集合分作 𝑘 个非空子集方案数.

---

<!-- page: 23 -->

上海交通大学 Shanghai Jiao Tong University                                                                    Nemesis                                                                                                Page 22
                                                                                                                    max {𝑥𝑎 − 𝑥𝑏 , 𝑦𝑎 − 𝑦𝑏 , 𝑧𝑎 − 𝑧𝑏 } − min {𝑥𝑎 − 𝑥𝑏 , 𝑦𝑎 − 𝑦𝑏 , 𝑧𝑎 − 𝑧𝑏 }
其中 𝑝𝑖 ≡ 3    mod 4, 𝑞𝑖 ≡ 1 mod 4, 那么
                                        if any 𝑎𝑖 is a half-integer
                                                                                                                    1Õ
                          
                          
                            0                                                                                     =       | (𝑥𝑎 − 𝑦𝑎 ) − (𝑥𝑏 − 𝑦𝑏 ) |
                           Ö
                             𝑟                                                                                     2 𝑐𝑦𝑐
                𝑟 2 (𝑛) =
                          
                           4   (𝑏𝑖 + 1)       if all 𝑎𝑖 are integers                                                                           Õ
                                                                                                                                                𝑛
                                                                                                                                                               𝑛𝑐 𝑛+2 − (𝑛 + 1)𝑐 𝑛+1 + 𝑐
                          
                           1                                                                                                                         𝑘𝑐 𝑘 =
                                                                                                                                                                       (𝑐 − 1) 2
𝑟 3 (𝑛) > 0 当且仅当 𝑛 不满足 4𝑎 (8𝑏 + 7) 的形式 (𝑎, 𝑏 为整数).                                                                                              𝑘=0
6.1.23 枚举勾股数 Pythagorean Triple                                                                                                                        (𝑎 + 𝑏 + 𝑐 ) 3 − 𝑎 3 − 𝑏 3 − 𝑐 3
                                                                                                                                  (𝑎 + 𝑏 ) (𝑏 + 𝑐 ) (𝑐 + 𝑎) =
枚举 𝑥 2 + 𝑦 2 = 𝑧 2 的三元组: 可令 𝑥 = 𝑚 2 − 𝑛 2 , 𝑦 = 2𝑚𝑛, 𝑧 = 𝑚 2 + 𝑛 2 , 枚举 𝑚 和 𝑛 即                                                                                        3
可 𝑂 (𝑛) 枚举勾股数. 判断素勾股数方法: 𝑚, 𝑛 至少一个为偶数并且 𝑚, 𝑛 互质, 那么 𝑥, 𝑦, 𝑧                                                       𝑎 3 + 𝑏 3 = (𝑎 + 𝑏 ) (𝑎 2 − 𝑎𝑏 + 𝑏 2 ), 𝑎 3 − 𝑏 3 = (𝑎 − 𝑏 ) (𝑎 2 + 𝑎𝑏 + 𝑏 2 )
就是素勾股数.                                                                                                     𝑛 mod 2 = 1 :
6.1.24 四面体体积 Tetrahedron Volume                                                                                   𝑎𝑛 + 𝑏 𝑛 = (𝑎 + 𝑏 ) (𝑎𝑛−1 − 𝑎𝑛−2𝑏 + 𝑎𝑛−3𝑏 2 − ... − 𝑎𝑏𝑛−2 + 𝑏 𝑛−1 )
If 𝑈 , 𝑉 , 𝑊 , 𝑢 , 𝑣 , 𝑤 are lengths of edges of the tetrahedron (first three form a triangle;                                                                Í𝑘      𝑖 份.
u opposite to U and so on)                                                                                  划分问题: 𝑛 个 𝑘 − 1 维向量最多把 𝑘 维空间分为 𝑖=0 𝐶𝑛
            q
                4𝑢 2 𝑣 2 𝑤 2 −
                                 Í                                   Î                                      6.2       Calculus, Integration Table 导数积分表
                                 𝑐𝑦𝑐 𝑢
                                         2 (𝑣 2 + 𝑤 2 − 𝑈 2 ) 2 +
                                                                     𝑐𝑦𝑐 (𝑣
                                                                                  2 + 𝑤2 − 𝑈 2 )
      𝑉 =                                                                                                                0
                                                                                                             ( 𝑢𝑣 ) 0 = 𝑢 𝑣−𝑢𝑣
                                                                                                                              0
                                                                                                                                                        (arctan 𝑥 ) 0 =     1
                                                                                                                                                                                            (arcsinh 𝑥 ) 0 = √ 1
                                                      12                                                                   𝑣2                                             1+𝑥 2                                1+𝑥 2
6.1.25 杨氏矩阵与钩子公式                                                                                             (𝑎𝑥 ) 0 = (ln 𝑎)𝑎𝑥                         (arccot 𝑥 ) 0 = −     1
                                                                                                                       0                                                    1+𝑥 2           (arccosh 𝑥 ) 0 = √ 1
                                                                                                             (tan 𝑥 ) = sec2 𝑥                          (arccsc 𝑥 ) 0 = −                                    𝑥 2 −1
满足: 格子 (𝑖, 𝑗 ) 没有元素, 则它右边和上边相邻格子也没有元素; 格子 (𝑖, 𝑗 ) 有元素 𝑎[𝑖 ] [ 𝑗 ] ,                                                                                                           √1
则它右边和上边相邻格子要么没有元素, 要么有元素且比 𝑎[𝑖 ] [ 𝑗 ] 大.
                                                                                                             (cot 𝑥 ) 0 = csc2 𝑥                                              𝑥    1−𝑥 2    (arctanh 𝑥 ) 0 = 1
                                                                                                                                                                                                           1−𝑥 2
                                                                                                             (sec 𝑥 ) 0 = tan 𝑥 sec 𝑥                   (arcsec 𝑥 ) 0 = √ 1                             0
                                                                              2
                                                                         𝑥 + 𝑥2                              (csc 𝑥 ) 0 = − cot 𝑥 csc 𝑥                                   𝑥       1−𝑥 2
                                                                                                                                                                                            (arccoth 𝑥 ) = 2 1
计数: 𝐹 1 = 1, 𝐹 2 = 2, 𝐹𝑛 = 𝐹𝑛−1 + (𝑛 − 1)𝐹𝑛−2 , 𝐹 (𝑥 ) = 𝑒                                                                                                                                              0
                                                                                                                                                                                                          𝑥 −1
钩子公式: 对于给定形状 𝜆 , 不同杨氏矩阵的个数为:                                                                                 (arcsin 𝑥 ) 0 = √ 1                        (tanh 𝑥 ) 0 = sech2 𝑥               (arccsch 𝑥 ) = −    √1
                                                                                                                                                                                                               |𝑥 | 1+𝑥 2
                                                 𝑛!
                                                                                                                                  1−𝑥 2                 (coth 𝑥 ) 0 = − csch2 𝑥
                                                                                                             (arccos 𝑥 ) 0 = − √ 1                      (sech 𝑥 ) 0 = − sech 𝑥 tanh 𝑥       (arcsech 𝑥 ) 0 = − √ 1
                                         𝑑𝜆 = Î                                                                                     1−𝑥 2                                                                      𝑥   1−𝑥 2
                                                ℎ𝜆 (𝑖, 𝑗 )                                                                                              (csch 𝑥 ) 0 = − csch 𝑥 coth 𝑥
ℎ𝜆 (𝑖, 𝑗 ) 表示该格子右边和上边的格子数量加1.
6.1.26 常见博弈游戏                                                                                               · 𝑎𝑥 2 + 𝑏𝑥 + 𝑐 (𝑎 > 0)
Nim-K游戏 𝑛 堆石子轮流拿, 每次最多                                阶梯Nim 在一个阶梯上, 每次选一个台                                                        
                                                                                                                                                     2𝑎𝑥 +𝑏
                                                                                                                                   √4𝑎𝑐 −𝑏 2 arctan √4𝑎𝑐 −𝑏 2 + 𝐶     (𝑏 2 < 4𝑎𝑐 )
                                                                                                                                       2
                                                                                                            ∫                     
可以拿k堆石子, 谁走最后一步输。结论: 把                                阶上任意个式子移到下一个台阶上, 不可移动                                         d𝑥
                                                                                                                              =                       √
                                                                                                                𝑎𝑥 2 +𝑏𝑥 +𝑐                  2𝑎𝑥 +𝑏 − 𝑏 −4𝑎𝑐
                                                                                                                                                        2
                                                                                                                                √𝑏 2 −4𝑎𝑐 ln 2𝑎𝑥 +𝑏+√𝑏 2 −4𝑎𝑐 + 𝐶 (𝑏 2 > 4𝑎𝑐 )
                                                                                                                                      1
每一堆石子的sg值 (即石子数量) 二进制分解,                              者输. 结论: SG值等于奇数层台阶上石子数
先手必败当且仅当每一位二进制位上1的个数                                  的异或和. 对于树形结构也适用, 奇数层节点                                ∫                                                ∫
                                                                                                                     𝑥                                      𝑏
是 (k+1) 的倍数.                                          上所有石子数异或起来即可.                                                         d𝑥 = 2𝑎 ln |𝑎𝑥 + 𝑏𝑥 + 𝑐 | − 2𝑎
                                                                                                                                  1         2                        d𝑥

Anti-Nim游戏 𝑛 堆石子轮流拿, 谁走
                                                                                                             √  𝑎𝑥 2 +𝑏𝑥 +𝑐                                     𝑎𝑥 2 +𝑏𝑥 +𝑐

最后一步输。结论: 先手胜当且仅当1. 所有堆                               图上博弈 给定无向图, 先手从某点开始                                   · ±𝑎𝑥 2 + 𝑏𝑥 + 𝑐 (𝑎 > 0)
                                                                                                            ∫                                                      √
                                                      走, 只能走相邻且未走过的点, 无法移动者输.
                                                                                                                                                 √
石子数都为1且游戏的SG值为0 (即有偶数个                                                                                        √ d𝑥         = √1𝑎 ln |2𝑎𝑥 + 𝑏 + 2 𝑎 𝑎𝑥 2 + 𝑏𝑥 + 𝑐 | + 𝐶
孤单堆-每堆只有1个石子数) 2. 存在某堆石                               对该图求最大匹配, 若某个点不一定在最大匹                                 ∫ √𝑎𝑥 2 +𝑏𝑥 +𝑐
                                                                                                                                                   √
子数大于1且游戏的SG值不为0.                                      配中则先手必败, 否则先手必胜.                                         𝑎𝑥 2 + 𝑏𝑥 + 𝑐d𝑥        =     2𝑎𝑥 +𝑏                         2
                                                                                                                                                    𝑎𝑥 2 + 𝑏𝑥 + 𝑐 + 4𝑎𝑐√−𝑏3 ln |2𝑎𝑥                            + 𝑏 +
                                                                                                                                              4𝑎                       8 𝑎
斐波那契博弈 有一堆物品, 两人轮流取                                                                                          √  √
物品, 先手最少取一个, 至多无上限, 但不能把                              最大最小定理求纳什均衡点 在二人                                      2 𝑎 𝑎𝑥 2 + 𝑏𝑥 + 𝑐 | + 𝐶
                                                                                                            ∫                               √                                                    √
                                                                                                                   𝑥                                                        √
物品取完, 之后每次取的物品数不能超过上一                                 零和博弈中, 可以用以下方式求出一个纳什均                                   √            d𝑥 = 𝑎1 𝑎𝑥 2 + 𝑏𝑥 + 𝑐 − √𝑏 3 ln |2𝑎𝑥 + 𝑏 + 2 𝑎 𝑎𝑥 2 + 𝑏𝑥 + 𝑐 | + 𝐶
次取的物品数的二倍且至少为一件, 取走最后                                 衡点: 在博弈双方中任选一方, 求混合策略 p                               ∫  𝑎𝑥 2 +𝑏𝑥 +𝑐                             2 𝑎
                                                                                                                                                 −𝑏
一件物品的人获胜. 结论: 先手胜当且仅当物                                使得对方选择任意一个纯策略时, 己方的最小                                   √ d𝑥          = − √1𝑎 arcsin √2𝑎𝑥       +𝐶
                                                                                                            ∫ √𝑐+𝑏𝑥 −𝑎𝑥                     𝑏 2 +4𝑎𝑐
                                                                                                                         2
品数 𝑛 不是斐波那契数.                                         收益最大 (等价于对方的最大收益最小). 据此
                                                                                                                                     2𝑎𝑥 −𝑏
                                                                                                                                            √                  2
威佐夫博弈 有两堆石子, 博弈双方每次                                   可以求出双方在此局面下的最优期望得分, 分                                    𝑐 + 𝑏𝑥 − 𝑎𝑥 d𝑥 = 4𝑎
                                                                                                                             2                𝑐 + 𝑏𝑥 − 𝑎𝑥 2 + 𝑏 √+4𝑎𝑐 arcsin √2𝑎𝑥   −𝑏
                                                                                                                                                                                        +𝐶
                                                      别等于己方最大的最小收益和对方最小的最                                   ∫                        √                         8 𝑎3            𝑏 2 +4𝑎𝑐
可以取一堆石子中的任意个, 不能不取, 或                                                                                              𝑥                                 2 + √𝑏 arcsin √2𝑎𝑥 −𝑏 + 𝐶
                                                      大收益. 一般而言, 可以得到形如                                       √            d𝑥  = − 1
                                                                                                                                   𝑎   𝑐 + 𝑏𝑥  −  𝑎𝑥
者取两堆石子中的相同个. 先取完者赢. 结                                                             Õ                                    p
                                                                                                               𝑐+𝑏𝑥 −𝑎𝑥 2                                2 𝑎3         𝑏 2 +4𝑎𝑐
j : 求出两堆石子
论        k     𝐴 和 𝐵 的差值 𝐶 , 如果                                                              𝑝 𝑗 𝑤𝑖,𝑗
                                                                                                             p
     √                                                     max min
                                                            p    𝑖                 Í                        · ± 𝑥𝑥 −𝑎
                                                                                                                   −𝑏 或 (𝑥 − 𝑎)(𝑥 − 𝑏) , (𝑎 < 𝑏)
 𝐶 ∗ 5+1
       2   = 𝑚𝑖𝑛 (𝐴, 𝐵) 那么后手赢, 否                                     𝑝 𝑗 ∈p,𝑝 𝑗 ≥0, 𝑝 𝑗 =1                  ∫                            q
                                                                     Í                                        √ d𝑥                           −𝑎
                                                                                                                               = 2 arcsin 𝑏𝑥 −𝑥 +𝐶
则先手赢.                                                 的形式. 当      𝑝 𝑗 𝑤𝑖,𝑗 可以表示成只与 𝑖 有                          (𝑥 −𝑎) (𝑏 −𝑥 )
                                                                                                            ∫ p                                 p                                   q
约瑟夫环 𝑛 个人 0, . . . , 𝑛 − 1, 令 𝑓𝑖,𝑚                    关的函数Í 𝑓 (𝑖 ) 时, 可以令初始时 𝑝𝑖 = 0, 不                          (𝑥 − 𝑎) (𝑏 − 𝑥 )d𝑥 = 2𝑥 −𝑎−𝑏      (𝑥 − 𝑎) (𝑏 − 𝑥 ) + (𝑏 −𝑎)
                                                                                                                                                                            2
                                                                                                                                                                                        −𝑎
                                                                                                                                                                              arcsin 𝑏𝑥 −𝑥 +𝐶
为 𝑖 个 人 报 𝑚 的 胜 利 者, 则 𝑓1,𝑚 =                         断调整 𝑝 𝑗 𝑤𝑖,𝑗 最小的那个i的概率 𝑝𝑖 , 直
                                                                    Í                                                                      4                             4
0, 𝑓𝑖,𝑚 = (𝑓𝑖 −1,𝑚 + 𝑚) mod 𝑖 .                       至无法调整或者 𝑝 𝑗 = 1 为止.                                   三角函数的积分 (省略 +𝐶 )
                                                                                                            ∫                                                                 ∫
                                                                                                            ∫ tan 𝑥d𝑥 = − ln | cos 𝑥 |                                        ∫ sec 𝑥d𝑥 = tan 𝑥
                                                                                                                                                                                    2
6.1.27 概率相关
                                                                                                            ∫ cot 𝑥d𝑥 = ln | sin 𝑥𝜋| 𝑥 
                                                                                                                                                                                    2 𝑥d𝑥 = − cot 𝑥
𝐷 (𝑋 ) = 𝐸 (𝑋 − 𝐸 (𝑋 ) ) 2 = 𝐸 (𝑋 2 ) − (𝐸 (𝑋 ) ) 2 , 𝐷 (𝑋 + 𝑌 ) = 𝐷 (𝑋
                                                                      Í𝑚) + 𝐷 (𝑌 ),                                                                                           ∫ csc
                              Í∞                                            𝑥𝑖2                               sec 𝑥d𝑥 = ln tan 4 + 2
𝐷 (𝑎𝑋 ) = 𝑎 𝐷 (𝑋 ), 𝐸 [𝑥 ] = 𝑖=1 𝑃 (𝑋 ≥ 𝑖 ) , 𝑚 个数的方差：𝑠 2 =
            2                                                           𝑖=1
                                                                                − 𝑥2                                                                                          ∫ sec 𝑥 tan 𝑥d𝑥 = sec 𝑥
                                                                         𝑚                                  ∫ ln | sec 𝑥 + tan 𝑥 | 𝑥
                                                                                                            =
                                                                                                                                                                              ∫ csc 𝑥 cot 𝑥d𝑥𝑥 = −1 csc 𝑥
6.1.28 邻接矩阵行列式的意义                                                                                             csc 𝑥d𝑥 = ln tan 2
                                                                                                                                                                              ∫ sin 𝑥d𝑥 = 2𝑥 − 14 sin 2𝑥
                                                                                                                                                                                   2
在无向图中取若干个环, 一种取法权值就是边权的乘积, 对行列式的贡献是 (−1) even , 其中                                                          = ln | csc 𝑥 − cot 𝑥 |
even 是偶环的个数.                                                                                                                                                                    cos2 𝑥d𝑥 = 2 + 4 sin 2𝑥
6.1.29 Others (某些近似数值公式在这里)                                                                                 ∫                                                    ∫
                                                                                                                  𝑛                      𝑛−1 𝑥 cos 𝑥 + 𝑛−1              𝑛−2 𝑥d𝑥
                                                                                                            ∫ sin 𝑥d𝑥 = −1 𝑛 sin
                                                                                                                                  1
                             Õ∞
                                 𝑥𝑛               𝑥2 𝑥3                                                                                                       𝑛 ∫ sin
                      𝑒𝑥 =            = 1+𝑥 +          +     +···                                             cos𝑛 𝑥d𝑥 = 𝑛 cos𝑛−1 𝑥 sin 𝑥 + 𝑛−1                   cos 𝑛−2 𝑥d𝑥
                             𝑛=0
                                 𝑛!                2!     3!                                                ∫ d𝑥                                        ∫ 𝑛 d𝑥
                                                                                                                                       cos 𝑥
                                 Õ
                                 ∞                                                                                𝑛     = − 𝑛−11
                                                                                                                                    sin𝑛−1 𝑥
                                                                                                                                               + 𝑛−2
                                                                                                                                                  𝑛−1∫ sin𝑛−2 𝑥
                                   (−1) 𝑛 𝑥 2𝑛+1                𝑥3 𝑥5                                       ∫ sind𝑥 𝑥               sin 𝑥       𝑛−2
                    sin(𝑥 ) =                          =𝑥 −        +    −···                                  cos𝑛 𝑥 = 𝑛−1 cos𝑛−1 𝑥 + 𝑛−1
                                                                                                                             1                               d𝑥
                                         (2𝑛 + 1)!              3!   5!                                                                                   cos𝑛−2 𝑥
                                 𝑛=0                                                                                        ∫
                                  Õ∞
                                         (−1) 𝑛 𝑥 2𝑛     𝑥2 𝑥4                                                                   cos𝑚 𝑥 sin𝑛 𝑥d𝑥
                     cos(𝑥 ) =                       =1−    +    −···
                                           (2𝑛)!         2!   4!                                                                                                           ∫
                                     𝑛=0                                                                                        1                                 𝑚−1
                     Õ
                     ∞                                                                                                    =           cos𝑚−1 𝑥 sin𝑛+1 𝑥 +                      cos𝑚−2 𝑥 sin𝑛 𝑥d𝑥
                       (−1) 𝑛+1 𝑥 𝑛                𝑥2 𝑥3                                                                    𝑚 +𝑛                                  𝑚 +𝑛
     ln(1 + 𝑥 ) =                          =𝑥 −      +   −···               for     −1<𝑥 ≤1                                                                                   ∫
                                 𝑛                 2   3                                                                            1                                 𝑛−1
                     𝑛=1                                                                                                  =−               cos𝑚+1 𝑥 sin𝑛−1 𝑥 +                    cos𝑚 𝑥 sin𝑛−2 𝑥d𝑥
                     Õ∞
                       (−1) 𝑛 𝑥 2𝑛+1       𝑥3 𝑥5                                                                                𝑚 +𝑛                                 𝑚+1
    arctan(𝑥 ) =                     =𝑥 −     +    − · · · for − 1 ≤ 𝑥 ≤ 1                                                    
                                                                                                                                                    𝑎 tan 𝑥2 +𝑏
                   𝑛=0
                         2𝑛  +  1           3    5                                                          ∫                        2
                                                                                                                               √𝑎2 −𝑏 2 arctan √𝑎2 −𝑏√2
                                                                                                                                                                          (𝑎 2 > 𝑏 2 )
                  ∞  
                                                                                                                  d𝑥
                 Õ   𝑘 𝑛               𝑘 (𝑘 − 1) 2 𝑘 (𝑘 − 1) (𝑘 − 2) 3                                        𝑎+𝑏 sin 𝑥    =
                                                                                                                                               𝑎 tan 𝑥 +𝑏 − 𝑏 2 −𝑎 2
    (1 + 𝑥 ) 𝑘 =        𝑥 = 1 + 𝑘𝑥 +            𝑥 +                 𝑥 +···                                                           1
                                                                                                                               √𝑏 2 −𝑎2 ln 𝑎 tan 𝑥 +𝑏+√𝑏 2 −𝑎2
                                                                                                                                                      2                    (𝑎 2 < 𝑏 2 )
                     𝑛                     2!               3!                                                                         q             2 
                                                                                                                                                           q               
                 𝑛=0                                                                                                           
                                                                                                                               
                                                Õ
                                                ∞                                                                              
                                                                                                                                  2       𝑎+𝑏
                                                                                                                                                arctan        𝑎−𝑏
                                                                                                                                                                    tan 𝑥
                                                                                                                                                                                (𝑎 2 > 𝑏 2 )
                                                  (2𝑛 − 1)!! 𝑥 2𝑛+1                                         ∫                  
                                                                                                                                𝑎+𝑏 𝑎−𝑏
                                                                                                                                                              𝑎+𝑏       2
                           arcsin(𝑥 ) = 𝑥 +                                                                        d𝑥                                        q
                                                       (2𝑛)!! 2𝑛 + 1                                          𝑎+𝑏 cos 𝑥 =              q           tan 𝑥2 + 𝑏𝑎+𝑏
                                                𝑛=1                                                                                       𝑎+𝑏
                                                        √ ∞                                                                       1
                                                                                                                                                ln           q −𝑎               (𝑎 2 < 𝑏 2 )
      𝜋                                                2 2 Õ (4𝑘 )!(1103 + 26390𝑘 )                                            
                                                                                                                                𝑎+𝑏 𝑎−𝑏
               1 1 1 1                  1                                                                                                           tan 𝑥2 − 𝑏𝑎+𝑏
          =1− + − + −··· , =                                                                                ∫                                                  −𝑎
       4       3 5 7 9                  𝜋     9801          (𝑘!) 4 3964𝑘                                                d𝑥
                                                                                                                                        = 𝑎𝑏1
                                                                                                                                               arctan 𝑏𝑎 tan 𝑥
                                                   𝑘=0                                                        𝑎 2 cos2 𝑥 +𝑏 2 sin2 𝑥
    Õ1
     𝑛                                                                                                    ∫
                    1          1                                                                                         d𝑥                           tan 𝑥 +𝑎
                                                                                                                                        = 2𝑎𝑏 ln 𝑏𝑏 tan
                                                                                                                                             1
           ≈ ln 𝑛 +     +                + Γ, (Γ ≈ 0.5772156649015328606065)                                                                               𝑥 −𝑎
                                                                                                            ∫ 𝑎 cos 𝑥 −𝑏 sin 1𝑥
                                                                                                                2     2     2     2
    𝑖=1
         𝑛          2     24(𝑛 + 0.5) 2
                                                                                                              𝑥 sin 𝑎𝑥d𝑥 = 𝑎2 sin 𝑎𝑥 − 𝑎1 𝑥 cos 𝑎𝑥
                       √     𝑛            1        1         1                                              ∫
                 𝑛! = 2𝜋𝑛 ( ) 𝑛 (1 +          +        + 𝑂( 3 ))                                              𝑥 2 sin 𝑎𝑥d𝑥 = − 𝑎1 𝑥 2 cos 𝑎𝑥 + 𝑎22 𝑥 sin 𝑎𝑥 + 𝑎23 cos 𝑎𝑥
                             𝑒           12𝑛 288𝑛 2         𝑛

---

<!-- page: 24 -->

     上海交通大学 Shanghai Jiao Tong University                                                      Nemesis                                                              Page 23
     ∫
       𝑥 cos 𝑎𝑥d𝑥 = 𝑎12 cos 𝑎𝑥 + 𝑎1 𝑥 sin 𝑎𝑥                                                        44            sys.__stdout__.write(_OUTPUT_BUFFER.getvalue())
     ∫
       𝑥 2 cos 𝑎𝑥d𝑥 = 𝑎1 𝑥 2 sin 𝑎𝑥 + 𝑎22 𝑥 cos 𝑎𝑥 − 𝑎23 sin 𝑎𝑥
     反三角函数的积分 (其中 𝑎 > 0 )
     ∫
     ∫                               2
                                         √
        arcsin 𝑥𝑎 d𝑥 = 𝑥 arcsin 𝑥𝑎 + 𝑎 2 − 𝑥 2 + 𝐶
                              2                             √
                                                                                                         7. Miscellany
        𝑥 arcsin 𝑥𝑎 d𝑥 = ( 𝑥2 − 𝑎4 ) arcsin 𝑥𝑎 + 𝑥4 𝑥 2 − 𝑥 2 + 𝐶                                        7.1      Zeller 日期公式
     ∫                        3                                 √
        𝑥 2 arcsin 𝑥𝑎 d𝑥 = 𝑥3 arcsin 𝑥𝑎 + 19 (𝑥 2 + 2𝑎 2 ) 𝑎 2 − 𝑥 2 + 𝐶                            1 // weekday=(id+1)%7;{Sun=0,Mon=1,...}   getId(1, 1, 1) = 0
     ∫                                      √
        arccos 𝑥𝑎 d𝑥 = 𝑥 𝑎𝑟𝑐𝑐𝑜𝑠 𝑥𝑎 − 𝑎 2 − 𝑥 2 + 𝐶                                                  2 int getId(int y, int m, int d) {
     ∫                         2      2                       √                                     3    if (m < 3) { y ‐‐; m += 12; }
        𝑥 arccos 𝑥𝑎 d𝑥 = ( 𝑥2 − 𝑎4 ) arccos 𝑥𝑎 − 𝑥4 𝑎 2 − 𝑥 2 + 𝐶                                   4    return 365 * y + y / 4 ‐ y / 100 + y / 400 + (153 * (m ‐
     ∫                         3                                 √
        𝑥 2 arccos 𝑥𝑎 d𝑥 = 𝑥3 arccos 𝑥𝑎 − 19 (𝑥 2 + 2𝑎 2 ) 𝑎 2 − 𝑥 2 + 𝐶                                          ↩→ 3)   + 2) / 5 + d ‐ 307; }
     ∫                                                                                               5 // y<0: 统一加400的倍数年
        arctan 𝑥𝑎 d𝑥 = 𝑥 arctan 𝑥𝑎 − 𝑎2 ln(𝑎 2 + 𝑥 2 ) + 𝐶
     ∫                                                                                               6 auto date(int id) {
        𝑥 arctan 𝑥𝑎 d𝑥 = 12 (𝑎 2 + 𝑥 2 ) arctan 𝑥𝑎 − 𝑎2 𝑥 + 𝐶                                        7    int x=id+1789995, n, i, j, y, m, d;
     ∫                        3                              3
        𝑥 2 arctan 𝑥𝑎 d𝑥 = 𝑥3 arctan 𝑥𝑎 − 𝑎6 𝑥 2 + 𝑎6 ln(𝑎 2 + 𝑥 2 ) + 𝐶                             8    n = 4 * x / 146097; x ‐= (146097 * n + 3) / 4;
     指数函数的积分                                                                                         9    i = (4000 * (x + 1)) / 1461001; x ‐= 1461 * i / 4 ‐ 31;
     ∫                                                                                              10    j = 80 * x / 2447; d = x ‐ 2447 * j / 80; x = j / 11;
        𝑎𝑥 d𝑥 = ln1𝑎 𝑎𝑥 + 𝐶
     ∫                                                                                              11    m = j + 2 ‐ 12 * x; y = 100 * (n ‐ 49) + i + x;
        e𝑎𝑥 d𝑥 = 𝑎1 𝑎𝑎𝑥 + 𝐶                                                                         12    return make_tuple(y, m, d); }
     ∫
        𝑥e𝑎𝑥 d𝑥 = 𝑎12 (𝑎𝑥 − 1)𝑎𝑎𝑥 + 𝐶
     ∫                                  ∫
        𝑥 𝑛 e𝑎𝑥 d𝑥 = 𝑎1 𝑥 𝑛 e𝑎𝑥 − 𝑛𝑎 𝑥 𝑛−1 e𝑎𝑥 d𝑥                                                        7.2      有理数二分: Stern-Brocot 树, Farey 序列
     ∫
        𝑥𝑎𝑥 d𝑥 = ln𝑥𝑎 𝑎𝑥 − (ln1𝑎) 2 𝑎𝑥 + 𝐶                                                          1 def build(a, b, c, d, level = 1):
     ∫                                     ∫
        𝑥 𝑛 𝑎𝑥 d𝑥 = ln1𝑎 𝑥 𝑛 𝑎𝑥 − ln𝑛𝑎 𝑥 𝑛−1𝑎𝑥 d𝑥                                                   2     x = a + c; y = b + d
     ∫                                                                                              3     build(a, b, x, y, level + 1)
        e𝑎𝑥 sin 𝑏𝑥d𝑥 = 𝑎2 +𝑏  1
                                  2e
                                    𝑎𝑥 (𝑎 sin 𝑏𝑥 − 𝑏 cos 𝑏𝑥 ) + 𝐶
     ∫                                                                                              4     build(x, y, c, d, level + 1)
        e cos 𝑏𝑥d𝑥 = 𝑎2 +𝑏 2 e𝑎𝑥 (𝑏 sin 𝑏𝑥 + 𝑎 cos 𝑏𝑥 ) + 𝐶
          𝑎𝑥                  1                                                                     5 build(0, 1, 1, 0) # 最简分数, Stern‐Brocot
     ∫                                                                                              6 build(0, 1, 1, 1) # 最简真分数, Farey
        e𝑎𝑥 sin𝑛 𝑏𝑥d𝑥            =             1
                                          𝑎 2 +𝑏 2 𝑛 2
                                                       e𝑎𝑥 sin𝑛−1 𝑏𝑥 (𝑎 sin 𝑏𝑥 − 𝑛𝑏 cos 𝑏𝑥 )    +
     𝑛 (𝑛−1)𝑏 2
                   ∫
                     e𝑎𝑥 sin𝑛−2 𝑏𝑥d𝑥
     ∫𝑎2 +𝑏 2 𝑛2
          𝑎𝑥       𝑛
                                                                                                         7.3      黄金三分
        e cos 𝑏𝑥d𝑥               =             1
                                          𝑎 2 +𝑏 2 𝑛 2
                                                       e𝑎𝑥 cos𝑛−1 𝑏𝑥 (𝑎 cos 𝑏𝑥 + 𝑛𝑏 sin 𝑏𝑥 )    +
     𝑛 (𝑛−1)𝑏 2
                   ∫                                                                                1 constexpr LD R = (sqrt(5) ‐ 1) / 2;
      𝑎 2 +𝑏 2 𝑛 2
                     e𝑎𝑥 cos𝑛−2 𝑏𝑥d𝑥                                                                2 auto split = [](LD l, LD r) { return l + (r ‐ l) * R; };
     对数函数的积分
     ∫
                                                                                                    3 LD solve(LD a, LD c, auto f) {
       ln 𝑥d𝑥 = 𝑥 ln 𝑥 − 𝑥 + 𝐶                                                                      4    LD b = split(a, c), bv = f(b);
     ∫ d𝑥                                                                                           5    for (int _ = T; _; _‐‐) {
              = ln ln 𝑥 + 𝐶
     ∫ 𝑥 ln 𝑥                                                                                       6       LD x = split(a, b), xv = f(x);
       𝑥 𝑛 ln 𝑥d𝑥 = 𝑛+1 1
                          𝑥 𝑛+1 (ln 𝑥 − 𝑛+1
                                          1
                                              ) +𝐶                                                  7       if (xv < bv) c = b, b = x, bv = xv; // 最小化, 注意符号
     ∫                               ∫
              𝑛               𝑛
       (ln 𝑥 ) d𝑥 = 𝑥 (ln 𝑥 ) − 𝑛 (ln 𝑥 ) 𝑛−1 d𝑥                                                    8       else a = c, c = x;
     ∫                                             𝑛
                                                     ∫
       𝑥 𝑚 (ln 𝑥 ) 𝑛 d𝑥 = 𝑚+1
                            1
                               𝑥 𝑚+1 (ln 𝑥 ) 𝑛 − 𝑚+1   𝑥 𝑚 (ln 𝑥 ) 𝑛−1 d𝑥                           9    } return bv; }
     6.3 Python Hint
 1    def RandomAndList():                                                                               7.4      DP 优化
 2       import random                                                                                   7.4.1     四边形不等式
 3       random.normalvariate(0.5, 0.1)
                                                                                                    1 // 𝑎 ≤ 𝑏 ≤ 𝑐 ≤ 𝑑 : 𝑤 (𝑏, 𝑐) ≤ 𝑤 (𝑎, 𝑑),
 4       l = [str(i) for i in range(9)]
 5       sorted(l), min(l), max(l), len(l)                                                                  ↩→   𝑤 (𝑎, 𝑐) + 𝑤 (𝑏, 𝑑) ≤ 𝑤 (𝑎, 𝑑) + 𝑤 (𝑏, 𝑐)
 6       random.shuffle(l)                                                                          2 for (int len = 2; len <= n; ++len) {
 7       l.sort(key=lambda x:x ^ 1,reverse=True)                                                    3    for (int l = 1, r = len; r <= n; ++l, ++r) {
 8       from functools import cmp_to_key                                                           4       f[l][r] = INF;
 9       l.sort(key=cmp_to_key(lambda x, y:(y^1)‐(x^1)))                                            5       for (int k = m[l][r ‐ 1]; k <= m[l + 1][r]; ++k) {
10       from itertools import *                                                                    6          if (f[l][r] > f[l][k] + f[k + 1][r] + w(l, r)) {
11       for i in product('ABCD', repeat=2):                                                        7             f[l][r] = f[l][k] + f[k + 1][r] + w(l, r);
12          pass # AA AB AC AD BA BB BC BD CA CB CC CD DA DB DC DD                                  8             m[l][r] = k;
13       for i in permutations('ABCD', repeat=2):
                                                                                                    9    } } }
14          pass # AB AC AD BA BC BD CA CB CD DA DB DC
15       for i in combinations('ABCD', repeat=2):
16          pass # AB AC AD BC BD CD                                                                     7.4.2 树形背包优化
17       for i in combinations_with_replacement('ABCD', repeat=2):
                                                                                                         限制: 必须取与根节点相连的一个连通块.
18          pass # AA AB AC AD BB BC BD CC CD DD
                                                                                                         转化: 一个点的子树对应于DFS序中的一个区间. 则每个点的决策为, 取该
19    def FractionOperation():
                                                                                                         点, 或者舍弃该点对应的区间. 从后往前dp, 设 𝑓 (𝑖, 𝑣) 表示从后往前考虑
20       from fractions import Fraction
21       a.numerator, a.denominator, str(a)                                                              到i号点, 总体积为V的最优价值, 设i号点对应的区间为 [𝑖, 𝑖 + size𝑖 − 1] , 转
22       a = Fraction(0.233).limit_denominator(1000)                                                     移为 𝑓 (𝑖, 𝑣) = max{𝑓 (𝑖 + 1, 𝑉 − 𝑣 𝑖 ) + 𝑤 𝑖 , 𝑓 (𝑖 + size𝑖 , 𝑣)} .
23    def DecimalOperation():                                                                            如果要求任意连通块, 则点分治后转为指定根的连通块问题即可.
24       from decimal import Decimal, getcontext, FloatOperation                                         7.4.3 𝑂 (𝑛 · max 𝑎𝑖 ) Subset Sum
25       getcontext().prec = 100
26       getcontext().rounding = getattr(decimal, 'ROUND_HALF_EVEN')                                 1 int SubsetSum(vector <int> &a, int t) {
27       # default; other: FLOOR, CELILING, DOWN, ...                                                2   int B = *max_element(a.begin(), a.end());
28       getcontext().traps[FloatOperation] = True                                                   3   int n = (int) a.size(), s = 0, i = 0;
29       Decimal((0, (1, 4, 1, 4), ‐3)) # 1.414                                                      4   while (i < n && s + a[i] <= t) s += a[i ++];
30       a = Decimal(1<<31) / Decimal(100000)                                                        5   if (i == n) return s;
31       print(f"{a:.9f}") # 21474.83648                                                             6   vector <int> f(2 * B + 1, ‐1), pre (B + 1, ‐1);
32       print(a.sqrt(), a.ln(), a.log10(), a.exp(), a ** 2)                                         7   f[s ‐ (t ‐ B)] = i;
33    def Complex():                                                                                 8   for ( ; i < n; i++) {
34       a = 1‐2j                                                                                    9     s += a[i];
35       print(a.real, a.imag, a.conjugate())
                                                                                                    10     for (int d = 0; d <= B; d++) pre[d] = max(0, f[d + B]);
36    def FastIO():
                                                                                                    11     for (int d = B; d >= 0; d‐‐)
37       import sys, atexit, io
38       _INPUT_LINES = sys.stdin.read().splitlines()                                               12       f[d + a[i]] = max(f[d + a[i]], f[d]);
39       input = iter(_INPUT_LINES).__next__                                                        13     for (int d = 2 * B; d > B; ‐‐d)
40       _OUTPUT_BUFFER = io.StringIO()                                                             14       for (int j = pre[d ‐ B]; j < f[d]; j++)
41       sys.stdout = _OUTPUT_BUFFER                                                                15         f[d ‐ a[j]] = max(f[d ‐ a[j]], j); }
42       @atexit.register                                                                           16   for (i = 0; i <= B; i++) if (f[B ‐ i] >= 0) return t ‐ i;}
43       def write():

---

<!-- page: 25 -->

     上海交通大学 Shanghai Jiao Tong University                         Nemesis                                                                Page 24
     7.5 Hash Table                                                      import sys
                                                                         sys.set_int_max_str_digits(0) # >= Python 3.11
1    template <class T,int P = 314159/*,451411,1141109,2119969*/>        T = 3
2    struct hashmap {                                                    for _ in range(T):
                                                                             assert str(3 ** 100000 * 10 ** 100000)[:9] == "133497141"
3    ULL id[P]; T val[P];                                                # 机房: 590ms; (1s T = 5)
4    int R[P]; // del: few clears                                        # QOJ: 201ms; (1s T = 16)
5    hashmap() {memset(id, ‐1, sizeof id);}                              # CF Python3: 1100ms; (1s T = 2.9) Pypy3: 310ms; (1s T = 12)
6    T get(const ULL &x) const {                                      提交前检查:
7       for (int i = int(x % P), j = 1; ~id[i]; i = (i + j) % P,
                                                                     • 保存, 编译, 测过样例，测过边界 n=0/1
          ↩→ j = (j + 2) % P /*unroll if needed*/) {
                                                                     • 数组开到 n/2n/m/n+32, LL/int128
 8          if (id[i] == x) return val[i]; }                         • 多测清空，调用初始化, 清空时数组大小, 没读入完就 break
 9      return 0; }                                                  • 取模取到正值，输入输出格式 (%Lf, %llu)
10   T& operator [] (const ULL &x) {                                 • 时间空间限制, 关闭流同步，时间卡开 Ofast
11      for (int i = int(x % P), j = 1;        ; i = (i + j) % P,
                                                                      提交后检查:
          ↩→ j = (j + 2) % P) {
12          if (id[i] == x) return val[i];                           • sum 多组输入: 应只用了输入内容 (memset TL, 错下标 WA), 是否正确撤销
13          else if (id[i] == ‐1llu) {
                                                                     • 输入是否保证顺序，保证是整数，保证三点共线
                                                                     • int/LL 溢出, INF/-1 大小，浮点数 eps 和 = 0, __builtin_popcountll
14             id[i] = x;
                                                                     • 类似 pair <LL, int> x = pair <int, int>(); 不会报警告
15             R[++R[0]] = i; // del: few clears
                                                                     • 离散化与二分: lower_bound upper_bound +‐1, begin(), end()
16             return val[i]; } } }
                                                                     • 自定义排序：排序方向，比较函数为小于，考虑坏点: 如叉积 (0, 0)
17   void clear() { // del: few clears
                                                                     • 样例是否为对称/回文的？考虑构造不对称的情况，正序逆序
18   for (int &x = R[0]; x; id[R[x]] = ‐1, val[R[x]] = 0, ‐‐x);      • 复制过的代码对应位置正确修改
19   void fullclear() {memset(id, ‐1, sizeof id); R[0] = 0; } };     • 检查变量重名，变量复用
                                                                     • 图: 边标号初始是否为 1, 单双向边, 反向边边权
     7.6 基数排序                                                        • 几何: 共线, sqrt(-0.0), nan / inf, 重点, 除零 det/dot, 旋转方向, 求得的是否是所求
1 const int SZ = 1 << 8; // almost always fit in L1 cache                7.9     Constant Table 常数表
2 void SORT(int a[], int c[], int n, int w) {                            Random primes generated at Thu Sep 5 00:24:54 2024
3    for(int i=0; i<SZ; i++) b[i] = 0;                                   5e2 367 383 397 419 433 463 499 509 563 577 587 593 601 607
                                                                         1e3 839 911 919 937 983 997 1013 1019 1069 1093 1097 1129
4    for(int i=1; i<=n; i++) b[(a[i]>>w) & (SZ‐1)]++;                    3e4 27917 28019 28219 28933 28979 29741 30011 30859 30869
5    for(int i=1; i<SZ; i++) b[i] += b[i ‐ 1];                           1e5 95261 97117 100343 101209 102881 105097 105401 106321
6    for(int i=n; i; i‐‐) c[b[(a[i]>>w) & (SZ‐1)]‐‐] = a[i];}            3e5 282143 287099 287149 291037 291817 294953 305131 306643
                                                                         1e6 961021 967061 969109 969821 981947 1002343 1047667
7 void Sort(int *a, int n){
                                                                         2e6 1862153 1868059 1870783 1902469 1970201 2031977 2095813
8    SORT(a, c, n, 0); SORT(c, a, n, 8);                                 5e6 4671167 4716931 4764673 4874651 4898809 5237899 5291129
9    SORT(a, c, n, 16); SORT(c, a, n, 24); }                             1e7 9379681 9444893 9578507 9642229 9743947 10069847 10187491
                                                                         2e7 19245167 19678793 19832651 19929407 20795521 21211783
                                                                         1e9 943614449 945142543 969904909 993067709 1025543047 1060397869
     7.7 Hacks: O3, 读入优化, Bitset, builtin                                2e9 1901826853 1976935109 2060076593 2061902989 2070121271
                                                                         NTT 976224257 r=3 (20) 985661441 r=3 (22) 998244353 r=3 (23)
 1   //fast = O3 + ffast‐math + fallow‐store‐data‐races                  1004535809 r=3 (21) 1007681537 r=3 (20) 1012924417 r=5 (21)
 2   #pragma GCC optimize("Ofast")                                       1045430273 r=3 (20) 1051721729 r=6 (20) 1053818881 r=7 (20)
 3   #pragma GCC target("sse2,abm,fma,mmx,avx2,tune=native")
 4   const int SZ = 1 << 16; int getc() {                                   𝑛       log10 𝑛           𝑛!   𝐶 (𝑛, 𝑛/2)   LCM(1 . . . 𝑛)      𝑃𝑛
 5      static char buf[SZ], *ptr = buf, *top = buf;                         2    0.30102999             2          2                 2       2
 6      if (ptr == top) {                                                    3    0.47712125             6          3                 6       3
 7         ptr = buf, top = buf + fread(buf, 1, SZ, stdin);                  4    0.60205999           24           6                12       5
                                                                             5    0.69897000          120          10                60       7
 8         if (top == buf) return ‐1; }
                                                                             6    0.77815125          720          20                60      11
 9      return *ptr++; }
                                                                             7    0.84509804         5040          35               420      15
10   idx=b._Find_first();idx!=b.size();idx=b._Find_next(idx);
                                                                             8    0.90308998        40320          70               840      22
11   struct HashFunc{size_t operator()(const KEY &key)const{}};              9    0.95424251      362880          126              2520      30
12   __builtin_uaddll_overflow(a, b, &c) // binary big int                  10              1    3628800          252              2520      42
13   void GospersHack(int k, int n) {                                       11    1.04139269    39916800          462            27720       56
14      for (int s = (1 << k) ‐ 1, c, r; s < (1 << n);                      12    1.07918125 479001600            924            27720       77
15      c = s & ‐s, r = s + c, s = (((r ^ s) >> 2) / c) | r); }             15    1.17609126      1.31e12        6435           360360      176
                                                                            20    1.30103000      2.43e18      184756       232792560       627
     7.8 试机赛与纪律文件                                                           25    1.39794001      1.55e25    5200300      26771144400      1958
 • 检查身份证件: 护照、学生证、胸牌以及现场所需通行证。                                              30    1.47712125      2.65e32 155117520           1.444e14     5604
 • 确认什么东西能带进场。特别注意: 智能手表、金属（钥匙）等等。                                         𝑃𝑛        3733840    20422650    96646760     190569292100   1e9114
 • 测试鼠标、键盘、显示器和座椅。如果有问题, 立刻联系工作人员。                                            𝑛≤            10       100      1e3      1e4        1e5     1e6
 • 测试比赛提交方式。如果有 submit 命令, 确认如何使用。                                         max 𝜔 (𝑛)         2        3        4        5          6       7
 • 设置自动保存，自动备份。                                                            max 𝑑 (𝑛)         4        12      32        64       128      240
 • 测试编译器版本。C++20 cin >> (s + 1); C++17 auto [x, y]: a; C++14 []               𝜋 (𝑛)          4        25      168     1229       9592   78498
   (auto x, auto y); C++11 auto; bits/stdc++.h; pb_ds。                        𝑛≤           1e7       1e8      1e9      1e10      1e11    1e12
     #include <ext/rope>                                                   max 𝜔 (𝑛)         8        8        9        10        10      11
     using namespace __gnu_cxx;                                            max 𝑑 (𝑛)       448       768     1344     2304       4032    6720
     rope <int> R; R.insert(y, x); R[x]; R.erase(x, 1);
     #include <ext/pb_ds/assoc_container.hpp>                                 𝜋 (𝑛)      664579 5761455 5.08e7 4.55e8 4.12e9            3.7e10
     #include <ext/pb_ds/tree_policy.hpp>                                     𝑛≤           1e13      1e14    1e15      1e16      1e17    1e18
     using namespace __gnu_pbds;                                           max 𝜔 (𝑛)        12        12      13        13        14      15
     tree <int, null_type, less<int>, rb_tree_tag,
     tree_order_statistics_node_update> s;                                 max 𝑑 (𝑛)      10752     17280   26880     41472     64512   103680
     s.insert(1); s.find_by_order(0); s.order_of_key(5);                      𝜋 (𝑛)             Prime number theorem: 𝜋 (𝑥) ∼ 𝑥/log(𝑥)
 • 测试 __int128, __float128, long double
 • 测试代码长度限制，尝试触发 NO-OUTPUT, OUTPUT-LIMIT, RUN-ERROR。                     Vimrc, Bashrc
 • 测试 pragma 是否 CE。
                                                                     1   source $VIMRUNTIME/mswin.vim
 • 测试 clar: 如果问不同类型的愚蠢的问题, 得到的回复是否不一样?
                                                                     2   behave mswin
 • 测试 ‐fsanitize address, undefined, define _GLIBCXX_DEBUG
 • 测试 time 命令是否能显示内存占用。/usr/bin/time ‐v ./a.out                      3   set mouse=a ci ai si nu ts=4 sw=4 is hls backup undofile
 • 测试 clock() 是否能够正常工作; 测试本地性能与提交性能。                                 4   color slate
                                                                     5   map <F7> : ! make %<<CR>
     const int N = 1 << 20;
     for (int T = 4; T; T‐‐) {                                       6   map <F8> : ! time ./%< <CR>
         int a[N], b[N], c[N];
         for (int i = 0; i < N; i++) a[i] = i, b[i] = N ‐ i;
         ntt_init(N); ntt(a, N, 1); ntt(b, N, 1);                    1 export CXXFLAGS='‐g ‐Wall ‐Wextra ‐Wconversion ‐Wshadow
         for (int i = 0; i < N; i++) c[i] = a[i] * b[i];                    ↩→ ‐std=gnu++20'
         ntt(c, N, ‐1); assert (c[0] == 0xad223d); }                 2 ulimit ‐s 1048576
     // 机房: 190ms; (1s T = 21)
     // QOJ: 340+‐40ms; (1s T = 11.9)
     // CF: 1050+‐100ms; (1s T = 3.9)                                    VSCode: 打开自动保存

---

<!-- page: 26 -->

上海交通大学 Shanghai Jiao Tong University                         Nemesis                                                     Page 25




                                                                          ∫ 𝑝ℎ𝑖 √
STL 积分/求和                                                              4.  0
                                                                                  1 − 𝑘 2 sin2 𝜃𝑑𝜃 = 𝑒𝑙𝑙𝑖𝑛𝑡 _2(𝑘, 𝑝ℎ𝑖)
        ∫1                                     Γ (𝑥 )Γ (𝑦)                ∫ +∞ 𝑒 −𝑡
   1.  0
         𝑡 𝑥 −1 (1 − 𝑡) 𝑦−1𝑑𝑡 = 𝑏𝑒𝑡𝑎(𝑥, 𝑦) = Γ (𝑥+𝑦)                   5. 𝑛𝑢𝑚 𝑡 𝑑𝑡 = −𝑒𝑥𝑝𝑖𝑛𝑡 (−𝑛𝑢𝑚)
      ∫ +∞                                                                Í+∞ −𝑛𝑢𝑚
   2. 0 𝑡 𝑛𝑢𝑚−1𝑒 −𝑡 𝑑𝑡 = 𝑡𝑔𝑎𝑚𝑚𝑎(𝑛𝑢𝑚) = 𝑒 𝑙𝑔𝑎𝑚𝑚𝑎 (𝑛𝑢𝑚) = Γ(𝑛𝑢𝑚)         6. 𝑛=1   𝑛         = 𝑟𝑖𝑒𝑚𝑎𝑛𝑛 _𝑧𝑒𝑡𝑎(𝑛𝑢𝑚)
      ∫ 𝑝ℎ𝑖                                                                   ∫ 𝑎𝑟𝑔
             √ 𝑑𝜃                                                                    −𝑡
                                                                           2            2
   3. 0                    = 𝑒𝑙𝑙𝑖𝑛𝑡 _1(𝑘, 𝑝ℎ𝑖)                         7. √ 0 𝑒 𝑑𝑡 = 𝑒𝑟 𝑓 (𝑎𝑟𝑔)
                                                                            𝜋
              1−𝑘 2 sin2 𝜃
