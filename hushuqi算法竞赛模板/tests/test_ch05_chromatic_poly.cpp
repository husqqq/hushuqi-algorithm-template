#include "../include/full/5_10_010.hpp"

signed main()
{
    auto empty = chromPoly({0, 0});
    assert(empty.size() == 3 && empty[0].val() == 0 && empty[1].val() == 0 && empty[2].val() == 1);
    auto edge = chromPoly({2, 1});
    assert(edge[0].val() == 0 && edge[1].val() == Z(-1).val() && edge[2].val() == 1);
    auto tri = chromPoly({6, 5, 3});
    assert(tri[0].val() == 0 && tri[1].val() == 2 && tri[2].val() == Z(-3).val() && tri[3].val() == 1);
    assert(chromPoly({1}).front().val() == 0);
    puts("OK");
    return 0;
}
