#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "../topic_common.hpp"

template <int A = 26> class SubseqAM
{
    // nxt[i][c] 是位置 i（含）之后字符 c 的首次出现位置。
    vector<array<int, A>> nxt;

  public:
    explicit SubseqAM(const vector<int> &s) : nxt(s.size() + 1)
    {
        // s 是 [0,A) 内的文本序列；从右向左建立跳转表。
        int n = s.size();
        nxt.back().fill(n);
        for (int i = n - 1; i >= 0; i--)
        {
            assert(0 <= s[i] && s[i] < A); // 调试检查，可删
            nxt[i] = nxt[i + 1];
            nxt[i][s[i]] = i;
        }
    }

    bool contains(const vector<int> &t) const
    {
        // t 是 [0,A) 内的模式序列；返回它是否为文本的子序列。
        int p = 0, n = nxt.size() - 1;
        for (int c : t)
        {
            assert(0 <= c && c < A); // 调试检查，可删
            if (p == n || nxt[p][c] == n)
            {
                return false;
            }
            p = nxt[p][c] + 1;
        }
        return true;
    }
};
