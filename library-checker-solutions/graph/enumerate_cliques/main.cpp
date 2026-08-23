#include <bits/stdc++.h>

using namespace std;

constexpr long long P = 998244353;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<long long> weight(n);
    for (auto& value : weight) {
        cin >> value;
    }
    vector<vector<int>> adjacent(n);
    vector<vector<char>> connected(n, vector<char>(n));
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adjacent[u].push_back(v);
        adjacent[v].push_back(u);
        connected[u][v] = connected[v][u] = true;
    }

    int limit = sqrt(2.0 * m) + 1;
    vector<char> active(n, true);
    auto solve = [&](auto&& self, int remaining) -> long long {
        if (remaining == 0) {
            return 0;
        }
        int selected = -1;
        for (int i = 0; i < n; i++) {
            if (active[i] && int(adjacent[i].size()) < limit) {
                selected = i;
                break;
            }
        }
        if (selected == -1) {
            vector<int> vertices;
            for (int i = 0; i < n; i++) {
                if (active[i]) {
                    vertices.push_back(i);
                }
            }
            long long answer = 0;
            for (int mask = 1; mask < (1 << vertices.size()); mask++) {
                bool clique = true;
                long long product = 1;
                for (int i = 0; i < int(vertices.size()) && clique; i++) {
                    if (!(mask >> i & 1)) {
                        continue;
                    }
                    product = product * weight[vertices[i]] % P;
                    for (int j = 0; j < i; j++) {
                        if ((mask >> j & 1) && !connected[vertices[i]][vertices[j]]) {
                            clique = false;
                            break;
                        }
                    }
                }
                if (clique) {
                    answer += product;
                    answer %= P;
                }
            }
            return answer;
        }

        long long answer = 0;
        int degree = adjacent[selected].size();
        for (int mask = 0; mask < (1 << degree); mask++) {
            bool clique = true;
            long long product = weight[selected];
            for (int i = 0; i < degree && clique; i++) {
                if (!(mask >> i & 1)) {
                    continue;
                }
                int u = adjacent[selected][i];
                if (!active[u]) {
                    clique = false;
                    break;
                }
                product = product * weight[u] % P;
                for (int j = 0; j < i; j++) {
                    if ((mask >> j & 1) && !connected[u][adjacent[selected][j]]) {
                        clique = false;
                        break;
                    }
                }
            }
            if (clique) {
                answer += product;
                answer %= P;
            }
        }
        active[selected] = false;
        answer += self(self, remaining - 1);
        return answer % P;
    };

    cout << solve(solve, n) << '\n';
    return 0;
}
