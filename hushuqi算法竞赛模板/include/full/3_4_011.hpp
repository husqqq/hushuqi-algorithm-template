#pragma once

#include "../offline.hpp"
#include "3_2_001.hpp"

vector<long long> rangeInv(const vector<int> &a, const vector<pair<int, int>> &qs)
{
    // a 是任意静态整数序列，qs 是零基半开区间；按原询问顺序返回严格逆序对数。
    // 返回值与累计量显式用 64 位，关闭整数宏时也能保存超过 32 位的答案。
    assert(a.size() < (1ULL << 30)); // 调试检查，可删；保证 BIT 的有符号下标更新可表示。
    assert(qs.size() < (1ULL << 31)); // 调试检查，可删
    int n = (int)a.size();
    if (qs.empty()) return {};
    vector<MoQuery> ask(qs.size()); // 三维依次为左端点、右端点、原询问编号。
    for (int i = 0; i < (int)qs.size(); i++)
    {
        auto [l, r] = qs[i];
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        ask[i] = {l, r, i};
    }
    Discrete<int> dis(a);
    vector<int32_t> rk(n); // 性能版排名与频次用 32 位减少热点工作集；不影响答案宽度。
    for (int i = 0; i < n; i++)
    {
        rk[i] = (int32_t)dis.id(a[i]);
    }
    BIT<int32_t> bit(dis.a.size());
    int len = 0; // 当前区间长度。
    long long ans = 0; // 当前严格逆序对数，至多 n(n-1)/2。
    auto addL = [&](int p)
    {
        // p 是新左端点；原元素中严格小于 a[p] 的元素与它构成新逆序对。
        ans += bit.sum(rk[p]);
        bit.add(rk[p], 1);
        len++;
    };
    auto addR = [&](int p)
    {
        // p 是新右端点；原元素中严格大于 a[p] 的元素与它构成新逆序对。
        ans += len - bit.sum(rk[p] + 1);
        bit.add(rk[p], 1);
        len++;
    };
    auto delL = [&](int p)
    {
        // p 是被移出的左端点；先删频次，再减去它与剩余元素的贡献。
        bit.add(rk[p], -1);
        len--;
        ans -= bit.sum(rk[p]);
    };
    auto delR = [&](int p)
    {
        // p 是被移出的右端点；先删频次，再减去剩余元素与它的贡献。
        bit.add(rk[p], -1);
        len--;
        ans -= len - bit.sum(rk[p] + 1);
    };
    // 递增对可先 reverse(a)，把每个原询问映射为 [n-r,n-l)，调用本函数；无需取反数值。
    // 非严格逆序对：addL/delL 改为 sum(rk[p]+1)，addR/delR 改为 len-sum(rk[p])。
    return mo<long long>(n, move(ask), addL, addR, delL, delR, [&]()
    {
        return ans;
    });
}
