#include "../include/full/5_6_006.hpp"

signed main()
{
    BipEdgeColor a(5, 5);
    a.add(0, 0);
    a.add(0, 1);
    a.add(1, 0);
    a.add(2, 2);
    auto x = a.work();
    assert(x.k == 2 && (int)x.c.size() == 4);
    vector<set<int>> l(5), r(5);
    vector<pair<int, int>> e{{0, 0}, {0, 1}, {1, 0}, {2, 2}};
    for (int i = 0; i < 4; i++)
    {
        assert(0 <= x.c[i] && x.c[i] < x.k);
        assert(l[e[i].first].insert(x.c[i]).second);
        assert(r[e[i].second].insert(x.c[i]).second);
    }

    BipEdgeColor star(100000, 100000);
    for (int i = 0; i < 100000; i++) star.add(0, i);
    auto y = star.work();
    assert(y.k == 100000 && (int)y.c.size() == 100000);
    sort(y.c.begin(), y.c.end());
    for (int i = 0; i < 100000; i++) assert(y.c[i] == i);
}
