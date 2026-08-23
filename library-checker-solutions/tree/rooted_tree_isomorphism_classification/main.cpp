#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<vector<int>> children(n);
    for (int i = 1; i < n; i++) {
        int parent;
        cin >> parent;
        children[parent].push_back(i);
    }

    map<vector<int>, int> typeId;
    vector<int> type(n);
    for (int u = n - 1; u >= 0; u--) {
        vector<int> signature;
        signature.reserve(children[u].size());
        for (int v : children[u]) {
            signature.push_back(type[v]);
        }
        sort(signature.begin(), signature.end());
        auto [it, inserted] = typeId.emplace(signature, typeId.size());
        type[u] = it->second;
    }

    cout << typeId.size() << '\n';
    for (int i = 0; i < n; i++) {
        cout << type[i] << " \n"[i + 1 == n];
    }
    return 0;
}
