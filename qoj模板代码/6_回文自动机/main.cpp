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

template <int A = 26, char S = 'a'> struct PAM
{
    struct Node
    {
        array<int, A> ch{};
        // len 是回文长度，link 是回文后缀链接，cnt 是出现次数。
        int len = 0, link = 0, cnt = 0;
    };

    vector<Node> t{{{}, -1, 0, 0}, {{}, 0, 0, 0}};
    vector<int> s{-1};
    // last 是当前最长回文后缀节点，counted 记录出现次数是否已经上传。
    int last = 1;
    bool counted = false;

    int getLink(int p) const
    {
        // p 是回文节点；返回可被当前末字符继续扩展的后缀回文节点。
        int n = s.size() - 1;
        while (n - t[p].len - 1 < 0 || s[n - t[p].len - 1] != s[n])
        {
            p = t[p].link;
        }
        return p;
    }

    // 追加字符，返回当前最长回文后缀节点；节点数-2 即不同回文串数。
    int add(char c)
    {
        // c 是追加到原串末尾的字符；返回新的最长回文后缀节点。
        assert(!counted); // 调试检查，可删
        int x = c - S;
        assert(0 <= x && x < A); // 调试检查，可删
        s.push_back(x);
        int p = getLink(last);
        if (!t[p].ch[x])
        {
            int q = t.size();
            t.push_back({{}, t[p].len + 2, 0, 0});
            if (t[q].len == 1)
            {
                t[q].link = 1;
            }
            else
            {
                t[q].link = t[getLink(t[p].link)].ch[x];
            }
            t[p].ch[x] = q;
        }
        last = t[p].ch[x];
        t[last].cnt++;
        return last;
    }

    void build(const string &x)
    {
        // x 是要继续追加的字符串。
        for (char c : x)
        {
            add(c);
        }
    }

    // 调用一次后 cnt 变为每个不同回文在整串中的出现次数。
    void count()
    {
        // 沿后缀链接上传出现次数；重复调用不再二次累计。
        if (counted)
        {
            return;
        }
        for (int p = (int)t.size() - 1; p >= 2; p--)
        {
            t[t[p].link].cnt += t[p].cnt;
        }
        counted = true;
    }
};

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    cin >> s;
    PAM<26, 'a'> pam;
    pam.build(s);
    pam.count();
    long long ans = 0;
    for (int u = 2; u < (int)pam.t.size(); u++)
    {
        ans = max(ans, pam.t[u].cnt * pam.t[u].len * pam.t[u].len);
    }
    cout << ans << '\n';
}
