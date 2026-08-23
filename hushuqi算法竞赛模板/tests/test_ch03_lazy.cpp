#include "../include/full/3_3_002.hpp"

signed main()
{
    LazySeg<Info, Tag> st(4);
    assert(st.query(0, 4).sum == 0 && st.query(0, 4).len == 4);
    st.apply(0, 4, Tag{3});
    assert(st.query(0, 4).sum == 12);
    const auto &view = st;
    assert(view.get(2).sum == 3);
    st.apply(1, 3, Tag{-2});
    assert(st.query(0, 4).sum == 8);
    assert(st.get(1).sum == 1 && st.get(2).sum == 1);
    assert(st.query(1, 3).sum == 2 && st.query(1, 3).len == 2);

    LazySeg<Info, Tag> empty(0);
    empty.apply(0, 0, Tag{7});
    assert(empty.query(0, 0).len == 0);
    cout << "OK\n";
    return 0;
}
