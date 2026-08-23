#include "../include/full/3_6_012.hpp"
#include "../include/full/3_6_013.hpp"

signed main()
{
    TaggedTrie<4> bt;
    bt.add(1, 0);
    bt.add(6, 0);
    bt.add(6, 1);
    bt.add(10, 1);
    assert(bt.maxXor(8, 0) == optional<unsigned long long>(14));
    assert(bt.maxElement(1, 1) == optional<unsigned long long>(10));
    bt.add(6, 0, -1);
    assert(bt.maxXor(8, 0) == optional<unsigned long long>(9));
    assert(bt.maxXor(8, 1) == optional<unsigned long long>(14));
    bt.add(1, 0, -1);
    assert(!bt.maxXor(8, 0));

    TaggedTrie<4> one;
    one.add(5, 0);
    assert(one.maxXor(5, 0) == optional<unsigned long long>(0));
    assert(one.maxElement(5, 0) == optional<unsigned long long>(5));

    SiblingTrie<int> st;
    int ab = st.insert("ab", 7);
    int ac = st.insert("ac", 8);
    int ab2 = st.insert("ab", 9);
    assert(ab == ab2 && st.tr[ab].end == 2 && st.tr[ab].val == 9);
    assert(st.tr[ac].end == 1 && st.find("ad") == -1);
    assert(st.lcp("abd") == 2 && st.lcp("ax") == 1);
    cout << "OK\n";
}
