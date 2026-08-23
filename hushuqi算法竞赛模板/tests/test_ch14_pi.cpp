#include "../include/full/14_1_021.hpp"

signed main()
{
    const string expected = "3.1415926535897932384626433832795028841972";
    assert(piMachin(40).str(40) == expected);
    assert(piChudnovsky(40).str(40) == expected);
    const string expected100 =
        "3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170680";
    assert(piMachin(100).str(100) == expected100);
    assert(piChudnovsky(100).str(100) == expected100);
    cout << "OK\n";
}
