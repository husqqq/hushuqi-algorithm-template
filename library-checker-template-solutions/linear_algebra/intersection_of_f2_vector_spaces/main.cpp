#include <bits/stdc++.h>
using namespace std;

inline vector<int> xorSpaceIntersection(const vector<int> &x, const vector<int> &y, int bits = 30)
{
    // x、y 分别是两个 F2 向量空间的独立基，bits 是位宽；返回交空间的一组独立基。
    int n = x.size(), m = y.size();
    vector<unsigned long long> row;
    for (int bit = 0; bit < bits; bit++)
    {
        unsigned long long r = 0;
        for (int i = 0; i < n; i++) if (x[i] >> bit & 1) r ^= 1ULL << i;
        for (int i = 0; i < m; i++) if (y[i] >> bit & 1) r ^= 1ULL << (n + i);
        row.push_back(r);
    }
    vector<int> pivot(n + m, -1);
    int rk = 0;
    for (int col = 0; col < n + m && rk < bits; col++)
    {
        int p = rk;
        while (p < bits && !(row[p] >> col & 1)) p++;
        if (p == bits) continue;
        swap(row[p], row[rk]);
        for (int i = 0; i < bits; i++) if (i != rk && (row[i] >> col & 1)) row[i] ^= row[rk];
        pivot[col] = rk++;
    }
    vector<int> ans;
    for (int free = n; free < n + m; free++) if (pivot[free] == -1)
    {
        unsigned long long coef = 1ULL << free;
        for (int col = 0; col < n + m; col++) if (pivot[col] != -1 && (row[pivot[col]] >> free & 1)) coef ^= 1ULL << col;
        int v = 0;
        for (int i = 0; i < n; i++) if (coef >> i & 1) v ^= x[i];
        if (v) ans.push_back(v);
    }
    return ans;
}

signed main()
{
    int t; cin >> t; while (t--)
    {
        int n, m; cin >> n; vector<int> a(n); for (int &x : a) cin >> x; cin >> m; vector<int> b(m); for (int &x : b) cin >> x;
        auto c = xorSpaceIntersection(a, b); cout << c.size(); for (int x : c) cout << ' ' << x; cout << '\n';
    }
}
