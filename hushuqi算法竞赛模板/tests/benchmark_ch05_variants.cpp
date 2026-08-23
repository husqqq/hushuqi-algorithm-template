#include "../include/full/5_6_004.hpp"
#include "../include/full/5_10_007.hpp"
#include "../include/full/5_10_008.hpp"

using Clock = chrono::steady_clock;

template <class F>
long long timed(F f)
{
    auto st = Clock::now();
    f();
    return chrono::duration_cast<chrono::microseconds>(Clock::now() - st).count();
}

void benchMatching(string name, int n, int deg, int rep, mt19937_64 &rng)
{
    vector<pair<int, int>> edge;
    for (int u = 0; u < n; u++)
    {
        for (int z = 0; z < deg; z++)
        {
            edge.push_back({u, (int)(rng() % n)});
        }
        edge.push_back({u, u});
    }
    int a = 0, b = 0;
    long long ta = timed([&]
                         {
                             for (int z = 0; z < rep; z++)
                             {
                                 HopcroftKarp hk(n, n);
                                 for (auto [u, v] : edge)
                                 {
                                     hk.addEdge(u, v);
                                 }
                                 a += hk.matching();
                             }
                         });
    long long tb = timed([&]
                         {
                             for (int z = 0; z < rep; z++)
                             {
                                 HopKarpCSR hk(n, n);
                                 for (auto [u, v] : edge)
                                 {
                                     hk.addEdge(u, v);
                                 }
                                 b += hk.matching();
                             }
                         });
    assert(a == b);
    cout << "HK " << name << " n=" << n << " m=" << edge.size() << " rep=" << rep
         << " adjacency_us=" << ta << " csr_us=" << tb << " checksum=" << a << '\n';
}

void benchColoring(string name, int n, int perThousand, mt19937_64 &rng)
{
    vector<unsigned long long> e(n);
    for (int u = 0; u < n; u++)
    {
        for (int v = u + 1; v < n; v++)
        {
            if ((int)(rng() % 1000) < perThousand)
            {
                e[u] |= 1ULL << v;
                e[v] |= 1ULL << u;
            }
        }
    }
    int a = -1, b = -1;
    long long ta = timed([&]
                         { a = chromaticNum(e); });
    long long tb = timed([&]
                         { b = Dsatur(e).solve().k; });
    assert(a == b);
    cout << "color " << name << " n=" << n << " p=" << perThousand / 1000.0
         << " subset_dp_us=" << ta << " dsatur_us=" << tb << " answer=" << a << '\n';
}

signed main()
{
    mt19937_64 rng(0);
    benchMatching("sparse", 10000, 8, 5, rng);
    benchMatching("medium", 3000, 64, 5, rng);
    benchColoring("sparse", 15, 200, rng);
    benchColoring("medium", 15, 500, rng);
    benchColoring("dense", 15, 800, rng);
    return 0;
}
