#include <bits/stdc++.h>

using namespace std;

template <int LOG = 30> struct BinaryTrie {
    struct Node {
        array<int, 2> next{-1, -1};
        int count = 0;
    };

    vector<Node> nodes{1};

    int count(int x) const {
        int p = 0;
        for (int bit = LOG - 1; bit >= 0; bit--) {
            p = nodes[p].next[x >> bit & 1];
            if (p == -1) {
                return 0;
            }
        }
        return nodes[p].count;
    }

    void add(int x, int value) {
        int p = 0;
        nodes[p].count += value;
        for (int bit = LOG - 1; bit >= 0; bit--) {
            int c = x >> bit & 1;
            if (nodes[p].next[c] == -1) {
                nodes[p].next[c] = nodes.size();
                nodes.emplace_back();
            }
            p = nodes[p].next[c];
            nodes[p].count += value;
        }
    }

    int minXor(int x) const {
        int p = 0;
        int answer = 0;
        for (int bit = LOG - 1; bit >= 0; bit--) {
            int c = x >> bit & 1;
            int same = nodes[p].next[c];
            if (same != -1 && nodes[same].count > 0) {
                p = same;
            } else {
                p = nodes[p].next[c ^ 1];
                answer |= 1 << bit;
            }
        }
        return answer;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;
    BinaryTrie trie;
    while (q--) {
        int op, x;
        cin >> op >> x;
        if (op == 0) {
            if (trie.count(x) == 0) {
                trie.add(x, 1);
            }
        } else if (op == 1) {
            if (trie.count(x) != 0) {
                trie.add(x, -1);
            }
        } else {
            cout << trie.minXor(x) << '\n';
        }
    }
    return 0;
}
