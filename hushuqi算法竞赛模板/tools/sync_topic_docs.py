#!/usr/bin/env python3
"""Refresh explicitly marked chapter snippets from generated print headers."""
from __future__ import annotations
import argparse
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MARK = re.compile(r"(<!-- BEGIN HEADER: ([\w_]+\.hpp) -->\n).*?(\n<!-- END HEADER -->)", re.S)
TOPIC_HEADING = re.compile(r"^###\s+(\d+\.\d+\.\d{3})\s+.*$", re.M)
HEADER_NOTE = re.compile(r"^头文件：[ `]*include/full/(\d+_\d+_\d+)\.hpp[ `]*，打印版由生成器同步。\s*$", re.M)
# 目录中的「编号 → 标题」，用于把依赖头改写成可读的注释。
TITLES = dict(re.findall(r"^- \*\*(\d+\.\d+\.\d{3})\*\* (.+?)\s*$",
                         (ROOT / "目录.md").read_text(encoding="utf-8"), re.M))

# 新补条目的说明写在这里，避免只贴代码却没有可直接使用的接口约定。
TOPIC_GUIDES = {
    "2.1.010": "`iddfs(g,s,t,maxDepth)` 返回最短简单路径（顶点序列），限制深度包含端点；`ida(g,s,t,h)` 面向非负边权，`h` 必须可采纳且 `h[t]=0`，返回最优代价和路径；`biBfs(g,s,t)` 用于有向无权图并返回最短路径。图以邻接表表示，点编号为 `[0,n)`，边端点必须合法。IDDFS/IDA* 的时间取决于搜索树和启发函数，额外空间为搜索深度；双向 BFS 为 `O(n+m)` 时间和空间。",
    "2.2.015": "`bound(a,target)` 返回不超过目标的最大子集和，或启用 `ceil=true` 返回不小于目标的最小子集和，并附所选下标；`closest` 返回绝对差最小的子集；`fourSum` 从四个数组各取一个元素，返回满足目标和的下标。元素可为负，累计和用 `__int128`；前 3 个接口的总长度最多 46，时间与空间均为 `O(2^{n/2})` 量级，`fourSum` 时间和空间为 `O(|A||B|+|C||D|)`。",
    "3.4.013": "构造 `FischerHeun(a)` 后，`argmin(l,r)` 返回半开区间 `[l,r)` 中最小值下标，`query(l,r)` 返回最小值；区间必须非空，空数组只能构造、不能查询。相等最小值允许返回任一位置。Cartesian tree 加 ±1 Euler 深度序列，预处理 `O(n)`、查询 `O(1)`、空间 `O(n)`；对象不可拷贝，因为内部块表保存微表引用。",
    "4.1.013": "`edit_distance::dp(a,b)` 计算字节串 Levenshtein 距离，时间 `O(|a||b|)`、空间 `O(|b|)`；`myers(pattern,text)` 用 Myers 位并行算法，模式长度必须不超过 64 字节，时间 `O(|text|)`、空间 `O(256)` 个机器字。该接口按字节比较，不按 UTF-8 字符比较；空模式返回文本字节长度。",
    "4.3.012": "构造 `FMIndex(text)` 后可调用 `search(pattern)` 得到后缀数组中的匹配行区间，`locate(pattern)` 返回按文本位置升序排列的匹配起点，`restore()` 还原原串。字符按无符号字节处理，内部以 0 作为唯一哨兵，因此输入可含任意字节。当前实现用倍增排序建 SA，预处理 `O(n log^2 n)`，rank 查询为 `O(log n)`，search 为 `O(|pattern| log n)`，locate 另需 `O(k log k)` 排序。",
    "5.2.013": "向 `YenPaths(n)` 加入非负有向边后，`solve(s,t,k)` 返回最多前 `k` 条互不重复的最短简单路径，路径含顶点序列、边编号和 `__int128` 总权值；平行边按边编号区分。点编号须在 `[0,n)`，负权不支持。重复调用会重新求解。复杂度约为 `O(k n (m+n log n))`，适合小 `k` 的 loopless K shortest paths。",
    "5.8.014": "`directedGlobalCut(n,edges)` 返回有向图全局最小割容量及一侧顶点集合；边为 `(u,v,c)`，容量须非负，且 `n>=2`、端点合法。实现固定根并对每个顶点分别求两个方向的最大流，时间约为 `O(n)` 次最大流，适合规模较小、需要直接可靠基线的有向图；容量与答案使用宽整数。",
    "5.9.009": "构造 `FunctionalGraph(to,weight)`，其中每个点恰有一条出边；可查询分量、环、入环深度和后继跳跃，并对跳过的点权求和/最小值。`jump(u,k)` 的 `k` 按无符号 64 位解释；路径聚合默认点权为 0。构造要求边端点合法、权数组为空或长度为 `n`。预处理 `O(n log n)` 时间和空间，跳跃及聚合查询 `O(log n)`。",
    "5.10.017": "`havelHakimi(d)` 判定并构造简单无向图度序列，成功时返回边表；`galeRyser(left,right)` 判定二分图度序列并给出左右点之间的构造边。度数为负、超出顶点数上界或总度数不匹配时返回 `nullopt`。堆式 Havel–Hakimi 约为 `O((n+m) log n)`；二分图构造按剩余度贪心，复杂度取决于生成边数。",
    "5.10.018": "`graphIso(a,b)` 对同阶简单有向图的邻接矩阵做精确同构判定，成功返回从 `a` 到 `b` 的顶点映射；`graphCanon(a)` 返回规范编码及对应顶点顺序。矩阵必须为方阵，规范标号限制 `n<=10`。实现枚举顶点排列，最坏时间 `O(n! n^2)`；用于小图或验证基线，不适合大图。",
    "6.4.007": "`EulerTourTree(n)` 维护初始为孤立点的动态森林：`link/cut` 修改边，`connected` 判连通，`componentSize/componentSum` 查询连通块，`componentAdd` 给整块加值，`add(u,d)` 给单点加值。非法顶点通过边界检查报错；重复连边、成环连边或不存在的删边返回 `false`。Treap 操作期望 `O(log n)`，空间 `O(n+m)`；它支持连通块聚合，不提供任意树路径聚合。",
    "7.4.009": "`hensel(a,root,p,k)` 将多项式系数升幂模根从 `p` 提升到 `p^k`；`a[i]` 是 `x^i` 系数，输入 `root` 须满足模 `p` 的方程且导数非零（简单根）。`p` 必须为素数、`k>=1`，若 `p^k` 超出 `long long` 可表示范围或根不能提升则返回 `nullopt`。时间 `O(k·deg(a))`、额外空间 `O(1)`。",
    "9.1.013": "`permanentRyser(a,mod)` 计算方阵的 permanent 模 `mod`，矩阵元素可为任意整数并在累加时归一化；要求方阵、`mod>0`、`n<=24`。空矩阵的结果定义为 `1 mod mod`。Gray-code 维护行和，时间 `O(n 2^n)`、空间 `O(n)`；它计算的是 permanent，不含行列式的符号交替。",
    "9.1.014": "`gf2::solve(rows,n)` 求解 GF(2) 方程组；每行用 `ceil((n+1)/64)` 个无符号 64 位字存储，变量在前 `n` 位，常数项在第 `n` 位。结果给出是否有解、秩、自由变量数、一个特解和零空间基；无解时只看 `consistent`。消元时间约 `O(m n ceil(n/64))`，空间 `O(m ceil(n/64)+n^2/64)`。",
    "9.3.008": "`lll(b,delta,maxSteps)` 对以行为基向量的整数格基执行浮点 Gram–Schmidt 的 LLL 约化，成功返回约化基；输入空基返回空基，线性相关、数值不稳定、系数溢出或步数超限返回 `nullopt`。要求矩阵为矩形、`1/4 < delta < 1`；这是实用近似实现，不提供精确算术保证。每轮 Gram–Schmidt 为 `O(n^2 m)`，总复杂度依赖迭代次数。",
    "10.2.022": "`subsetCount(a,T)` 返回长度 `T+1` 的多项式系数，第 `s` 项是从带下标区分的非负数列中选出总和为 `s` 的子集数，模数固定为 998244353。零权重会使所有系数乘 2；负权重非法。要求 `0<=T<2^22`，并受多项式模块的 NTT 长度约束。复杂度由截断多项式指数和 NTT 决定，约 `O(T log T)` 时间、`O(T)` 空间。",
    "12.1.011": "`interval_dp::stones(a,ring)` 求相邻石子合并最小代价，可选环形版本并返回最优起点；`matrixChain(p)` 用维数序列求矩阵链最少标量乘法；`brackets(s)` 求只含圆括号字符串的最短合法括号超序列长度。区间统一为半开 `[l,r)`，费用用 `__int128`；两类代价 DP 为 `O(n^3)` 时间、`O(n^2)` 空间，括号 DP 同为 `O(n^3)` / `O(n^2)`。",
    "12.3.007": "`SlopeTrick` 维护离散凸分段线性函数，支持加入 `max(a-x,0)`、`max(x-a,0)`、`|x-a|` 和常数，平移自变量区间，查询最小值与最小点区间；`eval(x)` 返回函数值。所有数值使用 `__int128`。堆更新均摊 `O(log n)`，查询最小点 `O(1)`；当前 `eval` 复制并遍历堆，时间 `O(n)`。",
}

def snippet(name: str) -> str:
    data = (ROOT / "include/print" / name).read_text(encoding="utf-8")
    data = re.sub(r'^#pragma once\s*\n', '', data, flags=re.M)
    data = re.sub(r'^#include (?:<[^>]+>|"support/checks.hpp")\s*\n', '', data, flags=re.M)
    data = re.sub(r'^using namespace std;\s*\n|^#define int long long\s*\n', '', data, flags=re.M)
    # 打印版不出现仓库路径：依赖的头改写成「依赖 编号 标题」注释。
    data = re.sub(r'^#include "(\d+)_(\d+)_(\d+)\.hpp"',
                  lambda m: f"// 依赖 {m[1]}.{m[2]}.{m[3]} {TITLES.get(f'{m[1]}.{m[2]}.{m[3]}', '').replace('`', '')}".rstrip(),
                  data, flags=re.M)
    data = re.sub(r'^#include "([^"]+)"', r'#include "include/print/\1"', data, flags=re.M)
    return "```cpp\n" + data.strip() + "\n```"

def sync_topics(data: str) -> str:
    matches = list(TOPIC_HEADING.finditer(data))
    for i in range(len(matches) - 1, -1, -1):
        match = matches[i]
        topic = match.group(1)
        name_match = HEADER_NOTE.search(data, match.end(), matches[i + 1].start() if i + 1 < len(matches) else len(data))
        if not name_match or topic not in TOPIC_GUIDES:
            continue
        end = matches[i + 1].start() if i + 1 < len(matches) else len(data)
        section = data[match.start():end]
        fences = list(re.finditer(r"```cpp\n.*?\n```", section, re.S))
        if len(fences) != 1:
            raise ValueError(f"{topic}: expected exactly one C++ block")
        code = snippet(name_match.group(1) + ".hpp")
        fence = fences[0]
        section = section[:fence.start()] + code + section[fence.end():]
        # 保留源码中的头文件路径提示，打印构建阶段会过滤该行。
        prose_start = section.find("\n\n")
        code_start = section.find("```cpp")
        if prose_start < 0 or code_start < 0:
            raise ValueError(f"{topic}: malformed topic section")
        section = section[:prose_start + 2] + TOPIC_GUIDES[topic] + "\n\n" + section[code_start:]
        data = data[:match.start()] + section + data[end:]
    return data

def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()
    changed = []
    for path in sorted(ROOT.glob("[0-9][0-9]-*.md")):
        data = path.read_text(encoding="utf-8")
        updated = MARK.sub(lambda m: m[1] + snippet(m[2]) + m[3], data)
        updated = sync_topics(updated)
        if data != updated:
            changed.append(path.name)
            if not args.check: path.write_text(updated, encoding="utf-8", newline="")
    if args.check and changed: raise SystemExit("stale chapter snippets: " + ", ".join(changed))
    print("TOPIC_DOCS_CURRENT" if args.check else f"updated {len(changed)} chapter snippets")

if __name__ == "__main__": main()
