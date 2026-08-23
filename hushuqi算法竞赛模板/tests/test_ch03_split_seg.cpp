#include "../include/full/3_3_010.hpp"

signed main()
{
    SplitSeg<int> st(0, 8);
    int a = st.makeRoot();
    st.add(a, 1, 2);
    st.add(a, 3, 1);
    st.add(a, 6, 3);
    int b = st.split(a, 2, 7);
    assert(st.rangeSum(a, 0, 8) == 2);
    assert(st.rangeSum(b, 0, 8) == 4);
    assert(st.kth(a, 1) == 1 && st.kth(a, 3) == -1);
    assert(st.kth(b, 1) == 3 && st.kth(b, 4) == 6);
    a = st.merge(a, b);
    assert(st.rangeSum(a, 0, 8) == 6);
    st.add(a, 1, -2);
    assert(st.kth(a, 1) == 3);
    cout << "OK\n";
}
