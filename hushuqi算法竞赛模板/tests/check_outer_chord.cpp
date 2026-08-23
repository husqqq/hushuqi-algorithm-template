#include "../include/full/5_10_016.hpp"

signed main()
{
    auto a = outerChordOK(4, {{0, 2}, {1, 3}});
    assert(a && (*a)[0] != (*a)[1]);
    auto b = outerChordOK(6, {{0, 2}, {3, 5}});
    assert(b);
    cout << "ok\n";
}
