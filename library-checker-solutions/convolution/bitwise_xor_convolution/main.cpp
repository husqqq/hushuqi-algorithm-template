#include <bits/stdc++.h>

using namespace std;

constexpr int P = 998244353;
constexpr int INV2 = (P + 1) / 2;

void fwt(vector<int>& a, bool inverse) {
    int n = a.size();
    for (int length = 1; 2 * length <= n; length *= 2) {
        for (int i = 0; i < n; i += 2 * length) {
            for (int j = 0; j < length; j++) {
                int x = a[i + j];
                int y = a[i + j + length];
                a[i + j] = x + y < P ? x + y : x + y - P;
                a[i + j + length] = x - y >= 0 ? x - y : x - y + P;
                if (inverse) {
                    a[i + j] = 1LL * a[i + j] * INV2 % P;
                    a[i + j + length] = 1LL * a[i + j + length] * INV2 % P;
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
    fwt(a, false);
    fwt(b, false);
    for (int i = 0; i < size; i++) {
        a[i] = 1LL * a[i] * b[i] % P;
    }
    fwt(a, true);
    for (int i = 0; i < size; i++) {
        cout << a[i] << " \n"[i + 1 == size];
    }
    return 0;
}
