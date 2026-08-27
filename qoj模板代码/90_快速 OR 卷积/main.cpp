#include <bits/stdc++.h>
using namespace std;
constexpr int mod = 998244353;
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int k;
    cin >> k;
    int n = 1 << k;
    vector<long long> a(n), b(n);
    for (auto &x : a) cin >> x;
    for (auto &x : b) cin >> x;
    for (int bit = 1; bit < n; bit <<= 1)
        for (int mask = 0; mask < n; mask++) if (mask & bit)
        {
            a[mask] += a[mask ^ bit]; if (a[mask] >= mod) a[mask] -= mod;
            b[mask] += b[mask ^ bit]; if (b[mask] >= mod) b[mask] -= mod;
        }
    for (int i = 0; i < n; i++) a[i] = a[i] * b[i] % mod;
    for (int bit = 1; bit < n; bit <<= 1)
        for (int mask = 0; mask < n; mask++) if (mask & bit)
        {
            a[mask] -= a[mask ^ bit]; if (a[mask] < 0) a[mask] += mod;
        }
    for (int i = 0; i < n; i++) cout << a[i] << " \n"[i + 1 == n];
}
