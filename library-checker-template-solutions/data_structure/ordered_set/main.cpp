
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/priority_queue.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
#define int long long

// 知识点 3.9.001：PBDS 头文件、命名空间、模板策略与迭代器体系。
namespace pbds_ordered
{
using namespace __gnu_pbds;

template <class T> using OrdSet = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

array<int, 2> rankInfo(const vector<int> &a, int x)
{
    // a 是待插入值，x 是查询值；返回严格小于 x 的数量与第一个不小于 x 的值，无后继时第二维为 -1。
    OrdSet<pair<int, int>> tr;
    for (int i = 0; i < (int)a.size(); i++)
    {
        tr.insert({a[i], i});
    }
    int rk = tr.order_of_key({x, -1});
    auto it = tr.lower_bound({x, -1});
    return {rk, it == tr.end() ? -1 : it->first};
}
}

// 知识点 3.9.002：PBDS priority_queue 的五种策略标签。
namespace pbds_heap_policy
{
using namespace __gnu_pbds;

template <class T> using BinaryHeap = __gnu_pbds::priority_queue<T, less<T>, binary_heap_tag>;
template <class T> using BinomialHeap = __gnu_pbds::priority_queue<T, less<T>, binomial_heap_tag>;
template <class T> using RcBinomialHeap = __gnu_pbds::priority_queue<T, less<T>, rc_binomial_heap_tag>;
template <class T> using PairingHeap = __gnu_pbds::priority_queue<T, less<T>, pairing_heap_tag>;
template <class T> using ThinHeap = __gnu_pbds::priority_queue<T, less<T>, thin_heap_tag>;

int heapTop(const vector<int> &a)
{
    // a 是初始键值；用默认推荐的 pairing heap 返回最大值，空表返回 -1。
    PairingHeap<int> hp;
    for (int x : a)
    {
        hp.push(x);
    }
    return hp.empty() ? -1 : hp.top();
}
}

// 知识点 3.9.003：PBDS 堆的点迭代器、modify、erase 与 join。
namespace pbds_mutable_heap
{
using namespace __gnu_pbds;

using Heap = __gnu_pbds::priority_queue<int, less<int>, pairing_heap_tag>;

int editHeap()
{
    // 无参数；演示点迭代器改键、删除与合并，返回最终堆顶。
    Heap a, b;
    auto p = a.push(10);
    auto q = a.push(30);
    a.modify(p, 40);
    a.erase(q);
    b.push(25);
    a.join(b);
    return a.top();
}
}

// 知识点 3.9.004：PBDS 堆、STL 堆与线段树的选型边界。
namespace pbds_heap_compare
{
using namespace __gnu_pbds;

using MutableHeap = __gnu_pbds::priority_queue<int, less<int>, pairing_heap_tag>;

array<int, 2> compareHeap(const vector<int> &a)
{
    // a 是键值表；分别用 STL 堆与可修改 PBDS 堆求最大值，返回两者结果。
    auto stl = a;
    make_heap(stl.begin(), stl.end());
    MutableHeap pbds;
    for (int x : a)
    {
        pbds.push(x);
    }
    return {stl.empty() ? -1 : stl.front(), pbds.empty() ? -1 : pbds.top()};
}
}

// 知识点 3.9.005：PBDS tree 的三种底层平衡策略。
namespace pbds_tree_policy
{
using namespace __gnu_pbds;

template <class T> using RBTree = tree<T, null_type, less<T>, rb_tree_tag>;
template <class T> using SplayTree = tree<T, null_type, less<T>, splay_tree_tag>;
template <class T> using OVTree = tree<T, null_type, less<T>, ov_tree_tag>;

array<int, 3> treeSizes(const vector<int> &a)
{
    // a 是待插入值；返回三种策略去重后的元素数。
    RBTree<int> x;
    SplayTree<int> y;
    OVTree<int> z;
    for (int v : a)
    {
        x.insert(v);
        y.insert(v);
        z.insert(v);
    }
    return {(int)x.size(), (int)y.size(), (int)z.size()};
}
}

// 知识点 3.9.006：PBDS tree 的 set/map、split 与 join。
namespace pbds_split_join
{
using namespace __gnu_pbds;

using Map = tree<int, int, less<int>, rb_tree_tag>;

void splitAt(Map &a, Map &b, int x)
{
    // a 是原映射，b 必须为空，x 是分界键；把所有严格大于 x 的键移入 b，无返回值。
    a.split(x, b);
}

void joinOrdered(Map &a, Map &b)
{
    // a、b 的键域必须严格分离且 a 的键全小于 b；把 b 合入 a 并清空 b。
    a.join(b);
}
}

// 知识点 3.9.009：自定义 Node_Update、metadata_type 与节点迭代器。
namespace pbds_metadata
{
using namespace __gnu_pbds;

template <class NodeIt, class NodeCIt, class Cmp, class Alloc> struct SizeUpdate
{
    using metadata_type = int;

    void operator()(NodeIt it, NodeCIt end) const
    {
        // it 是待更新节点，end 是空节点；把元信息更新为子树节点数，无返回值。
        int s = 1;
        auto l = it.get_l_child();
        auto r = it.get_r_child();
        if (l != end)
        {
            s += l.get_metadata();
        }
        if (r != end)
        {
            s += r.get_metadata();
        }
        const_cast<int &>(it.get_metadata()) = s;
    }
};

using Tree = tree<int, null_type, less<int>, rb_tree_tag, SizeUpdate>;

int rootSize(const vector<int> &a)
{
    // a 是待插入值；返回去重后的树根元信息，空树返回 0。
    Tree tr;
    for (int x : a)
    {
        tr.insert(x);
    }
    return tr.empty() ? 0 : tr.node_begin().get_metadata();
}
}

// 知识点 3.9.010：用自定义子树和回答键值前缀和。
namespace pbds_subtree_sum
{
using namespace __gnu_pbds;

template <class NodeIt, class NodeCIt, class Cmp, class Alloc> struct SumUpdate
{
    using metadata_type = int;

    void operator()(NodeIt it, NodeCIt end) const
    {
        // it 是待更新节点，end 是空节点；元信息保存本节点键与左右子树键之和。
        int s = **it;
        auto l = it.get_l_child();
        auto r = it.get_r_child();
        if (l != end)
        {
            s += l.get_metadata();
        }
        if (r != end)
        {
            s += r.get_metadata();
        }
        const_cast<int &>(it.get_metadata()) = s;
    }
};

using Tree = tree<int, null_type, less<int>, rb_tree_tag, SumUpdate>;

int prefixSum(const Tree &tr, int x)
{
    // tr 是键集合，x 是右边界；返回所有严格小于 x 的不同键之和。
    int ans = 0;
    auto it = tr.node_begin();
    auto end = tr.node_end();
    while (it != end)
    {
        auto l = it.get_l_child();
        int v = **it;
        if (v < x)
        {
            if (l != end)
            {
                ans += l.get_metadata();
            }
            ans += v;
            it = it.get_r_child();
        }
        else
        {
            it = l;
        }
    }
    return ans;
}
}

// 知识点 3.9.013：PBDS 的编译器边界与最小可用性检查。
namespace pbds_compat
{
using namespace __gnu_pbds;

bool available()
{
    // 无参数；当前代码能编译即表示使用 GNU libstdc++，返回 true。
#ifdef __GLIBCXX__
    return true;
#else
    return false;
#endif
}
}

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    pbds_ordered::OrdSet<int> st;
    while (n--)
    {
        int x;
        cin >> x;
        st.insert(x);
    }
    while (q--)
    {
        int type, x;
        cin >> type >> x;
        if (type == 0)
        {
            st.insert(x);
        }
        else if (type == 1)
        {
            st.erase(x);
        }
        else if (type == 2)
        {
            cout << (x > (int)st.size() ? -1 : *st.find_by_order(x - 1)) << '\n';
        }
        else if (type == 3)
        {
            cout << st.order_of_key(x + 1) << '\n';
        }
        else if (type == 4)
        {
            auto it = st.upper_bound(x);
            cout << (it == st.begin() ? -1 : *prev(it)) << '\n';
        }
        else
        {
            auto it = st.lower_bound(x);
            cout << (it == st.end() ? -1 : *it) << '\n';
        }
    }
    return 0;
}
