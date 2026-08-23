#include "../include/full/4_5_007.hpp"

signed main()
{
    string s = "abac", t = "baca";
    vector<array<int, 3>> ask{{0, 0, 4}, {1, 0, 4}, {4, 1, 4}, {4, 0, 0}};
    assert((PrefixSubLCS::solve(s, t, ask) == vector<int>{0, 1, 2, 0}));
    mt19937 rng(1);
    for (int z = 0; z < 100; z++)
    {
        string a(rng() % 7, 'a'), b(rng() % 7, 'a');
        for (char &c : a) c += rng() % 3;
        for (char &c : b) c += rng() % 3;
        vector<array<int, 3>> q;
        for (int i = 0; i < 20; i++)
        {
            int x = rng() % (a.size() + 1), l = rng() % (b.size() + 1), r = l + rng() % (b.size() - l + 1);
            q.push_back({x, l, r});
        }
        auto got = PrefixSubLCS::solve(a, b, q);
        for (int i = 0; i < (int)q.size(); i++)
        {
            auto [x, l, r] = q[i];
            vector<vector<int>> f(x + 1, vector<int>(r - l + 1));
            for (int u = 0; u < x; u++) for (int v = l; v < r; v++)
            {
                f[u + 1][v - l + 1] = a[u] == b[v] ? f[u][v - l] + 1 : max(f[u][v - l + 1], f[u + 1][v - l]);
            }
            assert(got[i] == f[x][r - l]);
        }
    }
}
