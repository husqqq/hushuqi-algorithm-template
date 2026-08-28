// QOJ candidate template extracted from the verified standalone solver.
// Keep this file outside the formal template until it is approved.

#include <bits/stdc++.h>
using namespace std;

constexpr long long NEG_INF = -(1LL << 60) * 3;

long long addExtended(long long a, long long b)
{
    return a == NEG_INF || b == NEG_INF ? NEG_INF : a + b;
}

struct Tag
{
    long long currentCurrent = 0;
    long long currentHistory = NEG_INF;
    long long historyHistory = 0;
    long long constantCurrent = NEG_INF;
    long long constantHistory = NEG_INF;
};

Tag compose(const Tag &first, const Tag &second)
{
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
    long long current = NEG_INF;
    long long historical = NEG_INF;
};

Info apply(Info value, const Tag &tag)
{
    return {
        max(addExtended(value.current, tag.currentCurrent), tag.constantCurrent),
        max({addExtended(value.current, tag.currentHistory),
             addExtended(value.historical, tag.historyHistory), tag.constantHistory})
    };
}

class SegmentTree
{
    int n;
    vector<Info> tree;
    vector<Tag> lazy;

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
    explicit SegmentTree(const vector<long long> &a) : n(a.size()), tree(4 * n), lazy(4 * n)
    {
        build(1, 0, n, a);
    }

    void add(int left, int right, long long value)
    {
        update(1, 0, n, left, right, Tag{value, value, 0, NEG_INF, NEG_INF});
    }

    void assign(int left, int right, long long value)
    {
        update(1, 0, n, left, right, Tag{NEG_INF, NEG_INF, 0, value, value});
    }

    Info query(int left, int right)
    {
        return query(1, 0, n, left, right);
    }
};

inline void solveHistoricalMaximum(istream &cin, ostream &cout)
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, operations;
    cin >> n >> operations;
    vector<long long> a(n);
    for (auto &x : a)
    {
        cin >> x;
    }
    SegmentTree tree(a);
    while (operations--)
    {
        int type, left, right;
        cin >> type >> left >> right;
        left--;
        if (type == 1 || type == 2)
        {
            long long value;
            cin >> value;
            if (type == 1)
            {
                tree.add(left, right, value);
            }
            else
            {
                tree.assign(left, right, value);
            }
        }
        else
        {
            Info result = tree.query(left, right);
            cout << (type == 3 ? result.current : result.historical) << '\n';
        }
    }
}
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solveHistoricalMaximum(cin, cout);
}
