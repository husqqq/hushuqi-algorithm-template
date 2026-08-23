#include <bits/stdc++.h>

using namespace std;

constexpr int P = 998244353;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<int> value(n), degree(n);
    for (auto& x : value) {
        cin >> x;
    }
    vector<pair<int, int>> edges(m);
    for (auto& [u, v] : edges) {
        cin >> u >> v;
        degree[u]++;
        degree[v]++;
    }
    vector<vector<int>> out(n);
    for (auto [u, v] : edges) {
        if (pair(degree[u], u) > pair(degree[v], v)) {
            swap(u, v);
        }
        out[u].push_back(v);
    }

    vector<int> marked(n, -1);
    int answer = 0;
    for (int u = 0; u < n; u++) {
        for (int v : out[u]) {
            marked[v] = u;
        }
        for (int v : out[u]) {
            for (int w : out[v]) {
                if (marked[w] == u) {
                    answer = (answer + 1LL * value[u] * value[v] % P * value[w]) % P;
                }
            }
        }
    }
    cout << answer << '\n';
    return 0;
}
