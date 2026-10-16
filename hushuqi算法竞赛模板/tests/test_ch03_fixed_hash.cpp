#ifdef TEMPLATE_PRINT
#include "../include/print/3_6_006.hpp"
#else
#include "../include/full/3_6_006.hpp"
#endif

void check(bool ok)
{
    // ok 是预期条件；关闭断言后仍实际执行被测调用与检查。
    if (!ok)
    {
        abort();
    }
}

struct ConstHash
{
    size_t operator()(int x) const
    {
        // x 是待哈希键；故意让全部键碰撞，验证探测链与回绕。
        return 1;
    }
};

void collision()
{
    // 保留一个空槽，覆盖接近容量的探测与 clear 后默认值重置。
    FixedHashMap<int, int, 4, ConstHash> mp;
    FixedHashSet<int, 4, ConstHash> st;
    for (int i = 0; i < 15; i++)
    {
        mp[i] = i + 100;
        check(st.insert(i));
    }
    const auto &view = mp;
    for (int i = 0; i < 15; i++)
    {
        check(view.find(i) && *view.find(i) == i + 100);
        check(st.contains(i) && !st.insert(i));
    }
    check(!view.find(99) && !st.contains(99));
    check(view.get(99) == 0 && view.size() == 15 && st.size() == 15);
    mp.clear();
    st.clear();
    check(mp.size() == 0 && st.size() == 0);
    check(!mp.find(1) && !st.contains(1));
    check(mp[1] == 0 && mp.size() == 1);
    check(st.insert(1) && st.size() == 1);
}

signed main()
{
    collision();
    mt19937_64 rng(0);
    FixedHashMap<int, int, 15> mp;
    unordered_map<int, int, CustomHash> ref;
    for (int q = 0; q < 20000; q++)
    {
        int x = rng() % 10000 - 5000;
        if (rng() & 1)
        {
            int v = rng();
            mp[x] = v;
            ref[x] = v;
        }
        else
        {
            auto p = mp.find(x);
            auto it = ref.find(x);
            check((p != nullptr) == (it != ref.end()));
            check(mp.get(x) == (it == ref.end() ? 0 : it->second));
            if (p)
            {
                check(*p == it->second);
            }
        }
    }
    check(mp.size() == ref.size());
    mp.clear();
    check(mp.size() == 0 && !mp.contains(0));

    FixedHashSet<int, 15> st;
    unordered_set<int, CustomHash> refSet;
    for (int q = 0; q < 20000; q++)
    {
        int x = rng() % 10000 - 5000;
        if (rng() & 1)
        {
            check(st.insert(x) == refSet.insert(x).second);
        }
        else
        {
            check(st.contains(x) == refSet.contains(x));
        }
    }
    check(st.insert(-123) == refSet.insert(-123).second);
    check(!st.insert(-123));
    check(st.size() == refSet.size());
    st.clear();
    check(st.size() == 0 && !st.contains(-123));
    cout << "OK\n";
}
