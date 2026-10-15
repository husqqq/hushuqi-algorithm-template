#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

struct SlopeTrick
{
    using W = __int128;
    priority_queue<W> l;
    priority_queue<W, vector<W>, greater<>> r;
    W dl = 0, dr = 0, minimum = 0;
    // 添加 max(a-x,0)。
    void addLeft(W a)
    {
        if (!r.empty()) minimum += max(W(0), a - (r.top() + dr));
        r.push(a - dr); W v = r.top() + dr; r.pop(); l.push(v - dl);
    }
    // 添加 max(x-a,0)。
    void addRight(W a)
    {
        if (!l.empty()) minimum += max(W(0), l.top() + dl - a);
        l.push(a - dl); W v = l.top() + dl; l.pop(); r.push(v - dr);
    }
    void addAbs(W a) { addLeft(a); addRight(a); }
    void addConst(W c) { minimum += c; }
    // f_new(x)=min_{x-b <= y <= x-a} f_old(y)，a<=b；a=b 是平移。
    void shift(W a, W b) { if (a > b) throw invalid_argument("shift needs a<=b"); dl += a; dr += b; }
    void shift(W a) { shift(a, a); }
    void prefixMin() { while (!r.empty()) r.pop(); }
    void suffixMin() { while (!l.empty()) l.pop(); }
    pair<optional<W>, optional<W>> argmin() const
    {
        return {l.empty() ? nullopt : optional<W>(l.top() + dl), r.empty() ? nullopt : optional<W>(r.top() + dr)};
    }
    W eval(W x) const
    {
        auto a = l; auto b = r; W ans = minimum;
        while (!a.empty()) { ans += max(W(0), a.top() + dl - x); a.pop(); }
        while (!b.empty()) { ans += max(W(0), x - b.top() - dr); b.pop(); }
        return ans;
    }
};
