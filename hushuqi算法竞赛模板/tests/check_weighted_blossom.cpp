#include "../include/full/5_7_006.hpp"

long long brute(int n, const vector<tuple<int, int, long long>> &e, int s, vector<int> &take)
{
    if (s == (1 << n) - 1)
    {
        return 0;
    }
    int u = 0;
    while (s >> u & 1)
    {
        u++;
    }
    long long ans = brute(n, e, s | (1 << u), take);
    for (auto [x, y, w] : e)
    {
        if (x == u && !(s >> y & 1))
        {
            ans = max(ans, w + brute(n, e, s | (1 << u) | (1 << y), take));
        }
        if (y == u && !(s >> x & 1))
        {
            ans = max(ans, w + brute(n, e, s | (1 << u) | (1 << x), take));
        }
    }
    return ans;
}

signed main()
{
    mt19937_64 rng(7);
    for (int it = 0; it < 2000; it++)
    {
        int n = 2 + rng() % 9;
        vector<tuple<int, int, long long>> e;
        for (int i = 0; i < n; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                if (rng() % 3)
                {
                    e.push_back({i, j, (long long)(rng() % 30)});
                }
            }
        }
        WBlossom<long long> b(n);
        auto ans = b.solve(e).first;
        vector<int> take;
        assert(ans == brute(n, e, 0, take));
    }
    WBlossom<long long> reuse(3);
    assert(reuse.solve({{0, 1, -4}, {1, 2, -2}}).first == 0);
    assert(reuse.solve({{0, 1, 7}, {1, 2, 3}}).first == 7);
    cout << "ok\n";
}
