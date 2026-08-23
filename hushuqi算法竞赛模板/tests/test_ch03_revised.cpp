#include "../include/full/3_1_006.hpp"
#include "../include/full/3_3_017.hpp"
#include "../include/full/3_3_018.hpp"
#include "../include/full/3_3_019.hpp"
#include "../include/full/3_4_008.hpp"

constexpr int mod3 = 998244353;

struct Aff3
{
    int a, b;
    bool operator==(const Aff3 &) const = default;
};

struct AffMonoid3
{
    using T = Aff3;

    static T id()
    {
        return {1, 0};
    }

    static T op(T x, T y)
    {
        return {y.a * x.a % mod3, (y.a * x.b + y.b) % mod3};
    }
};

void testRangeDSU()
{
    for (int n : {2, 4})
    {
        RangeDSU rd(n);
        DSU d(n);
        int cnt = 0;
        auto f = [&](int x, int y)
        {
            cnt++;
            d.merge(x, y);
        };
        rd.merge(0, 0, n, f);
        assert(cnt == 0);
        rd.merge(0, n / 2, n / 2, f);
        for (int i = 0; i < n / 2; i++)
        {
            assert(d.same(i, i + n / 2));
        }
    }

    // callback 的两个参数必须就是第 0 层本次合并的新根和被吸收根，才能直接维护连通块权值。
    mt19937_64 rng(4);
    for (int tc = 0; tc < 100; tc++)
    {
        int n = 1 + rng() % 20;
        vector<long long> w(n), got;
        for (auto &x : w)
        {
            x = 1 + rng() % 9;
        }
        got = w;
        RangeDSU rd(n);
        DSU brute(n);
        long long ans = 0;
        for (int q = 0; q < 100; q++)
        {
            int len = rng() % (n + 1);
            int x = rng() % (n - len + 1);
            int y = rng() % (n - len + 1);
            rd.merge(x, y, len, [&](int root, int absorbed)
            {
                ans += got[root] * got[absorbed];
                got[root] += got[absorbed];
            });
            for (int i = 0; i < len; i++)
            {
                brute.merge(x + i, y + i);
            }
            long long direct = 0;
            for (int i = 0; i < n; i++)
            {
                for (int j = i + 1; j < n; j++)
                {
                    if (brute.same(i, j))
                    {
                        direct += w[i] * w[j];
                    }
                }
            }
            assert(ans == direct);
        }
    }
}

unsigned long long testMajority()
{
    mt19937_64 rng(0);
    unsigned long long acc = 0;
    for (int tc = 0; tc < 100; tc++)
    {
        int n = 1 + rng() % 40;
        vector<int> a(n);
        for (auto &x : a)
        {
            x = (int)(rng() % 11) - 5;
        }
        vector<array<int, 3>> op;
        vector<pair<int, int>> plan;
        for (int q = 0; q < 100; q++)
        {
            if (rng() & 1)
            {
                int p = rng() % n, x = (int)(rng() % 15) - 7;
                op.push_back({0, p, x});
                plan.push_back({p, x});
            }
            else
            {
                int l = rng() % (n + 1), r = rng() % (n + 1);
                if (l > r)
                {
                    swap(l, r);
                }
                op.push_back({1, l, r});
            }
        }
        MajRange st(a, plan);
        for (auto e : op)
        {
            if (!e[0])
            {
                st.set(e[1], e[2]);
                a[e[1]] = e[2];
                continue;
            }
            int l = e[1], r = e[2];
            map<int, int> f;
            optional<int> want;
            for (int i = l; i < r; i++)
            {
                if (2 * ++f[a[i]] > r - l)
                {
                    want = a[i];
                }
            }
            auto got = st.query(l, r);
            assert(got == want);
            acc = acc * 1'000'003 + (got ? (unsigned long long)(*got + 9) : 0);
        }
    }
    return acc;
}

unsigned long long testSortSeg()
{
    mt19937_64 rng(1);
    unsigned long long acc = 0;
    for (int tc = 0; tc < 100; tc++)
    {
        int n = 1 + rng() % 30, K = 1000;
        vector<int> key(n);
        iota(key.begin(), key.end(), 0);
        shuffle(key.begin(), key.end(), rng);
        vector<Aff3> a(n);
        vector<pair<int, Aff3>> b(n);
        set<int> used;
        for (int i = 0; i < n; i++)
        {
            a[i] = {(int)(rng() % mod3), (int)(rng() % mod3)};
            b[i] = {key[i], a[i]};
            used.insert(key[i]);
        }
        SortSeg<AffMonoid3> st(K, key, a);
        auto brute = [&](int l, int r)
        {
            Aff3 x = AffMonoid3::id();
            for (int i = l; i < r; i++)
            {
                x = AffMonoid3::op(x, b[i].second);
            }
            return x;
        };
        for (int q = 0; q < 200; q++)
        {
            int l = rng() % (n + 1), r = rng() % (n + 1);
            if (l > r)
            {
                swap(l, r);
            }
            int op = rng() % 4;
            if (!op)
            {
                int p = rng() % n;
                used.erase(b[p].first);
                int k;
                do
                {
                    k = rng() % K;
                } while (used.contains(k));
                used.insert(k);
                Aff3 x{(int)(rng() % mod3), (int)(rng() % mod3)};
                b[p] = {k, x};
                st.assign(p, k, x);
            }
            else if (op == 1)
            {
                sort(b.begin() + l, b.begin() + r, [](auto x, auto y)
                {
                    return x.first < y.first;
                });
                st.sortAsc(l, r);
            }
            else if (op == 2)
            {
                sort(b.begin() + l, b.begin() + r, [](auto x, auto y)
                {
                    return x.first > y.first;
                });
                st.sortDesc(l, r);
            }
            else
            {
                Aff3 x = st.prod(l, r), y = brute(l, r);
                assert(x == y);
                acc ^= (unsigned long long)x.a << 32 ^ x.b;
            }
            assert(st.prodAll() == brute(0, n));
        }
    }
    return acc;
}

unsigned long long testLineMin()
{
    mt19937_64 rng(2);
    unsigned long long acc = 0;
    for (int tc = 0; tc < 150; tc++)
    {
        int n = 1 + rng() % 45;
        vector<long long> a(n);
        for (auto &x : a)
        {
            x = (long long)(rng() % 401) - 200;
        }
        LineMinSeg st(a);
        for (int q = 0; q < 200; q++)
        {
            int l = rng() % n, r = l + 1 + rng() % (n - l);
            if (rng() & 1)
            {
                long long b = (long long)(rng() % 21) - 10;
                long long c = (long long)(rng() % 41) - 20;
                st.add(l, r, b, c);
                for (int i = l; i < r; i++)
                {
                    a[i] += b * i + c;
                }
            }
            else
            {
                long long x = *min_element(a.begin() + l, a.begin() + r);
                long long y = st.min(l, r);
                assert(x == y);
                acc = acc * 1'000'003 + (unsigned long long)y;
            }
        }
    }
    return acc;
}

unsigned long long testRangeMode()
{
    mt19937_64 rng(3);
    unsigned long long acc = 0;
    for (int tc = 0; tc < 100; tc++)
    {
        int n = 1 + rng() % 60;
        vector<int> a(n);
        for (auto &x : a)
        {
            x = (int)(rng() % 11) - 5;
        }
        RangeMode st(a);
        for (int q = 0; q < 100; q++)
        {
            int l = rng() % (n + 1), r = rng() % (n + 1);
            if (l > r)
            {
                swap(l, r);
            }
            auto got = st.query(l, r);
            if (l == r)
            {
                assert(!got);
                continue;
            }
            map<int, int> f;
            int mx = 0;
            for (int i = l; i < r; i++)
            {
                mx = max(mx, ++f[a[i]]);
            }
            assert(got && got->second == mx && f[got->first] == mx);
            acc = acc * 1'000'003 + (unsigned long long)(got->first + 7) * 97 + got->second;
        }
    }
    return acc;
}

signed main()
{
    testRangeDSU();
    unsigned long long acc = testMajority();
    acc ^= testSortSeg();
    acc ^= testLineMin();
    acc ^= testRangeMode();
    cout << acc << '\n';
    return 0;
}
