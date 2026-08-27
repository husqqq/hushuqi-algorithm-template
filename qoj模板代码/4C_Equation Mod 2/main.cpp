#include <bits/stdc++.h>

#include <cstdint>

inline long long linearMulMod(long long a, long long b, long long mod)
{
    // mod>0 且 a、b 均为 [0,mod)；小模数直接乘，大模数才使用局部宽乘法。
    constexpr long long limit = 3037000499LL;
    if (mod <= limit)
    {
        return a * b % mod;
    }
    return (long long)((__int128)a * b % mod);
}
#include <bits/stdc++.h>
using namespace std;

template <uint32_t mod> struct LinearMint
{
    static_assert(mod > 1 && mod < (1U << 30) && (mod & 1));
    using M = LinearMint;
    uint32_t a = 0;

    static constexpr uint32_t inv32()
    {
        uint32_t x = mod;
        for (int i = 0; i < 4; i++) x *= 2 - mod * x;
        return x;
    }
    static constexpr uint32_t r = inv32();
    static constexpr uint32_t n2 = -uint64_t(mod) % mod;

    static constexpr uint32_t reduce(uint64_t x)
    {
        return (x + uint64_t(uint32_t(x) * uint32_t(-r)) * mod) >> 32;
    }
    LinearMint() = default;
    LinearMint(long long x) : a(reduce(uint64_t((x % mod + mod) % mod) * n2)) {}
    uint32_t val() const
    {
        uint32_t x = reduce(a);
        return x >= mod ? x - mod : x;
    }
    M &operator+=(M b)
    {
        if ((int32_t)(a += b.a - 2 * mod) < 0) a += 2 * mod;
        return *this;
    }
    M &operator-=(M b)
    {
        if ((int32_t)(a -= b.a) < 0) a += 2 * mod;
        return *this;
    }
    M &operator*=(M b)
    {
        a = reduce(uint64_t(a) * b.a);
        return *this;
    }
    M pow(unsigned long long b) const
    {
        M x = *this, ans = 1;
        while (b)
        {
            if (b & 1) ans *= x;
            x *= x;
            b >>= 1;
        }
        return ans;
    }
    M inv() const { return pow(mod - 2); }
    M &operator/=(M b) { return *this *= b.inv(); }
    friend M operator+(M a, M b) { return a += b; }
    friend M operator-(M a, M b) { return a -= b; }
    friend M operator*(M a, M b) { return a *= b; }
    friend M operator/(M a, M b) { return a /= b; }
    M operator-() const { return M() - *this; }
    friend bool operator==(M a, M b) { return a.val() == b.val(); }
    friend bool operator!=(M a, M b) { return !(a == b); }
    friend istream &operator>>(istream &in, M &x)
    {
        long long v;
        in >> v;
        x = v;
        return in;
    }
    friend ostream &operator<<(ostream &out, M x) { return out << x.val(); }
};

struct F2Vector
{
    int n = 0;
    vector<unsigned long long> a;

    F2Vector(int n = 0) : n(n), a((n + 63) / 64) {}
    bool get(int i) const { return a[i >> 6] >> (i & 63) & 1ULL; }
    void set(int i, bool x = true)
    {
        unsigned long long b = 1ULL << (i & 63);
        if (x) a[i >> 6] |= b;
        else a[i >> 6] &= ~b;
    }
    F2Vector &operator^=(const F2Vector &b)
    {
        assert(n == b.n);
        for (int i = 0; i < (int)a.size(); i++) a[i] ^= b.a[i];
        return *this;
    }
    string str() const
    {
        string s(n, '0');
        for (int i = 0; i < n; i++) s[i] += get(i);
        return s;
    }
};

inline vector<F2Vector> matMulF2(const vector<F2Vector> &a,
                                 const vector<F2Vector> &b, int k)
{
    // a 是 n*m 的按行位矩阵，b 是 m*k 的按行位矩阵；返回 a*b。
    if (a.empty()) return {};
    int m = b.size();
    vector<F2Vector> c(a.size(), F2Vector(k));
    for (int i = 0; i < (int)a.size(); i++)
    {
        for (int j = 0; j < m; j++) if (a[i].get(j)) c[i] ^= b[j];
    }
    return c;
}

inline int rankF2(vector<F2Vector> a)
{
    // a 是按行保存的 F2 矩阵；返回其秩。
    if (a.empty()) return 0;
    int n = a.size(), m = a[0].n, rk = 0;
    if (n > m)
    {
        vector<F2Vector> t(m, F2Vector(n));
        for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) if (a[i].get(j)) t[j].set(i);
        a.swap(t);
        swap(n, m);
    }
    for (int col = 0; col < m && rk < n; col++)
    {
        int p = rk;
        while (p < n && !a[p].get(col)) p++;
        if (p == n) continue;
        swap(a[p], a[rk]);
        for (int i = rk + 1; i < n; i++) if (a[i].get(col)) a[i] ^= a[rk];
        rk++;
    }
    return rk;
}

inline optional<vector<F2Vector>> matInvF2(vector<F2Vector> a)
{
    // a 是 n 阶 F2 方阵；可逆时返回逆矩阵的各行，否则返回空。
    int n = a.size();
    vector<F2Vector> b(n, F2Vector(n));
    for (int i = 0; i < n; i++) b[i].set(i);
    for (int col = 0; col < n; col++)
    {
        int p = col;
        while (p < n && !a[p].get(col)) p++;
        if (p == n) return nullopt;
        swap(a[p], a[col]);
        swap(b[p], b[col]);
        for (int i = 0; i < n; i++) if (i != col && a[i].get(col))
        {
            a[i] ^= a[col];
            b[i] ^= b[col];
        }
    }
    return b;
}

struct F2LinearSol
{
    F2Vector particular;
    vector<F2Vector> basis;
};

inline optional<F2LinearSol> gaussF2(vector<F2Vector> a, const F2Vector &rhs)
{
    // a 是 n*m 的按行矩阵，rhs 是 n 维右端；返回一组特解与齐次解空间基。
    int n = a.size(), m = a.empty() ? 0 : a[0].n, rk = 0;
    assert(rhs.n == n);
    vector<int> pivot(m, -1);
    vector<int> b(n);
    for (int i = 0; i < n; i++) b[i] = rhs.get(i);
    for (int col = 0; col < m && rk < n; col++)
    {
        int p = rk;
        while (p < n && !a[p].get(col)) p++;
        if (p == n) continue;
        swap(a[p], a[rk]);
        swap(b[p], b[rk]);
        for (int i = 0; i < n; i++) if (i != rk && a[i].get(col))
        {
            a[i] ^= a[rk];
            b[i] ^= b[rk];
        }
        pivot[col] = rk++;
    }
    for (int i = rk; i < n; i++) if (b[i]) return nullopt;
    F2LinearSol sol{F2Vector(m), {}};
    for (int col = 0; col < m; col++) if (pivot[col] != -1) sol.particular.set(col, b[pivot[col]]);
    for (int free = 0; free < m; free++) if (pivot[free] == -1)
    {
        F2Vector x(m);
        x.set(free);
        for (int col = 0; col < m; col++) if (pivot[col] != -1 && a[pivot[col]].get(free)) x.set(col);
        sol.basis.push_back(move(x));
    }
    return sol;
}
using namespace std;

struct ModLinearSol
{
    vector<long long> particular;
    vector<vector<long long>> basis;
};

inline int rankMod(vector<vector<long long>> a, int m, long long mod)
{
    // a 是行宽为 m 的矩阵，mod 是素数；返回有限域上的矩阵秩。
    assert(m >= 0 && mod > 1);
    int n = a.size(), rk = 0;
    for (auto &r : a)
    {
        assert((int)r.size() == m);
        for (auto &x : r) x = (x % mod + mod) % mod;
    }
    auto power = [&](long long x, long long b)
    {
        long long ans = 1;
        while (b)
        {
            if (b & 1) ans = linearMulMod(ans, x, mod);
            x = linearMulMod(x, x, mod);
            b >>= 1;
        }
        return ans;
    };
    for (int col = 0; col < m && rk < n; col++)
    {
        int p = rk;
        while (p < n && a[p][col] == 0) p++;
        if (p == n) continue;
        swap(a[p], a[rk]);
        long long inv = power(a[rk][col], mod - 2);
        for (int i = rk + 1; i < n; i++) if (a[i][col])
        {
            long long q = linearMulMod(a[i][col], inv, mod);
            for (int j = col; j < m; j++)
            {
                a[i][j] -= linearMulMod(q, a[rk][j], mod);
                if (a[i][j] < 0) a[i][j] += mod;
            }
        }
        rk++;
    }
    return rk;
}

inline optional<ModLinearSol> gaussMod(vector<vector<long long>> a, int n, long long mod)
{
    // 调试检查，可删。
    assert(n >= 0 && mod > 1);
    for (auto &r : a)
    {
        // 调试检查，可删。
        assert((int)r.size() == n + 1);
        for (auto &x : r)
        {
            x %= mod;
            if (x < 0)
            {
                x += mod;
            }
        }
    }
    int m = a.size();
    int rk = 0;
    vector<int> pivot(n, -1);
    auto power = [&](long long x, long long b)
    {
        long long ans = 1;
        while (b)
        {
            if (b & 1)
            {
                ans = linearMulMod(ans, x, mod);
            }
            x = linearMulMod(x, x, mod);
            b >>= 1;
        }
        return ans;
    };
    for (int col = 0; col < n && rk < m; col++)
    {
        int row = rk;
        while (row < m && a[row][col] == 0)
        {
            row++;
        }
        if (row == m)
        {
            continue;
        }
        swap(a[row], a[rk]);
        long long inv = power(a[rk][col], mod - 2);
        for (int j = col; j <= n; j++)
        {
            a[rk][j] = linearMulMod(a[rk][j], inv, mod);
        }
        for (int i = 0; i < m; i++)
        {
            if (i == rk)
            {
                continue;
            }
            long long q = a[i][col];
            for (int j = col; j <= n; j++)
            {
                a[i][j] -= linearMulMod(q, a[rk][j], mod);
                if (a[i][j] < 0)
                {
                    a[i][j] += mod;
                }
            }
        }
        pivot[col] = rk++;
    }
    for (int i = rk; i < m; i++)
    {
        if (a[i][n])
        {
            return nullopt;
        }
    }
    ModLinearSol sol{vector<long long>(n), {}};
    for (int col = 0; col < n; col++)
    {
        if (pivot[col] != -1)
        {
            sol.particular[col] = a[pivot[col]][n];
        }
    }
    for (int free = 0; free < n; free++)
    {
        if (pivot[free] != -1)
        {
            continue;
        }
        vector<long long> v(n);
        v[free] = 1;
        for (int col = 0; col < n; col++)
        {
            if (pivot[col] != -1)
            {
                v[col] = (mod - a[pivot[col]][free]) % mod;
            }
        }
        sol.basis.push_back(move(v));
    }
    return sol;
}

signed main()
{
    int n, m; cin >> n >> m; vector<F2Vector> a(n, F2Vector(m)); string s;
    for (int i = 0; i < n; i++) { cin >> s; for (int j = 0; j < m; j++) a[i].set(j, s[j] == '1'); }
    cin >> s; F2Vector b(n); for (int i = 0; i < n; i++) b.set(i, s[i] == '1');
    auto ans = gaussF2(a, b); if (!ans) return cout << -1 << '\n', 0;
    cout << ans->basis.size() << '\n' << ans->particular.str() << '\n'; for (auto &r : ans->basis) cout << r.str() << '\n';
}
