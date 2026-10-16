#ifdef TEMPLATE_PRINT
#include "../include/print/3_3_017.hpp"
#else
#include "../include/full/3_3_017.hpp"
#endif

void check(bool ok)
{
    // ok 是预期条件；关闭断言后仍执行检查。
    if (!ok)
    {
        abort();
    }
}

optional<int> brute(const vector<int> &a, int l, int r)
{
    // a 是当前数组，[l,r) 是区间；返回严格过半值，不存在时返回空。
    map<int, int> cnt;
    for (int i = l; i < r; i++)
    {
        if (++cnt[a[i]] > (r - l) / 2)
        {
            return a[i];
        }
    }
    return nullopt;
}

void allRanges(const MajRange &st, const vector<int> &a)
{
    // st 与 a 表示同一数组；逐一核对全部半开区间，包括空区间。
    for (int l = 0; l <= (int)a.size(); l++)
    {
        for (int r = l; r <= (int)a.size(); r++)
        {
            check(st.query(l, r) == brute(a, l, r));
        }
    }
}

void fixed()
{
    // 固定覆盖空数组、剩余票非零但无解、恰好一半、负值与整数端点。
    for (vector<int> a : {vector<int>{}, {0}, {0, 1, 2}, {0, 0, 1, 2},
                         {1, 2, 1, 3, 1}, {-1, -1, 0},
                         {LLONG_MIN, LLONG_MAX, LLONG_MIN}})
    {
        MajRange st(a);
        allRanges(st, a);
        for (int i = 0; i < (int)a.size(); i++)
        {
            // 初始的 {位置,值} 已自动登记，同值赋值不需要额外 plan。
            st.set(i, a[i]);
        }
        allRanges(st, a);
    }

    vector<int> a{LLONG_MIN, LLONG_MAX, 0, -1, 0};
    vector<pair<int, int>> plan{{0, 0}, {4, -1}, {0, 0}, {2, LLONG_MAX}};
    MajRange st(a, plan);
    for (auto [p, x] : plan)
    {
        st.set(p, x);
        a[p] = x;
        allRanges(st, a);
    }
}

void exhaustive()
{
    // 穷举长度不超过 8 的三值数组，核对每个区间的候选验证。
    int count = 1;
    for (int n = 0; n <= 8; n++, count *= 3)
    {
        for (int mask = 0; mask < count; mask++)
        {
            vector<int> a(n);
            int x = mask;
            for (int &v : a)
            {
                v = x % 3 - 1;
                x /= 3;
            }
            MajRange st(a);
            allRanges(st, a);
        }
    }
}

void randomTest(int seed)
{
    // seed 固定生成可复现的赋值和查询；每个未来赋值都预登记。
    mt19937_64 rng(seed);
    int n = 1 + rng() % 80;
    vector<int> a(n);
    for (int &x : a)
    {
        x = (int)(rng() % 7) - 3;
    }
    vector<array<int, 3>> ops; // 每项为 {操作类型,位置或左端点,新值或右端点}。
    vector<pair<int, int>> plan;
    for (int i = 0; i < 1500; i++)
    {
        if (rng() % 3 == 0)
        {
            int p = rng() % n;
            int x = (int)(rng() % 17) - 8;
            if (i % 11 == 0) x = LLONG_MIN;
            if (i % 13 == 0) x = LLONG_MAX;
            ops.push_back({0, p, x});
            plan.push_back({p, x});
            if (i % 5 == 0) plan.push_back({p, x});
        }
        else
        {
            int l = rng() % (n + 1), r = rng() % (n + 1);
            if (l > r) swap(l, r);
            ops.push_back({1, l, r});
        }
    }
    shuffle(plan.begin(), plan.end(), rng);
    MajRange st(a, plan);
    for (auto [type, x, y] : ops)
    {
        if (type == 0)
        {
            st.set(x, y);
            a[x] = y;
        }
        else
        {
            check(st.query(x, y) == brute(a, x, y));
        }
    }
    allRanges(st, a);
}

signed main()
{
    fixed();
    exhaustive();
    for (int seed = 0; seed < 64; seed++)
    {
        randomTest(seed);
        cout << "AC " << seed << '\n';
    }
}
