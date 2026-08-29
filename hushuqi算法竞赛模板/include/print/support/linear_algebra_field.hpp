#pragma once
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
    F2Vector part;
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
    for (int col = 0; col < m; col++) if (pivot[col] != -1) sol.part.set(col, b[pivot[col]]);
    for (int free = 0; free < m; free++) if (pivot[free] == -1)
    {
        F2Vector x(m);
        x.set(free);
        for (int col = 0; col < m; col++) if (pivot[col] != -1 && a[pivot[col]].get(free)) x.set(col);
        sol.basis.push_back(move(x));
    }
    return sol;
}

inline optional<F2Vector> solveF2(vector<F2Vector> a, const F2Vector &rhs)
{
    // a 是 n*m 的按行位矩阵，rhs 是 n 维右端；返回任意一组解，无解时返回空。
    int n = a.size(), m = a.empty() ? 0 : a[0].n, rk = 0;
    assert(rhs.n == n);
    vector<int> b(n), at;
    for (int i = 0; i < n; i++)
    {
        assert(a[i].n == m);
        b[i] = rhs.get(i);
    }
    for (int col = 0; col < m && rk < n; col++)
    {
        int p = rk;
        while (p < n && !a[p].get(col)) p++;
        if (p == n) continue;
        swap(a[p], a[rk]);
        swap(b[p], b[rk]);
        for (int i = rk + 1; i < n; i++)
        {
            if (a[i].get(col))
            {
                a[i] ^= a[rk];
                b[i] ^= b[rk];
            }
        }
        at.push_back(col);
        rk++;
    }
    for (int i = rk; i < n; i++)
    {
        if (b[i]) return nullopt;
    }
    F2Vector x(m);
    for (int i = rk - 1; i >= 0; i--)
    {
        int v = b[i];
        for (int j = 0; j < (int)x.a.size(); j++)
        {
            v ^= popcount(a[i].a[j] & x.a[j]) & 1;
        }
        x.set(at[i], v);
    }
    return x;
}

struct Mod3Row
{
    // n 是列数；one、two 分别记录系数为 1、2 的列，未记录列系数为 0。
    int n = 0;
    vector<unsigned long long> one, two;

    Mod3Row(int n = 0) : n(n), one((n + 63) / 64), two((n + 63) / 64) {}

    int get(int i) const
    {
        // i 是列下标；返回该列在 F3 中的系数。
        return one[i >> 6] >> (i & 63) & 1ULL ? 1 :
               (two[i >> 6] >> (i & 63) & 1ULL ? 2 : 0);
    }

    void set(int i, int x)
    {
        // i 是列下标，x 是 0、1 或 2；设置该列系数。
        unsigned long long b = 1ULL << (i & 63);
        one[i >> 6] &= ~b, two[i >> 6] &= ~b;
        if (x == 1) one[i >> 6] |= b;
        if (x == 2) two[i >> 6] |= b;
    }

    void add(const Mod3Row &b)
    {
        // b 是同宽行；把 b 加到当前行，系数按模 3 运算。
        assert(n == b.n);
        for (int i = 0; i < (int)one.size(); i++)
        {
            auto az = ~(one[i] | two[i]), bz = ~(b.one[i] | b.two[i]);
            auto o = (one[i] & bz) | (two[i] & b.two[i]) | (az & b.one[i]);
            auto t = (one[i] & b.one[i]) | (two[i] & bz) | (az & b.two[i]);
            one[i] = o, two[i] = t;
        }
    }

    void sub(const Mod3Row &b)
    {
        // b 是同宽行；从当前行减去 b，系数按模 3 运算。
        assert(n == b.n);
        for (int i = 0; i < (int)one.size(); i++)
        {
            auto az = ~(one[i] | two[i]), bz = ~(b.one[i] | b.two[i]);
            auto o = (one[i] & bz) | (two[i] & b.one[i]) | (az & b.two[i]);
            auto t = (one[i] & b.two[i]) | (two[i] & bz) | (az & b.one[i]);
            one[i] = o, two[i] = t;
        }
    }
};

inline optional<vector<int>> solveMod3(const vector<vector<int>> &mat, int vars)
{
    // mat 每行含 vars 个系数和一个右端，vars 是变量数；返回任意一组 F3 解，无解时返回空。
    assert(vars >= 0); // 调试检查，可删
    int rows = mat.size(), rk = 0;
    vector<Mod3Row> a(rows, Mod3Row(vars + 1));
    for (int i = 0; i < rows; i++)
    {
        assert((int)mat[i].size() == vars + 1); // 调试检查，可删
        for (int j = 0; j <= vars; j++)
        {
            int x = mat[i][j] % 3;
            if (x < 0) x += 3;
            a[i].set(j, x);
        }
    }
    vector<int> pivot(vars, -1);
    for (int col = 0; col < vars && rk < rows; col++)
    {
        int p = rk;
        while (p < rows && !a[p].get(col)) p++;
        if (p == rows) continue;
        swap(a[p], a[rk]);
        int lead = a[rk].get(col);
        for (int i = 0; i < rows; i++)
        {
            if (i == rk || !a[i].get(col)) continue;
            if (a[i].get(col) == lead) a[i].sub(a[rk]);
            else a[i].add(a[rk]);
        }
        pivot[col] = rk++;
    }
    for (int i = rk; i < rows; i++) if (a[i].get(vars)) return nullopt;
    vector<int> ans(vars);
    for (int col = 0; col < vars; col++) if (pivot[col] != -1)
        ans[col] = a[pivot[col]].get(col) * a[pivot[col]].get(vars) % 3;
    return ans;
}
