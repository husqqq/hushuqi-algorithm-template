#include "../include/full/5_5_007.hpp"

signed main()
{
    assert(euclideanMST({}) == 0);
    assert(euclideanMST({{2, 3}, {2, 3}}) == 0);
    long double x = euclideanMST({{0, 0}, {1, 0}, {1, 1}, {0, 1}});
    assert(fabsl(x - 3) < 1e-15L);
    auto edge = euclidMST({{{0, 0}}, {{1, 0}}, {{1, 1}}, {{0, 1}}, {{0, 0}}});
    assert(edge.size() == 4);
    DSU d(5);
    long long sum = 0;
    vector<array<long long, 2>> p{{0, 0}, {1, 0}, {1, 1}, {0, 1}, {0, 0}};
    for (auto [u, v] : edge)
    {
        assert(d.merge(u, v));
        long long dx = p[u][0] - p[v][0], dy = p[u][1] - p[v][1];
        sum += dx * dx + dy * dy;
    }
    assert(sum == 3);
}
