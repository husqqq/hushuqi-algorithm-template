#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<int> adjacent(n);
    while (m--) {
        int u, v;
        cin >> u >> v;
        adjacent[u] |= 1 << v;
        adjacent[v] |= 1 << u;
    }

    int size = 1 << n;
    vector<char> independent(size);
    independent[0] = true;
    for (int mask = 1; mask < size; mask++) {
        int v = countr_zero(unsigned(mask));
        int rest = mask ^ (1 << v);
        independent[mask] = independent[rest] && !(adjacent[v] & rest);
    }
    vector<int> answer(size, n);
    answer[0] = 0;
    for (int mask = 1; mask < size; mask++) {
        if (independent[mask]) {
            answer[mask] = 1;
            continue;
        }
        int vertex = countr_zero(unsigned(mask));
        for (int subset = mask; subset; subset = (subset - 1) & mask) {
            if ((subset >> vertex & 1) && independent[subset]) {
                answer[mask] = min(answer[mask], answer[mask ^ subset] + 1);
            }
        }
    }
    cout << answer.back() << '\n';
    return 0;
}
