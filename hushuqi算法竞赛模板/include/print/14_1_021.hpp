#pragma once

#include "14_1_020.hpp"

Decimal atanRecip(int q)
{
    // q 是大于 1 的正整数；返回按当前 Decimal 精度计算的 arctan(1/q)。
    assert(q > 1);
    Decimal x = Decimal(1) / Decimal(q);
    Decimal square = x * x;
    Decimal term = x;
    Decimal ans(0);
    for (int k = 0;; k++)
    {
        Decimal add = term / Decimal(2 * k + 1);
        if (add.isZero())
        {
            break;
        }
        ans += add;
        term = -term * square;
    }
    return ans;
}

Decimal piMachin(int digits)
{
    // digits 是结果保留的小数位数；返回 Machin 公式计算并四舍五入的圆周率。
    assert(0 <= digits && digits <= numeric_limits<int>::max() - 8);
    int old = Decimal::getPrecision();
    Decimal::setPrecision(digits + 8);
    Decimal ans = Decimal(16) * atanRecip(5) - Decimal(4) * atanRecip(239);
    ans.round(digits);
    Decimal::setPrecision(old);
    return ans;
}

Decimal piChudnovsky(int digits)
{
    // digits 是结果保留的小数位数；返回 Chudnovsky 级数计算并四舍五入的圆周率。
    assert(0 <= digits && digits <= numeric_limits<int>::max() - 8);
    int old = Decimal::getPrecision();
    Decimal::setPrecision(digits + 8);
    BigInt mul = 1;
    BigInt power = 1;
    int linear = 13591409;
    Decimal sum(linear);
    for (int k = 1;; k++)
    {
        int x = 6 + 12 * (k - 1);
        mul *= BigInt(x * x * x - 16 * x);
        mul /= BigInt(k * k * k);
        linear += 545140134;
        power *= BigInt("-262537412640768000");
        Decimal term(mul * BigInt(linear));
        term /= Decimal(power);
        if (term.isZero())
        {
            break;
        }
        sum += term;
    }
    Decimal ans = Decimal(426880) * Decimal(10005).sqrt() / sum;
    ans.round(digits);
    Decimal::setPrecision(old);
    return ans;
}
