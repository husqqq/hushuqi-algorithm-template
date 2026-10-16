#include "../include/full/4_3_002.hpp"

signed main()
{
    // 无参数；核对空串、重复字符、一般字符串和无符号字节顺序，成功返回 0。
    assert(suffixArray("").empty());
    assert(suffixArray("aaaa") == vector<int>({3, 2, 1, 0}));
    assert(suffixArray("banana") == vector<int>({5, 3, 1, 0, 4, 2}));
    string bytes;
    bytes.push_back((char)0xFF);
    bytes.push_back((char)0x00);
    bytes.push_back((char)0x80);
    assert(suffixArray(bytes) == vector<int>({1, 2, 0}));
    cout << "OK\n";
}
