#include "../include/full/4_3_004.hpp"

template <int A, char S> bool checkSam(const string &s)
{
    // s 属于指定字节域；直接枚举核对两种构造、子串数和重叠出现次数。
    map<string, int> freq;
    for (int l = 0; l < (int)s.size(); l++)
    {
        for (int r = l + 1; r <= (int)s.size(); r++)
        {
            freq[s.substr(l, r - l)]++;
        }
    }
    SAM<A, S> sam(s), inc;
    string pre;
    set<string> seen;
    bool ok = true;
    for (char c : s)
    {
        inc.add(c);
        pre += c;
        for (int l = 0; l < (int)pre.size(); l++)
        {
            seen.insert(pre.substr(l));
        }
        ok &= inc.distinct() == (long long)seen.size();
    }
    ok &= sam.distinct() == (long long)freq.size() && inc.distinct() == sam.distinct();
    ok &= sam.find("") == 0 && sam.occ("") == (int)s.size() + 1 && !sam.counted;
    string absent(s.size() + 1, S);
    ok &= sam.find(absent) == -1 && sam.occ(absent) == 0 && !sam.counted;
    for (auto &[q, want] : freq)
    {
        ok &= sam.find(q) > 0 && sam.occ(q) == want && inc.occ(q) == want;
        int v = sam.find(q);
        if (v > 0)
        {
            ok &= sam.t[v].len >= (int)q.size() && sam.t[sam.t[v].link].len < (int)q.size();
            int end = sam.t[v].first;
            ok &= end + 1 >= (int)q.size() && end < (int)s.size();
            if (end + 1 >= (int)q.size() && end < (int)s.size())
            {
                ok &= s.substr(end + 1 - q.size(), q.size()) == q;
            }
        }
    }
    sam.count();
    vector<int> before;
    for (auto &v : sam.t)
    {
        before.push_back(v.cnt);
    }
    sam.count();
    for (int i = 0; i < (int)sam.t.size(); i++)
    {
        ok &= before[i] == sam.t[i].cnt;
    }
    return ok;
}

signed main()
{
    // 无参数；每个种子输出一行，随机失败时保存二进制原串供复现。
    SAM<3> split("abcbc");
    assert(split.t.size() == 8 && split.distinct() == 12 && split.occ("bc") == 2);
    assert(split.find("d") == -1);
    int id = 0;
    for (int n = 0; n <= 9; n++)
    {
        for (int mask = 0; mask < (1LL << n); mask++)
        {
            string s(n, 'a');
            for (int i = 0; i < n; i++)
            {
                s[i] += mask >> i & 1;
            }
            bool ok = checkSam<2, 'a'>(s);
            cout << (ok ? "AC " : "WA ") << id++ << '\n';
            if (!ok) return 1;
        }
    }
    for (int seed = 0; seed < 600; seed++)
    {
        mt19937_64 rng(seed);
        string s(rng() % 30, 'a'), bytes(s.size(), '\0'), high(s.size(), '\0');
        for (int i = 0; i < (int)s.size(); i++)
        {
            s[i] += rng() % (seed % 2 ? 3 : 26);
            bytes[i] = (char)(rng() % 256);
            high[i] = (char)(128 + rng() % 16);
        }
        bool ok = checkSam<26, 'a'>(s) && checkSam<256, '\0'>(bytes)
            && checkSam<16, (char)128>(high);
        cout << (ok ? "AC " : "WA ") << id++ << '\n';
        if (!ok)
        {
            auto base = filesystem::temp_directory_path() / ("template-sam-WA-" + to_string(seed));
            ofstream(base.string() + "-lower.bin", ios::binary) << s;
            ofstream(base.string() + "-bytes.bin", ios::binary) << bytes;
            ofstream(base.string() + "-high.bin", ios::binary) << high;
            return 1;
        }
    }
    string all;
    for (int c = 0; c < 256; c++)
    {
        all += (char)c;
    }
    assert((checkSam<256, '\0'>(all)));
    int n = 100000;
    SAM<2> large(string(n, 'a') + string(n, 'b'));
    assert(large.distinct() == 1LL * n * n + 2LL * n);
    assert(large.occ("a") == n && large.occ("ab") == 1 && large.occ("") == 2 * n + 1);
}
