#include <bits/stdc++.h>

using namespace std;

vector<int> zFunction(const string& s) {
    int n = s.size();
    vector<int> z(n);
    z[0] = n;
    for (int i = 1, l = 0, r = 0; i < n; i++) {
        if (i < r) {
            z[i] = min(r - i, z[i - l]);
        }
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
            z[i]++;
        }
        if (i + z[i] > r) {
            l = i;
            r = i + z[i];
        }
    }
    return z;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;
    auto z = zFunction(s);
    for (int i = 0; i < (int)z.size(); i++) {
        cout << z[i] << " \n"[i + 1 == (int)z.size()];
    }
    return 0;
}
