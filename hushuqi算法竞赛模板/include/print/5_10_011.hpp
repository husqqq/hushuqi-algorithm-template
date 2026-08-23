#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class M1, class M2>
vector<int> matroidMeet(int n, M1 first, M2 second)
{
    // n 是底集大小，first、second 是独立性判定；返回升序最大公共独立集。
    vector<bool> in(n);
    while (true)
    {
        vector<int> pre(n, -2), q;
        for (int y = 0; y < n; y++)
        {
            if (!in[y])
            {
                auto a = in;
                a[y] = true;
                if (first(a))
                {
                    pre[y] = -1;
                    q.push_back(y);
                }
            }
        }
        int last = -1;
        for (int h = 0; h < (int)q.size() && last < 0; h++)
        {
            int x = q[h];
            if (!in[x])
            {
                auto a = in;
                a[x] = true;
                if (second(a))
                {
                    last = x;
                    break;
                }
                for (int y = 0; y < n; y++)
                {
                    if (in[y] && pre[y] == -2)
                    {
                        a[y] = false;
                        if (second(a))
                        {
                            pre[y] = x;
                            q.push_back(y);
                        }
                        a[y] = true;
                    }
                }
            }
            else
            {
                for (int y = 0; y < n; y++)
                {
                    if (!in[y] && pre[y] == -2)
                    {
                        auto a = in;
                        a[x] = false;
                        a[y] = true;
                        if (first(a))
                        {
                            pre[y] = x;
                            q.push_back(y);
                        }
                    }
                }
            }
        }
        if (last < 0)
        {
            break;
        }
        for (int x = last; x >= 0; x = pre[x])
        {
            in[x] = !in[x];
        }
    }
    vector<int> ans;
    for (int i = 0; i < n; i++)
    {
        if (in[i])
        {
            ans.push_back(i);
        }
    }
    return ans;
}
