#include "../include/full/3_3_001.hpp"

struct Text
{
    string s;

    friend Text operator+(Text a, Text b)
    {
        return {a.s + b.s};
    }
};

signed main()
{
    Seg<Text> st(vector<Text>{{"a"}, {"b"}, {"c"}, {"d"}});
    assert(st.query(0, 4).s == "abcd");
    assert(st.query(1, 3).s == "bc");
    st.set(2, {"X"});
    assert(st.query(0, 4).s == "abXd");
    assert(st.query(2, 2).s.empty());

    Seg<MaxInfo> mx(vector<MaxInfo>{{3}, {1}, {4}, {1}, {5}});
    auto ge4 = [](MaxInfo z) { return z.val >= 4; };
    assert(mx.findFirst(0, 5, ge4) == 2);
    assert(mx.findFirst(3, 5, ge4) == 4);
    assert(mx.findLast(0, 4, ge4) == 2);
    assert(mx.findFirst(1, 2, ge4) == -1);
    assert(mx.findLast(2, 2, ge4) == -1);
    mx.set(3, MaxInfo{6});
    assert(mx.findFirst(3, 5, ge4) == 3);
    assert(mx.findLast(0, 5, ge4) == 4);

    mt19937 rng(0);
    for (int n = 0; n <= 20; n++)
    {
        vector<MaxInfo> a(n);
        for (int i = 0; i < n; i++)
        {
            a[i] = MaxInfo(rng() % 11 - 5);
        }
        Seg<MaxInfo> cur(a);
        for (int l = 0; l <= n; l++)
        {
            for (int r = l; r <= n; r++)
            {
                for (int x = -6; x <= 6; x++)
                {
                    auto ok = [&](MaxInfo z) { return z.val >= x; };
                    int first = -1, last = -1;
                    for (int i = l; i < r; i++)
                    {
                        if (a[i].val >= x)
                        {
                            if (first == -1) first = i;
                            last = i;
                        }
                    }
                    assert(cur.findFirst(l, r, ok) == first);
                    assert(cur.findLast(l, r, ok) == last);
                }
            }
        }
    }
    cout << "OK\n";
}
