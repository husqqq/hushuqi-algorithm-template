#include "../include/full/4_5_001.hpp"

int bruteLcs(const string &a, const string &b)
{
    // a、b 是字节串；用普通二维 DP 返回 LCS 长度，独立核对跨块借位。
    vector f(a.size() + 1, vector<int>(b.size() + 1));
    for (int i = 0; i < (int)a.size(); i++)
    {
        for (int j = 0; j < (int)b.size(); j++)
        {
            f[i + 1][j + 1] = a[i] == b[j] ? f[i][j] + 1 : max(f[i][j + 1], f[i + 1][j]);
        }
    }
    return f.back().back();
}

signed main()
{
    // 无参数；核对空串、64 位块边界、连续借位、移位进位和固定种子随机串。
    assert(lcsBitset("", "abc") == 0 && lcsBitset("abc", "") == 0);
    for (int gap : {62, 63, 64, 126, 127, 128, 190, 191, 192})
    {
        assert(lcsBitset("ab", "a" + string(gap, 'c') + "b") == 2);
    }
    for (int n : {1, 63, 64, 65, 127, 128, 129, 191, 192, 193, 256})
    {
        string a(n, 'a');
        assert(lcsBitset(a, a) == n);
        assert(lcsBitset(a + 'b', 'b' + a) == n);
        assert(lcsBitset(a, string(n, 'b')) == 0);
    }
    mt19937_64 rng(20261016);
    for (int seed = 0; seed < 600; seed++)
    {
        int n = rng() % 260, m = rng() % 260;
        string a(n, '\0'), b(m, '\0');
        int sigma = seed % 3 == 0 ? 256 : seed % 3 == 1 ? 2 : 7;
        for (char &c : a)
        {
            c = (char)(rng() % sigma);
        }
        for (char &c : b)
        {
            c = (char)(rng() % sigma);
        }
        int want = bruteLcs(a, b);
        if (lcsBitset(a, b) != want || lcsBitset(b, a) != want)
        {
            cout << "WA " << seed << '\n';
            return 1;
        }
        cout << "AC " << seed << '\n';
    }
}
