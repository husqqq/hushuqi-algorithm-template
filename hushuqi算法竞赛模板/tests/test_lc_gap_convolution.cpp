#include "../include/full/10_1_017.hpp"
#include "../include/full/10_1_018.hpp"

signed main()
{
    vector<Z> a{1, 2, 3, 4, 5}, b{6, 7, 8, 9, 10};
    auto c = mulPrimeConv(5, a, b);
    vector<Z> want(5);
    for (int i = 0; i < 5; i++) for (int j = 0; j < 5; j++) want[i * j % 5] += a[i] * b[j];
    assert(c == want);
    mt19937_64 rng(3);
    for (int z = 0; z < 30; z++)
    {
        int n = rng() % 40 + 1, m = rng() % 40 + 1;
        vector<unsigned long long> x(n), y(m), w(n + m - 1);
        for (auto &v : x) v = rng();
        for (auto &v : y) v = rng();
        for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) w[i + j] += x[i] * y[j];
        assert(convU64(x, y) == w);
    }
    vector<unsigned long long> large(1 << 18);
    for (auto &x : large)
    {
        x = rng();
    }
    assert(convU64(large, {1}) == large);
}
