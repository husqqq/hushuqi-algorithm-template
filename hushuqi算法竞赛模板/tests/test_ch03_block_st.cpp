#include "../include/full/3_4_001.hpp"

struct MinOp
{
    int operator()(int x, int y) const
    {
        return min(x, y);
    }
};

signed main()
{
    mt19937_64 rng(0);
    for (int n : {1, 2, 15, 16, 17, 31, 32, 33, 999})
    {
        vector<int> a(n);
        for (auto &x : a)
        {
            x = rng() % 100000;
        }
        ST<int, MinOp> st(a);
        BlockST<int, MinOp> bst(a);
        for (int l = 0; l < n; l++)
        {
            for (int q = 0; q < 50; q++)
            {
                int r = l + 1 + rng() % (n - l);
                assert(st.query(l, r) == bst.query(l, r));
            }
        }
    }
    BlockST<int, MinOp> empty({});
    assert(empty.n == 0);
    cout << "OK\n";
}
