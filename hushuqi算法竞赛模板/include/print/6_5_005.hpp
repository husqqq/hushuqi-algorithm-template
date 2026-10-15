#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

vector<int> minInvOrder(const vector<int> &par, vector<long long> c,
                                  vector<long long> d, int root = 0)
{
    // par 是父亲数组（根为 -1），c、d 是非负点值，root 是根；
    // 返回祖先先于后代、使 sum_i sum_{j<i} c[p_i] d[p_j] 最小的一个排列。
    int n = par.size();
    assert((int)c.size() == n && (int)d.size() == n && par[root] == -1); // 调试检查，可删。

    struct DSU
    {
        vector<int> p, id; // p 是并查集父亲（根为负的集合大小），id[r] 是集合 r 当前对应的块代表。

        DSU(int n) : p(n, -1), id(n)
        {
            // n 是点数；每个点自成一个集合，代表为自身。
            iota(id.begin(), id.end(), 0);
        }

        int find(int x)
        {
            // x 是点；返回所在集合的根，带路径压缩。
            return p[x] < 0 ? x : p[x] = find(p[x]);
        }

        int operator[](int x)
        {
            // x 是点；返回所在集合当前对应的块代表。
            return id[find(x)];
        }

        void merge(int x, int y, int z)
        {
            // x、y 是两点；合并其集合并把代表设为 z。
            x = find(x);
            y = find(y);
            if (x == y)
            {
                id[x] = z;
                return;
            }
            if (-p[x] < -p[y])
            {
                swap(x, y);
            }
            p[x] += p[y];
            p[y] = x;
            id[x] = z;
        }
    };

    struct Block
    {
        __int128 d, c; // d、c 是块内 d 值之和与 c 值之和。
        int root, ver; // root 是块代表点，ver 是入堆时的版本号，过期版本出堆时跳过。

        static bool ratioLess(const Block &x, const Block &y)
        {
            // x、y 是块；返回 c/d 比值是否 x<y，全零块视为最小。
            if (y.c == 0 && y.d == 0)
            {
                return false;
            }
            if (x.c == 0 && x.d == 0)
            {
                return true;
            }
            return (__int128)x.c * y.d < (__int128)x.d * y.c;
        }

        bool operator<(const Block &o) const
        {
            // o 是另一块；先比 c/d 比值，再比 (root,ver)，使大根堆先取比值最大的块。
            if (ratioLess(*this, o))
            {
                return true;
            }
            if (ratioLess(o, *this))
            {
                return false;
            }
            return pair(root, ver) < pair(o.root, o.ver);
        }
    };

    DSU dsu(n);
    priority_queue<Block> q;
    vector<int> nxt(n), ver(n);
    vector<__int128> sumC(n), sumD(n);
    iota(nxt.begin(), nxt.end(), 0);
    for (int u = 0; u < n; u++)
    {
        sumC[u] = c[u];
        sumD[u] = d[u];
        if (u != root)
        {
            assert(0 <= par[u] && par[u] < n); // 调试检查，可删。
            q.push({sumD[u], sumC[u], u, 0});
        }
    }
    while (!q.empty())
    {
        auto x = q.top();
        q.pop();
        int u = x.root;
        if (ver[u] != x.ver)
        {
            continue;
        }
        int v = dsu[par[u]];
        sumD[v] += sumD[u];
        sumC[v] += sumC[u];
        dsu.merge(u, v, v);
        if (v != root)
        {
            q.push({sumD[v], sumC[v], v, ++ver[v]});
        }
        swap(nxt[u], nxt[v]);
    }

    vector<int> ans{nxt[root]};
    while (ans.back() != root)
    {
        ans.push_back(nxt[ans.back()]);
    }
    reverse(ans.begin(), ans.end());
    return ans;
}
