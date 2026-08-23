#include "../include/full/10_2_014.hpp"

signed main()
{
    constexpr int n = 1000000;
    constexpr int small = 4096;
    constexpr long long k = 123456789;
    vector<pair<int, Z>> f;
    for (int i = 0; i < 10; i++)
    {
        f.push_back({i * 97, Z(i + 1)});
    }

    Poly a(small);
    for (auto [i, x] : f)
    {
        if (i < small)
        {
            a[i] = x;
        }
    }
    if (sparsePow(small, f, k) != fpsPow(a, k, small))
    {
        cerr << "benchmark result mismatch\n";
        return 1;
    }

    auto start = chrono::steady_clock::now();
    auto ans = sparsePow(n, f, k);
    auto elapsed = chrono::duration<double, milli>(chrono::steady_clock::now() - start).count();
    unsigned long long sum = 0;
    for (int i = 0; i < n; i += 997)
    {
        sum = sum * 1000003 + ans[i].val();
    }
    cout << "sparsePow n=" << n << " terms=" << f.size()
         << " exponent=" << k << ": " << elapsed << " ms checksum=" << sum << '\n';
    return 0;
}
