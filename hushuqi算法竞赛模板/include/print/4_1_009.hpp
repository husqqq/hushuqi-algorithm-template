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

vector<int> sunday(string_view s, string_view p)
{
    // s 是文本串，p 是模式串；返回全部匹配起点，按下标递增排列。
    vector<int> ans;
    if (p.empty())
    {
        for (int i = 0; i <= (int)s.size(); i++)
        {
            ans.push_back(i);
        }
        return ans;
    }
    array<int, 256> sh;
    sh.fill(p.size() + 1);
    for (int i = 0; i < (int)p.size(); i++)
    {
        sh[(unsigned char)p[i]] = p.size() - i;
    }
    for (int i = 0; i + (int)p.size() <= (int)s.size();)
    {
        int j = 0;
        while (j < (int)p.size() && s[i + j] == p[j])
        {
            j++;
        }
        if (j == (int)p.size())
        {
            ans.push_back(i);
        }
        if (i + (int)p.size() == (int)s.size())
        {
            break;
        }
        i += sh[(unsigned char)(s[i + p.size()])];
    }
    return ans;
}
