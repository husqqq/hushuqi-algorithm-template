#include <bits/stdc++.h>

using namespace std;

constexpr int P = 998244353;

long long power(long long a, int b) {
    long long result = 1;
    while (b) {
        if (b & 1) {
            result = result * a % P;
        }
        a = a * a % P;
        b >>= 1;
    }
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> sequence(n);
    for (auto& value : sequence) {
        cin >> value;
    }

    vector<long long> current(1, 1), previous(1, 1);
    int length = 0, shift = 1;
    long long lastDiscrepancy = 1;
    for (int i = 0; i < n; i++) {
        long long discrepancy = sequence[i];
        for (int j = 1; j <= length; j++) {
            discrepancy = (discrepancy + current[j] * sequence[i - j]) % P;
        }
        if (discrepancy == 0) {
            shift++;
            continue;
        }
        auto old = current;
        long long factor = discrepancy * power(lastDiscrepancy, P - 2) % P;
        if (current.size() < previous.size() + shift) {
            current.resize(previous.size() + shift);
        }
        for (int j = 0; j < int(previous.size()); j++) {
            current[j + shift] = (current[j + shift] - factor * previous[j]) % P;
            if (current[j + shift] < 0) {
                current[j + shift] += P;
            }
        }
        if (2 * length <= i) {
            length = i + 1 - length;
            previous = move(old);
            lastDiscrepancy = discrepancy;
            shift = 1;
        } else {
            shift++;
        }
    }
    current.resize(length + 1);
    cout << length << '\n';
    for (int i = 1; i <= length; i++) {
        cout << (current[i] ? P - current[i] : 0) << " \n"[i == length];
    }
    if (length == 0) {
        cout << '\n';
    }
    return 0;
}
