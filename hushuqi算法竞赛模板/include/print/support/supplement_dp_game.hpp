#pragma once

#include <bits/stdc++.h>

using namespace std;
#define int long long

// 知识点 5.4.003：2-SAT 的字典序最小可行赋值。
namespace lex_two_sat
{
optional<vector<int>> lexTwoSat(int n, const vector<array<int, 4>> &clauses)
{
    // n 是变量数；clauses[i] 四维依次是 u、u 的目标布尔值、v、v 的目标布尔值，表示二者至少一个成立。
    auto feasible = [&](const vector<int> &fix) -> bool
    {
        // fix[x] 为 -1 或强制布尔值；返回加入强制条件后公式是否可满足。
        vector<vector<int>> g(2 * n), rg(2 * n);
        auto add = [&](int u, int v)
        {
            // u、v 是文字编号；加入蕴含 u=>v，无返回值。
            g[u].push_back(v);
            rg[v].push_back(u);
        };
        for (auto [u, a, v, b] : clauses)
        {
            assert(0 <= u && u < n && 0 <= v && v < n && (a == 0 || a == 1) && (b == 0 || b == 1));
            int x = 2 * u + a, y = 2 * v + b;
            add(x ^ 1, y);
            add(y ^ 1, x);
        }
        for (int i = 0; i < n; i++)
        {
            if (fix[i] >= 0) add(2 * i + (fix[i] ^ 1), 2 * i + fix[i]);
        }
        vector<int> vis(2 * n), ord, id(2 * n, -1);
        auto dfs = [&](auto &&self, int u) -> void
        {
            // self 是递归自身，u 是文字；完成正图后序遍历，无返回值。
            vis[u] = 1;
            for (int v : g[u])
            {
                if (!vis[v]) self(self, v);
            }
            ord.push_back(u);
        };
        auto rdfs = [&](auto &&self, int u, int c) -> void
        {
            // self 是递归自身，u 是文字，c 是分量编号；标记反图分量，无返回值。
            id[u] = c;
            for (int v : rg[u])
            {
                if (id[v] < 0) self(self, v, c);
            }
        };
        for (int i = 0; i < 2 * n; i++)
        {
            if (!vis[i]) dfs(dfs, i);
        }
        reverse(ord.begin(), ord.end());
        int c = 0;
        for (int u : ord)
        {
            if (id[u] < 0) rdfs(rdfs, u, c++);
        }
        for (int i = 0; i < n; i++)
        {
            if (id[2 * i] == id[2 * i + 1]) return false;
        }
        return true;
    };
    vector<int> ans(n, -1);
    if (!feasible(ans)) return nullopt;
    for (int i = 0; i < n; i++)
    {
        ans[i] = 0;
        if (!feasible(ans)) ans[i] = 1;
    }
    return ans;
}
}

// 知识点 12.2.002：旅行商状态压缩 DP。
namespace bitmask_dp
{
int tsp(const vector<vector<int>> &w)
{
    // w 是完整有向图权值矩阵；返回从 0 出发访问所有点并回到 0 的最小费用，无解返回 -1。
    int n = w.size();
    assert(1 <= n && n <= 22);
    const int inf = numeric_limits<int>::max() / 4;
    vector<vector<int>> dp(1LL << n, vector<int>(n, inf));
    dp[1][0] = 0;
    for (int s = 1; s < (1LL << n); s++)
    {
        for (int u = 0; u < n; u++)
        {
            if (!(s >> u & 1) || dp[s][u] == inf) continue;
            for (int v = 0; v < n; v++)
            {
                if (s >> v & 1) continue;
                dp[s | 1LL << v][v] = min(dp[s | 1LL << v][v], dp[s][u] + w[u][v]);
            }
        }
    }
    int ans = inf;
    for (int u = 0; u < n; u++)
    {
        ans = min(ans, dp.back()[u] + w[u][0]);
    }
    return ans == inf ? -1 : ans;
}
}

// 知识点 12.2.003：矩形棋盘切成 k 块的最小平方和。
namespace board_part
{
int splitBoard(const vector<vector<int>> &a, int parts)
{
    // a 是非空棋盘，parts 是正块数；返回沿格线切成 parts 个矩形后各块元素和平方的最小总和。
    int n = a.size(), m = a[0].size();
    assert(1 <= parts && parts <= n * m);
    vector<vector<int>> s(n + 1, vector<int>(m + 1));
    for (int i = 0; i < n; i++)
    {
        assert(static_cast<int>(a[i].size()) == m);
        for (int j = 0; j < m; j++)
        {
            s[i + 1][j + 1] = s[i][j + 1] + s[i + 1][j] - s[i][j] + a[i][j];
        }
    }
    auto sum = [&](int x1, int y1, int x2, int y2)
    {
        // 四个参数定义半开子矩形；返回元素和。
        return s[x2][y2] - s[x1][y2] - s[x2][y1] + s[x1][y1];
    };
    // Key[0..4] 依次是上、左、下、右边界与块数。
    using Key = array<int, 5>;
    map<Key, int> memo;
    const int inf = numeric_limits<int>::max() / 4;
    auto dfs = [&](auto &&self, int x1, int y1, int x2, int y2, int k) -> int
    {
        // self 是递归自身，四坐标定义矩形，k 是块数；返回最小平方和。
        Key key{x1, y1, x2, y2, k};
        if (auto it = memo.find(key); it != memo.end()) return it->second;
        if (k == 1)
        {
            int x = sum(x1, y1, x2, y2);
            return memo[key] = x * x;
        }
        int ans = inf;
        for (int x = x1 + 1; x < x2; x++)
        {
            for (int p = 1; p < k; p++)
            {
                ans = min(ans, self(self, x1, y1, x, y2, p) + self(self, x, y1, x2, y2, k - p));
            }
        }
        for (int y = y1 + 1; y < y2; y++)
        {
            for (int p = 1; p < k; p++)
            {
                ans = min(ans, self(self, x1, y1, x2, y, p) + self(self, x1, y, x2, y2, k - p));
            }
        }
        return memo[key] = ans;
    };
    return dfs(dfs, 0, 0, n, m, parts);
}
}

// 知识点 12.2.004：独立任务分配给两台机器的最短完工时间。
namespace twoMachSched
{
int twoMachines(const vector<int> &a)
{
    // a 是各任务的非负耗时；返回两台相同机器并行完成全部任务的最小 makespan。
    int sum = accumulate(a.begin(), a.end(), 0LL);
    vector<char> dp(sum + 1);
    dp[0] = 1;
    for (int x : a)
    {
        assert(x >= 0);
        for (int s = sum; s >= x; s--)
        {
            dp[s] |= dp[s - x];
        }
    }
    int ans = sum;
    for (int s = 0; s <= sum; s++)
    {
        if (dp[s]) ans = min(ans, max(s, sum - s));
    }
    return ans;
}
}

// 知识点 12.2.005：完全背包找硬币的最少枚数与方案数。
namespace coin_problem
{
array<int, 2> coinChange(const vector<int> &coin, int target, int mod)
{
    // coin 是不同正面值，target 是目标金额，mod 是方案数模数；返回最少枚数和无序方案数，无解时第一维为 -1。
    const int inf = numeric_limits<int>::max() / 4;
    vector<int> best(target + 1, inf), ways(target + 1);
    best[0] = 0;
    ways[0] = 1;
    for (int x : coin)
    {
        assert(x > 0);
        for (int s = x; s <= target; s++)
        {
            best[s] = min(best[s], best[s - x] + 1);
            ways[s] = (ways[s] + ways[s - x]) % mod;
        }
    }
    return {best[target] == inf ? -1 : best[target], ways[target]};
}
}

// 知识点 12.2.006：汉诺塔第 k 步移动。
namespace hanoi
{
array<int, 3> kthMove(int n, unsigned long long k)
{
    // n 是盘数，k 从 1 开始且不超过 2^n-1；返回盘号、起始柱和目标柱，柱编号为 0..2。
    assert(1 <= n && n < 64 && 1 <= k && k < (1ULL << n));
    int disk = countr_zero(k) + 1;
    unsigned long long before = (k >> disk) + 1;
    int dir = ((n - disk) & 1) ? 1 : -1;
    int from = (dir * static_cast<int>(before - 1) % 3 + 3) % 3;
    int to = (from + dir + 3) % 3;
    return {disk, from, to};
}
}

// 知识点 12.3.002：满足 Knuth 条件的区间 DP。
namespace quad_opt
{
template<class Cost>
int knuth(int n, Cost w)
{
    // n 是序列长度，w(l,r) 返回半开区间代价；返回 dp[0][n]，调用者必须证明 Knuth 单调性条件。
    vector<vector<int>> dp(n + 1, vector<int>(n + 1));
    vector<vector<int>> opt(n + 1, vector<int>(n + 1));
    for (int i = 0; i <= n; i++)
    {
        opt[i][i] = i;
    }
    const int inf = numeric_limits<int>::max() / 4;
    for (int len = 1; len <= n; len++)
    {
        for (int l = 0; l + len <= n; l++)
        {
            int r = l + len;
            dp[l][r] = inf;
            int lo = max(l, opt[l][r - 1]);
            int hi = min(r - 1, opt[l + 1][r]);
            if (len == 1) lo = hi = l;
            for (int k = lo; k <= hi; k++)
            {
                int cur = dp[l][k] + dp[k + 1][r] + w(l, r);
                if (cur < dp[l][r]) dp[l][r] = cur, opt[l][r] = k;
            }
        }
    }
    return dp[0][n];
}
}

// 知识点 12.3.003：决策单调的分治优化。
namespace dc_dp
{
template<class Cost>
vector<int> divideLayer(const vector<int> &pre, int n, Cost w)
{
    // pre[j] 是上一层状态，n 是本层最大下标，w(j,i) 是转移代价；返回本层 dp，调用者必须保证最优决策单调。
    const int inf = numeric_limits<int>::max() / 4;
    vector<int> dp(n + 1, inf);
    auto solve = [&](auto &&self, int l, int r, int ql, int qr) -> void
    {
        // self 是递归自身，[l,r] 是待算区间，[ql,qr] 是候选决策范围；填充 dp，无返回值。
        if (l > r) return;
        int mid = (l + r) / 2, best = ql;
        for (int j = ql; j <= min(qr, mid - 1); j++)
        {
            int cur = pre[j] + w(j, mid);
            if (cur < dp[mid]) dp[mid] = cur, best = j;
        }
        self(self, l, mid - 1, ql, best);
        self(self, mid + 1, r, best, qr);
    };
    solve(solve, 1, n, 0, n - 1);
    return dp;
}
}

// 知识点 13.1.005：Wythoff 两堆取石博弈。
namespace wythoff_game
{
bool firstWins(int a, int b)
{
    // a、b 是非负石子数；返回先手是否必胜。
    if (a > b) swap(a, b);
    int k = b - a;
    long double phi = (1 + sqrt(5.0L)) / 2;
    int x = floor(k * phi);
    while (static_cast<__int128>(x + 1) * (x + 1) - static_cast<__int128>(x + 1) * k <=
           static_cast<__int128>(k) * k)
    {
        x++;
    }
    while (static_cast<__int128>(x) * x - static_cast<__int128>(x) * k > static_cast<__int128>(k) * k)
    {
        x--;
    }
    return a != x;
}
}

// 知识点 13.1.006：Fibonacci 取石的最优首取量。
namespace fib_game
{
int zeckMin(int n)
{
    // n 是正石子数；返回 Zeckendorf 分解中的最小 Fibonacci 项。
    assert(n > 0);
    vector<int> f = {1, 2};
    while (f.back() <= n - f[f.size() - 2])
    {
        f.push_back(f.back() + f[f.size() - 2]);
    }
    int last = 0;
    for (auto it = f.rbegin(); it != f.rend(); ++it)
    {
        if (*it <= n) n -= *it, last = *it;
    }
    return last;
}
}

// 知识点 13.1.007：树上删边 Green Hackenbush 的 SG 值。
namespace edge_del
{
int treeSg(const vector<vector<int>> &g, int root = 0)
{
    // g 是无向树，root 是地面根；返回整个局面的 SG 值。
    auto dfs = [&](auto &&self, int u, int p) -> int
    {
        // self 是递归自身，u 是当前点，p 是父点；返回当前子树归约后的 SG 值。
        int ans = 0;
        for (int v : g[u])
        {
            if (v != p) ans ^= self(self, v, u) + 1;
        }
        return ans;
    };
    return dfs(dfs, root, -1);
}
}

// 知识点 13.1.008：固定取法集合的取石子 SG 表。
namespace stone_game
{
vector<int> subGameSG(int n, const vector<int> &take)
{
    // n 是最大石子数，take 是每次允许取走的正数；返回 0..n 的 SG 值。
    vector<int> sg(n + 1);
    for (int x = 1; x <= n; x++)
    {
        vector<char> seen(take.size() + 1);
        for (int d : take)
        {
            assert(d > 0);
            if (d <= x && sg[x - d] < static_cast<int>(seen.size())) seen[sg[x - d]] = 1;
        }
        while (sg[x] < static_cast<int>(seen.size()) && seen[sg[x]])
        {
            sg[x]++;
        }
    }
    return sg;
}
}

// 知识点 14.1.008：装箱问题的近似与小规模精确解。
namespace bin_packing
{
int firstFitDec(vector<int> a, int cap)
{
    // a 是物品体积，cap 是箱容量；返回降序首次适应使用的箱数。
    sort(a.begin(), a.end(), greater<>());
    vector<int> rem;
    for (int x : a)
    {
        assert(0 <= x && x <= cap);
        int p = 0;
        while (p < static_cast<int>(rem.size()) && rem[p] < x)
        {
            p++;
        }
        if (p == static_cast<int>(rem.size())) rem.push_back(cap);
        rem[p] -= x;
    }
    return rem.size();
}

int exactBins(const vector<int> &a, int cap)
{
    // a 是至多 24 个物品体积，cap 是箱容量；返回最少箱数。
    int n = a.size();
    assert(n <= 24);
    // State[0..1] 依次是已用箱数与最后一个箱的已用容量。
    using State = array<int, 2>;
    State inf{n + 1, 0};
    vector<State> dp(1LL << n, inf);
    dp[0] = {1, 0};
    for (int s = 0; s < (1LL << n); s++)
    {
        for (int i = 0; i < n; i++)
        {
            if (s >> i & 1) continue;
            assert(0 <= a[i] && a[i] <= cap);
            State cur = dp[s];
            if (cur[1] + a[i] <= cap) cur[1] += a[i];
            else cur[0]++, cur[1] = a[i];
            dp[s | 1LL << i] = min(dp[s | 1LL << i], cur);
        }
    }
    return n == 0 ? 0 : dp.back()[0];
}
}

// 知识点 14.1.009：障碍点网格最短路径计数。
namespace grid_count
{
constexpr int mod = 1000000007;

int qpow(int a, int n)
{
    // a 是底数，n 是非负指数；返回模 mod 幂。
    int r = 1;
    while (n)
    {
        if (n & 1) r = r * a % mod;
        a = a * a % mod;
        n >>= 1;
    }
    return r;
}

int gridPaths(int h, int w, vector<array<int, 2>> ban)
{
    // h、w 是正网格尺寸，ban[i] 两维是 0 下标障碍坐标；返回只向下或向右到右下角的路径数。
    int n = h + w + 5;
    vector<int> fac(n, 1), ifac(n, 1);
    for (int i = 1; i < n; i++)
    {
        fac[i] = fac[i - 1] * i % mod;
    }
    ifac.back() = qpow(fac.back(), mod - 2);
    for (int i = n - 1; i; i--)
    {
        ifac[i - 1] = ifac[i] * i % mod;
    }
    auto comb = [&](int a, int b)
    {
        // a、b 是组合数参数；返回模 mod 的 C(a,b)。
        if (b < 0 || b > a) return 0LL;
        return fac[a] * ifac[b] % mod * ifac[a - b] % mod;
    };
    ban.push_back({h - 1, w - 1});
    sort(ban.begin(), ban.end());
    vector<int> dp(ban.size());
    for (int i = 0; i < static_cast<int>(ban.size()); i++)
    {
        auto [x, y] = ban[i];
        dp[i] = comb(x + y, x);
        for (int j = 0; j < i; j++)
        {
            auto [u, v] = ban[j];
            if (u <= x && v <= y)
            {
                dp[i] = (dp[i] - dp[j] * comb(x - u + y - v, x - u)) % mod;
            }
        }
        if (dp[i] < 0) dp[i] += mod;
    }
    return dp.back();
}
}
