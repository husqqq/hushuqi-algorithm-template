// Generated from the protected candidate template.hpp and main.cpp.in.

// Candidate optimization based on hushuqi算法竞赛模板 5.10.001.
// Keep this file in the QOJ candidate area until it is approved.

#include <bits/stdc++.h>
using namespace std;

struct ChordalCertificate
{
    bool ok;
    vector<int> peo;
    vector<int> cycle;
};

ChordalCertificate chordalCertificate(int n, const vector<pair<int, int>> &edges)
{
    // edges 是简单无向边；返回一组 PEO，或一条长度至少四的诱导环。
    assert(n >= 0);
    vector<int> degree(n);
    for (auto [u, v] : edges)
    {
        assert(0 <= u && u < n && 0 <= v && v < n && u != v);
        degree[u]++;
        degree[v]++;
    }
    vector<int> offset(n + 1), neighbors(2 * edges.size());
    for (int u = 0; u < n; u++) offset[u + 1] = offset[u] + degree[u];
    vector<int> cursor = offset;
    for (auto [u, v] : edges)
    {
        neighbors[cursor[u]++] = v;
        neighbors[cursor[v]++] = u;
    }

    vector<int> bucketHead(n + 1, -1), next(n, -1), previous(n, -1), weight(n);
    auto erase = [&](int u)
    {
        int w = weight[u];
        if (previous[u] == -1) bucketHead[w] = next[u];
        else next[previous[u]] = next[u];
        if (next[u] != -1) previous[next[u]] = previous[u];
    };
    auto insert = [&](int u)
    {
        int w = weight[u];
        previous[u] = -1;
        next[u] = bucketHead[w];
        if (bucketHead[w] != -1) previous[bucketHead[w]] = u;
        bucketHead[w] = u;
    };
    for (int u = 0; u < n; u++) insert(u);

    vector<int> selectionOrder;
    selectionOrder.reserve(n);
    vector<char> selected(n);
    int maximumWeight = 0;
    while ((int)selectionOrder.size() < n)
    {
        while (bucketHead[maximumWeight] == -1) maximumWeight--;
        int u = bucketHead[maximumWeight];
        erase(u);
        selected[u] = true;
        selectionOrder.push_back(u);
        for (int edge = offset[u]; edge < offset[u + 1]; edge++)
        {
            int v = neighbors[edge];
            if (selected[v]) continue;
            erase(v);
            weight[v]++;
            insert(v);
            maximumWeight = max(maximumWeight, weight[v]);
        }
    }

    vector<int> position(n), parent(n, -1), childHead(n, -1), childNext(n, -1);
    for (int i = 0; i < n; i++) position[selectionOrder[i]] = i;
    for (int u = 0; u < n; u++)
    {
        for (int edge = offset[u]; edge < offset[u + 1]; edge++)
        {
            int v = neighbors[edge];
            if (position[v] < position[u] &&
                (parent[u] == -1 || position[parent[u]] < position[v]))
            {
                parent[u] = v;
            }
        }
        if (parent[u] != -1)
        {
            childNext[u] = childHead[parent[u]];
            childHead[parent[u]] = u;
        }
    }

    vector<int> adjacentStamp(n, -1);
    int badVertex = -1, badParent = -1, badNeighbor = -1;
    for (int center = 0; center < n; center++)
    {
        for (int edge = offset[center]; edge < offset[center + 1]; edge++)
        {
            adjacentStamp[neighbors[edge]] = center;
        }
        for (int u = childHead[center]; u != -1; u = childNext[u])
        {
            for (int edge = offset[u]; edge < offset[u + 1]; edge++)
            {
                int v = neighbors[edge];
                if (position[v] >= position[center] || adjacentStamp[v] == center) continue;
                if (badVertex == -1 || position[u] > position[badVertex])
                {
                    badVertex = u;
                    badParent = center;
                    badNeighbor = v;
                }
                break;
            }
        }
    }
    if (badVertex == -1)
    {
        reverse(selectionOrder.begin(), selectionOrder.end());
        return {true, move(selectionOrder), {}};
    }

    vector<char> forbidden(n);
    for (int edge = offset[badVertex]; edge < offset[badVertex + 1]; edge++)
    {
        int v = neighbors[edge];
        if (position[v] < position[badVertex] && v != badNeighbor && v != badParent)
        {
            forbidden[v] = true;
        }
    }
    vector<int> from(n, -1), queue(n);
    int queueBegin = 0, queueEnd = 0;
    from[badNeighbor] = badNeighbor;
    queue[queueEnd++] = badNeighbor;
    while (queueBegin < queueEnd && from[badParent] == -1)
    {
        int u = queue[queueBegin++];
        for (int edge = offset[u]; edge < offset[u + 1]; edge++)
        {
            int v = neighbors[edge];
            if (position[v] >= position[badVertex] || forbidden[v] || from[v] != -1) continue;
            from[v] = u;
            queue[queueEnd++] = v;
        }
    }
    assert(from[badParent] != -1);
    vector<int> path;
    for (int u = badParent;; u = from[u])
    {
        path.push_back(u);
        if (u == badNeighbor) break;
    }
    reverse(path.begin(), path.end());
    vector<int> cycle{badVertex};
    cycle.insert(cycle.end(), path.begin(), path.end());
    return {false, {}, move(cycle)};
}
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<pair<int, int>> edges(m);
    for (auto &[u, v] : edges) cin >> u >> v;
    ChordalCertificate answer = chordalCertificate(n, edges);
    if (answer.ok)
    {
        cout << "YES\n";
        for (int i = 0; i < n; i++) cout << answer.peo[i] << " \n"[i + 1 == n];
    }
    else
    {
        cout << "NO\n" << answer.cycle.size() << '\n';
        for (int i = 0; i < (int)answer.cycle.size(); i++)
        {
            cout << answer.cycle[i] << " \n"[i + 1 == (int)answer.cycle.size()];
        }
    }
}
