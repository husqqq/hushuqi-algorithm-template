#include "../include/full/4_3_012.hpp"

bool checkFM(const string &s, mt19937_64 &rng)
{
    // s 是待索引字节串，rng 生成模式；直接排序后缀并扫描文本核对全部公开接口。
    FMIndex fm(s);
    int n = s.size() + 1;
    vector<int> a(n), sa(n), rk(n);
    for (int i = 0; i + 1 < n; i++)
    {
        a[i] = (unsigned char)s[i] + 1;
    }
    iota(sa.begin(), sa.end(), 0);
    sort(sa.begin(), sa.end(), [&](int i, int j)
    {
        return lexicographical_compare(a.begin() + i, a.end(), a.begin() + j, a.end());
    });
    if (fm.sa != sa || fm.restore() != s) return false;
    for (int i = 0; i < n; i++)
    {
        rk[sa[i]] = i;
        if (fm.bwt[i] != a[(sa[i] + n - 1) % n]) return false;
    }
    array<int, 257> cnt{};
    for (int r = 0; r <= n; r++)
    {
        for (int x = 0; x <= 256; x++)
        {
            if (fm.rank(x, r) != cnt[x]) return false;
        }
        if (r < n)
        {
            if (fm.lf(r) != rk[(sa[r] + n - 1) % n]) return false;
            cnt[fm.bwt[r]]++;
        }
    }
    for (int q = 0; q < 40; q++)
    {
        string p;
        if (q % 2 && !s.empty())
        {
            int l = rng() % s.size();
            p = s.substr(l, rng() % 10);
        }
        else
        {
            p.resize(rng() % 10);
            for (char &c : p)
            {
                c = (char)rng();
            }
        }
        vector<int> want;
        for (int i = 0; i <= (int)s.size(); i++)
        {
            if (s.substr(i, p.size()) == p) want.push_back(i);
        }
        auto [l, r] = fm.search(p);
        if (r - l != (int)want.size() || fm.locate(p) != want) return false;
        for (int i = l; i < r; i++)
        {
            if (s.substr(sa[i], p.size()) != p) return false;
        }
    }
    return true;
}

signed main()
{
    // 无参数；覆盖空串、全部字节、SA-IS 分支边界与固定种子随机串。
    mt19937_64 fixed(20261016);
    assert(checkFM("", fixed));
    string bytes;
    for (int x = 0; x < 256; x++)
    {
        bytes.push_back((char)x);
    }
    assert(checkFM(bytes, fixed));
    for (int n : {1, 2, 8, 9, 10, 38, 39, 40, 63, 64, 65})
    {
        assert(checkFM(string(n, '\xff'), fixed));
    }
    for (int seed = 0; seed < 300; seed++)
    {
        mt19937_64 rng(seed);
        string s(rng() % 120, '\0');
        int sigma = seed % 3 == 0 ? 256 : seed % 3 == 1 ? 2 : 7;
        for (char &c : s)
        {
            c = (char)(rng() % sigma);
        }
        bool ok = checkFM(s, rng);
        cout << (ok ? "AC " : "WA ") << seed << '\n';
        if (!ok)
        {
            ofstream out(filesystem::temp_directory_path() / ("template-fm-index-WA-" + to_string(seed) + ".txt"));
            out << s.size() << '\n';
            for (unsigned char c : s)
            {
                out << (int)c << ' ';
            }
            out << '\n';
            return 1;
        }
    }
}
