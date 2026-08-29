#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class F>
vector<int> smawk(int rows, int cols, F better)
{
    // rows、cols 是矩阵行列数；better(row, oldCol, newCol) 表示新列严格更优；返回每行最左最优列。
    assert(rows >= 0 && cols > 0);
    if (rows == 0)
    {
        return {};
    }
    auto solve = [&](auto &&self, const vector<int> &rs, const vector<int> &cs) -> vector<int>
    {
        vector<int> reduced;
        reduced.reserve(cs.size());
        for (int col : cs)
        {
            while (!reduced.empty())
            {
                int row = rs[(int)reduced.size() - 1];
                if (!better(row, reduced.back(), col))
                {
                    break;
                }
                reduced.pop_back();
            }
            if ((int)reduced.size() < (int)rs.size())
            {
                reduced.push_back(col);
            }
        }
        vector<int> odd;
        for (int i = 1; i < (int)rs.size(); i += 2)
        {
            odd.push_back(rs[i]);
        }
        vector<int> ans(rs.size());
        if (!odd.empty())
        {
            vector<int> oa = self(self, odd, reduced);
            for (int i = 0; i < (int)oa.size(); i++)
            {
                ans[2 * i + 1] = oa[i];
            }
        }
        int p = 0;
        for (int i = 0; i < (int)rs.size(); i += 2)
        {
            int lim = (i + 1 == (int)rs.size() ? reduced.back() : ans[i + 1]);
            int best = reduced[p];
            while (reduced[p] < lim)
            {
                ++p;
                if (better(rs[i], best, reduced[p]))
                {
                    best = reduced[p];
                }
            }
            ans[i] = best;
        }
        return ans;
    };
    vector<int> rs(rows), cs(cols);
    iota(rs.begin(), rs.end(), 0);
    iota(cs.begin(), cs.end(), 0);
    return solve(solve, rs, cs);
}
