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

template <int A = 26, char S = 'a'>
struct DequePAM
{
  private:
    struct Node
    {
        // ch 是回文树转移；par、link、quick 分别是转移父亲、后缀链接和快速跳转。
        array<int, A> ch{};
        int par = 0;
        int link = 0;
        int quick = 0;
        // len 是回文长度，cnt 是边界记录引用数，sons 是后缀链接孩子数。
        int len = 0;
        int cnt = 0;
        int sons = 0;
    };

    struct Pos
    {
        // c 是当前位置字符编号；pre、suf 记录当前最长回文前缀、后缀节点。
        int c = 0;
        int pre = 1;
        int suf = 1;
    };

    // t 是节点池，spare 是可复用编号，d 是当前字符串，kinds 是本质不同回文数。
    vector<Node> t;
    vector<int> spare;
    deque<Pos> d;
    int kinds = 0;

    int newNode()
    {
        // 返回一个已经清空的节点编号，优先复用回收节点。
        if (spare.empty())
        {
            t.push_back({});
            return (int)t.size() - 1;
        }
        int p = spare.back();
        spare.pop_back();
        t[p] = Node{};
        return p;
    }

    void drop(int p)
    {
        // p 是已经脱离所有转移和边界记录的节点；清空后回收其编号。
        t[p] = Node{};
        spare.push_back(p);
    }

    int backCan(int c, int p) const
    {
        // c 是待加字符，p 是当前最长回文后缀；返回最先能从右端扩展的后缀回文。
        int n = d.size();
        while (true)
        {
            if (t[p].len == -1 || (t[p].len < n && d[n - t[p].len - 1].c == c))
            {
                return p;
            }
            int q = t[p].link;
            if (t[q].len == -1 || (t[q].len < n && d[n - t[q].len - 1].c == c))
            {
                return q;
            }
            p = t[p].quick;
        }
    }

    int frontCan(int c, int p) const
    {
        // c 是待加字符，p 是当前最长回文前缀；返回最先能从左端扩展的前缀回文。
        int n = d.size();
        while (true)
        {
            if (t[p].len == -1 || (t[p].len < n && d[t[p].len].c == c))
            {
                return p;
            }
            int q = t[p].link;
            if (t[q].len == -1 || (t[q].len < n && d[t[q].len].c == c))
            {
                return q;
            }
            p = t[p].quick;
        }
    }

  public:
    DequePAM() : t(2)
    {
        // 建立长度 -1 的奇根 0 和长度 0 的偶根 1。
        t[0].len = -1;
        t[0].par = 0;
        t[0].link = 0;
        t[0].quick = 0;
        t[1].len = 0;
        t[1].par = 0;
        t[1].link = 0;
        t[1].quick = 0;
    }

    void pushBack(char c)
    {
        // c 是新字符；加入右端并维护最长回文前后缀。
        int x = (unsigned char)c - (unsigned char)S;
        assert(0 <= x && x < A); // 调试检查，可删
        int par = d.empty() ? 0 : backCan(x, d.back().suf);
        int n = d.size();
        int v = t[par].ch[x];
        int w = 1;
        if (!v)
        {
            v = newNode();
            t[v].par = par;
            t[v].len = t[par].len + 2;
            if (par != 0)
            {
                w = t[backCan(x, t[par].link)].ch[x];
                assert(w); // 调试检查，可删
            }
            t[v].link = w;
            t[w].sons++;
            d.push_back({x, 1, 1});
            n++;
            if (t[w].link != 0 && d[n - t[w].len - 1].c == d[n - t[t[w].link].len - 1].c)
            {
                t[v].quick = t[w].quick;
            }
            else
            {
                t[v].quick = t[w].link;
            }
            t[par].ch[x] = v;
            kinds++;
        }
        else
        {
            d.push_back({x, 1, 1});
            n++;
            w = t[v].link;
        }
        d[n - 1].suf = v;
        d[n - t[v].len].pre = v;
        if (t[w].len >= 1 && d[n - t[v].len + t[w].len - 1].suf == w)
        {
            d[n - t[v].len + t[w].len - 1].suf = 1;
        }
        t[v].cnt++;
    }

    void pushFront(char c)
    {
        // c 是新字符；加入左端并维护最长回文前后缀。
        int x = (unsigned char)c - (unsigned char)S;
        assert(0 <= x && x < A); // 调试检查，可删
        int par = d.empty() ? 0 : frontCan(x, d.front().pre);
        int v = t[par].ch[x];
        int w = 1;
        if (!v)
        {
            v = newNode();
            t[v].par = par;
            t[v].len = t[par].len + 2;
            if (par != 0)
            {
                w = t[frontCan(x, t[par].link)].ch[x];
                assert(w); // 调试检查，可删
            }
            t[v].link = w;
            t[w].sons++;
            d.push_front({x, 1, 1});
            if (t[w].link != 0 && d[t[w].len].c == d[t[t[w].link].len].c)
            {
                t[v].quick = t[w].quick;
            }
            else
            {
                t[v].quick = t[w].link;
            }
            t[par].ch[x] = v;
            kinds++;
        }
        else
        {
            d.push_front({x, 1, 1});
            w = t[v].link;
        }
        d[0].pre = v;
        d[t[v].len - 1].suf = v;
        if (t[w].len >= 1 && d[t[v].len - t[w].len].pre == w)
        {
            d[t[v].len - t[w].len].pre = 1;
        }
        t[v].cnt++;
    }

    void popBack()
    {
        // 删除右端字符；当前串必须非空。
        assert(!d.empty()); // 调试检查，可删
        int n = d.size();
        int v = d.back().suf;
        int x = d.back().c;
        int w = t[v].link;
        if (t[v].len >= 2 && t[d[n - t[v].len + t[w].len - 1].suf].len < t[w].len)
        {
            d[n - t[v].len + t[w].len - 1].suf = w;
            d[n - t[v].len].pre = w;
        }
        else
        {
            d[n - t[v].len].pre = 1;
        }
        t[v].cnt--;
        if (!t[v].sons && !t[v].cnt)
        {
            t[t[v].par].ch[x] = 0;
            t[w].sons--;
            drop(v);
            kinds--;
        }
        d.pop_back();
    }

    void popFront()
    {
        // 删除左端字符；当前串必须非空。
        assert(!d.empty()); // 调试检查，可删
        int v = d.front().pre;
        int x = d.front().c;
        int w = t[v].link;
        if (t[v].len >= 2 && t[d[t[v].len - t[w].len].pre].len < t[w].len)
        {
            d[t[v].len - t[w].len].pre = w;
            d[t[v].len - 1].suf = w;
        }
        else
        {
            d[t[v].len - 1].suf = 1;
        }
        t[v].cnt--;
        if (!t[v].sons && !t[v].cnt)
        {
            t[t[v].par].ch[x] = 0;
            t[w].sons--;
            drop(v);
            kinds--;
        }
        d.pop_front();
    }

    int size() const
    {
        // 返回当前字符串长度。
        return d.size();
    }

    int distinct() const
    {
        // 返回当前字符串的本质不同非空回文子串数。
        return kinds;
    }

    int longPrefix() const
    {
        // 返回当前最长回文前缀长度，空串返回 0。
        return d.empty() ? 0 : t[d.front().pre].len;
    }

    int longSuffix() const
    {
        // 返回当前最长回文后缀长度，空串返回 0。
        return d.empty() ? 0 : t[d.back().suf].len;
    }
};
