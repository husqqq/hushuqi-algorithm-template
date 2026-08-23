#include "../include/full/3_6_006.hpp"

signed main()
{
    mt19937_64 rng(0);
    FixedHashMap<int, int, 15> mp;
    unordered_map<int, int, CustomHash> ref;
    for (int q = 0; q < 20000; q++)
    {
        int x = rng() % 10000 - 5000;
        if (rng() & 1)
        {
            int v = rng();
            mp[x] = v;
            ref[x] = v;
        }
        else
        {
            auto p = mp.find(x);
            auto it = ref.find(x);
            assert((p != nullptr) == (it != ref.end()));
            assert(mp.get(x) == (it == ref.end() ? 0 : it->second));
            if (p)
            {
                assert(*p == it->second);
            }
        }
    }
    assert(mp.size() == ref.size());
    mp.clear();
    assert(mp.size() == 0 && !mp.contains(0));

    FixedHashSet<int, 15> st;
    unordered_set<int, CustomHash> refSet;
    for (int q = 0; q < 20000; q++)
    {
        int x = rng() % 10000 - 5000;
        if (rng() & 1)
        {
            assert(st.insert(x) == refSet.insert(x).second);
        }
        else
        {
            assert(st.contains(x) == refSet.contains(x));
        }
    }
    assert(st.insert(-123) == refSet.insert(-123).second);
    assert(!st.insert(-123));
    assert(st.size() == refSet.size());
    st.clear();
    assert(st.size() == 0 && !st.contains(-123));
    cout << "OK\n";
}
