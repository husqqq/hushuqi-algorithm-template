#include <bits/stdc++.h>

using namespace std;

using i128 = __int128_t;

i128 readInt128() {
    string s;
    cin >> s;
    bool negative = s[0] == '-';
    i128 value = 0;
    for (int i = negative; i < (int)s.size(); i++) {
        value = value * 10 + s[i] - '0';
    }
    return negative ? -value : value;
}

void printInt128(i128 value) {
    if (value == 0) {
        cout << 0 << '\n';
        return;
    }
    if (value < 0) {
        cout << '-';
        value = -value;
    }
    string s;
    while (value) {
        s.push_back(char('0' + value % 10));
        value /= 10;
    }
    reverse(s.begin(), s.end());
    cout << s << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tests;
    cin >> tests;
    while (tests--) {
        i128 a = readInt128();
        i128 b = readInt128();
        printInt128(a + b);
    }
    return 0;
}
