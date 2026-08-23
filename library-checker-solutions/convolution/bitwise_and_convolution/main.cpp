#include <bits/stdc++.h>

using namespace std;

constexpr int P = 998244353;

void transform(vector<int>& a, bool inverse) {
    int n = a.size();
    for (int length = 1; 2 * length <= n; length *= 2) {
        for (int i = 0; i < n; i += 2 * length) {
            for (int j = 0; j < length; j++) {
                int& low = a[i + j];
                int high = a[i + j + length];
                if (!inverse) {
                    low += high;
                    if (low >= P) {
                        low -= P;
                    }
                } else {
                    low -= high;
                    if (low < 0) {
                        low += P;
                    }
                }
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    int size = 1 << n;
    vector<int> a(size), b(size);
    for (auto& x : a) {
        cin >> x;
    }
    for (auto& x : b) {
        cin >> x;
    }
    transform(a, false);
    transform(b, false);
    for (int i = 0; i < size; i++) {
        a[i] = 1LL * a[i] * b[i] % P;
    }
    transform(a, true);
    for (int i = 0; i < size; i++) {
        cout << a[i] << " \n"[i + 1 == size];
    }
    return 0;
}
