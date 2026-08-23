#include "../include/full/3_5_007.hpp"

signed main()
{
    OrderedMSet actual;
    multiset<int> expected;
    mt19937_64 rng(0);
    for (int q = 0; q < 20'000; q++)
    {
        int op = rng() % 3;
        int x = rng() % 101;
        if (!op || expected.empty())
        {
            actual.insert(x);
            expected.insert(x);
        }
        else if (op == 1)
        {
            assert(*actual.begin() == *expected.begin());
            actual.erase(actual.begin());
            expected.erase(expected.begin());
        }
        else
        {
            assert(*prev(actual.end()) == *prev(expected.end()));
            actual.erase(prev(actual.end()));
            expected.erase(prev(expected.end()));
        }
        assert(actual == expected);
    }
    cout << "OK\n";
}
