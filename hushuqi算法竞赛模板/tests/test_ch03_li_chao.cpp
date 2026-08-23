#include "../include/full/3_7_001.hpp"

signed main()
{
    LiChao<int> st({3, -2, 0, 3, 5});
    assert(!st.query(0));
    st.add({2, 3});
    st.add({-1, 5});
    assert(st.query(-2) == optional<__int128>(-1));
    assert(st.query(0) == optional<__int128>(3));
    assert(st.query(3) == optional<__int128>(2));
    assert(st.query(5) == optional<__int128>(0));

    LiChao<long long, __int128> wide({4'000'000'000LL});
    wide.add({4'000'000'000LL, 0});
    __int128 want = (__int128)4'000'000'000LL * 4'000'000'000LL;
    assert(wide.query(4'000'000'000LL) == optional<__int128>(want));
    cout << "OK\n";
}
