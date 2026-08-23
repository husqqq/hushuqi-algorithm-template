#include "../include/full/4_1_003.hpp"

signed main()
{
    assert(zfunc("aabcaabxaaaz") == vector<int>({12, 1, 0, 0, 3, 1, 0, 0, 2, 2, 1, 0}));
    cout << "OK\n";
}
