#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, a, b;
    cin >> n >> a >> b;
    using u64 = unsigned long long;
    vector<u64> composite((n / 2 + 64) / 64);
    auto test = [&](int value) {
        int index = value / 2;
        return composite[index / 64] >> (index % 64) & 1;
    };
    auto set = [&](int value) {
        int index = value / 2;
        composite[index / 64] |= 1ULL << (index % 64);
    };
    for (int i = 3; 1LL * i * i <= n; i += 2) {
        if (test(i)) continue;
        for (long long j = 1LL * i * i; j <= n; j += 2 * i) set(j);
    }

    int count = n >= 2;
    for (int value = 3; value <= n; value += 2) count += !test(value);
    int selected = count <= b ? 0 : (count - 1 - b) / a + 1;
    cout << count << ' ' << selected << '\n';

    int index = 0, printed = 0;
    auto output = [&](int prime) {
        if (index >= b && (index - b) % a == 0) {
            cout << prime << " \n"[++printed == selected];
        }
        index++;
    };
    if (n >= 2) output(2);
    for (int value = 3; value <= n; value += 2) {
        if (!test(value)) output(value);
    }
    if (!selected) cout << '\n';
    return 0;
}
