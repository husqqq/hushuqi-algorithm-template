#include <bits/stdc++.h>

using namespace std;

vector<int> lyndonFactorization(const string& s) {
    int n = s.size();
    vector<int> boundaries{0};
    for (int i = 0; i < n;) {
        int j = i + 1;
        int k = i;
        while (j < n && s[k] <= s[j]) {
            if (s[k] < s[j]) {
                k = i;
            } else {
                k++;
            }
            j++;
        }
        int length = j - k;
        while (i <= k) {
            i += length;
            boundaries.push_back(i);
        }
    }
    return boundaries;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;
    auto answer = lyndonFactorization(s);
    for (int i = 0; i < (int)answer.size(); i++) {
        cout << answer[i] << " \n"[i + 1 == (int)answer.size()];
    }
    return 0;
}
