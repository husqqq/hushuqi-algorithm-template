#include "../include/full/4_4_002.hpp"

signed main()
{
    DequePAM<26, 'a'> p; p.pushBack('a'); p.pushBack('b'); p.pushFront('b');
    assert(p.distinct() == 3 && p.longPrefix() == 3 && p.longSuffix() == 3);
    cout << "OK\n";
}
