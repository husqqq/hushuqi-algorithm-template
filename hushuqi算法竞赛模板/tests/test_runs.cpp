#include "../include/full/4_1_007.hpp"

vector<array<int, 3>> brute(const string &s)
{
    // s 是短字节串；逐区间、逐周期枚举，返回字典序的 {最小周期, 左端点, 右端点}。
    int n = s.size();
    vector<array<int, 3>> ans; // 每项依次为 {最小周期, 左端点, 右端点}。
    for (int l = 0; l < n; l++)
    {
        for (int r = l + 1; r <= n; r++)
        {
            int p = 0;
            for (int q = 1; 2 * q <= r - l && !p; q++)
            {
                bool ok = true;
                for (int i = l + q; i < r; i++)
                {
                    ok &= s[i] == s[i - q];
                }
                if (ok)
                {
                    p = q;
                }
            }
            if (!p)
            {
                continue;
            }
            bool left = l && s[l - 1] == s[l - 1 + p];
            bool right = r < n && s[r] == s[r - p];
            if (!left && !right)
            {
                ans.push_back({p, l, r});
            }
        }
    }
    sort(ans.begin(), ans.end());
    return ans;
}

signed main()
{
    // 无参数；每轮输出可复现种子，失败保存二进制输入并返回非零。
    auto check = [&](const string &s, int seed)
    {
        // s 是当前测试串，seed 是重放编号；核对完整结果并在失败时保存原串。
        bool ok = runs(s) == brute(s);
        cout << (ok ? "AC " : "WA ") << seed << '\n';
        if (!ok)
        {
            auto path = filesystem::temp_directory_path() / "template-runs-failure.bin";
            ofstream out(path, ios::binary);
            out.write(s.data(), s.size());
        }
        return ok;
    };
    int id = 0;
    for (int n = 0; n <= 13; n++)
    {
        for (int mask = 0; mask < (1LL << n); mask++)
        {
            string s(n, 'a');
            for (int i = 0; i < n; i++)
            {
                s[i] += mask >> i & 1;
            }
            if (!check(s, id++)) return 1;
        }
    }
    int pw = 1;
    for (int n = 0; n <= 9; n++, pw *= 3)
    {
        for (int mask = 0; mask < pw; mask++)
        {
            int x = mask;
            string s(n, 'a');
            for (int i = 0; i < n; i++, x /= 3)
            {
                s[i] += x % 3;
            }
            if (!check(s, id++)) return 1;
        }
    }
    // 0、128、255 的排列同时覆盖零字节、符号位和反字符序的端点。
    pw = 1;
    for (int n = 0; n <= 8; n++, pw *= 3)
    {
        for (int mask = 0; mask < pw; mask++)
        {
            int x = mask;
            string s(n, '\0');
            for (char &c : s)
            {
                c = (char)(x % 3 == 2 ? 255 : (x % 3) * 128);
                x /= 3;
            }
            if (!check(s, id++)) return 1;
        }
    }
    for (int seed = 0; seed < 600; seed++)
    {
        mt19937_64 rng(seed);
        for (int a : {2, 4, 26, 256})
        {
            string s(rng() % 48, '\0');
            for (char &c : s)
            {
                c = (char)(rng() % a);
            }
            if (!check(s, 1000000 + seed * 256 + a)) return 1;
        }
    }
    // 长串单独核对固定答案，避免暴力算法掩盖内存或复杂度问题。
    string s(200000, 'a');
    assert((runs(s) == vector<array<int, 3>>{{1, 0, 200000}}));
    for (int i = 0; i < (int)s.size(); i++)
    {
        s[i] = (char)(i % 256);
    }
    assert((runs(s) == vector<array<int, 3>>{{256, 0, 200000}}));
}
