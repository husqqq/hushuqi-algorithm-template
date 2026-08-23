#pragma once

#include "11_1_005.hpp"

optional<Line<Real>> midPerp(P a, P b)
{
    if (eq(a, b))
    {
        return nullopt;
    }
    auto m = (a + b) / 2;
    return Line<Real>{m, m + rot(b - a)};
}
