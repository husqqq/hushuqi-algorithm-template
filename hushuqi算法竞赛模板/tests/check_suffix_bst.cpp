#include "../include/full/4_3_009.hpp"

signed main()
{
    SuffixBST s;
    for (char c : string("ananab"))
    {
        s.pushFront(c);
    }
    vector<int> sa{5, 3, 1, 0, 4, 2};
    for (int k = 0; k < 6; k++)
    {
        assert(s.kth(k) == sa[k]);
        assert(s.rank(sa[k]) == k);
    }
    assert(s.distinct() == 15);
    s.popFront();
    assert(s.distinct() == 9);
}
