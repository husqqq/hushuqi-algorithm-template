#include "../include/full/5_6_005.hpp"

signed main()
{
    auto z = assignment({});
    assert(z && z->cost == 0 && z->col.empty());
    assert(!assignment({{1}, {2}}));
    assert(!assignment({{1, 2}, {3}}));

    vector<vector<long long>> a{{numeric_limits<long long>::min(), 0},
                                {0, numeric_limits<long long>::max()}};
    auto mn = assignment(a);
    auto mx = assignment(a, true);
    assert(!mn && !mx);
    auto wideMn = assignment<__int128>(a);
    auto wideMx = assignment<__int128>(a, true);
    assert(wideMn && wideMn->cost == -1);
    assert(wideMx && wideMx->cost == 0);

    auto r = assignment({{3, 8, 5}}, true);
    assert(r && r->cost == 8 && r->col == vector<int>{1});
}
