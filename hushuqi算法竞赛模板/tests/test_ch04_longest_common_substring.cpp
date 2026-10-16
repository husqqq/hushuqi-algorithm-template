#include "../include/full/4_3_004.hpp"

template <int A, char S> bool checkLcs(const string &a, const string &b)
{
    // a、b 属于指定字节域；独立滚动 DP 求长度，再检查返回坐标及内容。
    vector<int> dp(b.size() + 1);
    int best = 0;
    for (char c : a)
    {
        for (int j = b.size(); j > 0; j--)
        {
            dp[j] = c == b[j - 1] ? dp[j - 1] + 1 : 0;
            best = max(best, dp[j]);
        }
    }
    auto [l, r, x, y] = commonSubstr<A, S>(a, b);
    if (!(0 <= l && l <= r && r <= (int)a.size()
        && 0 <= x && x <= y && y <= (int)b.size())) return false;
    return r - l == best && y - x == best && a.substr(l, best) == b.substr(x, best)
        && (best || (l == 0 && r == 0 && x == 0 && y == 0));
}

signed main()
{
    // 无参数；穷举短串对，再以独立种子覆盖字节域和不对称长度。
    vector<string> words;
    for (int n = 0; n <= 5; n++)
    {
        for (int mask = 0; mask < (1LL << n); mask++)
        {
            string s(n, 'a');
            for (int i = 0; i < n; i++)
            {
                s[i] += mask >> i & 1;
            }
            words.push_back(s);
        }
    }
    for (auto &a : words)
    {
        for (auto &b : words)
        {
            assert((checkLcs<2, 'a'>(a, b)));
        }
    }
    for (int seed = 0; seed < 600; seed++)
    {
        mt19937_64 rng(seed);
        string a(rng() % 80, 'a'), b(rng() % 80, 'a');
        bool ok = true;
        for (int kind = 0; kind < 3; kind++)
        {
            for (auto *s : {&a, &b})
            {
                for (char &c : *s)
                {
                    c = kind == 0 ? 'a' + rng() % 3 : kind == 1 ? rng() % 256 : 128 + rng() % 16;
                }
            }
            if (kind == 0) ok &= checkLcs<3, 'a'>(a, b) && checkLcs<3, 'a'>(b, a);
            if (kind == 1) ok &= checkLcs<256, '\0'>(a, b) && checkLcs<256, '\0'>(b, a);
            if (kind == 2) ok &= checkLcs<16, (char)128>(a, b) && checkLcs<16, (char)128>(b, a);
            if (!ok)
            {
                auto base = filesystem::temp_directory_path() / ("template-lcs-WA-" + to_string(seed));
                ofstream(base.string() + "-a.bin", ios::binary) << a;
                ofstream(base.string() + "-b.bin", ios::binary) << b;
                break;
            }
        }
        cout << (ok ? "AC " : "WA ") << seed << '\n';
        if (!ok) return 1;
    }
    assert((checkLcs<26, 'a'>("ababc", "babca")));
    assert((checkLcs<26, 'a'>(string(500000, 'a'), "baab")));
    assert((checkLcs<26, 'a'>("baab", string(500000, 'a'))));
}
