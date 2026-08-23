#pragma once

#include <bits/stdc++.h>
#include "combinatorics_mod.hpp"
#include "../10_3_001.hpp"
#include "../10_3_002.hpp"
#include "../10_3_003.hpp"
#include "../10_3_004.hpp"
#include "../10_3_005.hpp"

using namespace std;
#define int long long

// 知识点 5.10.002：平面图判定与欧拉公式。
namespace planar_graph
{
optional<vector<int>> chordPlanar(int n, vector<array<int, 2>> es)
{
    // n 是已按环排列的点数，es[i] 是额外弦端点；返回每条弦放环内或环外的 0/1 方案，无解返回空。
    int m = es.size();
    vector<vector<int>> g(m);
    for (auto &[u, v] : es)
    {
        assert(0 <= u && u < n && 0 <= v && v < n);
        if (u > v) swap(u, v);
    }
    for (int i = 0; i < m; i++)
    {
        auto [a, b] = es[i];
        for (int j = i + 1; j < m; j++)
        {
            auto [c, d] = es[j];
            bool cross = (a < c && c < b && b < d) || (c < a && a < d && d < b);
            if (cross)
            {
                g[i].push_back(j);
                g[j].push_back(i);
            }
        }
    }
    vector<int> col(m, -1);
    for (int s = 0; s < m; s++)
    {
        if (col[s] >= 0) continue;
        col[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            for (int v : g[u])
            {
                if (col[v] < 0)
                {
                    col[v] = col[u] ^ 1;
                    q.push(v);
                }
                else if (col[v] == col[u]) return nullopt;
            }
        }
    }
    return col;
}

int faceCount(int v, int e, int comp = 1)
{
    // v、e、comp 是平面图的点数、边数和连通块数；返回任一平面嵌入的面数。
    return e - v + comp + 1;
}
}

// 知识点 5.10.012：Ramsey 定理中 R(3,3)=6 的构造性检查。
namespace ramsey
{
optional<array<int, 3>> monoTriangle(const vector<vector<int>> &col)
{
    // col 是完全图的对称 0/1 边染色；返回一个单色三角形，不存在时返回空。
    int n = col.size();
    for (int i = 0; i < n; i++)
    {
        assert(static_cast<int>(col[i].size()) == n);
        for (int j = i + 1; j < n; j++)
        {
            for (int k = j + 1; k < n; k++)
            {
                if (col[i][j] == col[i][k] && col[i][j] == col[j][k]) return array<int, 3>{i, j, k};
            }
        }
    }
    return nullopt;
}
}

// 知识点 7.1.012：十进制大整数快速整除判定。
namespace fast_divisibility
{
bool divisible(const string &s, int m)
{
    // s 是带可选正负号的十进制整数串，m 是非零除数；返回 s 是否能被 m 整除。
    assert(m != 0); // 调试检查，可删
    auto u = (unsigned long long)m;
    auto d = m < 0 ? 0ULL - u : u;
    unsigned long long r = 0;
    bool small = d <= 3037000499ULL;
    int p = !s.empty() && (s[0] == '+' || s[0] == '-');
    assert(p < (int)s.size()); // 调试检查，可删
    for (; p < (int)s.size(); p++)
    {
        assert(isdigit((unsigned char)s[p])); // 调试检查，可删
        unsigned digit = (unsigned)(s[p] - '0');
        if (small)
        {
            r = (r * 10 + digit) % d;
        }
        else
        {
            r = ((unsigned __int128)r * 10 + digit) % d;
        }
    }
    return r == 0;
}
}

// 知识点 7.2.007：不超过上界且因数个数最多的最小正整数。
namespace highly_composite
{
array<int, 2> mostDivisors(int lim)
{
    // lim 是正上界；返回的两维依次是最小整数及其因数个数。
    assert(lim >= 1); // 调试检查，可删
    constexpr array<int, 16> ps = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53};
    int best = 1;
    int bestCnt = 1;
    auto dfs = [&](auto &&self, int p, int last, int x, int cnt) -> void
    {
        // self 是递归自身，p 是质数下标，last 是指数上界，x 是当前乘积，cnt 是因数个数。
        if (cnt > bestCnt || (cnt == bestCnt && x < best))
        {
            best = x;
            bestCnt = cnt;
        }
        if (p == (int)ps.size())
        {
            return;
        }
        for (int e = 1; e <= last && x <= lim / ps[p]; e++)
        {
            x *= ps[p];
            self(self, p + 1, e, x, cnt * (e + 1));
        }
    };
    dfs(dfs, 0, 63, 1, 1);
    return {best, bestCnt};
}
}

// 知识点 7.2.008：阶乘最后一个非零十进制位。
namespace last_nonzero_digit
{
int lastDigit(int n)
{
    // n 是非负整数；返回 n! 去掉末尾全部零后的个位数字。
    assert(n >= 0); // 调试检查，可删
    constexpr array<int, 5> d = {1, 1, 2, 6, 4};
    if (n < 5)
    {
        return d[n];
    }
    int p = n / 5;
    int two = array<int, 4>{6, 2, 4, 8}[p % 4];
    return lastDigit(p) * d[n % 5] * two % 10;
}
}

// 知识点 8.1.007：容斥统计至少被一个给定正整数整除的数。
namespace incl_excl
{
int countMult(int n, vector<int> d)
{
    // n 是闭区间上界，d 是正除数表；返回 [1,n] 中至少被一个 d[i] 整除的整数个数。
    assert(n >= 0 && d.size() < 63); // 调试检查，可删
    for (int x : d)
    {
        assert(x > 0); // 调试检查，可删
    }
    int ans = 0;
    for (unsigned long long s = 1; s < (1ULL << d.size()); s++)
    {
        int l = 1;
        bool ok = true;
        for (int i = 0; i < (int)d.size(); i++)
        {
            if (!(s >> i & 1))
            {
                continue;
            }
            int g = gcd(l, d[i]);
            if (l > n / (d[i] / g))
            {
                ok = false;
                break;
            }
            l *= d[i] / g;
        }
        if (ok)
        {
            ans += (popcount(s) & 1 ? 1 : -1) * (n / l);
        }
    }
    assert(0 <= ans && ans <= n); // 调试检查，可删
    return ans;
}
}

// 知识点 8.1.008：扩展 min-max 容斥与第 k 大反演。
namespace minmax_inv
{
int kthMaxMinSum(const vector<int> &mn, int k)
{
    // mn[j] 是所有大小为 j 的子集最小值之和且 mn[0] 不用，k 从 1 开始；返回第 k 大元素。
    int n = (int)mn.size() - 1;
    assert(1 <= k && k <= n); // 调试检查，可删
    constexpr array<long long, 3> ps{998244353, 1000000007, 1000000009};
    assert(n < ps[0]); // 调试检查，可删
    array<long long, 3> rem{};
    for (int z = 0; z < 3; z++)
    {
        long long mod = ps[z];
        vector<long long> inv(n - k + 1);
        if (n > k)
        {
            inv[1] = 1;
        }
        for (int i = 2; i <= n - k; i++)
        {
            inv[i] = mod - (mod / i) * inv[mod % i] % mod;
        }
        long long c = 1;
        long long ans = 0;
        for (int j = k; j <= n; j++)
        {
            if (j > k)
            {
                c = c * (j - 1) % mod * inv[j - k] % mod;
            }
            long long x = mn[j] % mod;
            if (x < 0)
            {
                x += mod;
            }
            long long v = c * x % mod;
            if ((j - k) & 1)
            {
                ans -= v;
                if (ans < 0)
                {
                    ans += mod;
                }
            }
            else
            {
                ans += v;
                if (ans >= mod)
                {
                    ans -= mod;
                }
            }
        }
        rem[z] = ans;
    }
    __int128 ans = rem[0];
    __int128 m = ps[0];
    for (int z = 1; z < 3; z++)
    {
        long long mod = ps[z];
        long long a = rem[z] - (long long)(ans % mod);
        if (a < 0)
        {
            a += mod;
        }
        long long b = m % mod;
        long long e = mod - 2;
        long long iv = 1;
        while (e)
        {
            if (e & 1)
            {
                iv = iv * b % mod;
            }
            b = b * b % mod;
            e >>= 1;
        }
        ans += m * (a * iv % mod);
        m *= mod;
    }
    if (ans > m / 2)
    {
        ans -= m;
    }
    assert(numeric_limits<int>::min() <= ans && ans <= numeric_limits<int>::max()); // 调试检查，可删
    return (int)ans;
}
}

// 知识点 8.1.009：有限域上的单位根反演。
namespace roots_unity
{
constexpr int mod = 998244353;

int qpow(int a, int n)
{
    // a 是底数，n 是非负指数；返回模 mod 幂。
    assert(n >= 0); // 调试检查，可删
    a %= mod;
    if (a < 0)
    {
        a += mod;
    }
    int r = 1;
    while (n)
    {
        if (n & 1)
        {
            r = r * a % mod;
        }
        a = a * a % mod;
        n >>= 1;
    }
    return r;
}

int rootFilter(const vector<int> &a, int m, int rem)
{
    // a 是多项式系数，m 是整除 mod-1 的正整数，rem 是余数；返回次数同余 rem 的系数和。
    assert(m > 0 && (mod - 1) % m == 0); // 调试检查，可删
    rem %= m;
    if (rem < 0)
    {
        rem += m;
    }
    int w = qpow(3, (mod - 1) / m);
    int ans = 0;
    for (int j = 0; j < m; j++)
    {
        int x = qpow(w, j), y = 0;
        for (auto it = a.rbegin(); it != a.rend(); ++it)
        {
            int c = *it % mod;
            if (c < 0)
            {
                c += mod;
            }
            y = (y * x + c) % mod;
        }
        int e = ((m - rem) % m) * j % (mod - 1);
        ans = (ans + y * qpow(w, e)) % mod;
    }
    return ans * qpow(m, mod - 2) % mod;
}
}

// 知识点 8.1.010：球盒模型与鸽巢原理。
namespace balls_boxes
{
__int128 comb(__int128 n, int k)
{
    // n 是非负 128 位上标，k 是 64 位下标；返回精确 C(n,k)，要求结果能放入 __int128。
    assert(n >= 0); // 调试检查，可删
    if (k < 0 || (__int128)k > n)
    {
        return 0;
    }
    if ((__int128)k > n - k)
    {
        k = (int)(n - k);
    }
    __int128 ans = 1;
    const __int128 lim = (__int128)(((unsigned __int128)1 << 127) - 1);
    for (int i = 1; i <= k; i++)
    {
        __int128 x = n - k + i;
        int y = i;
        int g = gcd((int)(x % y), y);
        x /= g;
        y /= g;
        assert(ans % y == 0); // 调试检查，可删
        ans /= y;
        assert(ans <= lim / x); // 调试检查，可删
        ans *= x;
    }
    return ans;
}

__int128 unlabBoxDist(int balls, int boxes, bool nonempty)
{
    // balls 是无标号球数，boxes 是有标号盒数，nonempty 表示每盒非空；返回方案数。
    assert(balls >= 0 && boxes >= 0); // 调试检查，可删
    if (boxes == 0)
    {
        return balls == 0;
    }
    if (nonempty)
    {
        return balls < boxes ? 0 : comb(balls - 1, boxes - 1);
    }
    __int128 n = (__int128)balls + boxes - 1;
    return comb(n, boxes - 1);
}

int pigeonBound(int objects, int boxes)
{
    // objects 是对象数，boxes 是正盒数；返回必有一盒至少包含的对象数。
    assert(objects >= 0 && boxes > 0); // 调试检查，可删
    return objects / boxes + (objects % boxes != 0);
}
}

// 知识点 8.2.010：有标号有根树与无根树计数。
namespace tree_count
{
int qpow(int a, int n, int mod)
{
    // a、n、mod 是底数、非负指数和正模数；返回模幂。
    assert(n >= 0 && mod > 0); // 调试检查，可删
    a %= mod;
    if (a < 0)
    {
        a += mod;
    }
    int r = 1 % mod;
    while (n)
    {
        if (n & 1)
        {
            r = combMulMod(r, a, mod);
        }
        a = combMulMod(a, a, mod);
        n >>= 1;
    }
    return r;
}

array<int, 2> cayley(int n, int mod)
{
    // n 是有标号点数，mod 是正模数；返回 {无根树数, 根标号被区分的有根树数}。
    assert(n >= 1 && mod > 0); // 调试检查，可删
    int unrooted = n == 1 ? 1 % mod : qpow(n % mod, n - 2, mod);
    int rooted = qpow(n % mod, n - 1, mod);
    return {unrooted, rooted};
}
}

// 知识点 8.2.012：Burnside / Polya 置换染色计数。
namespace polya
{
__int128 burnside(const vector<vector<int>> &group, int colors)
{
    // group 中每项是位置置换，colors 是颜色数；返回群作用下不同染色轨道数。
    assert(!group.empty() && colors >= 0); // 调试检查，可删
    int n = group[0].size();
    const __int128 lim = (__int128)(((unsigned __int128)1 << 127) - 1);
    __int128 sum = 0;
    for (const auto &p : group)
    {
        assert((int)p.size() == n); // 调试检查，可删
        vector<int> cnt(n);
        for (int x : p)
        {
            assert(0 <= x && x < n); // 调试检查，可删
            cnt[x]++;
        }
        for (int x : cnt)
        {
            assert(x == 1); // 调试检查，可删
        }
        vector<int> vis(n);
        int cyc = 0;
        for (int i = 0; i < n; i++)
        {
            if (vis[i])
            {
                continue;
            }
            cyc++;
            int x = i;
            while (!vis[x])
            {
                vis[x] = 1;
                x = p[x];
            }
        }
        __int128 ways = 1;
        for (int i = 0; i < cyc; i++)
        {
            if (colors)
            {
                assert(ways <= lim / colors); // 调试检查，可删
            }
            ways *= colors;
        }
        assert(sum <= lim - ways); // 调试检查，可删
        sum += ways;
    }
    int sz = group.size();
    assert(sum % sz == 0); // 调试检查，可删
    return sum / sz;
}
}

// 知识点 8.2.013：标准杨表钩长公式。
namespace young_tab
{
__int128 youngTab(const vector<int> &row)
{
    // row 是非增的行长分拆；返回该形状标准杨表数量。
    __int128 cells = 0;
    for (int i = 0; i < (int)row.size(); i++)
    {
        assert(row[i] >= 0); // 调试检查，可删
        if (i)
        {
            assert(row[i - 1] >= row[i]); // 调试检查，可删
        }
        cells += row[i];
    }
    assert(cells < numeric_limits<int>::max()); // 调试检查，可删
    int n = (int)cells;
    vector<int> spf(n + 1), ps;
    for (int i = 2; i <= n; i++)
    {
        if (!spf[i])
        {
            spf[i] = i;
            ps.push_back(i);
        }
        for (int p : ps)
        {
            if (p > spf[i] || i > n / p)
            {
                break;
            }
            spf[i * p] = p;
        }
    }
    vector<int> e(n + 1);
    for (int p : ps)
    {
        for (int x = n / p; x; x /= p)
        {
            e[p] += x;
        }
    }
    vector<int> col(row.empty() ? 0 : row[0]);
    for (int x : row)
    {
        for (int j = 0; j < x; j++)
        {
            col[j]++;
        }
    }
    for (int i = 0; i < (int)row.size(); i++)
    {
        for (int j = 0; j < row[i]; j++)
        {
            int x = row[i] - j + col[j] - i - 1;
            while (x > 1)
            {
                int p = spf[x];
                e[p]--;
                x /= p;
            }
        }
    }
    const __int128 lim = (__int128)(((unsigned __int128)1 << 127) - 1);
    __int128 ans = 1;
    for (int p : ps)
    {
        assert(e[p] >= 0); // 调试检查，可删
        for (int i = 0; i < e[p]; i++)
        {
            assert(ans <= lim / p); // 调试检查，可删
            ans *= p;
        }
    }
    return ans;
}

}

// 知识点 9.1.009：Woodbury 低秩修正矩阵恒等式。
namespace woodbury
{
using Mat = vector<vector<long double>>;

Mat mul(const Mat &a, const Mat &b)
{
    // a、b 是维数相容矩阵；返回矩阵乘积。
    int n = a.size(), k = b.size(), m = b[0].size();
    Mat c(n, vector<long double>(m));
    for (int i = 0; i < n; i++)
    {
        assert(static_cast<int>(a[i].size()) == k);
        for (int p = 0; p < k; p++)
        {
            for (int j = 0; j < m; j++)
            {
                c[i][j] += a[i][p] * b[p][j];
            }
        }
    }
    return c;
}

Mat inv(Mat a)
{
    // a 是可逆方阵；返回高斯消元求得的逆矩阵。
    int n = a.size();
    Mat b(n, vector<long double>(2 * n));
    for (int i = 0; i < n; i++)
    {
        assert(static_cast<int>(a[i].size()) == n);
        copy(a[i].begin(), a[i].end(), b[i].begin());
        b[i][n + i] = 1;
    }
    for (int j = 0; j < n; j++)
    {
        int p = j;
        for (int i = j + 1; i < n; i++)
        {
            if (abs(b[i][j]) > abs(b[p][j])) p = i;
        }
        assert(abs(b[p][j]) > 1E-18L);
        swap(b[p], b[j]);
        long double x = b[j][j];
        for (auto &v : b[j])
        {
            v /= x;
        }
        for (int i = 0; i < n; i++)
        {
            if (i == j) continue;
            x = b[i][j];
            for (int k = 0; k < 2 * n; k++)
            {
                b[i][k] -= x * b[j][k];
            }
        }
    }
    Mat ans(n, vector<long double>(n));
    for (int i = 0; i < n; i++)
    {
        copy(b[i].begin() + n, b[i].end(), ans[i].begin());
    }
    return ans;
}

Mat woodbury(const Mat &ai, const Mat &u, const Mat &ci, const Mat &v)
{
    // ai、ci 分别是 A 与 C 的逆，u、v 是低秩因子；返回 (A+UCV) 的逆。
    Mat left = mul(ai, u);
    Mat mid = ci;
    Mat vaiu = mul(mul(v, ai), u);
    for (int i = 0; i < static_cast<int>(mid.size()); i++)
    {
        for (int j = 0; j < static_cast<int>(mid.size()); j++)
        {
            mid[i][j] += vaiu[i][j];
        }
    }
    Mat sub = mul(mul(left, inv(mid)), mul(v, ai));
    Mat ans = ai;
    for (int i = 0; i < static_cast<int>(ans.size()); i++)
    {
        for (int j = 0; j < static_cast<int>(ans.size()); j++)
        {
            ans[i][j] -= sub[i][j];
        }
    }
    return ans;
}
}

// 知识点 9.2.006：恰选 k 项的 0-1 分数规划。
namespace frac_prog
{
long double maxRatio(const vector<long double> &a, const vector<long double> &b, int k)
{
    // a、b 是分子分母贡献且 b[i] 为正，k 是恰选数量；返回最大比值。
    int n = a.size();
    assert(static_cast<int>(b.size()) == n && 1 <= k && k <= n);
    long double l = 0, r = 0;
    for (int i = 0; i < n; i++)
    {
        assert(b[i] > 0);
        r = max(r, a[i] / b[i]);
    }
    vector<long double> c(n);
    for (int it = 0; it < 100; it++)
    {
        long double mid = (l + r) / 2;
        for (int i = 0; i < n; i++)
        {
            c[i] = a[i] - mid * b[i];
        }
        nth_element(c.begin(), c.end() - k, c.end());
        long double s = accumulate(c.end() - k, c.end(), 0.0L);
        (s >= 0 ? l : r) = mid;
    }
    return (l + r) / 2;
}
}

// 知识点 13.2.001：条件概率、Bayes 与全概率公式。
namespace prob_formula
{
long double condProb(long double pab, long double pb)
{
    // pab 是 P(A∩B)，pb 是 P(B)>0；返回 P(A|B)。
    assert(pb > 0);
    return pab / pb;
}

vector<long double> bayes(const vector<long double> &prior, const vector<long double> &like)
{
    // prior 是先验概率，like 是各假设下证据似然；返回观察证据后的后验分布。
    assert(prior.size() == like.size());
    long double z = 0;
    for (int i = 0; i < static_cast<int>(prior.size()); i++)
    {
        z += prior[i] * like[i];
    }
    assert(z > 0);
    vector<long double> ans(prior.size());
    for (int i = 0; i < static_cast<int>(prior.size()); i++)
    {
        ans[i] = prior[i] * like[i] / z;
    }
    return ans;
}
}

// 知识点 13.2.002：常见离散概率分布。
namespace probability_distribution
{
long double binomialPmf(int n, int k, long double p)
{
    // n、k、p 是试验数、成功数和单次成功率；返回二项分布概率。
    if (k < 0 || k > n) return 0;
    long double c = 1;
    for (int i = 1; i <= k; i++)
    {
        c = c * (n - k + i) / i;
    }
    return c * pow(p, k) * pow(1 - p, n - k);
}

array<long double, 2> geomMoments(long double p)
{
    // p 是每次成功概率；返回计到首次成功的试验次数之期望与方差。
    assert(0 < p && p <= 1);
    return {1 / p, (1 - p) / (p * p)};
}
}

// 知识点 13.2.003：随机变量期望、方差与协方差。
namespace exp_var
{
array<long double, 2> moments(const vector<long double> &x, const vector<long double> &p)
{
    // x 是取值，p 是对应概率且总和为 1；返回期望与方差。
    assert(x.size() == p.size());
    long double e = 0, e2 = 0, sum = 0;
    for (int i = 0; i < static_cast<int>(x.size()); i++)
    {
        sum += p[i];
        e += p[i] * x[i];
        e2 += p[i] * x[i] * x[i];
    }
    assert(abs(sum - 1) <= 1E-12L);
    return {e, e2 - e * e};
}
}

// 知识点 13.3.001：多项式导数与不定积分系数表。
namespace calculus_formula
{
vector<long double> derivative(const vector<long double> &a)
{
    // a[i] 是 x^i 系数；返回导函数系数。
    if (a.size() <= 1) return {0};
    vector<long double> b(a.size() - 1);
    for (int i = 1; i < static_cast<int>(a.size()); i++)
    {
        b[i - 1] = i * a[i];
    }
    return b;
}

vector<long double> integral(const vector<long double> &a, long double c = 0)
{
    // a 是多项式系数，c 是积分常数；返回一个原函数的系数。
    vector<long double> b(a.size() + 1);
    b[0] = c;
    for (int i = 0; i < static_cast<int>(a.size()); i++)
    {
        b[i + 1] = a[i] / (i + 1);
    }
    return b;
}
}

// 知识点 13.3.002：调和数与前三阶幂和。
namespace harmonic_power_sum
{
array<__int128, 3> powerSums(int n)
{
    // n 是非负整数；返回 1..n 的一次、二次、三次幂和。
    __int128 x = n;
    __int128 s1 = x * (x + 1) / 2;
    return {s1, x * (x + 1) * (2 * x + 1) / 6, s1 * s1};
}

long double harmonic(int n)
{
    // n 是非负整数；返回直接累加得到的第 n 个调和数。
    long double ans = 0;
    for (int i = 1; i <= n; i++)
    {
        ans += 1.0L / i;
    }
    return ans;
}
}

// 知识点 13.3.003：正弦定理、余弦定理与 Heron 公式。
namespace triangle_formula
{
array<long double, 3> triangle(long double a, long double b, long double c)
{
    // a、b、c 是合法三角形边长；返回面积、c 对应角和外接圆半径。
    assert(a > 0 && b > 0 && c > 0 && a + b > c && a + c > b && b + c > a);
    long double s = (a + b + c) / 2;
    long double area = sqrt(s * (s - a) * (s - b) * (s - c));
    long double angle = acos(clamp((a * a + b * b - c * c) / (2 * a * b), -1.0L, 1.0L));
    return {area, angle, a * b * c / (4 * area)};
}
}

// 知识点 13.3.004：常用平面面积与立体体积公式。
namespace measure_formula
{
array<long double, 4> roundBodies(long double r, long double h, long double r2)
{
    // r 是圆或球半径，h 是锥台高度，r2 是锥台另一半径；返回圆面积、球面积、球体积、圆台体积。
    assert(r >= 0 && h >= 0 && r2 >= 0);
    long double pi = acos(-1.0L);
    return {pi * r * r, 4 * pi * r * r, 4 * pi * r * r * r / 3,
            pi * h * (r * r + r * r2 + r2 * r2) / 3};
}
}

// 知识点 13.3.006：常用常数与 NTT 质数表。
namespace constants_table
{
constexpr long double pi = 3.141592653589793238462643383279502884L;
constexpr long double e = 2.718281828459045235360287471352662498L;
constexpr long double phi = 1.618033988749894848204586834365638118L;
// ntt[i] 三维依次是质数、原根、可用的最大二次幂指数。
constexpr array<array<int, 3>, 3> ntt = {{{998244353, 3, 23}, {1004535809, 3, 21}, {469762049, 3, 26}}};
}
