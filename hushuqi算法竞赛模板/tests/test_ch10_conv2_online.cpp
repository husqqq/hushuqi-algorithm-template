#include "../include/full/10_1_005.hpp"
#include "../include/full/10_1_009.hpp"

signed main()
{
    vector<vector<Z>> a{{1, 2}, {3, 4}};
    vector<vector<Z>> b{{5, 6}, {7, 8}};
    auto c = conv2(a, b);
    assert((c == vector<vector<Z>>{{5, 16, 12}, {22, 60, 40}, {21, 52, 32}}));
    poly2::Mat ma(2, 2), mb(2, 2);
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            ma(i, j) = a[i][j];
            mb(i, j) = b[i][j];
        }
    }
    auto z = poly2::conv(ma, mb);
    auto p = poly2::conv(ma, ma, mb);
    assert(z.a == p[0].a && z.a == p[1].a);
    OnlineConv<long long> oc;
    vector<long long> x{1, 2, 3};
    vector<long long> y{4, 5, 6};
    for (int i = 0; i < 3; i++)
    {
        long long want = 0;
        for (int j = 0; j <= i; j++)
        {
            want += x[j] * y[i - j];
        }
        assert(oc.add(x[i], y[i]) == want);
    }
    mt19937_64 rng(0);
    OnlineNTT fast;
    vector<Z> f, g;
    for (int i = 0; i < 300; i++)
    {
        f.push_back(rng());
        g.push_back(rng());
        Z want = 0;
        for (int j = 0; j <= i; j++)
        {
            want += f[j] * g[i - j];
        }
        assert(fast.add(f.back(), g.back()) == want);
    }
    cout << "OK\n";
    return 0;
}
