#include "../include/full/10_1_010.hpp"

signed main()
{
    vector<long long> empty;
    divisorZeta(empty);
    divMob(empty);
    multipleZeta(empty);
    mulMob(empty);
    vector<long long> a{0, 1, 2, 3, 4, 5, 6};
    auto b = a;
    divisorZeta(b);
    assert((b == vector<long long>{0, 1, 3, 4, 7, 6, 12}));
    divMob(b);
    assert(b == a);
    b = a;
    multipleZeta(b);
    assert((b == vector<long long>{0, 21, 12, 9, 4, 5, 6}));
    mulMob(b);
    assert(b == a);
    cout << "OK\n";
    return 0;
}
