#include "../include/full/6_3_001.hpp"

signed main()
{
    vector<vector<int>> g{{1}, {0, 2}, {1}};
    HLD h(g);
    int calls = 0;
    h.path(1, 1, [&](int l, int r, bool rev)
    {
        calls++;
        assert(l < r);
        assert(!rev || rev);
    }, true);
    assert(calls == 0);
    h.path(0, 2, [&](int l, int r, bool rev)
    {
        calls++;
        assert(l < r);
        assert(!rev || rev);
    }, true);
    assert(calls == 1);
    return 0;
}
