#include "../include/full/10_1_013.hpp"

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

    Mint pow(int n) const
    {
        Mint a = *this;
        Mint r = 1;
        while (n)
        {
            if (n & 1)
            {
                r *= a;
            }
            a *= a;
            n >>= 1;
        }
        return r;
    }

    Mint &operator/=(const Mint &a)
    {
        return *this *= a.pow(mod - 2);
    }

    friend Mint operator+(Mint a, const Mint &b)
    {
        return a += b;
    }

    friend Mint operator-(Mint a, const Mint &b)
    {
        return a -= b;
    }

    friend Mint operator*(Mint a, const Mint &b)
    {
        return a *= b;
    }

    friend Mint operator/(Mint a, const Mint &b)
    {
        return a /= b;
    }

    auto operator<=>(const Mint &) const = default;
};

signed main()
{
    mt19937_64 rng(0);
    Mint sum = 0;
    auto mul = [](const vector<Mint> &a, const vector<Mint> &b)
    {
        vector<Mint> c(a.size());
        for (int s = 0; s < (int)a.size(); s++)
        {
            for (int t = s; ; t = (t - 1) & s)
            {
                c[s] += a[t] * b[s ^ t];
                if (t == 0)
                {
                    break;
                }
            }
        }
        return c;
    };
    for (int tc = 0; tc < 300; tc++)
    {
        int n = rng() % 6;
        int m = 1LL << n;
        vector<Mint> f(m);
        for (auto &x : f)
        {
            x = rng() % 1000;
        }
        vector<Mint> h = f;
        h[0] = 0;
        auto g = setExp(h);
        assert(setLog(g) == h);

        vector<Mint> p(n + 5);
        for (auto &x : p)
        {
            x = rng() % 1000;
        }
        vector<Mint> want(m), pw(m);
        pw[0] = 1;
        for (int k = 0; k < (int)p.size(); k++)
        {
            for (int s = 0; s < m; s++)
            {
                want[s] += p[k] * pw[s];
            }
            pw = mul(pw, f);
        }
        auto got = setCompose(p, f);
        assert(got == want);

        vector<Mint> w(m);
        for (auto &x : w)
        {
            x = rng() % 1000;
        }
        int q = n + 5;
        vector<Mint> proj(q);
        fill(pw.begin(), pw.end(), Mint(0));
        pw[0] = 1;
        for (int k = 0; k < q; k++)
        {
            for (int s = 0; s < m; s++)
            {
                proj[k] += w[s] * pw[s];
            }
            pw = mul(pw, f);
        }
        assert(setPowerProj(f, w, q) == proj);
        for (auto x : got)
        {
            sum += x;
        }
    }
    cout << sum.x << '\n';
    return 0;
}
