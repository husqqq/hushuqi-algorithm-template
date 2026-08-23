#include "../include/full/3_5_004.hpp"

signed main()
{
    Splay<int> tr;
    for (int x : {4, 1, 4, 7, 2})
    {
        tr.add(x);
    }
    assert(tr.rank(4) == 2);
    assert(tr.kth(2) == 4 && tr.kth(3) == 4);
    assert(tr.prev(4) == optional<int>(2));
    assert(tr.next(4) == optional<int>(7));
    assert(tr.erase(4) && tr.kth(2) == 4);
    assert(tr.erase(4) && !tr.erase(4));
    assert(!tr.prev(1) && !tr.next(7));
    cout << "OK\n";
}
