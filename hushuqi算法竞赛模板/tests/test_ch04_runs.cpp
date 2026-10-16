#include "../include/full/4_1_007.hpp"

signed main()
{
    // 无参数；核对空串、最小周期、半开边界和零字节，成功返回 0。
    assert(runs("").empty() && runs("a").empty());
    assert((runs("aaaa") == vector<array<int, 3>>{{1, 0, 4}}));
    assert((runs("ababa") == vector<array<int, 3>>{{2, 0, 5}}));
    assert((runs(string(5, '\0')) == vector<array<int, 3>>{{1, 0, 5}}));
    cout << "OK\n";
}
