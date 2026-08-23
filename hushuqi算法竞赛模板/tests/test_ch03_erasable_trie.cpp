#include "../include/full/3_6_003.hpp"

signed main()
{
    constexpr unsigned long long HI = 1ULL << 63;
    constexpr unsigned long long ALL = numeric_limits<unsigned long long>::max();
    EraseBinTrie<64> tr;
    tr.insert(0);
    tr.insert(HI);
    tr.insert(ALL, 2);
    assert(tr.erase(ALL) && tr.count(ALL) == 1);
    assert(tr.maxXor(0) == ALL && tr.minXor(0) == 0);
    assert(tr.kthElement(ALL, 1) == HI);
    assert(tr.erase(ALL) && !tr.erase(ALL));
    assert(tr.size() == 2 && tr.maxXor(0) == HI);
    assert(tr.countXorLess(ALL, HI) == 1);
    cout << "OK\n";
}
