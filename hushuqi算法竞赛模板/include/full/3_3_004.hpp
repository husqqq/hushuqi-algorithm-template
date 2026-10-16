#pragma once

#include "3_3_002.hpp"

// 依赖 3.3.002 懒标记线段树的 LazySeg 与区间加标记 Tag。
struct MinInfo
{
    long long mn = 0; // mn 是非空区间的最小值；默认构造一个值为 0 的叶子。

    MinInfo() = default;
    MinInfo(long long x) : mn(x)
    {
        // x 是叶子的初始值。
    }

    static MinInfo identity()
    {
        // 无参数；返回空区间的合并单位元，不向该临时结果施加区间加。
        return MinInfo(numeric_limits<long long>::max());
    }

    void apply(const Tag &t)
    {
        // t 是区间增量；统一平移非空区间的最小值。
        mn += t.add;
    }

    friend MinInfo operator+(MinInfo a, MinInfo b)
    {
        // a、b 是相邻区间的信息；返回合并后的最小值。
        return MinInfo(min(a.mn, b.mn));
    }
    // 改最大值时，将 mn/最小值/min/max() 成对换为 mx/最大值/max/lowest()。
    // 默认叶子仍为 0；Tag、增量符号与更新方向不变，不需要取反输入。
};

using AddMinSeg = LazySeg<MinInfo, Tag>;
