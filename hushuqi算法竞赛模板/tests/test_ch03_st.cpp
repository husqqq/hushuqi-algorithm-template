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

template <class T, class F> void test(const vector<T> &v, F f, int seed)
{
    ST<T, F> st(v, f);
    if (st.n != (int)v.size() || (v.empty() && !st.a.empty()))
    {
        cout << "WA " << seed << '\n';
        exit(1);
    }
    // 独立按原数组从左向右折叠；不调用另一种区间查询实现。
    for (int l = 0; l < (int)v.size(); l++)
    {
        T ans = v[l];
        for (int r = l + 1; r <= (int)v.size(); r++)
        {
            if (r > l + 1) ans = f(ans, v[r - 1]);
            if (st.query(l, r) != ans)
            {
                cout << "WA " << seed << '\n';
                cerr << "n=" << v.size() << " l=" << l << " r=" << r << '\n';
                exit(1);
            }
        }
    }
}

signed main()
{
    for (int seed = 0; seed < 8; seed++)
    {
        mt19937_64 rng(seed);
        for (int n : {0, 1, 2, 3, 7, 8, 9, 15, 16, 17, 31, 32, 33, 63, 64, 65, 127})
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
                test(v, Op{kind}, seed);
            }
            test(a, Band{}, seed);
        }
        vector<int> edge{LLONG_MIN, LLONG_MAX, 0, -1, LLONG_MIN, LLONG_MAX};
        test(edge, Op{0}, seed);
        test(edge, Op{1}, seed);
        cout << "AC " << seed << '\n';
    }
}
