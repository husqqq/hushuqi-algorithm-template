#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "support/topic_common.hpp"

template <int A = 26> class SqAM
{
    // go[u][c] 是状态 u 读字符 c 后到达的最早后继，-1 表示不存在。
    vector<array<int, A>> go;
    // pre[u] 是与状态 u 字符相同的上一次出现位置。
    vector<int> pre;
    // last[c] 是字符 c 最近一次出现所对应的状态。
    array<int, A> last;

  public:
    SqAM()
    {
        // 建立只含空文本状态的自动机。
        last.fill(-1);
        go.push_back({});
        go[0].fill(-1);
        pre.push_back(-1);
    }

    void add(int x)
    {
        // x 是 [0,A) 内的新字符编号；把它追加到文本末尾。
        assert(0 <= x && x < A); // 调试检查，可删
        int v = go.size();
        go.push_back({});
        go.back().fill(-1);
        pre.push_back(last[x]);
        for (int c = 0; c < A; c++)
        {
            for (int u = last[c]; u != -1 && go[u][x] == -1; u = pre[u])
            {
                go[u][x] = v;
            }
        }
        if (last[x] == -1)
        {
            go[0][x] = v;
        }
        last[x] = v;
    }

    bool contains(const vector<int> &s) const
    {
        // s 是 [0,A) 内的模式序列；返回它是否为当前文本的子序列。
        int u = 0;
        for (int x : s)
        {
            if (x < 0 || x >= A || go[u][x] == -1)
            {
                return false;
            }
            u = go[u][x];
        }
        return true;
    }

    int size() const
    {
        // 返回当前文本长度。
        return go.size() - 1;
    }
};
