#include "../include/full/4_3_004.hpp"

signed main()
{
    SAM<26, 'a'> sam("ababa");
    assert(sam.distinct() == 9 && sam.occ("aba") == 2);
    cout << "OK\n";
}
