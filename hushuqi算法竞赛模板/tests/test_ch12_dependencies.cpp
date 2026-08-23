#include "../include/full/12_1_001.hpp"

signed main()
{
    int x = 7;
    assert(chmin(x, (int)3) && x == 3 && !chmin(x, (int)5));
    assert(chmax(x, (int)9) && x == 9 && !chmax(x, (int)4));
    assert(addMod(8, 7, 10) == 5);
    auto f = zeroOnePack(5, {{2, 3}, {3, 5}, {4, 6}});
    assert(f[0] == 0 && f[5] == 8 && f[1] == -INF);
    cout << "OK\n";
}
