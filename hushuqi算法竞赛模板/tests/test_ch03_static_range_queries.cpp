#include "../include/full/3_4_010.hpp"
#include "../include/full/3_4_011.hpp"
#include "../include/full/3_4_012.hpp"

int bruteInversions(const vector<int> &a, int l, int r)
{
    // a 是原数组，[l,r) 是查询区间；返回该区间的严格逆序对数。
    int ans = 0;
    for (int i = l; i < r; i++)
    {
        for (int j = i + 1; j < r; j++)
        {
            ans += a[i] > a[j];
        }
    }
    return ans;
}

signed main()
{
    vector<int> empty;
    vector<pair<int, int>> emptyQueries{{0, 0}, {0, 0}};
    assert((distinct(empty, emptyQueries) == vector<int>{0, 0}));
    assert((rangeInv(empty, emptyQueries) == vector<int>{0, 0}));
    assert((prefix(empty) == vector<int>{0}));

    vector<int> sumValues{7, -3, 5, -11, 13};
    auto pre = prefix(sumValues);
    assert(pre[0] == 0 && pre[5] == 11);
    assert(pre[4] - pre[1] == -9 && pre[3] - pre[3] == 0);

    vector<int> a{3, 1, 2, 1};
    vector<pair<int, int>> fixed{{0, 4}, {1, 4}, {2, 2}, {0, 1}};
    assert((distinct(a, fixed) == vector<int>{3, 2, 0, 1}));
    assert((rangeInv(a, fixed) == vector<int>{4, 1, 0, 0}));

    mt19937_64 rng(20260819);
    unsigned long long hash = 0;
    for (int tc = 0; tc < 500; tc++)
    {
        int n = rng() % 71;
        int q = rng() % 101;
        vector<int> v(n);
        for (int &x : v)
        {
            x = (int)(rng() % 17) - 8;
        }
        vector<pair<int, int>> qs(q);
        for (auto &[l, r] : qs)
        {
            l = rng() % (n + 1);
            r = rng() % (n + 1);
            if (l > r)
            {
                swap(l, r);
            }
        }

        auto gotDistinct = distinct(v, qs);
        auto gotInversions = rangeInv(v, qs);
        for (int id = 0; id < q; id++)
        {
            auto [l, r] = qs[id];
            set<int> seen(v.begin() + l, v.begin() + r);
            assert(gotDistinct[id] == (int)seen.size());
            assert(gotInversions[id] == bruteInversions(v, l, r));
            hash = hash * 1000003ULL + (unsigned long long)gotDistinct[id] * 97 +
                   (unsigned long long)gotInversions[id];
        }
    }
    cout << hash << '\n';
    return 0;
}
