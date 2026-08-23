#include "../include/full/7_3_013.hpp"

signed main()
{
    const int N = 2000000;
    vector<char> ok(N + 1, true);
    ok[0] = false;
    for (int p = 2; p * p <= N; p++)
    {
        int q = p * p;
        for (int x = q; x <= N; x += q)
        {
            ok[x] = false;
        }
    }
    vector<int> pre(N + 1);
    for (int i = 1; i <= N; i++)
    {
        pre[i] = pre[i - 1] + ok[i];
    }
    mt19937_64 rng(0);
    unsigned long long agg = 0;
    for (int tc = 0; tc < 2000; tc++)
    {
        int n = rng() % (N + 1);
        long long got = countSqFree(n);
        assert(got == pre[n]);
        agg = agg * 1000000007ULL + got;
    }
    long long big = countSqFree(1000000000000000000LL);
    assert(big == 607927101854022750LL);
    cout << agg << ' ' << big << '\n';
    return 0;
}
