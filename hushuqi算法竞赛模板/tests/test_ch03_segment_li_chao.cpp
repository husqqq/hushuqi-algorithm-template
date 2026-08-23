#include "../include/full/3_7_010.hpp"

struct Segment
{
    // l、r 是有效横坐标半开区间，k、b 定义区间内的直线。
    long long l, r, k, b;
};

optional<__int128> brute(const vector<Segment> &a, long long x)
{
    // a 是全部已插入线段，x 是查询横坐标；返回 x 处有效直线的最小值。
    optional<__int128> ans;
    for (auto [l, r, k, b] : a)
    {
        if (l <= x && x < r)
        {
            __int128 y = (__int128)k * x + b;
            if (!ans || y < *ans)
            {
                ans = y;
            }
        }
    }
    return ans;
}

void testFixed()
{
    // 检查官方样例的半开端点、空答案与重叠线段。
    SegLiChao<long long, __int128> st({-4, -3, -2, -1, 0, 2, 3, 7});
    assert(!st.query(-4));
    st.add({-1, -1}, -3, 3);
    st.add({0, 1}, 0, 7);
    assert(st.query(-1) == optional<__int128>(0));
    assert(st.query(-2) == optional<__int128>(1));
    assert(st.query(0) == optional<__int128>(-1));
    assert(st.query(2) == optional<__int128>(-3));
    assert(st.query(3) == optional<__int128>(1));
    assert(!st.query(7));
    st.add({0, -10}, -4, 2);
    assert(st.query(-2) == optional<__int128>(-10));
    assert(st.query(0) == optional<__int128>(-10));
    assert(st.query(2) == optional<__int128>(-3));

    SegLiChao<long long, __int128> sparse({-5, 0, 5});
    sparse.add({3, 1}, -2, 4);
    assert(!sparse.query(-5));
    assert(sparse.query(0) == optional<__int128>(1));
    assert(!sparse.query(5));

    SegLiChao<long long, __int128> wide({4'000'000'000LL});
    wide.add({4'000'000'000LL, 0}, 0, 4'000'000'001LL);
    __int128 want = (__int128)4'000'000'000LL * 4'000'000'000LL;
    assert(wide.query(4'000'000'000LL) == optional<__int128>(want));
}

unsigned long long testRandom()
{
    // 用固定种子把线段树结果与直接枚举对拍，返回滚动校验和。
    mt19937_64 rng(0);
    unsigned long long acc = 0;
    vector<long long> xs;
    for (long long x = -10; x <= 10; x++)
    {
        xs.push_back(x);
    }
    for (int tc = 0; tc < 200; tc++)
    {
        SegLiChao<long long, __int128> st(xs);
        vector<Segment> a;
        for (int q = 0; q < 100; q++)
        {
            if (rng() & 1)
            {
                long long l = (long long)(rng() % 21) - 10;
                long long r = (long long)(rng() % 21) - 10;
                if (l == r)
                {
                    r = r == 10 ? r - 1 : r + 1;
                }
                if (l > r)
                {
                    swap(l, r);
                }
                long long k = (long long)(rng() % 21) - 10;
                long long b = (long long)(rng() % 41) - 20;
                a.push_back({l, r, k, b});
                st.add({k, b}, l, r);
            }
            else
            {
                long long x = (long long)(rng() % 21) - 10;
                auto got = st.query(x);
                auto want = brute(a, x);
                assert(got == want);
                unsigned long long v = got ? (unsigned long long)*got : 0x9e3779b97f4a7c15ULL;
                acc = acc * 1'000'003 + v;
            }
        }
    }
    return acc;
}

signed main()
{
    testFixed();
    cout << testRandom() << '\n';
    return 0;
}
