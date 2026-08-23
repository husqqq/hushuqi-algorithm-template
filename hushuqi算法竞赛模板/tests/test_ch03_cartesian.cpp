#include "../include/full/3_4_004.hpp"

signed main()
{
    Cart c = cartesian(vector<int>{3, 1, 1, 2});
    assert(c.root == 1);
    assert(c.parent == vector<int>({1, -1, 1, 2}));
    assert(c.left == vector<int>({-1, 0, -1, -1}));
    assert(c.right == vector<int>({-1, 2, 3, -1}));

    Cart e = cartesian(vector<int>{});
    assert(e.root == -1 && e.parent.empty() && e.left.empty() && e.right.empty());
    cout << "OK\n";
}
