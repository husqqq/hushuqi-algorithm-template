#include "../include/full/3_6_009.hpp"

signed main()
{
    vector<pair<int, int>> plan{{1, 4}, {3, 2}, {0, 4}};
    Freq f({2, 2, 3, 4}, plan);
    assert(f.frequency(0, 4, 2) == 2);
    assert(f.frequency(1, 3, 2) == 1);
    assert(f.frequency(0, 4, 9) == 0);
    f.setValue(1, 4);
    f.setValue(3, 2);
    assert(f.frequency(0, 4, 2) == 2);
    assert(f.frequency(0, 4, 4) == 1);

    vector<int> a(64);
    vector<pair<int, int>> many;
    for (int p = 0; p < (int)a.size(); p++)
    {
        a[p] = p * 1000003 - 17;
        many.push_back({p, -p * 1000033 + 19});
    }
    Freq g(a, many);
    for (int p = 0; p < (int)a.size(); p++)
    {
        g.setValue(p, many[p].second);
    }
    assert(g.frequency(0, (int)a.size(), many[17].second) == 1);

    Freq empty({}, {});
    assert(empty.frequency(0, 0, 123456789) == 0);
    cout << "OK\n";
}
