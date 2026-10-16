#ifdef TEMPLATE_PRINT
#include "../include/print/3_5_007.hpp"
#else
#include "../include/full/3_5_007.hpp"
#endif

void check(bool ok)
{
    // ok 是预期条件；关闭断言后仍执行检查。
    if (!ok)
    {
        abort();
    }
}

void fixed()
{
    // 重复键只删一份；最小负数也直接比较，不取反。
    OrderedMSet s{LLONG_MIN, 7, 7, LLONG_MAX};
    check(*s.begin() == LLONG_MIN && *s.rbegin() == LLONG_MAX);
    s.erase(s.begin());
    s.erase(prev(s.end()));
    check(s.size() == 2 && *s.begin() == 7 && *s.rbegin() == 7);
    s.erase(s.begin());
    check(s.size() == 1 && *s.begin() == 7);
    s.erase(prev(s.end()));
    check(s.empty());
    s.insert(0);
    check(s.size() == 1 && *s.begin() == 0);
    OrderedSet unique{7, 7};
    check(unique.size() == 1);
}

void randomTest(int seed)
{
    // seed 固定生成操作；用数组直接找最值，避免拿同一种容器互相核对。
    OrderedMSet actual;
    vector<int> expected;
    mt19937_64 rng(seed);
    for (int q = 0; q < 4000; q++)
    {
        int op = rng() % 3;
        int x = (int)(rng() % 101) - 50;
        if (q % 17 == 0) x = LLONG_MIN;
        if (q % 19 == 0) x = LLONG_MAX;
        if (!op || expected.empty())
        {
            actual.insert(x);
            expected.push_back(x);
        }
        else
        {
            auto it = op == 1 ? actual.begin() : prev(actual.end());
            auto p = op == 1 ? min_element(expected.begin(), expected.end())
                             : max_element(expected.begin(), expected.end());
            check(*it == *p);
            actual.erase(it);
            expected.erase(p);
        }
        check(actual.size() == expected.size());
        if (!expected.empty())
        {
            check(*actual.begin() == *min_element(expected.begin(), expected.end()));
            check(*actual.rbegin() == *max_element(expected.begin(), expected.end()));
        }
    }
    sort(expected.begin(), expected.end());
    check(equal(actual.begin(), actual.end(), expected.begin(), expected.end()));
}

signed main()
{
    fixed();
    for (int seed = 0; seed < 64; seed++)
    {
        randomTest(seed);
        cout << "AC " << seed << '\n';
    }
}
