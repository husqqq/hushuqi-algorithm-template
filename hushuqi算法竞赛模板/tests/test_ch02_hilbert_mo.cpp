#include "../include/full/2_3_001.hpp"

signed main()
{
    vector<MoQ> q{{0, 3, 0, 0}, {1, 5, 1, 0}, {2, 2, 2, 0}};
    vector<int> a{1, 2, 3, 4, 5};
    int cur = 0;
    auto ans = moHilbert<int>(
        q,
        [&](int i)
        {
            cur += a[i];
        },
        [&](int i)
        {
            cur -= a[i];
        },
        [&]()
        {
            return cur;
        });
    assert((ans == vector<int>{6, 14, 0}));
    cout << "OK\n";
}
