// Generated from hushuqi算法竞赛模板. Do not edit by hand.

// QOJ contest 3936: 1 前缀函数 - KMP

#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int inf = 1E9;
constexpr long long INF = 4E18;
constexpr long double eps = 1E-12L;

template <class T> bool chmin(T &a, const T &b)
{
    // a 是待更新值，b 是候选值；若 a 变小则返回 true。
    if (b >= a)
    {
        return false;
    }
    a = b;
    return true;
}
template <class T> bool chmax(T &a, const T &b)
{
    // a 是待更新值，b 是候选值；若 a 变大则返回 true。
    if (a >= b)
    {
        return false;
    }
    a = b;
    return true;
}

vector<int> prefix(const string &s)
{
    // s 是原字符串；返回前缀函数，p[i] 是 s[0..i] 的最长真 border 长度。
    vector<int> p(s.size());
    for (int i = 1; i < (int)s.size(); i++)
    {
        int j = p[i - 1];
        while (j && s[i] != s[j])
        {
            j = p[j - 1];
        }
        if (s[i] == s[j])
        {
            j++;
        }
        p[i] = j;
    }
    return p;
}

vector<int> kmp(const string &s, const string &t)
{
    // s 是文本串，t 是模式串；返回全部匹配起点，按下标递增排列。
    if (t.empty())
    {
        vector<int> a(s.size() + 1);
        iota(a.begin(), a.end(), 0);
        return a;
    }
    auto p = prefix(t);
    vector<int> ans;
    int j = 0;
    for (int i = 0; i < (int)s.size(); i++)
    {
        while (j && s[i] != t[j])
        {
            j = p[j - 1];
        }
        if (s[i] == t[j])
        {
            j++;
        }
        if (j == (int)t.size())
        {
            ans.push_back(i - j + 1);
            j = p[j - 1];
        }
    }
    return ans;
}

vector<int> borders(const string &s)
{
    // s 是原字符串；返回全部非空真 border 的长度，按递增排列。
    auto p = prefix(s);
    vector<int> a;
    int x = s.empty() ? 0 : p.back();
    while (x)
    {
        a.push_back(x);
        x = p[x - 1];
    }
    reverse(a.begin(), a.end());
    return a;
}

int minPeriod(const string &s)
{
    // s 是原字符串；返回最短整周期长度，空串返回 0。
    if (s.empty())
    {
        return 0;
    }
    auto p = prefix(s);
    int x = s.size() - p.back();
    return (int)s.size() % x == 0 ? x : (int)s.size();
}

template <int A = 26, char F = 'a'> struct KMPAutomaton
{
    // pattern 保存模式串，pi 是其前缀函数，go[state][字符] 是 DFA 转移后的匹配长度。
    string pattern;
    vector<int> pi;
    vector<array<int, A>> go;

    KMPAutomaton() = default;

    explicit KMPAutomaton(const string &s)
    {
        // s 是模式串；构造其所有前缀匹配状态的自动机。
        build(s);
    }

    int id(char c) const
    {
        // c 是字符集 [F,F+A) 中的字符；返回从 0 开始的字符编号。
        int x = c - F;
        assert(0 <= x && x < A);
        return x;
    }

    void build(const string &s)
    {
        // s 是模式串；重建前缀函数和状态转移表，无返回值。
        pattern = s;
        int n = pattern.size();
        for (char c : pattern)
        {
            id(c);
        }
        pi.assign(n, 0);
        for (int i = 1, j = 0; i < n; i++)
        {
            while (j && pattern[i] != pattern[j])
            {
                j = pi[j - 1];
            }
            if (pattern[i] == pattern[j])
            {
                j++;
            }
            pi[i] = j;
        }
        go.assign(n + 1, array<int, A>());
        for (int state = 0; state <= n; state++)
        {
            for (int x = 0; x < A; x++)
            {
                int j = state;
                char c = (char)(F + x);
                while (j && (j == n || pattern[j] != c))
                {
                    j = pi[j - 1];
                }
                if (j < n && pattern[j] == c)
                {
                    j++;
                }
                go[state][x] = j;
            }
        }
    }

    int next(int state, int x) const
    {
        // state 是当前已匹配前缀长度，x 是字符编号；返回读入该字符后的匹配长度。
        assert(0 <= state && state <= (int)pattern.size() && 0 <= x && x < A);
        return go[state][x];
    }

    int next(int state, char c) const
    {
        // state 是当前已匹配前缀长度，c 是字符集内字符；返回读入该字符后的匹配长度。
        return next(state, id(c));
    }

    bool matched(int state) const
    {
        // state 是当前状态；返回是否已经匹配完整模式串。
        return state == (int)pattern.size();
    }
};

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    cin >> s;
    auto p = prefix(s);
    for (int i = 0; i < (int)p.size(); i++)
    {
        cout << p[i] << " \n"[i + 1 == (int)p.size()];
    }
}
