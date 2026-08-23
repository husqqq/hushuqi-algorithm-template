#include "../include/full/7_3_012.hpp"

struct Mint
{
    static constexpr int mod = 998244353;
    int x = 0;

    Mint(int v = 0)
    {
        x = v % mod;
        if (x < 0)
        {
            x += mod;
        }
    }

    Mint &operator+=(const Mint &a)
    {
        x += a.x;
        if (x >= mod)
        {
            x -= mod;
        }
        return *this;
    }

    Mint &operator-=(const Mint &a)
    {
        x -= a.x;
        if (x < 0)
        {
            x += mod;
        }
        return *this;
    }

    Mint &operator*=(const Mint &a)
    {
        x = x * a.x % mod;
        return *this;
    }

    Mint pow(int e) const
    {
        Mint a = *this, r = 1;
        while (e)
        {
            if (e & 1)
            {
                r *= a;
            }
            a *= a;
            e >>= 1;
        }
        return r;
    }

    Mint &operator/=(const Mint &a)
    {
        return *this *= a.pow(mod - 2);
    }

    friend Mint operator+(Mint a, const Mint &b) { return a += b; }
    friend Mint operator-(Mint a, const Mint &b) { return a -= b; }
    friend Mint operator*(Mint a, const Mint &b) { return a *= b; }
    friend Mint operator/(Mint a, const Mint &b) { return a /= b; }
    auto operator<=>(const Mint &) const = default;
};

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
