#include "../include/full/3_3_004.hpp"

signed main()
{
    AddMinSeg st({5, 1, 7, -2, 4});
    assert(st.rangeMin(0, 5) == -2);
    st.add(1, 4, 3);
    assert(st.rangeMin(0, 5) == 1);
    assert(st.rangeMin(2, 4) == 1);
    st.add(0, 2, -5);
    assert(st.rangeMin(0, 3) == -1);
    st.add(2, 2, 100);
    assert(st.rangeMin(2, 3) == 10);
    cout << "OK\n";
}
