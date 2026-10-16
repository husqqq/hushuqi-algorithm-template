#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

vector<long double> quadRoots(long double a, long double b, long double c)
{
    // a、b、c 是有限实系数；返回升序实根，近重根合并；无实根或恒等式返回空表。
    // 要求返回的实根有限；小于最小可表示量的根可能舍入为零，不保证判别式近零时的根数。
    static_assert(numeric_limits<long double>::radix == 2);
    assert(isfinite(a) && isfinite(b) && isfinite(c));
    if (a == 0) return b == 0 ? vector<long double>{} : vector<long double>{-c / b};
    if (c == 0)
    {
        if (b == 0) return {0};
        vector<long double> ans{0, -b / a};
        sort(ans.begin(), ans.end());
        if (ans[0] == ans[1]) ans.pop_back();
        return ans;
    }

    // 分解系数为带符号尾数乘 2 的指数；非零尾数的绝对值在 [1,2)。
    int ea = ilogbl(a), eb = b == 0 ? 0 : ilogbl(b), ec = ilogbl(c);
    long double am = scalbnl(a, -ea), bm = scalbnl(b, -eb), cm = scalbnl(c, -ec);
    int e = ea + ec + 2; // 判别式公共指数；取偶数，使其平方根的指数恰为 e/2。
    if (b != 0) e = max(e, 2 * eb);
    if (e % 2) e++;
    long double bs = scalbnl(bm, eb - e / 2);
    long double ac = scalbnl(4 * am, ea + ec - e);
    long double bb = bs * bs, d = fmal(-ac, cm, bb);
    long double tol = 64 * numeric_limits<long double>::epsilon() * (bb + fabsl(ac * cm));
    if (d < -tol) return {};
    if (fabsl(d) <= tol) return {scalbnl(-bm / (2 * am), eb - ea)};

    // q 是稳定公式所得大根分子的尾数，实际分子为 q*2^(e/2)；分别恢复两根的指数。
    long double q = -0.5L * (bs + copysignl(sqrtl(d), b));
    vector<long double> ans{scalbnl(q / am, e / 2 - ea), scalbnl(cm / q, ec - e / 2)};
    sort(ans.begin(), ans.end());
    return ans;
}
