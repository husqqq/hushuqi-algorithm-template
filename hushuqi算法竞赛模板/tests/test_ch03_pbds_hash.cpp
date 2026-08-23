#include "../include/full/3_9_011.hpp"

signed main()
{
    pbds_hash::HashMap<int, int> mp;
    mp[4] = 7;
    mp[-3] = 11;
    assert(mp.find(4) != mp.end() && mp[4] == 7);
    assert(mp.find(-3) != mp.end() && mp[-3] == 11);
    assert(mp.find(0) == mp.end());

    CustomHash hash;
    pair<int, int> p{1, 2};
    assert(hash(p) == hash(p));
    cout << "OK\n";
}
