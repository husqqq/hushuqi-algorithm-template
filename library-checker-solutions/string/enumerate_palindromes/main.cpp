#include <bits/stdc++.h>

using namespace std;

pair<vector<int>, vector<int>> manacher(const string& s) {
    int n = s.size();
    vector<int> odd(n), even(n);

    for (int i = 0, l = 0, r = -1; i < n; i++) {
        int k = i > r ? 1 : min(odd[l + r - i], r - i + 1);
        while (0 <= i - k && i + k < n && s[i - k] == s[i + k]) {
            k++;
        }
        odd[i] = k;
        if (i + k - 1 > r) {
            l = i - k + 1;
            r = i + k - 1;
        }
    }

    for (int i = 0, l = 0, r = -1; i < n; i++) {
        int k = i > r ? 0 : min(even[l + r - i + 1], r - i + 1);
        while (0 <= i - k - 1 && i + k < n && s[i - k - 1] == s[i + k]) {
            k++;
        }
        even[i] = k;
        if (i + k - 1 > r) {
            l = i - k;
            r = i + k - 1;
        }
    }
    return {odd, even};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;
    auto [odd, even] = manacher(s);
    int n = s.size();
    for (int center = 0; center < 2 * n - 1; center++) {
        int length;
        if (center % 2 == 0) {
            length = 2 * odd[center / 2] - 1;
        } else {
            length = 2 * even[center / 2 + 1];
        }
        cout << length << " \n"[center + 1 == 2 * n - 1];
    }
    return 0;
}
