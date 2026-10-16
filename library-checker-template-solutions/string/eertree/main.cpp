#include <bits/stdc++.h>
using namespace std;
#define int long long

template <int A = 26, char S = 'a'> struct PAM
{
    static_assert(A > 0 && (unsigned char)S + A <= 256);
    struct Node
    {
        array<int, A> ch{}; // ch[c] 是两侧各加字符 c 得到的回文节点，0 表示不存在。
        // len 是回文长度，link 是回文后缀链接，cnt 是出现次数。
        int len = 0, link = 0, cnt = 0;
    };

    vector<Node> t{{{}, -1, 0, 0}, {{}, 0, 0, 0}};
    vector<int> s{-1}; // s 是已加入的字符，s[0]=-1 作为哨兵。
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

    int add(char c)
    {
        // c 是追加到原串末尾的字符；返回新的最长回文后缀节点。
        assert(!counted); // 调试检查，可删
        int x = (unsigned char)c - (unsigned char)S;
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
    // 无参数；输出回文树父边、后缀链接和每个前缀的最长回文后缀，成功返回 0。
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    cin >> s;
    PAM<26, 'a'> pam;
    vector<int> last;
    for (char c : s)
    {
        last.push_back(pam.add(c));
    }
    const auto &nodes = pam.t;
    vector<int> par(nodes.size(), -1);
    for (int u = 0; u < (int)nodes.size(); u++)
    {
        for (int c = 0; c < 26; c++)
        {
            if (nodes[u].ch[c])
            {
                par[nodes[u].ch[c]] = u;
            }
        }
    }
    cout << nodes.size() - 2 << '\n';
    for (int u = 2; u < (int)nodes.size(); u++)
    {
        cout << par[u] - 1 << ' ' << nodes[u].link - 1 << '\n';
    }
    for (int i = 0; i < (int)s.size(); i++)
    {
        cout << (i ? " " : "") << last[i] - 1;
    }
    cout << '\n';
}
