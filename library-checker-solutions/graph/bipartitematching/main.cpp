#include <bits/stdc++.h>

using namespace std;

struct Dinic {
    struct Edge {
        int to, rev, cap;
    };

    int n;
    vector<vector<Edge>> adj;
    vector<int> level, current;

    Dinic(int n = 0) : n(n), adj(n), level(n), current(n) {}

    void addEdge(int u, int v, int cap) {
        adj[u].push_back({v, (int)adj[v].size(), cap});
        adj[v].push_back({u, (int)adj[u].size() - 1, 0});
    }

    bool bfs(int source, int sink) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[source] = 0;
        q.push(source);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto& e : adj[u]) {
                if (e.cap > 0 && level[e.to] == -1) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[sink] != -1;
    }

    int dfs(int u, int sink, int flow) {
        if (u == sink) {
            return flow;
        }
        for (int& i = current[u]; i < (int)adj[u].size(); i++) {
            auto& e = adj[u][i];
            if (e.cap > 0 && level[e.to] == level[u] + 1) {
                int pushed = dfs(e.to, sink, min(flow, e.cap));
                if (pushed) {
                    e.cap -= pushed;
                    adj[e.to][e.rev].cap += pushed;
                    return pushed;
                }
            }
        }
        return 0;
    }

    int maxFlow(int source, int sink) {
        int flow = 0;
        while (bfs(source, sink)) {
            fill(current.begin(), current.end(), 0);
            while (int pushed = dfs(source, sink, numeric_limits<int>::max())) {
                flow += pushed;
            }
        }
        return flow;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int l, r, m;
    cin >> l >> r >> m;
    int source = l + r;
    int sink = source + 1;
    Dinic flow(sink + 1);

    for (int i = 0; i < l; i++) {
        flow.addEdge(source, i, 1);
    }
    for (int i = 0; i < r; i++) {
        flow.addEdge(l + i, sink, 1);
    }
    while (m--) {
        int u, v;
        cin >> u >> v;
        flow.addEdge(u, l + v, 1);
    }

    cout << flow.maxFlow(source, sink) << '\n';
    for (int u = 0; u < l; u++) {
        for (auto& e : flow.adj[u]) {
            if (l <= e.to && e.to < l + r && e.cap == 0) {
                cout << u << ' ' << e.to - l << '\n';
                break;
            }
        }
    }
    return 0;
}
