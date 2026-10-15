#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

struct ErasableDSU
{
    // p[x]<0 表示 x 是内部树根，-p[x] 是只增不减的物理节点数；否则 p[x] 是父亲。
    vector<int> p;
    // cnt[x] 仅在 x 是根时表示集合中的当前元素数。
    vector<int> cnt;
    // pos[x] 是当前代表原元素 x 的内部节点。
    vector<int> pos;

    ErasableDSU(int n = 0)
    {
        // n 是原元素个数；初始时每个元素自成集合。
        init(n);
    }

    void init(int n)
    {
        // n 是新的原元素个数；清空全部合并与删除状态，无返回值。
        assert(n >= 0); // 调试检查，可删
        p.assign(n, -1);
        cnt.assign(n, 1);
        pos.resize(n);
        iota(pos.begin(), pos.end(), 0);
    }

    int findNode(int x)
    {
        // x 是内部节点编号；压缩父链并返回内部树根。
        return p[x] < 0 ? x : p[x] = findNode(p[x]);
    }

    bool merge(int x, int y)
    {
        // x、y 是原元素编号；合并所在集合并返回是否真正发生合并。
        assert(0 <= x && x < (int)pos.size() && 0 <= y && y < (int)pos.size()); // 调试检查，可删
        int a = findNode(pos[x]);
        int b = findNode(pos[y]);
        if (a == b)
        {
            return false;
        }
        if (p[a] > p[b])
        {
            swap(a, b);
        }
        p[a] += p[b];
        p[b] = a;
        cnt[a] += cnt[b];
        return true;
    }

    bool erase(int x)
    {
        // x 是原元素编号；把它移到新的单点集合，原本已是单点时返回 false。
        assert(0 <= x && x < (int)pos.size()); // 调试检查，可删
        int r = findNode(pos[x]);
        if (cnt[r] == 1)
        {
            return false;
        }
        cnt[r]--;
        pos[x] = p.size();
        p.push_back(-1);
        cnt.push_back(1);
        return true;
    }

    bool same(int x, int y)
    {
        // x、y 是原元素编号；返回二者当前是否同属一个集合。
        assert(0 <= x && x < (int)pos.size() && 0 <= y && y < (int)pos.size()); // 调试检查，可删
        return findNode(pos[x]) == findNode(pos[y]);
    }

    int size(int x)
    {
        // x 是原元素编号；返回其当前集合中的原元素数。
        assert(0 <= x && x < (int)pos.size()); // 调试检查，可删
        return cnt[findNode(pos[x])];
    }
};
