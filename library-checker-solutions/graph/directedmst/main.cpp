#include <bits/stdc++.h>

using namespace std;

struct Edge {
    int from, to;
    long long weight;
};

struct MeldableHeap {
    struct Node;
    using Pointer = shared_ptr<Node>;
    struct Node {
        Edge edge;
        long long lazy = 0;
        Pointer child, next;
        Node(Edge edge) : edge(edge) {}
    };

    Pointer root;
    int size = 0;

    Pointer merge(Pointer a, Pointer b) {
        if (!a) {
            return b;
        }
        if (!b) {
            return a;
        }
        if (a->edge.weight + a->lazy > b->edge.weight + b->lazy) {
            swap(a, b);
        }
        b->lazy -= a->lazy;
        b->next = a->child;
        a->child = b;
        return a;
    }

    void push(Edge edge) {
        root = merge(root, make_shared<Node>(edge));
        size++;
    }

    Edge top() const { return root->edge; }

    void pop() {
        vector<Pointer> trees;
        for (Pointer node = root->child; node;) {
            Pointer first = node;
            node = node->next;
            first->next = nullptr;
            first->lazy += root->lazy;
            Pointer second;
            if (node) {
                second = node;
                node = node->next;
                second->next = nullptr;
                second->lazy += root->lazy;
            }
            trees.push_back(merge(first, second));
        }
        root = nullptr;
        for (int i = int(trees.size()) - 1; i >= 0; i--) {
            root = merge(root, trees[i]);
        }
        size--;
    }

    void meld(MeldableHeap& other) {
        root = merge(root, other.root);
        size += other.size;
        other.root = nullptr;
        other.size = 0;
    }

    void setOffset(long long value) { root->lazy = value; }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, source;
    cin >> n >> m >> source;
    vector<MeldableHeap> heap(2 * n);
    for (int i = 0; i < m; i++) {
        Edge edge;
        cin >> edge.from >> edge.to >> edge.weight;
        heap[edge.to].push(edge);
    }

    vector<int> contractionParent(2 * n, -1), dsu(2 * n, -1);
    auto find = [&](int u) {
        int root = u;
        while (dsu[root] != -1) {
            root = dsu[root];
        }
        while (u != root) {
            int next = dsu[u];
            dsu[u] = root;
            u = next;
        }
        return root;
    };

    vector<int> visit(2 * n);
    vector<Edge> selected(2 * n);
    int components = n;
    visit[source] = 1;
    int iteration = 1;
    for (int start = 0; start < n; start++) {
        if (visit[start]) {
            continue;
        }
        iteration++;
        int u = start;
        while (visit[u] == 0 || visit[u] == iteration) {
            if (visit[u] == iteration) {
                int contracted = components++;
                int v = u;
                do {
                    heap[v].setOffset(-selected[v].weight);
                    heap[contracted].meld(heap[v]);
                    contractionParent[v] = dsu[v] = contracted;
                    v = find(selected[v].from);
                } while (v != contracted);
                u = contracted;
            }
            visit[u] = iteration;
            while (heap[u].size && find(heap[u].top().from) == u) {
                heap[u].pop();
            }
            selected[u] = heap[u].top();
            u = find(selected[u].from);
        }
    }

    vector<char> used(components);
    vector<Edge> answerEdge(n);
    long long answer = 0;
    for (int i = components - 1; i >= 0; i--) {
        if (i == source || used[i]) {
            continue;
        }
        int u = selected[i].to;
        while (u != -1 && !used[u]) {
            used[u] = true;
            u = contractionParent[u];
        }
        answer += selected[i].weight;
        answerEdge[selected[i].to] = selected[i];
    }
    cout << answer << '\n';
    for (int i = 0; i < n; i++) {
        cout << (i == source ? source : answerEdge[i].from) << " \n"[i + 1 == n];
    }
    return 0;
}
