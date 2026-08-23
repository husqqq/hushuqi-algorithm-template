#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> a(n), tail;
    vector<int> position, parent(n, -1);
    for (auto& x : a) {
        cin >> x;
    }

    for (int i = 0; i < n; i++) {
        int length = lower_bound(tail.begin(), tail.end(), a[i]) - tail.begin();
        if (length == (int)tail.size()) {
            tail.push_back(a[i]);
            position.push_back(i);
        } else {
            tail[length] = a[i];
            position[length] = i;
        }
        if (length > 0) {
            parent[i] = position[length - 1];
        }
    }

    vector<int> answer;
    for (int u = position.back(); u != -1; u = parent[u]) {
        answer.push_back(u);
    }
    reverse(answer.begin(), answer.end());

    cout << answer.size() << '\n';
    for (int i = 0; i < (int)answer.size(); i++) {
        cout << answer[i] << " \n"[i + 1 == (int)answer.size()];
    }
    return 0;
}
