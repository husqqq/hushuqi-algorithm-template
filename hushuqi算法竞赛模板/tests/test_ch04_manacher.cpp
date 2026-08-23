#include "../include/full/4_1_004.hpp"

signed main()
{
    Manacher m("abacaba");
    assert(m.odd[3] == 4 && m.isPal(0, 7) && m.isPal(1, 6));
    cout << "OK\n";
}
