#include <bits/stdc++.h>

using namespace std;

constexpr int LOG = 20;

struct Node {
    int value = 0;
    array<int, LOG> parent{};
};

struct Version {
    int tail = 0;
    int size = 0;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;
    vector<Node> nodes(1);
    vector<Version> versions(q + 1);
    for (int i = 0; i < q; i++) {
        int op, t;
        cin >> op >> t;
        Version base = versions[t + 1];
        if (op == 0) {
            int x;
            cin >> x;
            Node node;
            node.value = x;
            node.parent[0] = base.tail;
            for (int k = 1; k < LOG; k++) {
                node.parent[k] = nodes[node.parent[k - 1]].parent[k - 1];
            }
            nodes.push_back(node);
            versions[i + 1] = {(int)nodes.size() - 1, base.size + 1};
        } else {
            int front = base.tail;
            int distance = base.size - 1;
            for (int k = 0; distance; k++, distance >>= 1) {
                if (distance & 1) {
                    front = nodes[front].parent[k];
                }
            }
            cout << nodes[front].value << '\n';
            versions[i + 1] = {base.tail, base.size - 1};
        }
    }
    return 0;
}
