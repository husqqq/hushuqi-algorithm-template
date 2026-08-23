#pragma once

#include "12_1_001.hpp"

namespace board_part
{
template <class Acc = long long>
Acc splitBoard(const vector<vector<int>> &a, int parts)
{
    static_assert(numeric_limits<Acc>::is_integer && numeric_limits<Acc>::is_signed &&
                  sizeof(Acc) >= sizeof(long long));
    assert(!a.empty() && !a[0].empty());
    int n = a.size();
    int m = a[0].size();
    assert(1 <= parts && parts <= n * m);
    vector<vector<Acc>> s(n + 1, vector<Acc>(m + 1));
    for (int i = 0; i < n; i++)
    {
        assert((int)a[i].size() == m);
        for (int j = 0; j < m; j++)
        {
            s[i + 1][j + 1] = s[i][j + 1] + s[i + 1][j] - s[i][j] + a[i][j];
        }
    }
    auto sum = [&](int x1, int y1, int x2, int y2)
    {
        return s[x2][y2] - s[x1][y2] - s[x2][y1] + s[x1][y1];
    };
    using Key = array<int, 5>;
    map<Key, optional<Acc>> memo;
    auto dfs = [&](auto &&self, int x1, int y1, int x2, int y2, int k) -> optional<Acc>
    {
        Key key{x1, y1, x2, y2, k};
        if (auto it = memo.find(key); it != memo.end())
        {
            return it->second;
        }
        if (k == 1)
        {
            Acc x = sum(x1, y1, x2, y2);
            return memo[key] = x * x;
        }
        if (k > (x2 - x1) * (y2 - y1))
        {
            return memo[key] = nullopt;
        }
        optional<Acc> ans;
        for (int x = x1 + 1; x < x2; x++)
        {
            for (int p = 1; p < k; p++)
            {
                auto l = self(self, x1, y1, x, y2, p);
                auto r = self(self, x, y1, x2, y2, k - p);
                if (l && r && (!ans || *l + *r < *ans))
                {
                    ans = *l + *r;
                }
            }
        }
        for (int y = y1 + 1; y < y2; y++)
        {
            for (int p = 1; p < k; p++)
            {
                auto l = self(self, x1, y1, x2, y, p);
                auto r = self(self, x1, y, x2, y2, k - p);
                if (l && r && (!ans || *l + *r < *ans))
                {
                    ans = *l + *r;
                }
            }
        }
        return memo[key] = ans;
    };
    return *dfs(dfs, 0, 0, n, m, parts);
}
}
