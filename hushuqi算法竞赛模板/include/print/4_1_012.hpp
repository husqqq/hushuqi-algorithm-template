#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int inf = 1E9;
constexpr long long INF = 4E18;
constexpr long double eps = 1E-12L;

template <class T> bool chmin(T &a, const T &b)
{
    // a 是待更新值，b 是候选值；若 a 变小则返回 true。
    if (b >= a)
    {
        return false;
    }
    a = b;
    return true;
}
template <class T> bool chmax(T &a, const T &b)
{
    // a 是待更新值，b 是候选值；若 a 变大则返回 true。
    if (a >= b)
    {
        return false;
    }
    a = b;
    return true;
}

using CP = complex<long double>;

void fft(vector<CP> &a, bool inv)
{
    // a 是复数系数数组，inv 表示是否执行逆变换；变换原地完成。
    int n = a.size();
    for (int i = 1, j = 0; i < n; i++)
    {
        int b = n >> 1;
        for (; j & b; b >>= 1)
        {
            j ^= b;
        }
        j ^= b;
        if (i < j)
        {
            swap(a[i], a[j]);
        }
    }
    for (int len = 2; len <= n; len <<= 1)
    {
        CP w(cosl(2 * acosl(-1.L) / len), (inv ? -1 : 1) * sinl(2 * acosl(-1.L) / len));
        for (int i = 0; i < n; i += len)
        {
            CP x = 1;
            for (int j = 0; j < len / 2; j++, x *= w)
            {
                CP u = a[i + j], v = a[i + j + len / 2] * x;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
            }
        }
    }
    if (inv)
    {
        for (auto &x : a)
        {
            x /= n;
        }
    }
}

vector<long double> convReal(const vector<long double> &a, const vector<long double> &b)
{
    // a、b 是实数系数数组；返回二者的浮点线性卷积。
    if (a.empty() || b.empty())
    {
        return {};
    }
    int s = a.size() + b.size() - 1;
    int n = bit_ceil((unsigned long long)s);
    vector<CP> x(n), y(n);
    for (int i = 0; i < (int)a.size(); i++)
    {
        x[i] = a[i];
    }
    for (int i = 0; i < (int)b.size(); i++)
    {
        y[i] = b[i];
    }
    fft(x, false);
    fft(y, false);
    for (int i = 0; i < n; i++)
    {
        x[i] *= y[i];
    }
    fft(x, true);
    vector<long double> c(s);
    for (int i = 0; i < s; i++)
    {
        c[i] = x[i].real();
    }
    return c;
}

vector<bool> wildcardMC(const string &s, string t, unsigned long long seed, char any = '*')
{
    // s 是文本串，t 是模式串，seed 是本轮随机种子；返回每个合法起点是否匹配。
    int n = s.size();
    int m = t.size();
    if (!m)
    {
        return vector<bool>(n + 1, true);
    }
    if (m > n)
    {
        return {};
    }
    mt19937_64 rng(seed);
    constexpr unsigned long long mask = (1ULL << 48) - 1;
    constexpr long double scale = 1ULL << 48;
    array<long double, 256> w;
    for (long double &x : w)
    {
        x = 1 + ((rng() & mask) + 0.5L) / scale;
    }
    vector<long double> a(n), b(m);
    for (int i = 0; i < n; i++)
    {
        a[i] = w[(unsigned char)s[i]];
    }
    int fixed = 0;
    for (int i = 0; i < m; i++)
    {
        char c = t[m - 1 - i];
        if (c != any)
        {
            b[i] = 1 / w[(unsigned char)c];
            fixed++;
        }
    }
    auto c = convReal(a, b);
    long double tol = 1E-9L * max<int>(1, fixed);
    vector<bool> ans(n - m + 1);
    for (int i = 0; i + m <= n; i++)
    {
        ans[i] = fabsl(c[i + m - 1] - fixed) <= tol;
    }
    return ans;
}
