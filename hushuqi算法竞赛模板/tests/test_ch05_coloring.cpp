#include "../include/full/5_10_006.hpp"
#include "../include/full/5_10_007.hpp"
#include "../include/full/5_10_008.hpp"

void check(const vector<unsigned long long> &e, const Coloring &res)
{
    assert(res.color.size() == e.size());
    for (int u = 0; u < (int)e.size(); u++)
    {
        assert(0 <= res.color[u] && res.color[u] < res.k);
        for (int v = u + 1; v < (int)e.size(); v++)
        {
            if (e[u] >> v & 1)
            {
                assert(res.color[u] != res.color[v]);
            }
        }
    }
}

signed main()
{
    assert(Dsatur({}).solve().k == 0);
    mt19937_64 rng(0);
    for (int it = 0; it < 1000; it++)
    {
        int n = rng() % 12;
        vector<unsigned long long> e(n);
        Clique64 g(n);
        for (int u = 0; u < n; u++)
        {
            for (int v = u + 1; v < n; v++)
            {
                if (rng() % 3 == 0)
                {
                    e[u] |= 1ULL << v;
                    e[v] |= 1ULL << u;
                    g.add(u, v);
                }
            }
        }
        auto res = Dsatur(e).solve();
        assert(res.k == chromaticNum(e));
        check(e, res);
        assert((int)g.maxClique().size() <= res.k);
    }

    vector<unsigned long long> complete(20);
    for (int u = 0; u < 20; u++)
    {
        for (int v = u + 1; v < 20; v++)
        {
            complete[u] |= 1ULL << v;
            complete[v] |= 1ULL << u;
        }
    }
    auto res = Dsatur(complete).solve();
    assert(res.k == 20);
    check(complete, res);
    puts("OK");
    return 0;
}
