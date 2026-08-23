#include "../include/full/6_4_006.hpp"

signed main()
{
    vector<long long> a = {1, 2, 3};
    vector<DynCompOp> op = {
        {3, 0}, {0, 0, 1}, {3, 1}, {2, 0, 0, 5},
        {3, 1}, {0, 1, 2}, {3, 2}, {1, 0, 1}, {3, 0}};
    assert(offlineCSum(a, op) == vector<long long>({1, 3, 8, 11, 6}));
    puts("OK");
    return 0;
}
