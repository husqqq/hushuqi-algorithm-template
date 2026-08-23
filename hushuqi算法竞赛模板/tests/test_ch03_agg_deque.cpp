#include "../include/full/3_7_007.hpp"

struct JoinText
{
    string operator()(const string &a, const string &b) const
    {
        return a + b;
    }
};

struct Counted
{
    static int copied;
    int x = 0;

    Counted() = default;
    Counted(int v) : x(v)
    {
    }
    Counted(const Counted &other) : x(other.x)
    {
        copied++;
    }
    Counted(Counted &&other) noexcept : x(other.x)
    {
        copied++;
    }
    Counted &operator=(const Counted &other)
    {
        x = other.x;
        copied++;
        return *this;
    }
    Counted &operator=(Counted &&other) noexcept
    {
        x = other.x;
        copied++;
        return *this;
    }
};

int Counted::copied = 0;

struct SumCounted
{
    Counted operator()(const Counted &a, const Counted &b) const
    {
        return Counted(a.x + b.x);
    }
};

constexpr int mod = 998244353;

struct Aff
{
    int a = 1, b = 0;
};

struct Compose
{
    Aff operator()(const Aff &x, const Aff &y) const
    {
        return {y.a * x.a % mod, (y.a * x.b + y.b) % mod};
    }
};

Aff fold(const deque<Aff> &a)
{
    Aff ans;
    for (const Aff &x : a)
    {
        ans = Compose{}(ans, x);
    }
    return ans;
}

signed main()
{
    AggDeque<string, JoinText> q(JoinText{}, "");
    q.pushBack("b");
    q.pushFront("a");
    q.pushBack("c");
    assert(q.query() == "abc");
    assert(q.front() == "a" && q.back() == "c");
    q.popFront();
    assert(q.query() == "bc");
    q.popBack();
    q.pushFront("a");
    assert(q.query() == "ab");
    q.popBack();
    q.popFront();
    assert(q.empty() && q.query().empty());

    AggDeque<Counted, SumCounted> oneSide(SumCounted{}, Counted{});
    for (int i = 0; i < 100; i++)
    {
        oneSide.pushBack(Counted(i));
    }
    Counted::copied = 0;
    assert(oneSide.front().x == 0 && oneSide.back().x == 99);
    // 端点读取只复制返回值，不能触发把整列搬到另一侧的 rebuild。
    assert(Counted::copied <= 2);

    mt19937_64 rng(0);
    for (int tc = 0; tc < 100; tc++)
    {
        AggDeque<Aff, Compose> aq(Compose{}, Aff{});
        deque<Aff> brute;
        for (int step = 0; step < 300; step++)
        {
            int op = brute.empty() ? rng() % 2 : rng() % 6;
            if (op < 2)
            {
                Aff x{1 + (int)(rng() % (mod - 1)), (int)(rng() % mod)};
                if (!op)
                {
                    aq.pushFront(x);
                    brute.push_front(x);
                }
                else
                {
                    aq.pushBack(x);
                    brute.push_back(x);
                }
            }
            else if (op == 2)
            {
                aq.popFront();
                brute.pop_front();
            }
            else if (op == 3)
            {
                aq.popBack();
                brute.pop_back();
            }
            else
            {
                Aff x = fold(brute), y = aq.query();
                assert(x.a == y.a && x.b == y.b);
                assert(aq.front().a == brute.front().a && aq.front().b == brute.front().b);
                assert(aq.back().a == brute.back().a && aq.back().b == brute.back().b);
            }
        }
    }
    cout << "OK\n";
}
