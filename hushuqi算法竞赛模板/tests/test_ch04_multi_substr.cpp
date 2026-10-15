#include "../include/full/4_5_004.hpp"

int bruteCommon(const vector<string> &a)
{
    // a 是任意字节串集合；枚举首串的全部子串，返回在每串中都出现的最长长度。
    if (a.empty()) return 0;
    int ans = 0;
    for (int l = 0; l < (int)a[0].size(); l++)
    {
        for (int r = l + 1; r <= (int)a[0].size(); r++)
        {
            string p = a[0].substr(l, r - l);
            bool ok = true;
            for (const string &s : a)
            {
                ok &= s.find(p) != string::npos;
            }
            if (ok) ans = max(ans, r - l);
        }
    }
    return ans;
}

signed main()
{
    // 无参数；逐种子核对字节串、最短串位置与不平衡输入，失败种子可独立重现。
    assert(multiSubstr({}) == 0);
    assert(multiSubstr({"banana", "ananas", "nana"}) == 4);
    assert(multiSubstr({"banana", "", "nana"}) == 0);
    string bytes;
    for (int x = 0; x < 256; x++)
    {
        bytes.push_back((char)x);
    }
    assert(multiSubstr({bytes}) == 256);
    assert(multiSubstr({bytes + bytes, bytes.substr(127, 3), bytes}) == 3);
    vector<string> uneven(2000, "ab");
    uneven[0] = string(200000, 'a') + 'b';
    assert(multiSubstr(uneven) == 2);
    reverse(uneven.begin(), uneven.end());
    assert(multiSubstr(uneven) == 2);
    for (int seed = 0; seed < 700; seed++)
    {
        mt19937_64 rng(seed);
        vector<string> a(rng() % 6);
        int sigma = seed % 3 == 0 ? 256 : seed % 3 == 1 ? 2 : 7;
        for (string &s : a)
        {
            s.resize(rng() % 14);
            for (char &c : s)
            {
                c = (char)(rng() % sigma);
            }
        }
        int want = bruteCommon(a);
        bool ok = multiSubstr(a) == want;
        reverse(a.begin(), a.end());
        ok &= multiSubstr(a) == want;
        shuffle(a.begin(), a.end(), rng);
        ok &= multiSubstr(a) == want;
        cout << (ok ? "AC " : "WA ") << seed << '\n';
        if (!ok)
        {
            ofstream out(filesystem::temp_directory_path() / ("template-multi-substr-WA-" + to_string(seed) + ".txt"));
            out << a.size() << '\n';
            for (const string &s : a)
            {
                out << s.size();
                for (unsigned char c : s)
                {
                    out << ' ' << (int)c;
                }
                out << '\n';
            }
            return 1;
        }
    }
}
