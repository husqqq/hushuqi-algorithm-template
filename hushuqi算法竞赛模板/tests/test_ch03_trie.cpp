#include "../include/full/3_6_001.hpp"

signed main()
{
    Trie<> tr;
    tr.insert("");
    tr.insert("ab");
    tr.insert("abc");
    tr.insert("ab");
    assert(tr.stringCnt("") == 1);
    assert(tr.stringCnt("ab") == 2);
    assert(tr.prefixCnt("ab") == 3);
    assert(tr.prefixCnt("") == 4);
    assert(tr.lcp("abd") == 2);
    assert(tr.find("A") == -1);
    cout << "OK\n";
}
