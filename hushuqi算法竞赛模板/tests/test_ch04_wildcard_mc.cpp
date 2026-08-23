#include "../include/full/4_1_012.hpp"

signed main()
{
    assert(wildcardMC("abc", "a*c", 0)[0]);
    assert(!wildcardMC("abc", "a*d", 0)[0]);
    cout << "OK\n";
}
