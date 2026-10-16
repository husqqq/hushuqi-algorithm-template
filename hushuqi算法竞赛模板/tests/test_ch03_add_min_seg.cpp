#ifndef MIN_HEADER
#define MIN_HEADER "../include/full/3_3_004.hpp"
#endif
#include MIN_HEADER

void check(bool ok)
{
    // ok 是独立预期是否成立；失败抛出异常，不受 NDEBUG 影响。
    if (!ok) throw runtime_error("区间加最小值结果不符");
}

void verify(AddMinSeg &seg, const vector<long long> &a, int l, int r, long long bound)
{
    // seg 是被测结构，a 是独立数组，[l,r) 是查询范围，bound 是最值搜索阈值。
    long long want = numeric_limits<long long>::max();
    int first = -1, last = -1;
    for (int i = l; i < r; i++)
    {
        want = min(want, a[i]);
        if (a[i] <= bound)
        {
            if (first == -1) first = i;
            last = i;
        }
    }
    check(seg.query(l, r).mn == want);
    auto ok = [bound](MinInfo x) { return x.mn <= bound; };
    check(seg.findFirst(l, r, ok) == first);
    check(seg.findLast(l, r, ok) == last);
}

signed main()
{
    AddMinSeg st(vector<MinInfo>{5, 1, 7, -2, 4});
    check(st.query(0, 5).mn == -2);
    st.apply(1, 4, Tag{3});
    check(st.query(0, 5).mn == 1);
    check(st.query(2, 4).mn == 1);
    st.apply(0, 2, Tag{-5});
    check(st.query(0, 3).mn == -1);
    st.apply(2, 2, Tag{100});
    check(st.query(2, 3).mn == 10);

    long long lo = numeric_limits<long long>::lowest();
    long long hi = numeric_limits<long long>::max();
    AddMinSeg edge(vector<MinInfo>{lo, hi, hi - 1});
    edge.apply(0, 1, Tag{1});
    edge.apply(1, 2, Tag{-1});
    edge.apply(2, 3, Tag{1});
    check(edge.get(0).mn == lo + 1);
    check(edge.get(1).mn == hi - 1);
    check(edge.get(2).mn == hi);
    check(edge.query(2, 3).mn == hi);
    edge.apply(0, 0, Tag{hi}); // 空区间不得执行任何加法。
    check(edge.query(0, 0).mn == hi);
    AddMinSeg late(vector<MinInfo>{-5, 3, 7, 0, 12});
    late.apply(0, 5, Tag{20});
    late.apply(1, 4, Tag{-8});
    late.apply(0, 5, Tag{5});
    const AddMinSeg &view = late;
    check(view.get(0).mn == 20 && view.get(2).mn == 24);
    late.init(vector<MinInfo>{hi, lo});
    check(late.query(0, 1).mn == hi && late.query(1, 2).mn == lo);
    late.init(0);
    late.apply(0, 0, Tag{hi});
    verify(late, {}, 0, 0, 0);
    late.init(3);
    check(late.query(0, 3).mn == 0);
    late.apply(0, 3, Tag{3000000000LL});
    check(late.query(0, 3).mn == 3000000000LL);

    for (int seed = 0; seed < 16; seed++)
    {
        try
        {
            mt19937_64 rng(20261016 + seed);
            for (int n : {0, 1, 2, 3, 5, 8, 17, 31})
            {
                vector<long long> a(n);
                vector<MinInfo> init(n);
                for (int i = 0; i < n; i++)
                {
                    a[i] = (long long)(rng() % 2001) - 1000;
                    init[i] = MinInfo(a[i]);
                }
                AddMinSeg seg(init);
                for (int t = 0; t < 250; t++)
                {
                    int l = rng() % (n + 1), r = rng() % (n + 1);
                    if (l > r) swap(l, r);
                    long long v = (long long)(rng() % 2001) - 1000;
                    seg.apply(l, r, Tag{v});
                    for (int i = l; i < r; i++)
                    {
                        a[i] += v;
                    }
                    int ql = rng() % (n + 1), qr = rng() % (n + 1);
                    if (ql > qr) swap(ql, qr);
                    long long bound = (long long)(rng() % 10001) - 5000;
                    verify(seg, a, ql, qr, bound);
                    if (n)
                    {
                        int i = rng() % n;
                        const AddMinSeg &ref = seg;
                        check(ref.get(i).mn == a[i]);
                    }
                    if (t == 124)
                    {
                        seg.init(init);
                        for (int i = 0; i < n; i++)
                        {
                            a[i] = init[i].mn;
                        }
                    }
                }
                for (int l = 0; l <= n; l++)
                {
                    for (int r = l; r <= n; r++)
                    {
                        verify(seg, a, l, r, 0);
                    }
                }
            }
        }
        catch (...)
        {
            cout << "WA " << seed << '\n';
            throw;
        }
        cout << "AC " << seed << '\n';
    }
}
