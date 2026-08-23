#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int inf = 1E9;
constexpr long long INF = 4E18;
constexpr long double eps = 1E-12L;

template <class T> bool chmin(T &a, const T &b)
{
    // a 是当前值，b 是候选值；若 b 更小则更新 a 并返回 true。
    return b < a ? a = b, true : false;
}
template <class T> bool chmax(T &a, const T &b)
{
    // a 是当前值，b 是候选值；若 b 更大则更新 a 并返回 true。
    return a < b ? a = b, true : false;
}

template <class T> struct ImpTreap
{
  private:
    struct Node
    {
        // l、r 是左右儿子，sz 是子树元素数，pri 是随机小根堆优先级。
        int l = 0, r = 0, sz = 0;
        unsigned pri = 0;
        // val 是本点值，sum 是子树和，mul、add 表示尚未下传的 x -> x*mul+add。
        T val{}, sum{}, mul{1}, add{};
        // rev 表示当前子树的中序顺序需要翻转。
        bool rev = false;
    };

    // tr 是节点池，root 是当前序列根，seed 是 xorshift 优先级状态。
    vector<Node> tr{Node{}};
    int root = 0;
    unsigned seed = 712367821;

    unsigned rnd()
    {
        // 推进内部伪随机状态并返回新的优先级。
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;
        return seed;
    }

    int sizeNode(int x) const
    {
        // x 是节点编号；返回其子树元素数，0 号空节点返回 0。
        return tr[x].sz;
    }

    T sumNode(int x) const
    {
        // x 是节点编号；返回其子树和，0 号空节点返回零元。
        return tr[x].sum;
    }

    int newNode(const T &x)
    {
        // x 是新元素值；新建一个无儿子的节点并返回编号。
        tr.push_back({0, 0, 1, rnd(), x, x, T(1), T{}, false});
        return (int)tr.size() - 1;
    }

    void pull(int x)
    {
        // x 是非空节点；由儿子与本点值重算大小和子树和。
        tr[x].sz = sizeNode(tr[x].l) + 1 + sizeNode(tr[x].r);
        tr[x].sum = sumNode(tr[x].l) + tr[x].val + sumNode(tr[x].r);
    }

    void applyAffine(int x, const T &mul, const T &add)
    {
        // x 是子树根，mul、add 表示后执行的 x -> x*mul+add；原地更新节点摘要和懒标记。
        if (!x)
        {
            return;
        }
        tr[x].val = tr[x].val * mul + add;
        tr[x].sum = tr[x].sum * mul + add * T(sizeNode(x));
        tr[x].mul = tr[x].mul * mul;
        tr[x].add = tr[x].add * mul + add;
    }

    void applyReverse(int x)
    {
        // x 是子树根；交换左右儿子并记录一次延迟翻转。
        if (!x)
        {
            return;
        }
        swap(tr[x].l, tr[x].r);
        tr[x].rev = !tr[x].rev;
    }

    void push(int x)
    {
        // x 是非空节点；把翻转与仿射懒标记按语义下传给两个儿子。
        if (tr[x].rev)
        {
            applyReverse(tr[x].l);
            applyReverse(tr[x].r);
            tr[x].rev = false;
        }
        if (tr[x].mul != T(1) || tr[x].add != T{})
        {
            applyAffine(tr[x].l, tr[x].mul, tr[x].add);
            applyAffine(tr[x].r, tr[x].mul, tr[x].add);
            tr[x].mul = T(1);
            tr[x].add = T{};
        }
    }

    void pullAll(int x)
    {
        // x 是新建 Cartesian 树的根；递归重算该子树全部摘要。
        if (!x)
        {
            return;
        }
        pullAll(tr[x].l);
        pullAll(tr[x].r);
        pull(x);
    }

    pair<int, int> split(int x, int k)
    {
        // x 是子树根，k 是左段长度；返回前 k 项与其余项的两棵树根。
        if (!x)
        {
            return {0, 0};
        }
        push(x);
        int z = sizeNode(tr[x].l);
        if (k <= z)
        {
            auto [a, b] = split(tr[x].l, k);
            tr[x].l = b;
            pull(x);
            return {a, x};
        }
        auto [a, b] = split(tr[x].r, k - z - 1);
        tr[x].r = a;
        pull(x);
        return {x, b};
    }

    int merge(int x, int y)
    {
        // x、y 是相邻两段的根；按中序保持 x 在前，返回合并后的根。
        if (!x || !y)
        {
            return x ? x : y;
        }
        if (tr[x].pri < tr[y].pri)
        {
            push(x);
            tr[x].r = merge(tr[x].r, y);
            pull(x);
            return x;
        }
        push(y);
        tr[y].l = merge(x, tr[y].l);
        pull(y);
        return y;
    }

  public:
    explicit ImpTreap(int cap = 0)
    {
        // cap 是预计节点总数，只用于预留池容量；构造空序列。
        assert(cap >= 0); // 调试检查，可删
        reserveNodes(cap);
    }

    explicit ImpTreap(const vector<T> &a, int cap = 0)
    {
        // a 是初始序列，cap 是预计节点总数；在线性时间建出当前序列。
        assert(cap >= 0); // 调试检查，可删
        reserveNodes(max<int>(cap, (int)a.size()));
        build(a);
    }

    void reserveNodes(int cap)
    {
        // cap 是预计节点总数；预留 0 号哨兵外的池容量，不改变现有序列。
        assert(cap >= 0); // 调试检查，可删
        tr.reserve(cap + 1);
    }

    void clear()
    {
        // 清空当前序列和节点池内容，保留已申请容量并重置随机状态。
        tr.clear();
        tr.push_back({});
        root = 0;
        seed = 712367821;
    }

    void build(const vector<T> &a)
    {
        // a 是新的完整序列；清空旧序列，用优先级 Cartesian 树在线性时间重建。
        clear();
        reserveNodes((int)a.size());
        vector<int> st;
        st.reserve(a.size());
        for (const T &x : a)
        {
            int u = newNode(x), last = 0;
            while (!st.empty() && tr[st.back()].pri > tr[u].pri)
            {
                last = st.back();
                st.pop_back();
            }
            tr[u].l = last;
            if (!st.empty())
            {
                tr[st.back()].r = u;
            }
            st.push_back(u);
        }
        root = st.empty() ? 0 : st[0];
        pullAll(root);
    }

    int size() const
    {
        // 返回当前序列元素数。
        return sizeNode(root);
    }

    bool empty() const
    {
        // 返回当前序列是否为空。
        return root == 0;
    }

    void insert(int k, const T &x)
    {
        // k 是插入位置，x 是新值；把 x 插到原第 k 项前，允许 k=size()。
        assert(0 <= k && k <= size()); // 调试检查，可删
        auto [a, b] = split(root, k);
        root = merge(merge(a, newNode(x)), b);
    }

    void erase(int k)
    {
        // k 是删除位置；删除原第 k 项，已分配节点不回收。
        assert(0 <= k && k < size()); // 调试检查，可删
        auto [a, b] = split(root, k);
        root = merge(a, split(b, 1).second);
    }

    void reverse(int l, int r)
    {
        // l、r 是半开区间端点；原地翻转 [l,r)，空区间允许。
        assert(0 <= l && l <= r && r <= size()); // 调试检查，可删
        auto [a, b] = split(root, l);
        auto [c, d] = split(b, r - l);
        applyReverse(c);
        root = merge(merge(a, c), d);
    }

    void affine(int l, int r, const T &mul, const T &add)
    {
        // l、r 是半开区间端点，mul、add 是变换参数；对 [l,r) 每项执行 x -> x*mul+add。
        assert(0 <= l && l <= r && r <= size()); // 调试检查，可删
        auto [a, b] = split(root, l);
        auto [c, d] = split(b, r - l);
        applyAffine(c, mul, add);
        root = merge(merge(a, c), d);
    }

    T sum(int l, int r)
    {
        // l、r 是半开区间端点；返回 [l,r) 的元素和，空区间返回零元。
        assert(0 <= l && l <= r && r <= size()); // 调试检查，可删
        auto [a, b] = split(root, l);
        auto [c, d] = split(b, r - l);
        T ans = sumNode(c);
        root = merge(merge(a, c), d);
        return ans;
    }

    vector<T> values()
    {
        // 把当前序列按下标顺序复制到 vector；会下传懒标记但不改变序列语义。
        vector<T> a;
        a.reserve(size());
        auto dfs = [&](auto &&self, int x) -> void
        {
            if (!x)
            {
                return;
            }
            push(x);
            self(self, tr[x].l);
            a.push_back(tr[x].val);
            self(self, tr[x].r);
        };
        dfs(dfs, root);
        return a;
    }
};

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<long long> a(n);
    for (auto &x : a)
    {
        cin >> x;
    }
    ImpTreap<long long> seq(a, n);
    while (q--)
    {
        int type, l, r;
        cin >> type >> l >> r;
        if (type == 0)
        {
            seq.reverse(l, r);
        }
        else
        {
            cout << seq.sum(l, r) << '\n';
        }
    }
    return 0;
}
