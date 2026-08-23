#include <bits/stdc++.h>

using namespace std;

template <class T> vector<int> cartesianTree(const vector<T>& a) {
    int n = a.size();
    vector<int> parent(n, -1), stack;
    stack.reserve(n);

    for (int i = 0; i < n; i++) {
        int last = -1;
        while (!stack.empty() && a[i] < a[stack.back()]) {
            last = stack.back();
            stack.pop_back();
        }
        if (!stack.empty()) {
            parent[i] = stack.back();
        }
        if (last != -1) {
            parent[last] = i;
        }
        stack.push_back(i);
    }

    int root = stack.front();
    parent[root] = root;
    return parent;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> a(n);
    for (auto& x : a) {
        cin >> x;
    }

    auto parent = cartesianTree(a);
    for (int i = 0; i < n; i++) {
        cout << parent[i] << " \n"[i + 1 == n];
    }
    return 0;
}
