#pragma once

#include "../base.hpp"

namespace contour_dtl
{
struct PathEntry
{
    int cen = -1, branch = -1, distance = 0;
};

struct Fenwick
{
    vector<long long> bit;
    Fenwick() = default;
    explicit Fenwick(int n) : bit(n + 1) {}
    void add(int p, long long value)
    {
        for (++p; p < (int)bit.size(); p += p & -p) bit[p] += value;
    }
    long long sumPrefix(int p) const
    {
        long long res = 0;
        for (; p; p -= p & -p) res += bit[p];
        return res;
    }
};

struct Bucket
{
    vector<int> coord;
    Fenwick sumTree, diffTree;
    void build(vector<int> x)
    {
        sort(x.begin(), x.end());
        x.erase(unique(x.begin(), x.end()), x.end());
        coord = move(x);
        sumTree = Fenwick(coord.size());
        diffTree = Fenwick(coord.size());
    }
    int lower(int x) const
    {
        return lower_bound(coord.begin(), coord.end(), x) - coord.begin();
    }
    void addPoint(int x, long long value)
    {
        int p = lower(x);
        if (p < (int)coord.size() && coord[p] == x) sumTree.add(p, value);
    }
    void addRange(int l, int r, long long value)
    {
        int left = lower_bound(coord.begin(), coord.end(), l) - coord.begin();
        int right = lower_bound(coord.begin(), coord.end(), r) - coord.begin();
        if (left < right)
        {
            diffTree.add(left, value);
            diffTree.add(right, -value);
        }
    }
    long long rangeSum(int l, int r) const
    {
        int left = lower_bound(coord.begin(), coord.end(), l) - coord.begin();
        int right = lower_bound(coord.begin(), coord.end(), r) - coord.begin();
        return sumTree.sumPrefix(right) - sumTree.sumPrefix(left);
    }
    long long pval(int x) const
    {
        int p = upper_bound(coord.begin(), coord.end(), x) - coord.begin();
        return diffTree.sumPrefix(p);
    }
};

class Index
{
    int n = 0;
    vector<vector<int>> graph;
    vector<char> removed;
    vector<int> subtree, parent;
    vector<vector<PathEntry>> paths;
    vector<Bucket> all;
    vector<vector<Bucket>> branches;
    vector<vector<vector<int>>> branchCoord;
    vector<vector<int>> allCoord;

    int findCentroid(int start)
    {
        vector<int> order{start};
        parent[start] = -1;
        for (int i = 0; i < (int)order.size(); i++)
        {
            int u = order[i];
            for (int v : graph[u]) if (!removed[v] && v != parent[u]) parent[v] = u, order.push_back(v);
        }
        int total = order.size();
        for (int i = total - 1; i >= 0; i--)
        {
            int u = order[i];
            subtree[u] = 1;
            for (int v : graph[u]) if (!removed[v] && parent[v] == u) subtree[u] += subtree[v];
        }
        for (int u : order)
        {
            int largest = total - subtree[u];
            for (int v : graph[u]) if (!removed[v] && parent[v] == u) largest = max(largest, subtree[v]);
            if (largest * 2 <= total) return u;
        }
        return start;
    }
    void enumerate(int cen, int first, int branch)
    {
        vector<array<int, 3>> stack{{first, cen, 1}};
        while (!stack.empty())
        {
            auto [u, p, d] = stack.back();
            stack.pop_back();
            paths[u].push_back({cen, branch, d});
            allCoord[cen].push_back(d);
            branchCoord[cen][branch].push_back(d);
            for (int v : graph[u]) if (!removed[v] && v != p) stack.push_back({v, u, d + 1});
        }
    }
    void buildComp(int start)
    {
        int cen = findCentroid(start);
        removed[cen] = true;
        paths[cen].push_back({cen, -1, 0});
        allCoord[cen].push_back(0);
        for (int v : graph[cen]) if (!removed[v])
        {
            int branch = branches[cen].size();
            branches[cen].emplace_back();
            branchCoord[cen].emplace_back();
            enumerate(cen, v, branch);
        }
        for (int v : graph[cen]) if (!removed[v]) buildComp(v);
    }

public:
    explicit Index(const vector<vector<int>> &g)
        : n(g.size()), graph(g), removed(n), subtree(n), parent(n), paths(n), all(n), branches(n),
          branchCoord(n), allCoord(n)
    {
        if (n) buildComp(0);
        for (int c = 0; c < n; c++)
        {
            all[c].build(allCoord[c]);
            for (int b = 0; b < (int)branches[c].size(); b++) branches[c][b].build(branchCoord[c][b]);
        }
    }
    const vector<PathEntry> &path(int u) const { return paths[u]; }
    Bucket &whole(int c) { return all[c]; }
    const Bucket &whole(int c) const { return all[c]; }
    Bucket &part(int c, int b) { return branches[c][b]; }
    const Bucket &part(int c, int b) const { return branches[c][b]; }
};
}

template <class T = long long> class PtSetRngSum
{
    contour_dtl::Index index;
    vector<T> value;
public:
    PtSetRngSum(const vector<vector<int>> &g, const vector<T> &a) : index(g), value(a.size())
    {
        assert(g.size() == a.size());
        for (int u = 0; u < (int)a.size(); u++) set(u, a[u]);
    }
    T get(int u) const { return value[u]; }
    void set(int u, T x)
    {
        T delta = x - value[u];
        value[u] = x;
        for (auto e : index.path(u))
        {
            index.whole(e.cen).addPoint(e.distance, delta);
            if (e.branch >= 0) index.part(e.cen, e.branch).addPoint(e.distance, delta);
        }
    }
    T prod(int u, int dl, int dr) const
    {
        T ans = 0;
        for (auto e : index.path(u))
        {
            ans += index.whole(e.cen).rangeSum(dl - e.distance, dr - e.distance);
            if (e.branch >= 0) ans -= index.part(e.cen, e.branch).rangeSum(dl - e.distance, dr - e.distance);
        }
        return ans;
    }
};

template <class T = long long> class PtGetRngAdd
{
    contour_dtl::Index index;
    vector<T> value;
public:
    PtGetRngAdd(const vector<vector<int>> &g, const vector<T> &a) : index(g), value(a) {}
    T get(int u) const
    {
        T ans = value[u];
        for (auto e : index.path(u))
        {
            ans += index.whole(e.cen).pval(e.distance);
            if (e.branch >= 0) ans -= index.part(e.cen, e.branch).pval(e.distance);
        }
        return ans;
    }
    void apply(int u, int dl, int dr, T delta)
    {
        for (auto e : index.path(u))
        {
            index.whole(e.cen).addRange(dl - e.distance, dr - e.distance, delta);
            if (e.branch >= 0) index.part(e.cen, e.branch).addRange(dl - e.distance, dr - e.distance, delta);
        }
    }
};
