#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "support/topic_common.hpp"

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
