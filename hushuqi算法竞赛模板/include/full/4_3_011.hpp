#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

struct BasicFactors
{
    // s 是原字符串，rk[k][i] 是长 2^k 子串 s[i..i+2^k) 的排名。
    string s;
    vector<vector<int>> rk;

    BasicFactors(string s) : s(move(s))
    {
        // s 按无符号字节字典序比较；建立所有二次幂长度子串的分层排名。
        int n = this->s.size();
        vector<int> first;
        for (unsigned char c : this->s) first.push_back(c);
        rk.push_back(move(first));
        for (int k = 1; (1LL << k) <= n; k++)
        {
            int len = 1LL << (k - 1);
            vector<int> id(n - 2 * len + 1), next(id.size());
            iota(id.begin(), id.end(), 0);
            sort(id.begin(), id.end(), [&](int i, int j)
            {
                return pair{rk[k - 1][i], rk[k - 1][i + len]} <
                       pair{rk[k - 1][j], rk[k - 1][j + len]};
            });
            int cls = 0;
            for (int i = 0; i < (int)id.size(); i++)
            {
                if (i && pair{rk[k - 1][id[i - 1]], rk[k - 1][id[i - 1] + len]} !=
                         pair{rk[k - 1][id[i]], rk[k - 1][id[i] + len]}) cls++;
                next[id[i]] = cls;
            }
            rk.push_back(move(next));
        }
    }

    int compare(int l, int r, int len) const
    {
        // l、r 是两个子串起点，len 是共同长度；按字典序返回 -1、0 或 1。
        assert(0 <= l && 0 <= r && 0 <= len && l + len <= (int)s.size() && r + len <= (int)s.size()); // 调试检查，可删
        if (!len) return 0;
        int k = bit_width((unsigned long long)len) - 1, d = len - (1LL << k);
        pair<int, int> a{rk[k][l], rk[k][l + d]}, b{rk[k][r], rk[k][r + d]};
        return (a > b) - (a < b);
    }
};

struct BorderInfo
{
    // mn、mx 是最短和最长真 border 长度，cnt 是真 border 数量。
    int mn = 0, mx = 0, cnt = 0;
};

struct BasicDictQuery
{
    int l, r;
};

inline vector<BorderInfo> basicDict(const string &s,
                                    const vector<pair<int, int>> &qs)
{
    // s 是字符串；qs 是 0 下标闭区间；返回每个区间的真 border 统计。
    int n = s.size(), q = qs.size();
    vector<BorderInfo> ans(q);
    if (!n || !q) return ans;
    vector<BasicDictQuery> que(q);
    vector<vector<int>> atL(n + 2), atR(n + 2);
    for (int i = 0; i < q; i++)
    {
        int l = qs[i].first + 1, r = qs[i].second + 1;
        assert(1 <= l && l <= r && r <= n);
        que[i] = {l, r};
        atL[l].push_back(i);
        atR[r].push_back(i);
    }
    vector<int> sa(n + 1), rk(n + 1), id(2 * n + 3), cnt(256);
    for (int i = 1; i <= n; i++) cnt[(unsigned char)s[i - 1]]++;
    for (int i = 1; i < 256; i++) cnt[i] += cnt[i - 1];
    for (int i = n; i; i--) sa[cnt[(unsigned char)s[i - 1]]--] = i;
    int cls = 0;
    for (int i = 1; i <= n; i++)
    {
        if (i == 1 || s[sa[i] - 1] != s[sa[i - 1] - 1]) cls++;
        rk[sa[i]] = cls;
    }
    vector<int> nxt(n + 2), leftMost(q, n + 1), rightMost(q),
        leftRev(q, n + 1), rightRev(q), low(q, n + 1), high(q), number(q),
        buf(n + 3);
    auto addOne = [&](int idq, int x, int l, int r, int step)
    {
        if (l <= x && x <= r && (x - l) % step == 0)
        {
            low[idq] = min(low[idq], x);
            high[idq] = max(high[idq], x);
            number[idq]++;
        }
    };
    auto addTwo = [&](int idq, int l1, int r1, int d1,
                      int l2, int r2, int d2)
    {
        if (r1 - l1 <= d1)
        {
            for (int x = l1; x <= r1; x += d1) addOne(idq, x, l2, r2, d2);
            return;
        }
        if (r2 - l2 <= d2)
        {
            for (int x = l2; x <= r2; x += d2) addOne(idq, x, l1, r1, d1);
            return;
        }
        if ((l1 - l2) % d1) return;
        l1 = max(l1, l2), r1 = min(r1, r2);
        if (l1 <= r1)
        {
            low[idq] = min(low[idq], l1);
            high[idq] = max(high[idq], r1);
            number[idq] += (r1 - l1) / d1 + 1;
        }
    };
    auto work = [&](int len)
    {
        fill(buf.begin(), buf.begin() + cls + 1, n + 1);
        for (int i = n - len + 1; i >= 1; i--) nxt[i] = buf[rk[i]], buf[rk[i]] = i;
        fill(buf.begin(), buf.begin() + cls + 1, -1);
        for (int i = 2 - len; i <= n; i++)
        {
            if (1 <= i && i <= n) buf[rk[i]] = i;
            int j = i + len - 1;
            if (1 <= j && j <= n) for (int idq : atR[j]) if (que[idq].r - que[idq].l + 1 > len)
                rightMost[idq] = buf[rk[que[idq].l]];
            j = i - len + 1;
            if (1 <= j && j <= n) for (int idq : atL[j]) if (que[idq].r - que[idq].l + 1 > len)
                rightRev[idq] = min(buf[rk[que[idq].r - len + 1]], que[idq].r - len + 1);
        }
        fill(buf.begin(), buf.begin() + cls + 1, n + 1);
        for (int i = n; i + len - 1; i--)
        {
            if (1 <= i && i <= n) buf[rk[i]] = i;
            int j = i + 2 * len - 2;
            if (1 <= j && j <= n) for (int idq : atR[j]) if (que[idq].r - que[idq].l + 1 > len)
                leftMost[idq] = max(buf[rk[que[idq].l]], nxt[que[idq].l]);
            if (1 <= i && i <= n) for (int idq : atL[i]) if (que[idq].r - que[idq].l + 1 > len)
                leftRev[idq] = buf[rk[que[idq].r - len + 1]];
        }
        for (int i = 0; i < q; i++)
        {
            if (que[i].r - que[i].l + 1 <= len || leftMost[i] > rightMost[i] || leftRev[i] > rightRev[i]) continue;
            int d1 = nxt[leftMost[i]] - leftMost[i], d2 = nxt[leftRev[i]] - leftRev[i];
            swap(leftMost[i], rightMost[i]);
            leftRev[i] += len - que[i].l;
            rightRev[i] += len - que[i].l;
            leftMost[i] = que[i].r - leftMost[i] + 1;
            rightMost[i] = que[i].r - rightMost[i] + 1;
            addTwo(i, leftMost[i], rightMost[i], d1, leftRev[i], rightRev[i], d2);
        }
        if (cls == n) return;
        fill(buf.begin(), buf.begin() + cls + 1, 0);
        int z = 0;
        for (int i = n; i > n - len; i--) id[++z] = i;
        for (int i = 1; i <= n; i++) if (sa[i] > len) id[++z] = sa[i] - len;
        for (int i = 1; i <= n; i++) buf[rk[i]]++;
        for (int i = 2; i <= cls; i++) buf[i] += buf[i - 1];
        for (int i = n; i; i--) sa[buf[rk[id[i]]]--] = id[i], id[i] = rk[i];
        cls = 0;
        for (int i = 1; i <= n; i++)
        {
            int x = sa[i], y = sa[i - 1];
            if (i == 1 || id[x] != id[y] || id[x + len] != id[y + len]) cls++;
            rk[x] = cls;
        }
    };
    for (int len = 1; len <= n; len <<= 1) work(len);
    for (int i = 0; i < q; i++) if (number[i]) ans[i] = {low[i], high[i], number[i]};
    return ans;
}
