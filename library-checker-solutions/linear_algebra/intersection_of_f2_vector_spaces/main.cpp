#include <bits/stdc++.h>

using namespace std;

vector<int> intersectionBasis(const vector<int>& first, const vector<int>& second) {
    array<int, 30> value{}, tag{};
    auto insertRelation = [&](int x, int relation, vector<int>* dependencies) {
        for (int bit = 29; bit >= 0; bit--) {
            if (!(x >> bit & 1)) {
                continue;
            }
            if (value[bit]) {
                x ^= value[bit];
                relation ^= tag[bit];
            } else {
                value[bit] = x;
                tag[bit] = relation;
                return;
            }
        }
        if (relation && dependencies) {
            dependencies->push_back(relation);
        }
    };
    for (int x : first) {
        insertRelation(x, x, nullptr);
    }
    vector<int> candidates;
    for (int x : second) {
        insertRelation(x, 0, &candidates);
    }
    array<int, 30> basis{};
    for (int x : candidates) {
        int original = x;
        for (int bit = 29; bit >= 0; bit--) {
            if (!(x >> bit & 1)) continue;
            if (basis[bit]) x ^= basis[bit];
            else {
                basis[bit] = x;
                break;
            }
        }
    }
    vector<int> result;
    for (int bit = 29; bit >= 0; bit--) {
        if (basis[bit]) {
            result.push_back(basis[bit]);
        }
    }
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tests;
    cin >> tests;
    while (tests--) {
        int n, m;
        cin >> n;
        vector<int> first(n);
        for (int& x : first) cin >> x;
        cin >> m;
        vector<int> second(m);
        for (int& x : second) cin >> x;
        auto answer = intersectionBasis(first, second);
        cout << answer.size();
        for (int x : answer) cout << ' ' << x;
        cout << '\n';
    }
    return 0;
}
