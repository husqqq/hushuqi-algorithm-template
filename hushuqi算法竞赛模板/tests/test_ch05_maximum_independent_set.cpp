#include "../include/full/5_6_008.hpp"

signed main()
{
    auto a = maxIS(5, {{0, 1}, {1, 2}, {2, 3}, {3, 4}});
    assert(a.size() == 3);
    auto b = maxIS(40, {});
    assert(b.size() == 40);
    vector<pair<int, int>> e;
    for (int i = 0; i < 40; i++)
    {
        for (int j = i + 1; j < 40; j++) e.push_back({i, j});
    }
    auto c = maxIS(40, e);
    assert(c.size() == 1);
    auto d = maxIS(3, {{0, 0}});
    assert(d.size() == 2 && find(d.begin(), d.end(), 0) == d.end());
}
