#pragma once

// QOJ candidate template extracted from the verified standalone solver.
// Keep this file outside the formal template until it is approved.

#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr long long NEG_INF = -(1LL << 60) * 3;

long long addExtended(long long a, long long b)
{
    // a、b 是普通值或 NEG_INF；返回带负无穷吸收元的加法结果。
    return a == NEG_INF || b == NEG_INF ? NEG_INF : a + b;
}

struct Tag
{
    long long currentCurrent = 0; // 当前值到当前值的系数。
    long long currentHistory = NEG_INF; // 当前值对历史最大值的贡献。
    long long historyHistory = 0; // 历史最大值的保留系数。
    long long constantCurrent = NEG_INF; // 对当前值的常数赋值。
    long long constantHistory = NEG_INF; // 对历史最大值的常数贡献。
};

Tag compose(const Tag &first, const Tag &second)
{
    // first、second 按先后顺序作用；返回先 first 后 second 的复合标记。
    return {
        max(addExtended(first.currentCurrent, second.currentCurrent), NEG_INF),
        max({addExtended(first.currentCurrent, second.currentHistory),
             addExtended(first.currentHistory, second.historyHistory), NEG_INF}),
        max(addExtended(first.historyHistory, second.historyHistory), NEG_INF),
        max(addExtended(first.constantCurrent, second.currentCurrent), second.constantCurrent),
        max({addExtended(first.constantHistory, second.historyHistory),
             addExtended(first.constantCurrent, second.currentHistory), second.constantHistory})
    };
}

struct Info
{
    long long current = NEG_INF; // 区间当前最大值。
    long long historical = NEG_INF; // 区间历史最大值。
};

Info apply(Info value, const Tag &tag)
{
    // value 是节点摘要，tag 是区间标记；返回作用标记后的摘要。
    return {
        max(addExtended(value.current, tag.currentCurrent), tag.constantCurrent),
        max({addExtended(value.current, tag.currentHistory),
             addExtended(value.historical, tag.historyHistory), tag.constantHistory})
    };
}

class SegmentTree
{
    int n; // 数组长度。
    vector<Info> tree; // 当前节点摘要。
    vector<Tag> lazy; // 尚未下传到儿子的复合标记。

    void build(int node, int left, int right, const vector<long long> &a)
    {
        if (left + 1 == right)
        {
            tree[node] = {a[left], a[left]};
            return;
        }
        int middle = (left + right) / 2;
        build(node * 2, left, middle, a);
        build(node * 2 + 1, middle, right, a);
        pull(node);
    }

    void pull(int node)
    {
        tree[node] = {max(tree[node * 2].current, tree[node * 2 + 1].current),
                      max(tree[node * 2].historical, tree[node * 2 + 1].historical)};
    }

    void applyNode(int node, const Tag &tag)
    {
        tree[node] = apply(tree[node], tag);
        lazy[node] = compose(lazy[node], tag);
    }

    void push(int node)
    {
        applyNode(node * 2, lazy[node]);
        applyNode(node * 2 + 1, lazy[node]);
        lazy[node] = {};
    }

    void update(int node, int left, int right, int queryLeft, int queryRight, const Tag &tag)
    {
        if (queryRight <= left || right <= queryLeft)
        {
            return;
        }
        if (queryLeft <= left && right <= queryRight)
        {
            applyNode(node, tag);
            return;
        }
        push(node);
        int middle = (left + right) / 2;
        update(node * 2, left, middle, queryLeft, queryRight, tag);
        update(node * 2 + 1, middle, right, queryLeft, queryRight, tag);
        pull(node);
    }

    Info query(int node, int left, int right, int queryLeft, int queryRight)
    {
        if (queryRight <= left || right <= queryLeft)
        {
            return {};
        }
        if (queryLeft <= left && right <= queryRight)
        {
            return tree[node];
        }
        push(node);
        int middle = (left + right) / 2;
        Info a = query(node * 2, left, middle, queryLeft, queryRight);
        Info b = query(node * 2 + 1, middle, right, queryLeft, queryRight);
        return {max(a.current, b.current), max(a.historical, b.historical)};
    }

  public:
    // a 是初始数组；建立支持区间加、区间赋值和当前/历史最大值查询的线段树。
    explicit SegmentTree(const vector<long long> &a) : n(a.size()), tree(4 * n), lazy(4 * n)
    {
        assert(n > 0);
        build(1, 0, n, a);
    }

    // [left,right) 是 0 下标区间，value 是增量；区间加并更新历史最大值。
    void add(int left, int right, long long value)
    {
        update(1, 0, n, left, right, Tag{value, value, 0, NEG_INF, NEG_INF});
    }

    // [left,right) 是 0 下标区间，value 是新值；区间赋值并更新历史最大值。
    void assign(int left, int right, long long value)
    {
        update(1, 0, n, left, right, Tag{NEG_INF, NEG_INF, 0, value, value});
    }

    // [left,right) 是 0 下标区间；返回当前最大值及历史最大值摘要。
    Info query(int left, int right)
    {
        return query(1, 0, n, left, right);
    }
};
