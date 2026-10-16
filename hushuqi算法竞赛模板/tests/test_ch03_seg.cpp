#ifdef TEMPLATE_PRINT
#include "../include/print/3_3_001.hpp"
#include "../include/print/1_1_008.hpp"
#else
#include "../include/full/3_3_001.hpp"
#include "../include/full/1_1_008.hpp"
#endif

struct Text
{
    string s;
    friend Text operator+(const Text &a, const Text &b)
    {
        return {a.s + b.s};
    }
};

struct Aff
{
    Z a = 1, b = 0;
    friend Aff operator+(Aff f, Aff g)
    {
        return {g.a * f.a, g.a * f.b + g.b};
    }
};

void check(bool ok, int seed, int step, int l, int r, const char *what)
{
    if (ok) return;
    cout << "WA " << seed << '\n';
    cerr << what << " step=" << step << " l=" << l << " r=" << r << '\n';
    exit(1);
}

void numeric(int seed)
{
    mt19937_64 rng(seed);
    for (int n : {0, 1, 2, 3, 7, 8, 9, 15, 16, 17, 31, 32, 33, 63, 64, 65, 127})
    {
        vector<int> a(n);
        vector<Sum> v(n);
        vector<MaxInfo> w(n);
        for (int i = 0; i < n; i++)
        {
            a[i] = (int)(rng() % 101) - 50;
            v[i] = Sum(a[i]);
            w[i] = MaxInfo(a[i]);
        }
        Seg<Sum> sum(v);
        Seg<MaxInfo> mx(w);
        for (int step = 0; step < 600; step++)
        {
            if (n && step % 3 == 0)
            {
                int p = rng() % n;
                a[p] = (int)(rng() % 101) - 50;
                sum.set(p, Sum(a[p]));
                mx.set(p, MaxInfo(a[p]));
            }
            int l = rng() % (n + 1), r = rng() % (n + 1);
            if (l > r) swap(l, r);
            if (step % 17 == 0) l = 0, r = n;
            int total = 0, high = LLONG_MIN;
            int x = (int)(rng() % 111) - 55, first = -1, last = -1;
            for (int i = l; i < r; i++)
            {
                total += a[i];
                high = max(high, a[i]);
                if (a[i] >= x)
                {
                    if (first == -1) first = i;
                    last = i;
                }
            }
            const auto &cs = sum;
            const auto &cm = mx;
            check(cs.query(l, r).val == total, seed, step, l, r, "sum");
            check(cm.query(l, r).val == high, seed, step, l, r, "max");
            int calls = 0;
            auto ok = [&](MaxInfo z) { calls++; return z.val >= x; };
            check(cm.findFirst(l, r, ok) == first, seed, step, l, r, "first");
            check(calls <= 8 * (int)bit_width((unsigned long long)(n + 1)) + 8, seed, step, l, r, "first pruning");
            calls = 0;
            check(cm.findLast(l, r, ok) == last, seed, step, l, r, "last");
            check(calls <= 8 * (int)bit_width((unsigned long long)(n + 1)) + 8, seed, step, l, r, "last pruning");
        }
        // 重建到更长、空、原长度；长度构造的每个叶子均为单位元。
        sum.init(n + 5);
        check(sum.query(0, n + 5).val == 0, seed, -1, 0, n + 5, "resize");
        mx.init(0);
        check(mx.query(0, 0).val == LLONG_MIN, seed, -1, 0, 0, "empty rebuild");
        mx.init(w);
        check(mx.n == n, seed, -1, 0, n, "vector rebuild");
        for (int i = 0; i < n; i++)
        {
            check(mx.query(i, i + 1).val == w[i].val, seed, -1, i, i + 1, "rebuild leaves");
        }
    }
    // 最小负数既是 max 单位元，也可以是有效叶子；填充叶子不得成为搜索答案。
    Seg<MaxInfo> edge(vector<MaxInfo>{{LLONG_MIN}, {LLONG_MAX}, {LLONG_MIN}});
    auto all = [](MaxInfo z) { return z.val >= LLONG_MIN; };
    check(edge.findFirst(0, 3, all) == 0 && edge.findLast(0, 3, all) == 2, seed, -1, 0, 3, "extreme leaves");
    check(edge.findLast(3, 3, all) == -1, seed, -1, 3, 3, "empty search");
    // 非负区间和大于阈值只提供安全剪枝，可能有假阳性。
    vector<Sum> pos(65, Sum{1});
    pos[20] = Sum{5}; pos[50] = Sum{5};
    Seg<Sum> weak(pos);
    auto gt3 = [](Sum z) { return z.val > 3; };
    check(weak.findFirst(0, 65, gt3) == 20 && weak.findLast(0, 65, gt3) == 50, seed, -1, 0, 65, "false positives");
    check(weak.findFirst(0, 20, gt3) == -1, seed, -1, 0, 20, "false positives no answer");
}

void text(int seed)
{
    mt19937_64 rng(seed + 123);
    for (int n : {0, 1, 2, 3, 7, 8, 9, 31, 32, 33})
    {
        vector<Text> v(n);
        for (int i = 0; i < n; i++)
        {
            v[i].s = string(1, 'a' + rng() % 26);
        }
        Seg<Text> st(v);
        for (int step = 0; step < 100; step++)
        {
            if (n)
            {
                int p = rng() % n;
                v[p].s = step % 5 ? string(1, 'A' + rng() % 26) : "";
                st.set(p, v[p]);
            }
            for (int l = 0; l <= n; l++)
            {
                string s;
                for (int r = l; r <= n; r++)
                {
                    if (r > l) s += v[r - 1].s;
                    check(st.query(l, r).s == s, seed, step, l, r, "ordered text");
                }
            }
        }
        st.init(0);
        check(st.query(0, 0).s.empty(), seed, -1, 0, 0, "text identity");
    }
}

void affine(int seed)
{
    // 固定核对复合方向、零斜率及恒等函数。
    Seg<Aff> fixed(vector<Aff>{{Z(2), Z(1)}, {Z(3), Z(4)}, {Z(0), Z(mod - 1)}, {Z(1), Z(0)}});
    auto first = fixed.query(0, 2);
    check((first.a * Z(5) + first.b).val() == 37, seed, -1, 0, 2, "fixed affine order");
    auto all = fixed.query(0, 4);
    check(all.a.val() == 0 && all.b.val() == mod - 1, seed, -1, 0, 4, "constant function");
    mt19937_64 rng(seed + 456);
    for (int n : {0, 1, 2, 3, 7, 8, 9, 31, 32, 33, 64, 65, 127})
    {
        // 每项为函数斜率、截距；暴力逐函数求值，不调用 Aff::operator+。
        vector<array<int, 2>> a(n);
        vector<Aff> v(n);
        for (int i = 0; i < n; i++)
        {
            a[i] = {(int)(rng() % mod), (int)(rng() % mod)};
            v[i] = {Z(a[i][0]), Z(a[i][1])};
        }
        Seg<Aff> st(v);
        for (int step = 0; step < 800; step++)
        {
            if (n && step % 2 == 0)
            {
                int p = rng() % n;
                a[p] = {(int)(rng() % mod), (int)(rng() % mod)};
                if (step % 11 == 0) a[p] = {0, mod - 1};
                if (step % 13 == 0) a[p] = {1, 0};
                st.set(p, {Z(a[p][0]), Z(a[p][1])});
            }
            int l = rng() % (n + 1), r = rng() % (n + 1);
            if (l > r) swap(l, r);
            if (step % 17 == 0) l = 0, r = n;
            int x = step % 3 ? (int)(rng() % mod) : mod - 1;
            int y = x, y0 = 0, y1 = 1;
            for (int i = l; i < r; i++)
            {
                y = (a[i][0] * y + a[i][1]) % mod;
                y0 = (a[i][0] * y0 + a[i][1]) % mod;
                y1 = (a[i][0] * y1 + a[i][1]) % mod;
            }
            const auto &cs = st;
            auto f = cs.query(l, r);
            check((f.a * Z(x) + f.b).val() == y, seed, step, l, r, "affine evaluation");
            check(f.b.val() == y0 && f.a.val() == (y1 - y0 + mod) % mod, seed, step, l, r, "affine coefficients");
        }
        st.init(n + 3);
        auto f = st.query(0, n + 3);
        check(f.a.val() == 1 && f.b.val() == 0, seed, -1, 0, n + 3, "affine identity");
        st.init(v);
        for (int i = 0; i < n; i++)
        {
            auto g = st.query(i, i + 1);
            check(g.a == v[i].a && g.b == v[i].b, seed, -1, i, i + 1, "affine rebuild");
        }
    }
}

signed main()
{
    for (int seed = 0; seed < 16; seed++)
    {
        numeric(seed);
        text(seed);
        affine(seed);
        cout << "AC " << seed << '\n';
    }
}
