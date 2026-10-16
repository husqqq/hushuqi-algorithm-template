#include "../include/full/4_1_004.hpp"

signed main()
{
    // 无参数；核对奇偶中心、空串、边界和任意字节串，成功返回 0。
    Manacher m("abacaba");
    assert(m.odd[3] == 4 && m.isPal(0, 7) && m.isPal(1, 6));
    assert(m.isPal(0, 0) && m.isPal(2, 5) && !m.isPal(0, 6));
    Manacher empty("");
    assert(empty.odd.empty() && empty.even.empty() && empty.isPal(0, 0));
    mt19937 rng(20261016);
    for (int seed = 0; seed < 200; seed++)
    {
        string s(rng() % 80, '\0');
        for (char &c : s)
        {
            c = (char)rng();
        }
        Manacher got(s);
        for (int l = 0; l <= (int)s.size(); l++)
        {
            for (int r = l; r <= (int)s.size(); r++)
            {
                bool want = true;
                for (int i = 0; l + i < r - 1 - i; i++)
                {
                    want &= s[l + i] == s[r - 1 - i];
                }
                assert(got.isPal(l, r) == want);
            }
        }
    }
    cout << "OK\n";
}
