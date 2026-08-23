#include "../include/full/3_7_003.hpp"

signed main()
{
    PerLiChao<int> st(-3, 6);
    int r0 = -1;
    int r1 = st.add(r0, {2, 3});
    int r2 = st.add(r1, {-1, 5});
    assert(!st.query(r0, 0));
    assert(st.query(r1, 5) == optional<__int128>(13));
    assert(st.query(r2, -2) == optional<__int128>(-1));
    assert(st.query(r2, 5) == optional<__int128>(0));
    assert(st.query(r1, 5) == optional<__int128>(13));

    PerLiChao<long long, __int128> wide(0, 4'000'000'001LL);
    int rw = wide.add(-1, {4'000'000'000LL, 0});
    __int128 want = (__int128)4'000'000'000LL * 4'000'000'000LL;
    assert(wide.query(rw, 4'000'000'000LL) == optional<__int128>(want));
    cout << "OK\n";
}
