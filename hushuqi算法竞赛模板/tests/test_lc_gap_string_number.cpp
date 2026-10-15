#include "../include/full/4_5_007.hpp"

vector<int> bruteLCS(const string &s, const string &t, const vector<array<int, 3>> &ask)
{
    // s、t 是字节串，ask 各项为 {首串前缀长,次串左端,次串右端}；逐询问 DP 返回同序答案。
    vector<int> ans;
    for (auto [a, l, r] : ask)
    {
        vector<vector<int>> f(a + 1, vector<int>(r - l + 1));
        for (int i = 0; i < a; i++)
        {
            for (int j = l; j < r; j++)
            {
                f[i + 1][j - l + 1] = s[i] == t[j] ? f[i][j - l] + 1
                    : max(f[i][j - l + 1], f[i + 1][j - l]);
            }
        }
        ans.push_back(f[a][r - l]);
    }
    return ans;
}

signed main()
{
    // 无参数；固定边界后逐种子枚举全部前缀、子串，以独立 DP 核对答案和原查询顺序。
    vector<array<int, 3>> ask{{0, 0, 4}, {1, 0, 4}, {4, 1, 4}, {4, 0, 0}};
    assert((prefixSubLCS("abac", "baca", ask) == vector<int>{0, 1, 2, 0}));
    assert(prefixSubLCS("", "", {{0, 0, 0}}) == vector<int>{0});
    assert(prefixSubLCS("", "abc", {{0, 0, 3}}) == vector<int>{0});
    assert(prefixSubLCS("abc", "", {{3, 0, 0}}) == vector<int>{0});
    assert(prefixSubLCS("abc", "abc", {}).empty());
    assert(prefixSubLCS(string(200000, 'a'), string(200000, 'a'), {{1, 0, 1}}) == vector<int>{1});
    string bytes;
    for (int x = 0; x < 256; x++)
    {
        bytes.push_back((char)x);
    }
    assert((prefixSubLCS(bytes, bytes, {{256, 0, 256}, {128, 127, 129}, {128, 128, 256}})
        == vector<int>{256, 1, 0}));
    for (int seed = 0; seed < 600; seed++)
    {
        mt19937_64 rng(seed);
        string s(rng() % 11, '\0'), t(rng() % 12, '\0');
        int sigma = seed % 3 == 0 ? 256 : seed % 3 == 1 ? 2 : 5;
        for (char &c : s)
        {
            c = (char)(rng() % sigma);
        }
        for (char &c : t)
        {
            c = (char)(rng() % sigma);
        }
        vector<array<int, 3>> qs;
        for (int a = 0; a <= (int)s.size(); a++)
        {
            for (int l = 0; l <= (int)t.size(); l++)
            {
                for (int r = l; r <= (int)t.size(); r++)
                {
                    qs.push_back({a, l, r});
                }
            }
        }
        shuffle(qs.begin(), qs.end(), rng);
        auto want = bruteLCS(s, t, qs);
        bool ok = prefixSubLCS(s, t, qs) == want;
        vector<array<int, 3>> sparse;
        for (auto x : qs)
        {
            if (x[0] % 3 == seed % 3) sparse.push_back(x);
        }
        ok &= prefixSubLCS(s, t, sparse) == bruteLCS(s, t, sparse);
        cout << (ok ? "AC " : "WA ") << seed << '\n';
        if (!ok)
        {
            ofstream out(filesystem::temp_directory_path() / ("template-prefix-lcs-WA-" + to_string(seed) + ".txt"));
            for (const string &v : {s, t})
            {
                out << v.size();
                for (unsigned char c : v)
                {
                    out << ' ' << (int)c;
                }
                out << '\n';
            }
            return 1;
        }
    }
}
