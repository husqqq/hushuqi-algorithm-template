#include <bits/stdc++.h>

using namespace std;

struct Node {
    int length = 0;
    int link = 0;
    int parent = 0;
    array<int, 26> next{};
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;
    vector<Node> nodes(2);
    nodes[0].length = -1;
    nodes[0].link = 0;
    nodes[1].length = 0;
    nodes[1].link = 0;

    auto extendable = [&](int node, int position) {
        int left = position - nodes[node].length - 1;
        return left >= 0 && s[left] == s[position];
    };

    int suffix = 1;
    vector<int> prefixSuffix;
    for (int position = 0; position < (int)s.size(); position++) {
        int current = suffix;
        while (!extendable(current, position)) {
            current = nodes[current].link;
        }
        int character = s[position] - 'a';
        if (nodes[current].next[character] == 0) {
            Node node;
            node.length = nodes[current].length + 2;
            node.parent = current;
            if (node.length == 1) {
                node.link = 1;
            } else {
                int link = nodes[current].link;
                while (!extendable(link, position)) {
                    link = nodes[link].link;
                }
                node.link = nodes[link].next[character];
            }
            nodes[current].next[character] = nodes.size();
            nodes.push_back(node);
        }
        suffix = nodes[current].next[character];
        prefixSuffix.push_back(suffix - 1);
    }

    cout << nodes.size() - 2 << '\n';
    for (int i = 2; i < (int)nodes.size(); i++) {
        cout << nodes[i].parent - 1 << ' ' << nodes[i].link - 1 << '\n';
    }
    for (int i = 0; i < (int)prefixSuffix.size(); i++) {
        cout << prefixSuffix[i] << " \n"[i + 1 == (int)prefixSuffix.size()];
    }
    return 0;
}
