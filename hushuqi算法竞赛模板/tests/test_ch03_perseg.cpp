#include "../include/full/3_3_008.hpp"

signed main()
{
    PerSeg st(8);
    int r0 = 0;
    int r1 = st.add(r0, 5, 1, 50);
    int r2 = st.add(r1, 2, 2, 14);
    int r3 = st.add(r2, 7, 1, -3);
    assert(st.query(r0, 0, 8) == 0);
    assert(st.query(r3, 2, 6) == 3);
    assert((st.queryInfo(r3, 2, 8) == array<long long, 2>{4, 61}));
    assert((st.queryInfo(r2, 0, 3) == array<long long, 2>{2, 14}));
    assert(st.kth(r0, r3, 0) == 2);
    assert(st.kth(r0, r3, 1) == 2);
    assert(st.kth(r1, r3, 0) == 2);
    assert(st.kth(r1, r3, 2) == 7);
    cout << "OK\n";
}
