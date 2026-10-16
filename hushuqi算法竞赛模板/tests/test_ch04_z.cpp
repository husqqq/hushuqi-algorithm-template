#include "../include/full/4_1_003.hpp"

signed main()
{
    // 无参数；核对固定样例和 256 字节随机串，成功返回 0。
    assert(zfunc("aabcaabxaaaz") == vector<int>({12, 1, 0, 0, 3, 1, 0, 0, 2, 2, 1, 0}));
    assert(zfunc("").empty());
    assert(zfunc(string(200, '\0')) == [&]
    {
        vector<int> want(200);
        for (int i = 0; i < 200; i++)
        {
            want[i] = 200 - i;
        }
        return want;
    }());
    mt19937 rng(20261016);
    for (int seed = 0; seed < 200; seed++)
    {
        string s(1 + rng() % 200, '\0');
        for (char &c : s)
        {
            c = (char)rng();
        }
        vector<int> want(s.size());
        for (int i = 0; i < (int)s.size(); i++)
        {
            while (i + want[i] < (int)s.size() && s[want[i]] == s[i + want[i]])
            {
                want[i]++;
            }
        }
        assert(zfunc(s) == want);
    }
    cout << "OK\n";
}
