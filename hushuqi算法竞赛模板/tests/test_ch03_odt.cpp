#include "../include/full/3_5_006.hpp"

signed main()
{
    ODT<int> odt({1, 1, 3, 3, 2});
    odt.assign(1, 4, 5);
    odt.apply(2, 5, [](int &x)
    {
        x += 2;
    });
    assert(odt.values() == vector<int>({1, 5, 7, 7, 4}));
    assert(odt.kth(0, 5, 0) == 1);
    assert(odt.kth(0, 5, 3) == 7);

    ODT<int> empty({});
    empty.assign(0, 0, 9);
    assert(empty.values().empty());
    cout << "OK\n";
}
