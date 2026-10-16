#ifdef TEMPLATE_PRINT
#include "../include/print/3_6_006.hpp"
#else
#include "../include/full/3_6_006.hpp"
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
    // 覆盖整数两端、零值、覆盖写、缺失查询和二元整数键。
    HashMap<int, int> mp;
    for (int x : {LLONG_MIN, LLONG_MAX, -1LL, 0LL, 1LL})
    {
        mp[x] = x;
        check(mp.contains(x) && mp.at(x) == x);
    }
    check(mp.size() == 5);
    auto n = mp.size();
    check(mp.find(17) == mp.end() && mp.size() == n);
    mp[LLONG_MIN] = 0;
    check(mp.at(LLONG_MIN) == 0 && mp.size() == n);
    check(mp.erase(LLONG_MAX) == 1 && !mp.contains(LLONG_MAX));
    mp.clear();
    check(mp.empty());
    check(mp[LLONG_MAX] == 0 && mp.size() == 1);

    HashMap<pair<int, int>, int> pairs;
    vector<pair<int, int>> keys{{0, 0}, {LLONG_MIN, LLONG_MAX},
                                {LLONG_MAX, LLONG_MIN}, {-1, 0}, {0, -1}};
    for (int i = 0; i < (int)keys.size(); i++)
    {
        pairs[keys[i]] = i;
    }
    for (int i = 0; i < (int)keys.size(); i++)
    {
        check(pairs.at(keys[i]) == i);
    }
    check(pairs.size() == keys.size());

    HashMap<unsigned long long, int> wide;
    wide[0] = 1;
    wide[1ULL << 63] = 2;
    wide[ULLONG_MAX] = 3;
    check(wide.size() == 3 && wide.at(0) == 1);
    check(wide.at(1ULL << 63) == 2 && wide.at(ULLONG_MAX) == 3);
}

void randomTest(int seed)
{
    // seed 固定生成操作；map/set 给出独立的有序参照。
    mt19937_64 rng(seed);
    HashMap<int, int> mp;
    HashSet<int> st;
    map<int, int> ref;
    set<int> seen;
    mp.max_load_factor(.7);
    st.max_load_factor(.7);
    mp.reserve(1024);
    st.reserve(1024);
    for (int i = 0; i < 4000; i++)
    {
        int x = (int)(rng() % 501) - 250;
        if (i % 17 == 0) x = LLONG_MIN;
        if (i % 19 == 0) x = LLONG_MAX;
        int op = rng() % 8;
        if (op <= 1)
        {
            int v = (int)(rng() % 1'000'000'000'000'000'001ULL);
            mp[x] = v;
            ref[x] = v;
            check(st.insert(x).second == seen.insert(x).second);
        }
        else if (op == 2)
        {
            check(mp.erase(x) == ref.erase(x));
            check(st.erase(x) == seen.erase(x));
        }
        else if (op == 3)
        {
            check(++mp[x] == ++ref[x]);
        }
        else if (op == 4 && i % 31 == 0)
        {
            mp.clear();
            ref.clear();
            st.clear();
            seen.clear();
        }
        else
        {
            auto a = mp.find(x);
            auto b = ref.find(x);
            check((a == mp.end()) == (b == ref.end()));
            if (b != ref.end()) check(a->second == b->second);
            check(st.contains(x) == seen.contains(x));
        }
        check(mp.size() == ref.size() && st.size() == seen.size());
    }
    for (auto [x, v] : ref)
    {
        check(mp.at(x) == v);
    }
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
