#include "../include/full/3_5_002.hpp"

signed main()
{
    FHQ<int> tr;
    for (int x : {4, 1, 4, 7, 2})
    {
        tr.insert(x);
    }
    assert(tr.rank(4) == 2);
    assert(tr.kth(2) == 4 && tr.kth(3) == 4);
    assert(tr.erase(4) && tr.kth(2) == 4);
    assert(tr.erase(4) && !tr.erase(4));
    assert(tr.rank(8) == 3);
    cout << "OK\n";
}
