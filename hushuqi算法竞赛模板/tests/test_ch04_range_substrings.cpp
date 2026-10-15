#include "../include/full/4_3_010.hpp"

long long bruteSubs(const string &s, int l, int r)
{
    // s 是字节串，[l,r) 是半开区间；直接枚举并去重，返回非空子串数。
    set<string> seen;
    for (int i = l; i < r; i++)
    {
        for (int j = i + 1; j <= r; j++)
        {
            seen.insert(s.substr(i, j - i));
        }
    }
    return seen.size();
}

bool checkSubs(const string &s, int seed)
{
    // s 是小写串，seed 决定询问重排并标识失败输入；核对全部空、非空区间。
    int n = s.size();
    vector<array<int, 2>> qs; // 各项依次为左端、右端。
    for (int l = 0; l <= n; l++)
    {
        for (int r = l; r <= n; r++)
        {
            qs.push_back({l, r});
        }
    }
    mt19937_64 rng(seed);
    shuffle(qs.begin(), qs.end(), rng);
    auto got = rangeSubs(s, qs);
    bool ok = rangeSubs(s, {}).empty();
    for (int i = 0; i < (int)qs.size(); i++)
    {
        auto [l, r] = qs[i];
        long long want = bruteSubs(s, l, r);
        ok &= got[i] == want && subDistinct(s, l, r) == want;
    }
    if (!ok)
    {
        ofstream out(filesystem::temp_directory_path() / ("template-range-subs-WA-" + to_string(seed) + ".txt"));
        out << s << '\n' << qs.size() << '\n';
        for (auto [l, r] : qs)
        {
            out << l << ' ' << r << '\n';
        }
    }
    return ok;
}

signed main()
{
    // 无参数；穷举短二元串、逐种子随机对拍，并核对深链、周期和超过 32 位的计数。
    assert((rangeSubs("banana", {{0, 6}, {1, 6}, {1, 4}, {2, 2}})
        == vector<long long>{15, 9, 5, 0}));
    assert(rangeSubs("", {{0, 0}}) == vector<long long>{0});
    assert(rangeSubs(string(200000, 'a'), {{0, 1}, {1, 1}}) == (vector<long long>{1, 0}));
    int id = 0;
    for (int n = 0; n <= 8; n++)
    {
        for (int mask = 0; mask < (1LL << n); mask++)
        {
            string s(n, 'a');
            for (int i = 0; i < n; i++)
            {
                s[i] += mask >> i & 1;
            }
            bool ok = checkSubs(s, id);
            cout << (ok ? "AC " : "WA ") << id << '\n';
            if (!ok) return 1;
            id++;
        }
    }
    for (int seed = 0; seed < 600; seed++)
    {
        mt19937_64 rng(seed);
        string s(rng() % 20, 'a');
        for (char &c : s)
        {
            c += rng() % (seed % 3 == 0 ? 26 : 3);
        }
        bool ok = checkSubs(s, id);
        string bytes(s.size(), '\0');
        for (char &c : bytes)
        {
            c = (char)(rng() & 255);
        }
        ok &= subDistinct(bytes, 0, bytes.size()) == bruteSubs(bytes, 0, bytes.size());
        cout << (ok ? "AC " : "WA ") << id << '\n';
        if (!ok) return 1;
        id++;
    }
    int n = 200000;
    assert((rangeSubs(string(n, 'a'), {{0, n}, {17, n}, {n, n}})
        == vector<long long>{n, n - 17, 0}));
    string period(n, 'a');
    for (int i = 0; i < n; i++)
    {
        period[i] += i % 26;
    }
    assert(rangeSubs(period, {{0, n}}) == vector<long long>{26LL * n - 325});
    string random(n / 2, 'a');
    mt19937_64 rng(20261016);
    for (char &c : random)
    {
        c += rng() % 26;
    }
    long long want = subDistinct(random, 0, random.size());
    assert(want > numeric_limits<int32_t>::max());
    assert(rangeSubs(random, {{0, (int)random.size()}}) == vector<long long>{want});
}
