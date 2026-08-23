#include "../include/full/12_3_006.hpp"

signed main()
{
    assert(smawk(0, 3, [](int, int, int) { return false; }).empty());
    assert((smawk(4, 1, [](int, int, int) { return false; }) == vector<int>{0, 0, 0, 0}));
    vector<vector<int>> tied(5, vector<int>(7, 0));
    assert((smawk(5, 7, [&](int r, int x, int y) { return tied[r][y] < tied[r][x]; }) ==
            vector<int>{0, 0, 0, 0, 0}));
    vector<vector<int>> a{{0, 4, 9}, {1, 1, 5}, {4, 2, 2}, {8, 3, 1}};
    auto p = smawk(4, 3, [&](int r, int x, int y) { return a[r][y] < a[r][x]; });
    assert((p == vector<int>{0, 0, 1, 2}));
    mt19937_64 rng(1234567);
    for (int it = 0; it < 2000; ++it)
    {
        int h = rng() % 20, w = 1 + rng() % 20;
        vector<vector<int>> m(h, vector<int>(w));
        int center = 0;
        for (int i = 0; i < h; ++i)
        {
            center = min<int>(w - 1, center + rng() % 3);
            int bias = rng() % 20;
            for (int j = 0; j < w; ++j) m[i][j] = (j - center) * (j - center) + bias;
        }
        auto better = [&](int r, int x, int y) { return m[r][y] < m[r][x]; };
        auto got = smawk(h, w, better);
        vector<int> want(h);
        for (int i = 0; i < h; ++i)
            for (int j = 1; j < w; ++j)
                if (m[i][j] < m[i][want[i]]) want[i] = j;
        assert(got == want);
    }
    cout << "OK\n";
}
