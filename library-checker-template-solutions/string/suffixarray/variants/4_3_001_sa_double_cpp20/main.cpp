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

vector<int> saNaive(const vector<int> &s)
{
    // s 是整数序列；返回按字典序排列的全部后缀起点。
    int n = s.size();
    vector<int> sa(n);
    iota(sa.begin(), sa.end(), 0);
    sort(sa.begin(),
         sa.end(),
         [&](int x, int y)
         {
             while (x < n && y < n && s[x] == s[y])
             {
                 x++;
                 y++;
             }
             return y < n && (x == n || s[x] < s[y]);
         });
    return sa;
}

vector<int> saDouble(const vector<int> &s)
{
    // s 是整数序列；返回倍增法求得的后缀数组。
    int n = s.size();
    vector<int> sa(n), rk(n), nrk(n), v = s;
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
    for (int i = 0; i < n; i++)
    {
        rk[i] = lower_bound(v.begin(), v.end(), s[i]) - v.begin();
    }
    iota(sa.begin(), sa.end(), 0);
    for (int k = 1; k < n; k *= 2)
    {
        auto cmp = [&](int x, int y)
        {
            if (rk[x] != rk[y])
            {
                return rk[x] < rk[y];
            }
            int a = x + k < n ? rk[x + k] : -1;
            int b = y + k < n ? rk[y + k] : -1;
            return a < b;
        };
        sort(sa.begin(), sa.end(), cmp);
        nrk[sa[0]] = 0;
        for (int i = 1; i < n; i++)
        {
            nrk[sa[i]] = nrk[sa[i - 1]] + cmp(sa[i - 1], sa[i]);
        }
        swap(rk, nrk);
    }
    return sa;
}

vector<int> saIs(const vector<int> &s, int m)
{
    // s 是整数序列，元素须在 [0,m] 内，m 是最大符号值；返回后缀数组。
    int n = s.size();
    if (n == 0)
    {
        return {};
    }
    if (n == 1)
    {
        return {0};
    }
    if (n == 2)
    {
        return s[0] < s[1] ? vector<int>{0, 1} : vector<int>{1, 0};
    }
    if (n < 10)
    {
        return saNaive(s);
    }
    if (n < 40)
    {
        return saDouble(s);
    }

    vector<int> sa(n, -1), sl(m + 1), ss(m + 1);
    vector<bool> ls(n);
    for (int i = n - 2; i >= 0; i--)
    {
        ls[i] = s[i] == s[i + 1] ? ls[i + 1] : s[i] < s[i + 1];
    }
    for (int i = 0; i < n; i++)
    {
        if (ls[i])
        {
            sl[s[i] + 1]++;
        }
        else
        {
            ss[s[i]]++;
        }
    }
    for (int i = 0; i <= m; i++)
    {
        ss[i] += sl[i];
        if (i < m)
        {
            sl[i + 1] += ss[i];
        }
    }

    auto induce = [&](const vector<int> &lms)
    {
        fill(sa.begin(), sa.end(), -1);
        vector<int> b = ss;
        for (int x : lms)
        {
            if (x < n)
            {
                sa[b[s[x]]++] = x;
            }
        }
        b = sl;
        sa[b[s[n - 1]]++] = n - 1;
        for (int i = 0; i < n; i++)
        {
            int x = sa[i];
            if (x > 0 && !ls[x - 1])
            {
                sa[b[s[x - 1]]++] = x - 1;
            }
        }
        b = sl;
        for (int i = n - 1; i >= 0; i--)
        {
            int x = sa[i];
            if (x > 0 && ls[x - 1])
            {
                sa[--b[s[x - 1] + 1]] = x - 1;
            }
        }
    };

    vector<int> id(n + 1, -1), lms;
    for (int i = 1; i < n; i++)
    {
        if (!ls[i - 1] && ls[i])
        {
            id[i] = lms.size();
            lms.push_back(i);
        }
    }
    induce(lms);
    int z = lms.size();
    if (z)
    {
        vector<int> ord;
        for (int x : sa)
        {
            if (x >= 0 && id[x] != -1)
            {
                ord.push_back(x);
            }
        }
        vector<int> ns(z);
        int nm = 0;
        ns[id[ord[0]]] = 0;
        for (int i = 1; i < z; i++)
        {
            int x = ord[i - 1], y = ord[i];
            int ex = id[x] + 1 < z ? lms[id[x] + 1] : n;
            int ey = id[y] + 1 < z ? lms[id[y] + 1] : n;
            bool same = ex - x == ey - y;
            while (same && x < ex)
            {
                if (s[x] != s[y])
                {
                    same = false;
                }
                x++;
                y++;
            }
            if (same && (x == n || s[x] != s[y]))
            {
                same = false;
            }
            if (!same)
            {
                nm++;
            }
            ns[id[ord[i]]] = nm;
        }
        vector<int> rsa = saIs(ns, nm);
        for (int i = 0; i < z; i++)
        {
            ord[i] = lms[rsa[i]];
        }
        induce(ord);
    }
    return sa;
}

template <class T> vector<int> suffixArray(const vector<T> &s)
{
    // s 是可比较序列；先离散化，再返回按字典序排列的全部后缀起点。
    int n = s.size();
    vector<int> p(n), a(n);
    iota(p.begin(), p.end(), 0);
    sort(p.begin(),
         p.end(),
         [&](int x, int y)
         {
             return s[x] < s[y];
         });
    int m = 0;
    for (int i = 0; i < n; i++)
    {
        if (i && s[p[i - 1]] != s[p[i]])
        {
            m++;
        }
        a[p[i]] = m;
    }
    return saIs(a, m);
}

vector<int> suffixArray(const string &s)
{
    // s 按无符号字节比较；返回按后缀字典序排列的起点下标。
    vector<int> a;
    a.reserve(s.size());
    for (unsigned char c : s)
    {
        a.push_back(c);
    }
    return saIs(a, 255);
}

template <class T> vector<int> lcpArray(const vector<T> &s, const vector<int> &sa)
{
    // s 是原序列，sa 是后缀数组；返回相邻后缀 LCP，ans[i]=lcp(sa[i],sa[i+1])。
    int n = s.size();
    vector<int> rk(n), lcp(max<int>(0, n - 1));
    for (int i = 0; i < n; i++)
    {
        rk[sa[i]] = i;
    }
    for (int i = 0, h = 0; i < n; i++)
    {
        if (rk[i] == 0)
        {
            continue;
        }
        int j = sa[rk[i] - 1];
        while (i + h < n && j + h < n && s[i + h] == s[j + h])
        {
            h++;
        }
        lcp[rk[i] - 1] = h;
        if (h)
        {
            h--;
        }
    }
    return lcp;
}

vector<int> lcpArray(const string &s, const vector<int> &sa)
{
    // s 是原字符串，sa 是后缀数组；返回相邻后缀的 LCP 数组。
    vector<int> a;
    a.reserve(s.size());
    for (unsigned char c : s)
    {
        a.push_back(c);
    }
    return lcpArray(a, sa);
}

signed main()
{
    ios::sync_with_stdio(false); cin.tie(nullptr);
    string s; cin >> s; vector<int> a; for (unsigned char c : s) a.push_back(c);
    auto sa = saDouble(a);
    for (int i = 0; i < (int)sa.size(); ++i) cout << (i ? " " : "") << sa[i];
    cout << '\n';
}
