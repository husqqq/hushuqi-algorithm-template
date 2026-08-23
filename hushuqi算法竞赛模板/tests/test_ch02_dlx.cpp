#include "../include/full/2_1_009.hpp"

bool bruteExact(const vector<vector<int>> &rows, int m)
{
    // rows 是每行覆盖的列集合，m 是列数；返回是否存在精确覆盖。
    int n = rows.size();
    assert(n < 63);
    for (unsigned long long mask = 0; mask < (1ULL << n); mask++)
    {
        unsigned long long used = 0;
        bool ok = true;
        for (int i = 0; i < n && ok; i++)
        {
            if (!(mask >> i & 1))
            {
                continue;
            }
            unsigned long long bits = 0;
            for (auto c : rows[i])
            {
                bits |= 1ULL << c;
            }
            if (used & bits)
            {
                ok = false;
            }
            used |= bits;
        }
        if (ok && used == ((1ULL << m) - 1))
        {
            return true;
        }
    }
    return false;
}

bool checkAnswer(const vector<vector<int>> &rows, int m, const vector<int> &answer)
{
    // rows、m 描述实例，answer 是行下标；返回答案是否恰好覆盖每列一次。
    vector<int> cnt(m);
    for (auto id : answer)
    {
        assert(0 <= id && id < (int)rows.size());
        for (auto c : rows[id])
        {
            assert(0 <= c && c < m);
            cnt[c]++;
        }
    }
    return all_of(cnt.begin(),
                  cnt.end(),
                  [](int x)
                  {
                      return x == 1;
                  });
}

signed main()
{
    DLX sample(7, 18);
    sample.addRow(1, {1, 4, 7});
    sample.addRow(2, {1, 4});
    sample.addRow(3, {4, 5, 7});
    sample.addRow(4, {3, 5, 6});
    sample.addRow(5, {2, 3, 6, 7});
    sample.addRow(6, {2, 7});
    auto got = sample.solve();
    assert(got);
    sort(got->begin(), got->end());
    assert(*got == vector<int>({2, 4, 6}));
    auto again = sample.solve();
    assert(again);
    sort(again->begin(), again->end());
    assert(again == got);

    DLX empty(0);
    auto zero = empty.solve();
    assert(zero && zero->empty());

    DLX bad(2);
    bad.addRow(0, {1});
    bad.addRow(1, {1});
    assert(!bad.solve());

    mt19937_64 rng(0);
    for (int tc = 0; tc < 300; tc++)
    {
        int m = rng() % 7;
        int n = rng() % 9;
        vector<vector<int>> rows(n);
        DLX dlx(m, n * 3);
        for (int i = 0; i < n; i++)
        {
            for (int c = 0; c < m; c++)
            {
                if (rng() & 1)
                {
                    rows[i].push_back(c);
                }
            }
            vector<int> one;
            for (auto c : rows[i])
            {
                one.push_back(c + 1);
            }
            dlx.addRow(i, one);
        }
        auto answer = dlx.solve();
        bool want = bruteExact(rows, m);
        assert((bool)answer == want);
        if (answer)
        {
            assert(checkAnswer(rows, m, *answer));
        }
    }
    cout << "DLX OK\n";
    return 0;
}
