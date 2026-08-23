#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<int> originalU(m), originalV(m), degree(n);
    for (int i = 0; i < m; i++) {
        cin >> originalU[i] >> originalV[i];
        degree[originalU[i]]++;
        degree[originalV[i]]++;
    }

    vector<int> vertexOrder(n);
    iota(vertexOrder.begin(), vertexOrder.end(), 0);
    stable_sort(vertexOrder.begin(), vertexOrder.end(), [&](int a, int b) { return degree[a] < degree[b]; });
    vector<int> rank(n);
    for (int i = 0; i < n; i++) {
        rank[vertexOrder[i]] = i;
    }

    vector<pair<int, int>> simpleEdges;
    vector<vector<int>> originalIds;
    map<pair<int, int>, int> edgeIndex;
    for (int id = 0; id < m; id++) {
        int u = rank[originalU[id]], v = rank[originalV[id]];
        if (u < v) {
            swap(u, v);
        }
        auto key = pair{u, v};
        auto [it, inserted] = edgeIndex.emplace(key, simpleEdges.size());
        if (inserted) {
            simpleEdges.push_back(key);
            originalIds.push_back({});
        }
        originalIds[it->second].push_back(id);
    }

    int edgeCount = simpleEdges.size();
    vector<long long> multiplicity(edgeCount);
    vector<vector<pair<int, int>>> adjacent(n);
    for (int id = 0; id < edgeCount; id++) {
        multiplicity[id] = originalIds[id].size();
        auto [u, v] = simpleEdges[id];
        adjacent[u].push_back({v, id});
        adjacent[v].push_back({u, id});
    }
    for (int u = 0; u < n; u++) {
        sort(adjacent[u].begin(), adjacent[u].end());
    }

    vector<long long> paths(n), answerSimple(edgeCount);
    for (int top = n - 1; top >= 0; top--) {
        for (auto [middle, firstEdge] : adjacent[top]) {
            if (middle >= top) {
                break;
            }
            int end =
                lower_bound(adjacent[middle].begin(), adjacent[middle].end(), pair{top, -1}) - adjacent[middle].begin();
            for (int index = 0; index < end; index++) {
                auto [bottom, secondEdge] = adjacent[middle][index];
                paths[bottom] += multiplicity[firstEdge] * multiplicity[secondEdge];
            }
        }
        for (auto [middle, firstEdge] : adjacent[top]) {
            if (middle >= top) {
                break;
            }
            int end =
                lower_bound(adjacent[middle].begin(), adjacent[middle].end(), pair{top, -1}) - adjacent[middle].begin();
            for (int index = 0; index < end; index++) {
                auto [bottom, secondEdge] = adjacent[middle][index];
                long long other = paths[bottom] - multiplicity[firstEdge] * multiplicity[secondEdge];
                answerSimple[firstEdge] += other * multiplicity[secondEdge];
                answerSimple[secondEdge] += other * multiplicity[firstEdge];
            }
        }
        for (auto [middle, firstEdge] : adjacent[top]) {
            if (middle >= top) {
                break;
            }
            int end =
                lower_bound(adjacent[middle].begin(), adjacent[middle].end(), pair{top, -1}) - adjacent[middle].begin();
            for (int index = 0; index < end; index++) {
                paths[adjacent[middle][index].first] = 0;
            }
        }
    }

    vector<long long> answer(m);
    for (int id = 0; id < edgeCount; id++) {
        for (int original : originalIds[id]) {
            answer[original] = answerSimple[id];
        }
    }
    for (int i = 0; i < m; i++) {
        cout << answer[i] << " \n"[i + 1 == m];
    }
    return 0;
}
