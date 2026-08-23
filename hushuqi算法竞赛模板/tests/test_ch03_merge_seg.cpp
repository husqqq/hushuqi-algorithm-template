#include "../include/full/3_3_009.hpp"

signed main()
{
    MergeSeg st(-4, 6);
    int a = st.makeRoot(), b = st.makeRoot();
    st.add(a, -2, 3);
    st.add(a, 4, 1);
    st.add(b, -2, 5);
    st.add(b, 0, 2);
    assert(st.rangeSum(a, -4, 6) == 4);
    assert(st.rangeSum(b, -2, 1) == 7);
    a = st.merge(a, b);
    assert(st.rangeSum(a, -4, 6) == 11);
    assert(st.rangeSum(a, -2, -1) == 8);
    cout << "OK\n";
}
