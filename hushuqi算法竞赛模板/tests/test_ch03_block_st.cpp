#ifdef TEMPLATE_PRINT
#include "../include/print/3_4_001.hpp"
#else
#include "../include/full/3_4_001.hpp"
#endif

struct Op
{
    int kind;
    int operator()(int x, int y) const
    {
        switch (kind)
        {
        case 0: return min(x, y);
        case 1: return max(x, y);
        case 2: return gcd(x, y);
        case 3: return x & y;
        case 4: return x | y;
        case 5: return x;
        default: return y;
        }
    }
};

struct Band
{
    array<int, 2> operator()(array<int, 2> x, array<int, 2> y) const
    {
        return {x[0], y[1]};
    }
};

template <class T, class F, int B> void test(const vector<T> &v, F f, int seed)
{
    BlockST<T, F, B> st({}, f);
    auto check = [&](bool ok, int l, int r)
    {
        if (ok) return;
        cout << "WA " << seed << '\n';
        cerr << "B=" << B << " n=" << v.size() << " l=" << l << " r=" << r << '\n';
        exit(1);
    };
    auto verify = [&]
    {
        check(st.n == v.size() && st.m == (v.size() + B - 1) / B, -1, -1);
        // 独立左折叠，覆盖同块、相邻块及含多个完整中间块的所有区间。
        for (int l = 0; l < (int)v.size(); l++)
        {
            T ans = v[l];
            for (int r = l + 1; r <= (int)v.size(); r++)
            {
                if (r > l + 1) ans = f(ans, v[r - 1]);
                check(st.query(l, r) == ans, l, r);
            }
        }
    };
    st.build(v);
    verify();
    st.build({});
    check(st.n == 0 && st.m == 0 && st.val.empty() && st.pre.empty() && st.suf.empty() && st.st.empty(), -1, -1);
    if (!v.empty())
    {
        st.build({v.back()});
        check(st.query(0, 1) == v.back(), 0, 1);
    }
    st.build(v);
    verify(); // 重建须保留有状态的合并运算。
}

template <int B> void run(int seed)
{
    mt19937_64 rng(seed);
    for (int n : {0LL, 1LL, 2LL, 3LL, B - 1, B, B + 1, 2 * B - 1, 2 * B, 2 * B + 1, 3 * B, 4 * B + 1})
    {
        vector<int> v(n);
        vector<array<int, 2>> a(n);
        for (int i = 0; i < n; i++)
        {
            v[i] = rng() % 1024;
            a[i] = {i, n - i};
        }
        for (int kind = 0; kind < 7; kind++)
        {
            test<int, Op, B>(v, Op{kind}, seed);
        }
        test<array<int, 2>, Band, B>(a, Band{}, seed);
    }
    vector<int> edge{LLONG_MIN, LLONG_MAX, 0, -1, LLONG_MIN, LLONG_MAX};
    test<int, Op, B>(edge, Op{0}, seed);
    test<int, Op, B>(edge, Op{1}, seed);
}

signed main()
{
    for (int seed = 0; seed < 8; seed++)
    {
        run<1>(seed);
        run<2>(seed);
        run<3>(seed);
        run<16>(seed);
        run<31>(seed);
        cout << "AC " << seed << '\n';
    }
}
