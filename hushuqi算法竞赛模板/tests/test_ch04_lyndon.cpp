#include "../include/full/4_1_006.hpp"

signed main()
{
    // 无参数；核对固定分解、空串和任意字节随机串，成功返回 0。
    assert(lyndon("ababbab") == vector<int>({0, 5, 7}));
    assert(lyndon("") == vector<int>({0}));
    mt19937 rng(20261016);
    auto less = [](const string &a, const string &b)
    {
        // a、b 是待比较字节串；返回按无符号字节字典序得到的 a<b。
        int n = min(a.size(), b.size());
        for (int i = 0; i < n; i++)
        {
            if ((unsigned char)a[i] != (unsigned char)b[i])
            {
                return (unsigned char)a[i] < (unsigned char)b[i];
            }
        }
        return a.size() < b.size();
    };
    for (int seed = 0; seed < 200; seed++)
    {
        string s(rng() % 80, '\0');
        for (char &c : s)
        {
            c = (char)rng();
        }
        vector<int> a = lyndon(s);
        assert(!a.empty() && a.front() == 0 && a.back() == (int)s.size());
        for (int i = 0; i + 1 < (int)a.size(); i++)
        {
            assert(a[i] < a[i + 1]);
            string x = s.substr(a[i], a[i + 1] - a[i]);
            for (int j = a[i] + 1; j < a[i + 1]; j++)
            {
                assert(less(x, s.substr(j, a[i + 1] - j)));
            }
            if (i + 2 < (int)a.size())
            {
                assert(!less(x, s.substr(a[i + 1], a[i + 2] - a[i + 1])));
            }
        }
    }
    cout << "OK\n";
}
