#pragma once
#include "11_1_001.hpp"
#include "11_1_002.hpp"
#include "11_1_003.hpp"
#include "11_1_005.hpp"

optional<Line<Real>> midPerp(P a, P b)
{
    // a、b 表示两个点；重合时返回空，否则返回端点式中垂线。
    if (eq(a, b))
    {
        return nullopt;
    }
    auto m = (a + b) / 2;
    return Line<Real>{m, m + rot(b - a)};
}
