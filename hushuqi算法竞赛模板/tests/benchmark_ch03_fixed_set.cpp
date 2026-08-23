#include "../include/full/3_6_006.hpp"

using Clock = chrono::steady_clock;

struct FixedSet
{
    FixedHashSet<unsigned long long, 20> st;

    bool insert(unsigned long long x) { return st.insert(x); }
    bool contains(unsigned long long x) const { return st.contains(x); }
    int size() const { return st.size(); }
};

struct StdSet
{
    HashSet<unsigned long long> st;

    StdSet()
    {
        st.max_load_factor(.7);
        st.reserve(1000000);
    }

    bool insert(unsigned long long x) { return st.insert(x).second; }
    bool contains(unsigned long long x) const { return st.contains(x); }
    int size() const { return st.size(); }
};

template <class S> long long run(const vector<unsigned long long> &key,
                                unsigned long long &sum, int &sz)
{
    // key 是固定操作键；返回百万次混合操作的微秒数，并输出校验和与最终大小。
    S st;
    auto begin = Clock::now();
    unsigned long long ans = 0;
    for (int i = 0; i < (int)key.size(); i++)
    {
        if (i % 3)
        {
            ans += st.insert(key[i]);
        }
        else
        {
            ans += st.contains(key[i] ^ (i & 7));
        }
    }
    auto end = Clock::now();
    sum = ans;
    sz = st.size();
    return chrono::duration_cast<chrono::microseconds>(end - begin).count();
}

signed main()
{
    vector<unsigned long long> key(1000000);
    mt19937_64 rng(123456789);
    for (auto &x : key)
    {
        x = rng();
    }

    vector<long long> fixed, standard;
    for (int round = 0; round < 9; round++)
    {
        unsigned long long a = 0, b = 0;
        int sa = 0, sb = 0;
        long long x = run<FixedSet>(key, a, sa);
        long long y = run<StdSet>(key, b, sb);
        if (a != b || sa != sb)
        {
            cerr << "benchmark result mismatch\n";
            abort();
        }
        if (round >= 2)
        {
            fixed.push_back(x);
            standard.push_back(y);
        }
    }
    sort(fixed.begin(), fixed.end());
    sort(standard.begin(), standard.end());
    cout << "fixed_us=" << fixed[3] << " unordered_us=" << standard[3] << '\n';
}
