#include "../include/full/4_1_007.hpp"

signed main()
{
    // 无参数；核对全相同字符串的唯一极大重复区间，返回 0。
    auto a = runs("aaaa");
    assert(a == vector<Run>({{1, 0, 4}}));
    cout << "OK\n";
}
