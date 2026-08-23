#include "../include/full/3_6_006.hpp"

signed main()
{
    HashMap<int, int> mp;
    mp.max_load_factor(.7);
    mp.reserve(16);
    mp[0] = 4;
    mp[1000000000000000000LL] = 9;
    mp[-7]++;
    assert(mp.find(1) == mp.end());
    assert(mp[0] == 4 && mp[1000000000000000000LL] == 9 && mp[-7] == 1);
    cout << "4 9 1\n";
}
