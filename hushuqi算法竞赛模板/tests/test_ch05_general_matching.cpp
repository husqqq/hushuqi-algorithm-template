#include "../include/full/5_7_001.hpp"
#include "../include/full/5_7_002.hpp"
#include "../include/full/5_7_003.hpp"
#include "../include/full/5_7_005.hpp"
#include "../include/full/5_7_007.hpp"

int bruteMatch(int n, const vector<pair<int, int>> &edge)
{
    // n 是点数，edge 是无向边；返回穷举得到的最大匹配边数。
    vector<unsigned long long> g(n);
    for (auto [u, v] : edge)
    {
        if (u != v)
        {
            g[u] |= 1ULL << v;
            g[v] |= 1ULL << u;
        }
    }
    vector<int> f(1ULL << n, -1);
    f[0] = 0;
    auto go = [&](auto &&self, unsigned long long s) -> int
    {
        if (f[s] != -1)
        {
            return f[s];
        }
        int u = countr_zero(s);
        int ans = self(self, s ^ (1ULL << u));
        unsigned long long can = g[u] & s;
        while (can)
        {
            int v = countr_zero(can);
            can &= can - 1;
            ans = max(ans, self(self, s ^ (1ULL << u) ^ (1ULL << v)) + 1);
        }
        return f[s] = ans;
    };
    return go(go, (1ULL << n) - 1);
}

__int128 bruteWeight(const vector<vector<optional<long long>>> &w, unsigned long long s)
{
    // w 是带权无向图，s 是尚未决定的点集；返回最大匹配权。
    if (!s)
    {
        return 0;
    }
    int u = countr_zero(s);
    __int128 ans = bruteWeight(w, s ^ (1ULL << u));
    for (int v = u + 1; v < (int)w.size(); v++)
    {
        if ((s >> v & 1) && w[u][v].has_value())
        {
            ans = max(ans,
                      bruteWeight(w, s ^ (1ULL << u) ^ (1ULL << v)) +
                          (__int128)*w[u][v]);
        }
    }
    return ans;
}

void checkMarriage(const vector<vector<int>> &pa, const vector<vector<int>> &pb,
                   const Marriage &res)
{
    // pa、pb 是完整偏好表，res 是候选结果；检查互逆匹配与稳定性。
    int n = pa.size();
    vector<vector<int>> ra(n, vector<int>(n)), rb(n, vector<int>(n));
    for (int u = 0; u < n; u++)
    {
        for (int i = 0; i < n; i++)
        {
            ra[u][pa[u][i]] = i;
            rb[u][pb[u][i]] = i;
        }
    }
    for (int u = 0; u < n; u++)
    {
        assert(res.b[res.a[u]] == u);
    }
    for (int u = 0; u < n; u++)
    {
        for (int v = 0; v < n; v++)
        {
            bool left = ra[u][v] < ra[u][res.a[u]];
            bool right = rb[v][u] < rb[v][res.b[v]];
            assert(!(left && right));
        }
    }
}

void checkSparseMarriage(int nLeft, int nRight, const vector<StableEdge> &edges,
                         const SparseMatch &res)
{
    // nLeft、nRight、edges 描述严格不完整偏好，res 是候选结果；检查互逆性和每条可接受边的稳定性。
    const int inf = numeric_limits<int>::max();
    vector<vector<int>> leftRank(nLeft, vector<int>(nRight, inf));
    vector<vector<int>> rightRank(nRight, vector<int>(nLeft, inf));
    for (auto [x, y, rx, ry] : edges)
    {
        leftRank[x][y] = rx;
        rightRank[y][x] = ry;
    }
    assert((int)res.left.size() == nLeft && (int)res.right.size() == nRight);
    for (int x = 0; x < nLeft; ++x)
    {
        if (res.left[x] != -1)
        {
            int y = res.left[x];
            assert(0 <= y && y < nRight && res.right[y] == x);
            assert(leftRank[x][y] != inf);
        }
    }
    for (int y = 0; y < nRight; ++y)
    {
        if (res.right[y] != -1)
        {
            assert(res.left[res.right[y]] == y);
        }
    }
    for (auto [x, y, rx, ry] : edges)
    {
        bool wantLeft = res.left[x] == -1 || rx < leftRank[x][res.left[x]];
        bool wantRight = res.right[y] == -1 || ry < rightRank[y][res.right[y]];
        assert(!(wantLeft && wantRight));
    }
}

signed main()
{
    mt19937_64 rng(0);
    for (int it = 0; it < 1200; it++)
    {
        int n = rng() % 10 + 1;
        vector<pair<int, int>> edge;
        Blossom bl(n);
        for (int u = 0; u < n; u++)
        {
            for (int v = u + 1; v < n; v++)
            {
                if (rng() % 3 == 0)
                {
                    edge.push_back({u, v});
                    bl.add(u, v);
                }
            }
        }
        int want = bruteMatch(n, edge);
        assert(bl.work() == want);
        assert(tutteMatch(n, edge, 712367 + it) == want);
        for (int u = 0; u < n; u++)
        {
            if (bl.match[u] != -1)
            {
                assert(bl.match[bl.match[u]] == u);
            }
        }
    }

    for (int it = 0; it < 150; it++)
    {
        int n = rng() % 9 + 1;
        vector<vector<optional<long long>>> w(n, vector<optional<long long>>(n));
        for (int u = 0; u < n; u++)
        {
            for (int v = u + 1; v < n; v++)
            {
                if (rng() % 2)
                {
                    long long x = (long long)(rng() % 41) - 20;
                    w[u][v] = x;
                    w[v][u] = x;
                }
            }
        }
        auto [got, edge] = weightMatch(w);
        assert(got == bruteWeight(w, (1ULL << n) - 1));
        vector<bool> used(n);
        __int128 sum = 0;
        for (auto [u, v] : edge)
        {
            assert(!used[u] && !used[v] && w[u][v].has_value());
            used[u] = true;
            used[v] = true;
            sum += *w[u][v];
        }
        assert(sum == got);
    }

    vector<vector<optional<long long>>> extreme(2, vector<optional<long long>>(2));
    extreme[0][1] = LLONG_MIN;
    extreme[1][0] = LLONG_MIN;
    assert(weightMatch(extreme).first == 0);

    vector<vector<int>> pa = {{0, 1, 2}, {1, 0, 2}, {1, 2, 0}};
    vector<vector<int>> pb = {{1, 0, 2}, {0, 2, 1}, {2, 1, 0}};
    checkMarriage(pa, pb, stableMatch(pa, pb));
    assert(stableMatch({}, {}).a.empty());

    vector<StableEdge> fixed{{0, 0, 0, 1}, {0, 1, 1, 0},
                                   {1, 0, 0, 0}, {2, 2, 0, 0}};
    auto fixedResult = sparseMatch(3, 4, fixed);
    assert((fixedResult.left == vector<int>{1, 0, 2}));
    assert((fixedResult.right == vector<int>{1, 0, 2, -1}));
    checkSparseMarriage(3, 4, fixed, fixedResult);
    auto emptyResult = sparseMatch(2, 3, {});
    assert((emptyResult.left == vector<int>{-1, -1}));
    assert((emptyResult.right == vector<int>{-1, -1, -1}));

    for (int it = 0; it < 1200; ++it)
    {
        int nLeft = rng() % 7, nRight = rng() % 7;
        vector<vector<int>> leftOrder(nLeft), rightOrder(nRight);
        vector<vector<char>> acceptable(nLeft, vector<char>(nRight));
        for (int x = 0; x < nLeft; ++x)
        {
            for (int y = 0; y < nRight; ++y)
            {
                if (rng() % 3)
                {
                    acceptable[x][y] = true;
                    leftOrder[x].push_back(y);
                    rightOrder[y].push_back(x);
                }
            }
            shuffle(leftOrder[x].begin(), leftOrder[x].end(), rng);
        }
        for (auto &order : rightOrder)
        {
            shuffle(order.begin(), order.end(), rng);
        }
        vector<vector<int>> lr(nLeft, vector<int>(nRight, -1));
        vector<vector<int>> rr(nRight, vector<int>(nLeft, -1));
        for (int x = 0; x < nLeft; ++x)
        {
            for (int i = 0; i < (int)leftOrder[x].size(); ++i)
            {
                lr[x][leftOrder[x][i]] = i;
            }
        }
        for (int y = 0; y < nRight; ++y)
        {
            for (int i = 0; i < (int)rightOrder[y].size(); ++i)
            {
                rr[y][rightOrder[y][i]] = i;
            }
        }
        vector<StableEdge> edges;
        for (int x = 0; x < nLeft; ++x)
        {
            for (int y = 0; y < nRight; ++y)
            {
                if (acceptable[x][y])
                {
                    edges.push_back({x, y, lr[x][y], rr[y][x]});
                }
            }
        }
        auto got = sparseMatch(nLeft, nRight, edges);
        checkSparseMarriage(nLeft, nRight, edges, got);
    }

    puts("OK");
    return 0;
}
