#include "../include/full/7_3_012.hpp"

using Mint = MInt<998244353>;

signed main()
{
    mt19937_64 rng(0);
    Mint sum = 0;
    for (int tc = 0; tc < 200; tc++)
    {
        int N = rng() % 500 + 1;
        QuotDirich q(N);
        vector<Mint> f(N + 1), g(N + 1), h(N + 1), sf(N + 1), sg(N + 1), sh(N + 1);
        f[1] = 1;
        for (int i = 2; i <= N; i++)
        {
            f[i] = rng() % 100;
        }
        for (int i = 1; i <= N; i++)
        {
            g[i] = rng() % 100;
            sf[i] = sf[i - 1] + f[i];
            sg[i] = sg[i - 1] + g[i];
        }
        for (int d = 1; d <= N; d++)
        {
            for (int k = 1; k <= N / d; k++)
            {
                h[d * k] += f[d] * g[k];
            }
        }
        for (int i = 1; i <= N; i++)
        {
            sh[i] = sh[i - 1] + h[i];
        }
        auto F = q.sumTable<Mint>([&](unsigned long long x) { return sf[x]; });
        auto G = q.sumTable<Mint>([&](unsigned long long x) { return sg[x]; });
        auto H = q.conv(F, G);
        assert(q.divide(H, F) == G);
        for (int i = 1; i < q.n; i++)
        {
            assert(H[i] == sh[q.value(i)]);
            sum += H[i];
        }
    }
    cout << sum.x << '\n';
    return 0;
}
