#include "../include/full/4_3_004.hpp"

signed main()
{
    string a = "ababc", b = "babca";
    auto x = commonSubstr(a, b);
    assert(x[1] - x[0] == 4);
    assert(a.substr(x[0], x[1] - x[0]) == b.substr(x[2], x[3] - x[2]));
    assert((commonSubstr("a", "b") == array<int, 4>{0, 0, 0, 0}));
    cout << "OK\n";
}
