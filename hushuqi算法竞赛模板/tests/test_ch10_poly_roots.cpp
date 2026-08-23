#include "../include/full/10_2_015.hpp"

signed main()
{
    Poly f{1};
    vector<Z> want{1, 7, 23, 998244352};
    for (auto x : want)
    {
        f *= Poly{-x, 1};
        f *= Poly{-x, 1};
    }
    f *= Poly{-Z(3), 0, 1};
    auto got = distinctRoot(f, 0);
    sort(want.begin(), want.end(), [](Z a, Z b)
    {
        return a.val() < b.val();
    });
    assert(got == want);
    auto repeated = finiteRoots(f, 0);
    assert(repeated.size() == 8);
    cout << "OK\n";
    return 0;
}
