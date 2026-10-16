#include "../include/full/3_7_007.hpp"
#include "../include/full/1_1_008.hpp"

int seed;

void check(bool ok)
{
    if (!ok)
    {
        cout << "WA " << seed << '\n';
        exit(1);
    }
}

struct Counted
{
    static int copied;
    int x;

    explicit Counted(int v) : x(v) {}
    Counted(const Counted &o) : x(o.x) { copied++; }
    Counted(Counted &&o) noexcept : x(o.x) { copied++; }
    Counted &operator=(const Counted &o)
    {
        x = o.x;
        copied++;
        return *this;
    }
};
int Counted::copied = 0;

void endpoints()
{
    int calls = 0;
    auto op = [&calls](const Counted &a, const Counted &b)
    {
        calls++;
        return Counted(a.x + b.x);
    };
    static_assert(!is_default_constructible_v<Counted>);
    static_assert(!is_default_constructible_v<decltype(op)>);
    for (int side = 0; side < 2; side++)
    {
        AggDeque<Counted, decltype(op)> q(op, Counted(0));
        check(q.empty() && q.size() == 0 && q.query().x == 0);
        const int n = 4097;
        for (int i = 1; i <= n; i++)
        {
            if (side) q.pushFront(Counted(i));
            else q.pushBack(Counted(i));
        }
        Counted::copied = 0;
        const auto &cq = q;
        static_assert(is_same_v<decltype(cq.front()), const Counted &>);
        static_assert(is_same_v<decltype(cq.back()), const Counted &>);
        for (int i = 0; i < 1000; i++)
        {
            check(cq.front().x == (side ? n : 1));
            check(cq.back().x == (side ? 1 : n));
        }
        check(Counted::copied == 0);
        int sum = n * (n + 1) / 2;
        calls = 0;
        for (int i = 1; i <= n; i++)
        {
            check(q.query().x == sum);
            if (side) q.popBack();
            else q.popFront();
            sum -= i;
        }
        check(q.empty() && q.query().x == 0);
        check(calls < 8 * n);
        check(Counted::copied < 64 * n);
        q.pushFront(Counted(17));
        q.popBack();
        q.pushBack(Counted(-23));
        q.popFront();
        check(q.empty() && q.query().x == 0);
    }
}

void texts()
{
    auto op = [](const string &a, const string &b) { return a + b; };
    AggDeque<string, decltype(op)> q(op, "");
    q.pushBack("b");
    q.pushFront("a");
    q.pushBack("c");
    check(q.query() == "abc");
    q.popFront();
    check(q.query() == "bc");
    q.popBack();
    q.pushFront("a");
    check(q.query() == "ab");
    auto copy = q;
    q.popBack();
    q.popFront();
    check(q.empty() && q.query().empty());
    check(copy.query() == "ab");
}

void affine()
{
    using Aff = array<Z, 2>; // 依次为斜率、截距，表示 x -> ax+b。
    auto op = [](const Aff &a, const Aff &b) -> Aff
    {
        return {b[0] * a[0], b[0] * a[1] + b[1]};
    };
    AggDeque<Aff, decltype(op)> q(op, Aff{1, 0});
    deque<Aff> a;
    mt19937_64 rng(seed);
    for (int step = 0; step < 6000; step++)
    {
        int mode = a.empty() ? rng() % 2 : rng() % 6;
        if (a.size() > 100) mode = 2 + rng() % 2;
        if (mode < 2)
        {
            Aff f{Z((long long)(rng() % mod)), Z((long long)(rng() % mod))};
            if (step % 13 == 0) f[0] = 0;
            if (mode == 0)
            {
                a.push_front(f);
                q.pushFront(f);
            }
            else
            {
                a.push_back(f);
                q.pushBack(f);
            }
        }
        else if (mode == 2)
        {
            a.pop_front();
            q.popFront();
        }
        else if (mode == 3)
        {
            a.pop_back();
            q.popBack();
        }
        const auto &cq = q;
        check(cq.size() == (int)a.size() && cq.empty() == a.empty());
        if (!a.empty()) check(cq.front() == a.front() && cq.back() == a.back());
        Aff f = cq.query();
        // 直接依次作用于三个输入，独立检查聚合方向，不再调用合并回调。
        for (Z x : {Z(0), Z(1), Z((long long)(rng() % mod))})
        {
            Z y = x;
            for (const auto &g : a)
            {
                y = g[0] * y + g[1];
            }
            check(f[0] * x + f[1] == y);
        }
    }
    int side = 0;
    while (!a.empty())
    {
        check(q.front() == a.front() && q.back() == a.back());
        if (side++ % 2)
        {
            a.pop_front();
            q.popFront();
        }
        else
        {
            a.pop_back();
            q.popBack();
        }
    }
    check(q.empty() && q.query() == Aff{1, 0});
}

signed main(signed argc, char **argv)
{
    int first = argc > 1 ? stoll(argv[1]) : 0;
    int last = argc > 1 ? first + 1 : 16;
    for (seed = first; seed < last; seed++)
    {
        texts();
        endpoints();
        affine();
        cout << "AC " << seed << '\n';
    }
}
