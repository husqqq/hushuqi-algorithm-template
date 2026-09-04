#include "../include/full/10_2_021.hpp"

signed main()
{
    assert(lagrange({Z(0), 1, 2}, {1, 4, 9}, 10) == Z(121));
    assert(lagrange({Z(2), 5, 9}, {7, 3, 11}, 5) == Z(3));
    cout << "OK\n";
    return 0;
}
