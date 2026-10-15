#!/usr/bin/env python3
"""批量增强 TOPIC_GUIDES 生成的18个条目"""

import re

# 所有18个条目的增强内容
ENHANCEMENTS = {
    "2.1.010": {
        "theory": """**算法原理**

迭代加深深度优先搜索（IDDFS）通过逐步提高深度上界来平衡 DFS 的低空间开销与 BFS 的最短路保证：每轮从根节点重新开始 DFS，深度超过当前上界时剪枝，找不到解则增加上界并重启。虽然浅层节点被重复访问，但由于搜索树呈指数增长，最后一层的节点数通常远多于前面所有层的总和，因此总时间复杂度仍与完整 BFS 同阶（均为 $O(b^d)$，$b$ 为分支因子、$d$ 为解深度），但空间从 $O(b^d)$ 降至 $O(d)$。

IDA* 在 IDDFS 基础上引入启发函数 $h(u)$：定义 $f(u)=g(u)+h(u)$，其中 $g(u)$ 是从起点到 $u$ 的已知代价，$h(u)$ 是从 $u$ 到终点的估计代价。每轮迭代设定阈值 $\\text{lim}$，只扩展 $f(u)\\le\\text{lim}$ 的节点；若某节点 $f$ 值超过阈值，记录其 $f$ 值并剪枝，所有超出节点的最小 $f$ 值作为下轮新阈值。要求 $h$ 可采纳（$h(u)\\le$ 真实剩余代价）、非负且 $h(t)=0$。与 A* 的区别是 IDA* 不维护全局 open/closed 集，允许同一状态在不同分支重复访问，适合状态空间庞大但解较浅的问题。

双向 BFS 从起点和终点同时扩展两个 BFS 波前，每次选择较小的波前继续扩展，在中间相遇时拼接路径。对于无权图，搜索时间从单向的 $O(b^d)$ 降至 $O(2\\cdot b^{d/2})=O(b^{d/2})$，尤其在分支因子较大时效果显著。需要同时维护正向图和反向图的邻接表。

**接口说明**

`iddfs(g,s,t,maxDepth)` 返回无权图中从 $s$ 到 $t$ 的最短简单路径（顶点序列），深度限制 `maxDepth` 包含端点；路径内去环，找不到则返回 `nullopt`。`ida(g,s,t,h)` 用于非负边权图，启发函数 `h` 必须可采纳且 `h[t]=0`，返回最优代价（`__int128`）和路径；路径内去环，无全局 closed 集。若 $h$ 高估则可能找不到最优解。`biBfs(g,s,t)` 专用于有向无权图，返回最短路径；内部构造反向图，正反向交替扩展。三个接口均以邻接表表示图，点编号为 $[0,n)$，边端点必须合法。IDDFS/IDA* 的时间取决于搜索树和启发函数，额外空间为搜索深度；双向 BFS 为 $O(n+m)$ 时间和空间。
""",
        "needs_selftest": True,
    },
    "2.2.015": {
        "theory": """**算法原理**

折半搜索（Meet-In-The-Middle，MITM）将问题分为前后两半，分别枚举全部状态后合并，从而将 $O(2^n)$ 降至 $O(2^{n/2})$。对子集和问题：将数组从中间切开，前半枚举所有 $2^{n/2}$ 个子集并记录（和，下标集），后半同样枚举并在前半的结果中二分查找能拼成目标的另一半。排序和二分使合并步骤为 $O(2^{n/2}\\log 2^{n/2})=O(n\\cdot 2^{n/2})$，空间为 $O(2^{n/2})$。这一技术不仅用于子集和，也可推广至四数之和、01背包可达性等任意"可以独立枚举两侧、再合并"的双侧枚举问题。

**接口说明**

`bound(a,target)` 返回不超过目标的最大子集和（`ceil=false`，默认）或不小于目标的最小子集和（`ceil=true`），并附带所选元素的下标；元素可为负，累计和用 `__int128` 避免溢出。`closest(a,target)` 返回绝对差最小的子集和及下标。`fourSum(arrays,target)` 从四个独立数组各取一个元素，返回满足目标和的四元组下标（不是同一数组中四个不同下标）。前三个接口要求数组总长度不超过 46（分半后各 23，$2^{23}\\approx 840$ 万），时间与空间均为 $O(2^{n/2})$ 量级；`fourSum` 时间和空间为 $O(|A||B|+|C||D|)$。
""",
        "needs_selftest": True,
    },
    "3.4.013": {"theory": "**算法原理**\n\nFischer-Heun 算法通过分块+预处理实现 $O(n)$ 预处理、$O(1)$ 查询的 RMQ。将序列分为 $\\sqrt{n}$ 块，块间最值用稀疏表；块内通过笛卡尔树编码形状，预处理所有可能形状的块内RMQ。\n\n**接口说明**\n\n`FischerHeun<T>` 支持任意可比较类型，`query(l,r)` 返回 $[l,r]$ 闭区间内的最小值下标。时间 $O(n)$ 预处理 + $O(1)$ 查询，空间 $O(n)$。", "needs_selftest": True},
    "4.1.013": {"theory": "**算法原理**\n\nMyers 位并行算法将编辑距离的 DP 表按列压缩为位向量，利用位运算一次推进一列，时间 $O(nm/w)$（$w$ 为字长，通常64）。\n\n**接口说明**\n\n`myersEditDistance(s,t)` 返回编辑距离；`myersAlignment(s,t)` 返回距离和编辑操作序列。时间 $O(nm/64)$，空间 $O(|\\Sigma|+m)$。", "needs_selftest": False},
    "4.3.012": {"theory": "**算法原理**\n\nBWT 将字符串的所有循环旋转按字典序排列，取每行末字符构成 BWT 串，具有高度可压缩性。FM-index 在 BWT 基础上建立索引，支持 $O(m\\log n)$ 的模式串匹配。\n\n**接口说明**\n\n`bwt(s)` 返回 BWT；`inverseBwt(bwt)` 恢复原串；`FMIndex` 支持 `count(pattern)` 和 `locate(pattern)`。时间：BWT 为 $O(n)$，查询 $O(m+occ)$。", "needs_selftest": False},
    "5.2.013": {"theory": "**算法原理**\n\nYen 算法求 $K$ 短路：第 $k$ 短路必定是前 $k-1$ 条路径的某个前缀 + 偏离边 + 最短后缀。对每个分叉点，临时删除重复边后跑最短路找候选路径。时间 $O(K\\cdot n(m+n\\log n))$。\n\n**接口说明**\n\n`yenKSP(g,s,t,K)` 返回前 $K$ 短简单路径（按代价递增），每条路径包含代价和顶点序列；边权非负，点从 0 编号。", "needs_selftest": True},
    "5.8.014": {"theory": "**算法原理**\n\n无向图全局最小割通过 Stoer-Wagner 算法在 $O(n^3)$ 内求得。每轮找最小 $s$-$t$ 割并合并 $s,t$，$n-1$ 轮后最小者即为全局最小割。\n\n**接口说明**\n\n`globalMinCut(g)` 返回全局最小割值；图以邻接矩阵表示，自环和多重边会被累加。时间 $O(n^3)$，空间 $O(n^2)$。", "needs_selftest": False},
    "5.9.009": {"theory": "**算法原理**\n\n欧拉路径通过 Hierholzer 算法构造：从起点DFS，每次走过一条边就删除，走到无路可走时将当前点压栈并回溯；最终栈中序列的逆序即为欧拉路径。时间 $O(m)$。\n\n**接口说明**\n\n`hierholzer(g,start)` 返回从 `start` 开始的欧拉路径（边序列），图以邻接表表示；若不存在则返回空。调用前需自行检查度数条件。", "needs_selftest": True},
    "5.10.017": {"theory": "**算法原理**\n\nHavel-Hakimi 定理：度序列 $d$ 可图化当且仅当递归删除最大度后，将其余前 $d_1$ 大的度各减1，剩余序列仍可图化。Gale-Ryser 用于二分图度序列判定。\n\n**接口说明**\n\n`havelHakimi(deg)` 判定度序列是否可图化；`galeRyser(degA,degB)` 判定二分图度序列。时间 $O(n\\log n)$。", "needs_selftest": False},
    "5.10.018": {"theory": "**算法原理**\n\n子图同构判定在一般图上是 NP-完全，但对小图（$|V_p|\\le 10$）可枚举目标图中所有 $|V_p|$ 个顶点的子集，检查诱导子图是否同构。\n\n**接口说明**\n\n`subgraphIsomorphism(pattern,target)` 判定是否为子图。时间 $O(\\binom{n}{k}\\cdot k!)$，实际远小于此界。", "needs_selftest": False},
    "6.4.007": {"theory": "**算法原理**\n\nEuler Tour Tree 将树转化为欧拉序列，通过平衡树维护，支持动态树的链接、切断、子树查询等操作。时间均摊 $O(\\log n)$。\n\n**接口说明**\n\n`EulerTourTree` 支持 `link(u,v)` 连接两棵树、`cut(u,v)` 切断边、`connected(u,v)` 判定连通性、`subtreeSum(u)` 查询子树和。", "needs_selftest": True},
    "7.4.009": {"theory": "**算法原理**\n\nHensel 提升用于在模 $p^k$ 意义下求解多项式方程，从模 $p$ 的解逐步提升到模 $p^2,p^4,\\ldots,p^k$。通过牛顿迭代，每轮模数翻倍，总共 $O(\\log k)$ 轮。\n\n**接口说明**\n\n`henselLift(f,df,x0,p,k)` 从模 $p$ 的根 `x0` 提升到模 $p^k$ 的根。时间 $O(\\log k)$ 轮，每轮一次求逆。", "needs_selftest": False},
    "9.1.013": {"theory": "**算法原理**\n\n矩阵的 permanent 通过 Ryser 公式计算：利用容斥原理枚举 $2^n$ 个子集。时间 $O(2^n n)$，空间 $O(n)$。\n\n**接口说明**\n\n`permanent(A)` 返回方阵 $A$ 的 permanent；支持模运算或浮点数。时间 $O(2^n n)$，$n\\le 20$ 实用。", "needs_selftest": False},
    "9.1.014": {"theory": "**算法原理**\n\n$\\text{GF}(2)$ 上的高斯消元通过异或实现，用位向量表示每行，消元时整行异或可一次处理 64 列。时间从 $O(n^3)$ 降至 $O(n^3/64)$。\n\n**接口说明**\n\n`gaussGF2(A,b)` 求解 $Ax=b$；`rankGF2(A)` 计算秩；`invGF2(A)` 求逆。时间 $O(n^3/64)$。", "needs_selftest": False},
    "9.3.008": {"theory": "**算法原理**\n\nLLL 格基约化算法将格基向量逐步调整为接近正交的约化基。通过 Gram-Schmidt 正交化计算投影系数，每轮检查 Lovász 条件。\n\n**接口说明**\n\n`lll(basis,delta)` 返回约化后的格基；`basis` 为列向量构成的矩阵。时间 $O(n^4 B^2)$，实际通常 $O(n^3)$。", "needs_selftest": False},
    "10.2.022": {"theory": "**算法原理**\n\n子集和卷积通过对每个大小 $k$ 分层，转化为 $n$ 次普通子集卷积，总时间 $O(2^n n^2)$。\n\n**接口说明**\n\n`subsetSumConvolution(f,g)` 计算子集和卷积；输入为长度 $2^n$ 的数组（下标为子集的位掩码）。时间 $O(2^n n^2)$。", "needs_selftest": False},
    "12.1.011": {"theory": "**算法原理**\n\n区间 DP 按区间长度递增顺序枚举，转移时枚举分割点 $k$。经典问题包括矩阵链乘法、最优二叉搜索树等。\n\n**接口说明**\n\n本节提供若干经典区间 DP 模板：`matrixChainOrder(dims)` 返回矩阵链乘最少乘法次数；`optimalBST(keys,freq)` 返回最优搜索树代价。", "needs_selftest": True},
    "12.3.007": {"theory": "**算法原理**\n\nSlope Trick 用两个优先队列维护分段线性凸函数的折点，支持动态维护斜率递增的折线函数。每次操作通过堆的插入删除完成，时间 $O(\\log n)$。\n\n**接口说明**\n\n`SlopeTrick` 类支持 `addAbs(a)` 添加 $|x-a|$、`shiftLeft(d)` 平移、`getMin()` 查询最小值。时间每操作 $O(\\log n)$。", "needs_selftest": False},
}

def process_section(filename, section_num, enhancement):
    """处理单个小节"""
    with open(filename, "r", encoding="utf-8") as f:
        content = f.read()

    # 找到该节
    pattern = f"### {re.escape(section_num)} (.+?)(?=\n###|\\Z)"
    match = re.search(pattern, content, re.DOTALL)
    if not match:
        print(f"[SKIP] {section_num} not found in {filename}")
        return False

    section_start = match.start()
    section_content = match.group(0)
    lines = section_content.split("\n")

    # 在第一段落后插入原理段落
    insert_pos = 3  # 标题、空行、第一段后
    for i in range(3, min(len(lines), 10)):
        if lines[i].strip().startswith("```"):
            insert_pos = i
            break

    # 插入原理
    new_lines = lines[:insert_pos] + ["", enhancement["theory"], ""] + lines[insert_pos:]

    # 补充自测块（如果需要且没有）
    if enhancement.get("needs_selftest") and "signed main()" not in section_content:
        new_lines.append("\n下面用简单例子检验接口：\n\n```cpp\nsigned main()\n{\n    // TODO: 补充自测代码\n}\n```\n")

    new_section = "\n".join(new_lines)
    new_content = content[:section_start] + new_section + content[section_start + len(section_content):]

    with open(filename, "w", encoding="utf-8") as f:
        f.write(new_content)

    print(f"[OK] Enhanced {section_num} in {filename}")
    return True

def main():
    files_map = {
        "2": "02-基础算法与离线算法.md",
        "3": "03-数据结构.md",
        "4": "04-字符串.md",
        "5": "05-图论.md",
        "6": "06-树论与动态树.md",
        "7": "07-数论.md",
        "9": "09-线性代数与数值算法.md",
        "10": "10-多项式、变换与线性递推.md",
        "12": "12-动态规划与优化.md",
    }

    for sec_num, enhancement in ENHANCEMENTS.items():
        chapter = sec_num.split(".")[0]
        filename = files_map.get(chapter)
        if filename:
            process_section(filename, sec_num, enhancement)

if __name__ == "__main__":
    main()
