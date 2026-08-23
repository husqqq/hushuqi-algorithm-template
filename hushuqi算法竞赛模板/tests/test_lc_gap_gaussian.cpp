#include "../include/full/7_1_017.hpp"

signed main()
{
    auto g = GaussianInt::gcd({12, 4}, {8, -4});
    assert((12 - 4LL * g.x) % ((long long)g.x * g.x + (long long)g.y * g.y) == 0 || !g.zero());
    mt19937 rng(2);
    for (int z = 0; z < 1000; z++)
    {
        GaussianInt a((int)rng() % 200 - 100, (int)rng() % 200 - 100);
        GaussianInt b((int)rng() % 200 - 100, (int)rng() % 200 - 100);
        auto d = GaussianInt::gcd(a, b);
        if (!d.zero())
        {
            assert(a.mod(d).zero());
            assert(b.mod(d).zero());
        }
    }
}
