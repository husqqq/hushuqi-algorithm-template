#include "../include/full/10_2_016.hpp"

vector<Z> bruteEval(const vector<Z> &f, Z a, Z r, int m)
{
    vector<Z> ans(m);
    Z x = a;
    for (int i = 0; i < m; i++)
    {
        Z pw = 1;
        for (auto c : f)
        {
            ans[i] += c * pw;
            pw *= x;
        }
        x *= r;
    }
    return ans;
}

signed main()
{
    mt19937_64 rng(0);
    unsigned long long agg = 0;
    for (int tc = 0; tc < 1000; tc++)
    {
        int n = rng() % 160 + 1, m = rng() % 180;
        vector<Z> f(n);
        for (auto &x : f)
        {
            x = rng() % mod;
        }
        Z a = rng() % (mod - 1) + 1;
        Z r = rng() % (mod - 2) + 2;
        auto y = geomEval(f, a, r, m);
        assert(y == bruteEval(f, a, r, m));
        auto sample = geomEval(f, a, r, n);
        auto got = geomInterp(sample, a, r);
        assert(got == f);
        for (auto x : y)
        {
            agg = agg * 1000000007ULL + x.val();
        }
    }
    vector<Z> f{1, 2, 3};
    assert(geomEval(f, 7, 0, 4) == vector<Z>({162, 1, 1, 1}));
    cout << agg << '\n';
    return 0;
}
