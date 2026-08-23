#include "../include/full/3_4_001.hpp"

struct MinOp
{
    int operator()(int a, int b) const
    {
        return min(a, b);
    }
};

signed main()
{
    ST<int, MinOp> st({5, 2, 2, 7, -1, 3});
    assert(st.query(0, 6) == -1);
    assert(st.query(1, 4) == 2);
    assert(st.query(3, 4) == 7);
    cout << "OK\n";
}
