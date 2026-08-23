#include "../include/full/6_5_001.hpp"

signed main()
{
    vector<vector<int>> g{{1}, {0, 2}, {1}};
    auto f = treePack(g, vector<long long>{5, -2, 4}, 0, 3);
    assert(f.size() == 4);
    assert(f[0] && *f[0] == 0);
    assert(f[1] && *f[1] == 5);
    assert(f[2] && *f[2] == 9);
    assert(f[3] && *f[3] == 7);
    auto wide = treePack<__int128>(g, vector<__int128>{5, -2, 4}, 0, 3);
    assert(wide[3] && *wide[3] == 7);
    return 0;
}
