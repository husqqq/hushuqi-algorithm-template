#include <bits/stdc++.h>

using namespace std;

struct Edge {
    int left, right, original;
};

struct DSU {
    vector<int> parent;

    DSU(int n) : parent(n, -1) {}

    int leader(int x) { return parent[x] < 0 ? x : parent[x] = leader(parent[x]); }

    int merge(int x, int y) {
        x = leader(x);
        y = leader(y);
        if (parent[x] > parent[y]) {
            swap(x, y);
        }
        parent[x] += parent[y];
        parent[y] = x;
        return x;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int leftSize, rightSize, m;
    cin >> leftSize >> rightSize >> m;
    vector<pair<int, int>> input(m);
    vector<int> inputLeftDegree(leftSize), inputRightDegree(rightSize);
    for (int i = 0; i < m; i++) {
        int left, right;
        cin >> left >> right;
        input[i] = {left, right};
        inputLeftDegree[left]++;
        inputRightDegree[right]++;
    }
    int colors = 0;
    for (int value : inputLeftDegree) {
        colors = max(colors, value);
    }
    for (int value : inputRightDegree) {
        colors = max(colors, value);
    }

    // Merge low-degree vertices on the same side before regularization.
    // Then the regular supergraph has only O(m) edges even for a huge star.
    auto compress = [&](const vector<int>& degree) {
        int n = degree.size();
        DSU dsu(n);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> queue;
        for (int i = 0; i < n; i++) {
            queue.push({degree[i], i});
        }
        while (queue.size() > 1) {
            auto [firstDegree, first] = queue.top();
            queue.pop();
            auto [secondDegree, second] = queue.top();
            queue.pop();
            if (firstDegree + secondDegree > colors) {
                break;
            }
            int root = dsu.merge(first, second);
            queue.push({firstDegree + secondDegree, root});
        }
        vector<int> roots;
        for (int i = 0; i < n; i++) {
            if (dsu.leader(i) == i) {
                roots.push_back(i);
            }
        }
        vector<int> index(n);
        for (int i = 0; i < int(roots.size()); i++) {
            index[roots[i]] = i;
        }
        vector<int> result(n);
        for (int i = 0; i < n; i++) {
            result[i] = index[dsu.leader(i)];
        }
        return pair{result, int(roots.size())};
    };
    auto [leftId, compressedLeft] = compress(inputLeftDegree);
    auto [rightId, compressedRight] = compress(inputRightDegree);
    int vertices = max(compressedLeft, compressedRight);
    vector<Edge> allEdges;
    allEdges.reserve(max(1, 2 * m + 2 * vertices));
    vector<int> leftDegree(vertices), rightDegree(vertices);
    for (int i = 0; i < m; i++) {
        auto [left, right] = input[i];
        left = leftId[left];
        right = rightId[right];
        allEdges.push_back({left, right, i});
        leftDegree[left]++;
        rightDegree[right]++;
    }
    int left = 0, right = 0;
    while (left < vertices && right < vertices) {
        while (left < vertices && leftDegree[left] == colors) {
            left++;
        }
        while (right < vertices && rightDegree[right] == colors) {
            right++;
        }
        if (left == vertices || right == vertices) {
            break;
        }
        int count = min(colors - leftDegree[left], colors - rightDegree[right]);
        for (int i = 0; i < count; i++) {
            allEdges.push_back({left, right, -1});
        }
        leftDegree[left] += count;
        rightDegree[right] += count;
    }

    vector<int> answer(m);
    auto perfectMatching = [&](const vector<int>& edgeIds) {
        vector<vector<int>> adjacent(vertices);
        for (int id : edgeIds) {
            adjacent[allEdges[id].left].push_back(id);
        }
        vector<int> matchLeft(vertices, -1), matchRight(vertices, -1), distance(vertices), matchedEdges;
        while (true) {
            queue<int> queue;
            fill(distance.begin(), distance.end(), -1);
            for (int u = 0; u < vertices; u++) {
                if (matchLeft[u] == -1) {
                    distance[u] = 0, queue.push(u);
                }
            }
            while (!queue.empty()) {
                int u = queue.front();
                queue.pop();
                for (int id : adjacent[u]) {
                    int v = allEdges[id].right;
                    int next = matchRight[v];
                    if (next != -1 && distance[next] == -1) {
                        distance[next] = distance[u] + 1, queue.push(next);
                    }
                }
            }
            auto dfs = [&](auto&& self, int u) -> bool {
                for (int id : adjacent[u]) {
                    int v = allEdges[id].right;
                    int next = matchRight[v];
                    if (next == -1 || (distance[next] == distance[u] + 1 && self(self, next))) {
                        matchLeft[u] = id;
                        matchRight[v] = u;
                        return true;
                    }
                }
                distance[u] = -1;
                return false;
            };
            int augmented = 0;
            for (int u = 0; u < vertices; u++) {
                if (matchLeft[u] == -1 && dfs(dfs, u)) {
                    augmented++;
                }
            }
            if (!augmented) {
                break;
            }
        }
        for (int id : matchLeft) {
            matchedEdges.push_back(id);
        }
        return matchedEdges;
    };

    auto color = [&](auto&& self, vector<int> edgeIds, int degree, int firstColor) -> void {
        if (degree == 0) {
            return;
        }
        if (degree == 1) {
            for (int id : edgeIds) {
                if (allEdges[id].original != -1) {
                    answer[allEdges[id].original] = firstColor;
                }
            }
            return;
        }
        if (degree & 1) {
            auto matching = perfectMatching(edgeIds);
            vector<char> selected(allEdges.size());
            for (int id : matching) {
                selected[id] = true;
                if (allEdges[id].original != -1) {
                    answer[allEdges[id].original] = firstColor;
                }
            }
            vector<int> remaining;
            remaining.reserve(edgeIds.size() - matching.size());
            for (int id : edgeIds) {
                if (!selected[id]) {
                    remaining.push_back(id);
                }
            }
            self(self, move(remaining), degree - 1, firstColor + 1);
            return;
        }
        vector<vector<pair<int, int>>> graph(2 * vertices);
        for (int id : edgeIds) {
            int u = allEdges[id].left;
            int v = vertices + allEdges[id].right;
            graph[u].push_back({v, id});
            graph[v].push_back({u, id});
        }
        vector<char> used(allEdges.size());
        vector<int> pointer(2 * vertices), firstHalf, secondHalf;
        for (int start = 0; start < 2 * vertices; start++) {
            while (pointer[start] < int(graph[start].size()) && used[graph[start][pointer[start]].second]) {
                pointer[start]++;
            }
            if (pointer[start] == int(graph[start].size())) {
                continue;
            }
            vector<pair<int, int>> stack{{start, -1}};
            vector<int> tour;
            while (!stack.empty()) {
                int u = stack.back().first;
                while (pointer[u] < int(graph[u].size()) && used[graph[u][pointer[u]].second]) {
                    pointer[u]++;
                }
                if (pointer[u] == int(graph[u].size())) {
                    int edge = stack.back().second;
                    stack.pop_back();
                    if (edge != -1) {
                        tour.push_back(edge);
                    }
                } else {
                    auto [v, edge] = graph[u][pointer[u]++];
                    if (!used[edge]) {
                        used[edge] = true;
                        stack.push_back({v, edge});
                    }
                }
            }
            for (int i = 0; i < int(tour.size()); i++) {
                (i & 1 ? secondHalf : firstHalf).push_back(tour[i]);
            }
        }
        self(self, move(firstHalf), degree / 2, firstColor);
        self(self, move(secondHalf), degree / 2, firstColor + degree / 2);
    };
    vector<int> ids(allEdges.size());
    iota(ids.begin(), ids.end(), 0);
    color(color, move(ids), colors, 0);
    cout << colors << '\n';
    for (int value : answer) {
        cout << value << '\n';
    }
    return 0;
}
