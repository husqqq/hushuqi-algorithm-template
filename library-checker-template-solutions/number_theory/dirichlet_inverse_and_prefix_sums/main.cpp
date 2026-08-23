
#include <bits/stdc++.h>
using namespace std;
#define int long long

template <int Mod> struct StaticModInt
{
    int x = 0;

    StaticModInt(long long v = 0)
    {
        x = v % Mod;
        if (x < 0)
        {
            x += Mod;
        }
    }

    StaticModInt &operator+=(const StaticModInt &a)
    {
        x += a.x;
        if (x >= Mod)
        {
            x -= Mod;
        }
        return *this;
    }

    StaticModInt &operator-=(const StaticModInt &a)
    {
        x -= a.x;
        if (x < 0)
        {
            x += Mod;
        }
        return *this;
    }

    StaticModInt &operator*=(const StaticModInt &a)
    {
        x = x * a.x % Mod;
        return *this;
    }

    StaticModInt pow(int e) const
    {
        StaticModInt a = *this, r = 1;
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

    StaticModInt &operator/=(const StaticModInt &a)
    {
        assert(a.x != 0);
        return *this *= a.pow(Mod - 2);
    }

    friend StaticModInt operator+(StaticModInt a, const StaticModInt &b) { return a += b; }
    friend StaticModInt operator-(StaticModInt a, const StaticModInt &b) { return a -= b; }
    friend StaticModInt operator*(StaticModInt a, const StaticModInt &b) { return a *= b; }
    friend StaticModInt operator/(StaticModInt a, const StaticModInt &b) { return a /= b; }
    auto operator<=>(const StaticModInt &) const = default;
};

struct QuotientDirichlet
{
    unsigned long long lim, sq, t;
    int n;

    static unsigned long long isqrt(unsigned long long x)
    {
        unsigned long long r = sqrtl((long double)x);
        while (r + 1 <= x / (r + 1))
        {
            r++;
        }
        while (r && r > x / r)
        {
            r--;
        }
        return r;
    }

    static unsigned long long icbrt(unsigned long long x)
    {
        unsigned long long r = cbrtl((long double)x);
        auto ok = [&](unsigned long long y)
        {
            return y == 0 || y <= x / y / y;
        };
        while (ok(r + 1))
        {
            r++;
        }
        while (!ok(r))
        {
            r--;
        }
        return r;
    }

    QuotientDirichlet(unsigned long long N) : lim(N)
    {
        assert(1 <= N && N <= (1ULL << 50));
        sq = isqrt(N);
        t = sq * sq + sq <= N ? sq : sq - 1;
        n = t + sq + 1;
    }

    int id(unsigned long long x) const
    {
        return x <= t ? x : n - lim / x;
    }

    unsigned long long value(int i) const
    {
        return i <= (int)t ? i : lim / (n - i);
    }

    template <class T, class F> vector<T> sumTable(F prefix) const
    {
        vector<T> a(n);
        for (int i = 1; i < n; i++)
        {
            a[i] = prefix(value(i));
        }
        return a;
    }

    template <class T> vector<T> conv(vector<T> F, vector<T> G) const
    {
        assert((int)F.size() == n && (int)G.size() == n);
        if (lim == 1)
        {
            return {T(0), F[1] * G[1]};
        }
        vector<T> f(n), g(n), H(n);
        for (int i = 1; i < n; i++)
        {
            f[i] = F[i] - F[i - 1];
            g[i] = G[i] - G[i - 1];
        }
        unsigned long long k = icbrt(lim);
        unsigned long long s = k * k;
        for (unsigned long long a = 1; a <= k; a++)
        {
            H[id(a * a)] += f[a] * g[a];
            if (a * (a + 1) <= t)
            {
                for (unsigned long long b = a + 1; b <= t / a; b++)
                {
                    H[a * b] += f[a] * g[b] + f[b] * g[a];
                }
            }
            unsigned long long q = min(s / a, t);
            for (unsigned long long b = max(a, t / a) + 1; b <= q; b++)
            {
                H[id(a * b)] += f[a] * g[b] + f[b] * g[a];
            }
            if (lim / sq <= s / a)
            {
                unsigned long long p = lim / (s / a + 1) + 1;
                for (unsigned long long b = p; b <= sq; b++)
                {
                    H[id(a * (lim / b))] += f[a] * g[n - b] + g[a] * f[n - b];
                }
            }
        }
        for (int i = 1; i < n; i++)
        {
            H[i] += H[i - 1];
        }
        for (unsigned long long z = 1; lim / z > s; z++)
        {
            unsigned long long m = lim / z;
            unsigned long long u = isqrt(m);
            H[n - z] = T(0);
            for (unsigned long long a = 1; a <= u; a++)
            {
                int j = id(m / a);
                H[n - z] += f[a] * G[j] + g[a] * F[j];
            }
            H[n - z] -= F[u] * G[u];
        }
        return H;
    }

    template <class T> vector<T> divide(vector<T> H, vector<T> F) const
    {
        assert((int)H.size() == n && (int)F.size() == n && F[1] != T(0));
        if (lim == 1)
        {
            return {T(0), H[1] / F[1]};
        }
        T inv = T(1) / F[1];
        for (auto &x : F)
        {
            x *= inv;
        }
        vector<T> f(n), g(n), h(n);
        for (int i = 1; i < n; i++)
        {
            f[i] = F[i] - F[i - 1];
            h[i] = H[i] - H[i - 1];
        }
        unsigned long long k = icbrt(lim);
        unsigned long long s = max(sq, k * k);
        g[1] = H[1];
        for (int i = 2; i < n; i++)
        {
            unsigned long long a = value(i);
            if (a > s)
            {
                break;
            }
            g[i] = h[i] - g[1] * f[i];
            if (a * a <= s)
            {
                h[id(a * a)] -= f[i] * g[i];
            }
            unsigned long long u = min<unsigned long long>(i - 1, s / a);
            for (unsigned long long b = 2; b <= u; b++)
            {
                h[id(a * b)] -= f[i] * g[b] + f[b] * g[i];
            }
        }
        vector<T> G(n);
        for (int i = 1; i < n; i++)
        {
            G[i] = G[i - 1] + g[i];
        }
        for (unsigned long long z = lim / (s + 1); z > 0; z--)
        {
            unsigned long long m = lim / z;
            unsigned long long u = isqrt(m);
            G[n - z] = H[n - z] - g[1] * F[n - z] + F[u] * G[u];
            for (unsigned long long a = 2; a <= u; a++)
            {
                int j = id(m / a);
                G[n - z] -= f[a] * G[j] + g[a] * F[j];
            }
        }
        for (auto &x : G)
        {
            x *= inv;
        }
        return G;
    }
};

signed main()
{
    using Mint = StaticModInt<998244353>;
    int t; cin >> t;
    while (t--)
    {
        unsigned long long n; cin >> n;
        QuotientDirichlet q(n);
        vector<Mint> f(q.n), one(q.n, Mint(1));
        one[0] = 0;
        for (int i = 1; i < q.n; i++) cin >> f[i].x;
        auto g = q.divide(one, f);
        for (int i = 1; i < q.n; i++) cout << g[i].x << " \n"[i + 1 == q.n];
    }
}
