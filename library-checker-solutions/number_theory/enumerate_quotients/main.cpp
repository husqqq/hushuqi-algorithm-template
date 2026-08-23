#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    i64 n;
    cin >> n;
    vector<i64> answer;
    for (i64 l = 1, r; l <= n; l = r + 1) {
        i64 value = n / l;
        r = n / value;
        answer.push_back(value);
    }
    reverse(answer.begin(), answer.end());
    cout << answer.size() << '\n';
    for (int i = 0; i < (int)answer.size(); i++) {
        cout << answer[i] << " \n"[i + 1 == (int)answer.size()];
    }
    return 0;
}
