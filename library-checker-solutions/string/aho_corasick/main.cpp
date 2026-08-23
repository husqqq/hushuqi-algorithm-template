#ifndef TEMPLATE_INLINE_1EB4B5908578DC435AE4
#define TEMPLATE_INLINE_1EB4B5908578DC435AE4

#include <bits/stdc++.h>

using namespace std;

template <int Alphabet = 26> class AhoCorasick {
  public:
    struct Node {
        array<int, Alphabet> child, next;
        int parent = -1, suffixLink = 0;
        Node() { child.fill(-1), next.fill(0); }
    };

  private:
    vector<Node> nodes{1};
    vector<int> endpoints;

  public:
    int add(const string& pattern, char first = 'a') {
        int node = 0;
        for (char character : pattern) {
            int symbol = character - first;
            if (nodes[node].child[symbol] == -1) {
                nodes[node].child[symbol] = nodes.size();
                nodes.push_back({});
                nodes.back().parent = node;
            }
            node = nodes[node].child[symbol];
        }
        endpoints.push_back(node);
        return node;
    }

    void build() {
        queue<int> queue;
        for (int symbol = 0; symbol < Alphabet; ++symbol) {
            int child = nodes[0].child[symbol];
            if (child == -1) continue;
            nodes[0].next[symbol] = child;
            queue.push(child);
        }
        while (!queue.empty()) {
            int node = queue.front();
            queue.pop();
            nodes[node].next = nodes[nodes[node].suffixLink].next;
            for (int symbol = 0; symbol < Alphabet; ++symbol) {
                int child = nodes[node].child[symbol];
                if (child == -1) continue;
                nodes[child].suffixLink = nodes[node].next[symbol];
                nodes[node].next[symbol] = child;
                queue.push(child);
            }
        }
    }

    int transition(int node, int symbol) const { return nodes[node].next[symbol]; }
    const vector<Node>& getNodes() const { return nodes; }
    const vector<int>& getEndpoints() const { return endpoints; }
};
#endif


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    AhoCorasick automaton;
    for (int i = 0; i < n; ++i) {
        string pattern;
        cin >> pattern;
        automaton.add(pattern);
    }
    automaton.build();
    const auto& nodes = automaton.getNodes();
    cout << nodes.size() << '\n';
    for (int node = 1; node < nodes.size(); ++node) {
        cout << nodes[node].parent << ' ' << nodes[node].suffixLink << '\n';
    }
    const auto& endpoints = automaton.getEndpoints();
    for (int i = 0; i < endpoints.size(); ++i) cout << endpoints[i] << " \n"[i + 1 == endpoints.size()];
}
