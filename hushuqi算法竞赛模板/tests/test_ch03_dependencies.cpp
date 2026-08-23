#include <bits/stdc++.h>
using namespace std;

namespace trie_topic {
#include "../include/full/3_6_002.hpp"
}
#undef int

signed main()
{
    trie_topic::BinaryTrie<8> trie;
    trie.insert(3);
    trie.insert(5, 2);
    assert(trie.size() == 3 && trie.count(5) == 2);
    assert(trie.minXor(4) == 1 && trie.maxXor(4) == 7);
    assert(trie.kthXor(4, 0) == 1 && trie.kthXor(4, 2) == 7);
    assert(trie.countXorLess(4, 2) == 2);
    trie.clear();
    assert(trie.empty());
    cout << "OK\n";
}
