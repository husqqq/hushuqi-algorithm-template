
#include <bits/stdc++.h>
using namespace std;
#define int long long

struct PrefixSubstringLCS
{
    struct Query
    {
        int a, l, r, id; // a 是首串前缀长度，l、r 是次串半开区间，id 是原查询编号。
    };

    static vector<int> solve(const string &s, const string &t, const vector<array<int, 3>> &ask)
    {
        // s、t 是小写字符串；ask 每项为 {前缀长度,左端,右端}，返回对应 LCS 长度。
        int m = s.size(), n = t.size(), q = ask.size();
        vector<int> ans(q);
        vector<vector<Query>> byA(m);
        for (int i = 0; i < q; i++)
        {
            auto [a, l, r] = ask[i];
            assert(0 <= a && a <= m && 0 <= l && l <= r && r <= n);
            if (a && l < r)
            {
                byA[a - 1].push_back({a, l, r, i});
            }
        }
        vector<int> h(n);
        iota(h.begin(), h.end(), 0);
        for (int a = 0; a < m; a++)
        {
            int cur = -1;
            for (int j = 0; j < n; j++)
            {
                if (s[a] == t[j] || h[j] < cur)
                {
                    swap(h[j], cur);
                }
            }
            vector<vector<Query>> byR(n);
            for (auto x : byA[a])
            {
                byR[x.r - 1].push_back(x);
            }
            vector<int> bit(n + 1);
            auto add = [&](int p)
            {
                for (p++; p <= n; p += p & -p)
                {
                    bit[p]++;
                }
            };
            auto sum = [&](int p)
            {
                int ret = 0;
                for (; p; p &= p - 1)
                {
                    ret += bit[p];
                }
                return ret;
            };
            int cnt = 0;
            for (int r = 0; r < n; r++)
            {
                if (h[r] != -1)
                {
                    add(h[r]);
                    cnt++;
                }
                for (auto x : byR[r])
                {
                    ans[x.id] = (x.r - x.l) - (cnt - sum(x.l));
                }
            }
        }
        return ans;
    }
};

signed main()
{
    int q; string s, t;
    cin >> q >> s >> t;
    vector<array<int, 3>> ask(q);
    for (auto &x : ask) cin >> x[0] >> x[1] >> x[2];
    for (int x : PrefixSubstringLCS::solve(s, t, ask)) cout << x << '\n';
}
