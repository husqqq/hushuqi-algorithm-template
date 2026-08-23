#include "../include/full/10_2_017.hpp"

vector<vector<Z>> bruteInv(const vector<vector<Z>> &f, int n, int m)
{
    vector<vector<Z>> g(n, vector<Z>(m));
    Z iv = f[0][0].inv();
    g[0][0] = iv;
    for (int d = 1; d < n + m - 1; d++)
    {
        for (int i = 0; i < n; i++)
        {
            int j = d - i;
            if (j < 0 || j >= m)
            {
                continue;
            }
            Z s = 0;
            for (int x = 0; x <= i && x < (int)f.size(); x++)
            {
                for (int y = 0; y <= j && y < (int)f[x].size(); y++)
                {
                    if (x || y)
                    {
                        s += f[x][y] * g[i - x][j - y];
                    }
                }
            }
            g[i][j] = -s * iv;
        }
    }
    return g;
}

signed main()
{
    mt19937_64 rng(0);
    unsigned long long agg = 0;
    for (int tc = 0; tc < 500; tc++)
    {
        int n = rng() % 13 + 1, m = rng() % 13 + 1;
        vector<vector<Z>> f(n, vector<Z>(m));
        f[0][0] = rng() % (mod - 1) + 1;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                if (i || j)
                {
                    f[i][j] = rng() % mod;
                }
            }
        }
        auto got = fpsInv2(f, n, m);
        assert(got == bruteInv(f, n, m));
        for (auto &row : got)
        {
            for (auto x : row)
            {
                agg = agg * 1000000007ULL + x.val();
            }
        }
    }
    cout << agg << '\n';
    return 0;
}
