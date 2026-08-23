#include "../include/full/7_1_007.hpp"
#include "../include/full/3_3_007.hpp"
#include "../include/full/3_3_021.hpp"
#include "../include/full/3_3_022.hpp"

using Fn = array<Z, 2>; // 第 0 维是一次项系数，第 1 维是常数项。

struct Compose
{
    Fn operator()(const Fn &f, const Fn &g) const
    {
        // f、g 分别是左段和右段函数；返回先 f 后 g 的复合 g(f(x))。
        return {f[0] * g[0], f[1] * g[0] + g[1]};
    }
};

Z eval(const Fn &f, Z x)
{
    // f 是一次函数，x 是输入；返回 f(x)。
    return f[0] * x + f[1];
}

Fn fold(const vector<Fn> &a, int l, int r)
{
    // a 是函数序列，[l,r) 是半开区间；返回从左到右复合后的函数。
    Fn ans{Z(1), Z(0)};
    Compose op;
    for (int i = l; i < r; i++)
    {
        ans = op(ans, a[i]);
    }
    return ans;
}

void testAffineLazy()
{
    // 用小模仿射操作核对泛型 AffineInfo 和 AffineTag 的复合方向。
    vector<AffineInfo<Z>> fixed;
    for (int x : {1, 2, 3, 4})
    {
        fixed.emplace_back(Z(x));
    }
    LazySeg<AffineInfo<Z>, AffineTag<Z>> ordered(fixed);
    ordered.apply(1, 3, {Z(2), Z(3)});
    ordered.apply(0, 4, {Z(5), Z(7)});
    const auto &view = ordered;
    assert(view.get(0).sum == Z(12));
    assert(view.get(1).sum == Z(42));
    assert(view.get(2).sum == Z(52));
    assert(view.get(3).sum == Z(27));
    ordered.apply(1, 2, {Z(3), Z(1)});
    assert(ordered.get(1).sum == Z(127));
    assert(ordered.query(0, 4).sum == Z(218));

    vector<Z> pointBase{Z(1), Z(2), Z(3), Z(4)};
    AffPointSeg<Z> pointSeg(pointBase);
    pointSeg.affine(1, 3, Z(2), Z(3));
    pointSeg.affine(0, 4, Z(5), Z(7));
    const auto &pointView = pointSeg;
    assert(pointView.get(0) == Z(12));
    assert(pointView.get(1) == Z(42));
    assert(pointView.get(2) == Z(52));
    assert(pointView.get(3) == Z(27));
    pointSeg.affine(1, 2, Z(3), Z(1));
    assert(pointView.get(1) == Z(127));
    pointSeg.affine(0, 4, Z(0), Z(0));
    assert(pointView.get(0) == Z(0) && pointView.get(3) == Z(0));
    pointSeg.affine(2, 4, Z(1), Z(9));
    assert(pointView.get(1) == Z(0) && pointView.get(2) == Z(9));

    mt19937_64 rng(2026081901);
    for (int tc = 0; tc < 80; tc++)
    {
        int n = 1 + rng() % 35;
        vector<Z> a(n);
        for (Z &x : a)
        {
            x = Z((long long)(rng() % mod));
        }
        vector<AffineInfo<Z>> info;
        info.reserve(n);
        for (Z x : a)
        {
            info.emplace_back(x);
        }
        LazySeg<AffineInfo<Z>, AffineTag<Z>> st(info);
        AffPointSeg<Z> pointSeg(a);
        const auto &pointView = pointSeg;
        for (int step = 0; step < 200; step++)
        {
            int l = rng() % (n + 1), r = rng() % (n + 1);
            if (l > r)
            {
                swap(l, r);
            }
            if (rng() & 1)
            {
                Z mul((long long)(rng() % mod));
                Z add((long long)(rng() % mod));
                st.apply(l, r, {mul, add});
                pointSeg.affine(l, r, mul, add);
                for (int i = l; i < r; i++)
                {
                    a[i] = a[i] * mul + add;
                }
            }
            else
            {
                Z ans = 0;
                for (int i = l; i < r; i++)
                {
                    ans += a[i];
                }
                assert(st.query(l, r).sum == ans);
            }
            int p = rng() % n;
            assert(st.get(p).sum == a[p]);
            assert(pointView.get(p) == a[p]);
        }
    }
}

void testSparsePoint()
{
    // 用函数复合核对稀疏点赋值树的点查、区间查和非交换顺序。
    mt19937_64 rng(2026081902);
    constexpr int n = 127;
    Fn e{Z(1), Z(0)};
    SparsePtSeg<Fn, Compose> st(0, n, e, Compose{});
    st.reserveNodes(n);
    vector<Fn> a(n, e);
    for (int step = 0; step < 5000; step++)
    {
        if (rng() % 3)
        {
            int p = rng() % n;
            Fn f{Z((long long)(rng() % mod)), Z((long long)(rng() % mod))};
            st.set(p, f);
            a[p] = f;
            assert(st.get(p) == f);
        }
        else
        {
            int l = rng() % (n + 1), r = rng() % (n + 1);
            if (l > r)
            {
                swap(l, r);
            }
            assert(st.prod(l, r) == fold(a, l, r));
        }
    }

    DynamicSeg add(-100, 100);
    vector<long long> b(200);
    for (int step = 0; step < 1000; step++)
    {
        int p = (int)(rng() % 200);
        long long v = (long long)(rng() % 101) - 50;
        add.add(p - 100, v);
        b[p] += v;
        int l = rng() % 201, r = rng() % 201;
        if (l > r)
        {
            swap(l, r);
        }
        long long ans = 0;
        for (int i = l; i < r; i++)
        {
            ans += b[i];
        }
        assert(add.sum(l - 100, r - 100) == ans);
    }
}

void testSparseAffine()
{
    // 用全零小数组随机核对动态开点仿射区间和和只读查询携带的外层标记。
    mt19937_64 rng(2026081903);
    for (int tc = 0; tc < 80; tc++)
    {
        int n = 1 + rng() % 63;
        SparseAffSeg<Z> st(0, n);
        vector<Z> a(n);
        for (int step = 0; step < 350; step++)
        {
            int l = rng() % (n + 1), r = rng() % (n + 1);
            if (l > r)
            {
                swap(l, r);
            }
            if (rng() % 3)
            {
                Z mul((long long)(rng() % mod));
                Z add((long long)(rng() % mod));
                st.affine(l, r, mul, add);
                for (int i = l; i < r; i++)
                {
                    a[i] = a[i] * mul + add;
                }
            }
            else
            {
                Z ans = 0;
                for (int i = l; i < r; i++)
                {
                    ans += a[i];
                }
                assert(st.sum(l, r) == ans);
            }
        }
    }

    // 再固定覆盖题目上界附近的整段、两端和跨中点查询。
    constexpr long long n = 1'000'000'000;
    SparseAffSeg<Z> st(0, n);
    st.affine(0, n, Z(1), Z(7));
    assert(st.sum(0, n) == Z(n) * Z(7));
    st.affine(1, n - 1, Z(2), Z(3));
    assert(st.sum(0, n) == Z(14) + Z(n - 2) * Z(17));
    st.affine(0, n / 2, Z(0), Z(5));
    assert(st.sum(0, n / 2) == Z(n / 2) * Z(5));
    assert(st.sum(n / 2, n) == Z(n / 2 - 1) * Z(17) + Z(7));
}

void testRangeSet()
{
    // 用仿射函数序列核对整段替换时的重复复合和查询顺序。
    mt19937_64 rng(2026081904);
    Compose op;
    Fn e{Z(1), Z(0)};
    RangeSetSeg<Fn, Compose> empty({}, e, op);
    assert(empty.prod(0, 0) == e);
    for (int tc = 0; tc < 80; tc++)
    {
        int n = 1 + rng() % 70;
        vector<Fn> a(n);
        for (Fn &f : a)
        {
            f = {Z((long long)(rng() % mod)), Z((long long)(rng() % mod))};
        }
        RangeSetSeg<Fn, Compose> st(a, e, op);
        st.reserveTags(350);
        assert(st.prod(0, 0) == e);
        for (int step = 0; step < 350; step++)
        {
            int l = rng() % (n + 1), r = rng() % (n + 1);
            if (l > r)
            {
                swap(l, r);
            }
            if (rng() & 1)
            {
                Fn f{Z((long long)(rng() % mod)), Z((long long)(rng() % mod))};
                st.assign(l, r, f);
                for (int i = l; i < r; i++)
                {
                    a[i] = f;
                }
            }
            else
            {
                Fn got = st.prod(l, r);
                Fn want = fold(a, l, r);
                assert(got == want);
                Z x((long long)(rng() % mod));
                assert(eval(got, x) == eval(want, x));
            }
        }
    }
}

void testHugeSparsePoint()
{
    // 在题目最大值域的两端和中点验证稀疏点树不依赖值域大小分配节点。
    constexpr long long n = 1'000'000'000;
    Fn e{Z(1), Z(0)};
    SparsePtSeg<Fn, Compose> st(0, n, e, Compose{});
    Fn a{Z(2), Z(3)}, b{Z(5), Z(7)}, c{Z(11), Z(13)};
    st.set(0, a);
    st.set(n / 2, b);
    st.set(n - 1, c);
    assert(st.get(1) == e);
    assert(st.prod(0, 1) == a);
    assert(st.prod(n / 2, n / 2 + 1) == b);
    assert(st.prod(n - 1, n) == c);
    assert(st.prod(0, n) == Compose{}(Compose{}(a, b), c));
}

signed main()
{
    testAffineLazy();
    testSparsePoint();
    testSparseAffine();
    testRangeSet();
    testHugeSparsePoint();
    cout << "OK\n";
    return 0;
}
