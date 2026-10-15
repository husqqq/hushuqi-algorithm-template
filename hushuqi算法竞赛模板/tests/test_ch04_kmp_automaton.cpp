#include "../include/full/4_1_002.hpp"

template <int A, char F>
void check(const string &s)
{
    // s 是模式串；逐个状态、字符用直接后缀比较核对转移与两种 next 重载。
    KMPAutomaton<A, F> a(s);
    for (int state = 0; state <= (int)s.size(); state++)
    {
        for (int x = 0; x < A; x++)
        {
            char c = (char)((unsigned char)F + x);
            string t = s.substr(0, state) + c;
            int want = min(s.size(), t.size());
            while (want && t.compare(t.size() - want, want, s, 0, want))
            {
                want--;
            }
            assert(a.next(state, x) == want);
            assert(a.next(state, c) == want);
        }
    }
}

signed main()
{
    // 无参数；核对重叠匹配、空模式、任意字节与长重复串构造，成功输出 OK。
    KMPAutomaton<26, 'a'> a("aba");
    int state = 0;
    vector<int> hit;
    string s = "ababa";
    for (int i = 0; i < (int)s.size(); i++)
    {
        state = a.next(state, s[i]);
        if (a.matched(state))
        {
            hit.push_back(i - (int)a.pattern.size() + 1);
        }
    }
    assert((hit == vector<int>{0, 2}));
    KMPAutomaton<2, '0'> empty("");
    assert(empty.matched(0));
    assert(empty.next(0, '1') == 0);
    KMPAutomaton<2, '0'> fresh;
    assert(fresh.matched(0) && fresh.next(0, '0') == 0);
    fresh.build("010");
    assert(fresh.next(3, '1') == 2);
    fresh.build("");
    assert(fresh.next(0, '1') == 0);
    for (int n = 0; n <= 9; n++)
    {
        for (int mask = 0; mask < (1 << n); mask++)
        {
            string p(n, '0');
            for (int i = 0; i < n; i++)
            {
                p[i] += mask >> i & 1;
            }
            check<2, '0'>(p);
        }
    }
    string bytes;
    for (int x : {0, 255, 128, 0, 255})
    {
        bytes.push_back((char)x);
    }
    check<256, '\0'>(bytes);
    check<2, (char)254>(string({(char)255, (char)254, (char)255}));
    KMPAutomaton<2> repeated(string(200000, 'a'));
    for (int i = 0; i <= 200000; i++)
    {
        assert(repeated.next(i, 'a') == min<int>(i + 1, 200000));
        assert(repeated.next(i, 'b') == 0);
    }
    cout << "OK\n";
}
