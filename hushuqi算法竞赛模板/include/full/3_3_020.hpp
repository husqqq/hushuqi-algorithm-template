#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class T> class PerAffSeg
{
    struct Node
    {
        // l、r 是左右儿子编号；sum 是当前区间已经计入本节点懒标记后的元素和。
        int32_t l = 0, r = 0;
        T sum{};
        // mul、add 是尚未下传的变换 x -> mul*x+add。
        T mul{}, add{};
    };

    // n 是初始数组长度，one 是 T 的乘法单位元，base 是初始版本根。
    int n;
    T one;
    int base = 0;
    // t 是全部版本共用的节点池；0 号节点只作哨兵，不是合法版本根。
    vector<Node> t;

    int newNode()
    {
        // 新建空节点并返回编号；新节点的仿射标记初始化为恒等变换。
        t.push_back(Node{});
        int p = (int)t.size() - 1;
        assert(p <= numeric_limits<int32_t>::max()); // 调试检查，可删
        t[p].mul = one;
        return p;
    }

    int clone(int p)
    {
        // p 是旧版本中的非空节点；复制它并返回新节点编号。
        assert(p > 0); // 调试检查，可删
        t.push_back(t[p]);
        int q = (int)t.size() - 1;
        assert(q <= numeric_limits<int32_t>::max()); // 调试检查，可删
        return q;
    }

    bool idle(int p) const
    {
        // p 是非空节点；返回其待下传仿射标记是否为恒等变换。
        return t[p].mul == one && t[p].add == T{};
    }

    void pull(int p)
    {
        // p 是已经无本层懒标记的内部节点；由两个儿子重算当前区间和。
        t[p].sum = t[t[p].l].sum + t[t[p].r].sum;
    }

    void applyAt(int p, int l, int r, const T &mul, const T &add)
    {
        // p 是可独占修改的节点，[l,r) 是其区间；就地叠加后执行的仿射变换。
        t[p].sum = t[p].sum * mul + T(r - l) * add;
        t[p].mul = t[p].mul * mul;
        t[p].add = t[p].add * mul + add;
    }

    int copyApply(int p, int l, int r, const T &mul, const T &add)
    {
        // p 是旧节点，[l,r) 是其区间；复制后施加仿射变换并返回新根。
        int q = clone(p);
        applyAt(q, l, r, mul, add);
        return q;
    }

    void push(int p, int l, int r)
    {
        // p 是已复制且可修改的内部节点，[l,r) 是其区间；持久化地下传本层懒标记。
        if (r - l == 1 || idle(p))
        {
            return;
        }
        int m = (l + r) / 2;
        T mul = t[p].mul, add = t[p].add;
        t[p].l = copyApply(t[p].l, l, m, mul, add);
        t[p].r = copyApply(t[p].r, m, r, mul, add);
        t[p].mul = one;
        t[p].add = T{};
    }

    int srcL(int p, int l, int r)
    {
        // p 是源版本节点，[l,r) 是其内部区间；返回已落实 p 标记的左儿子版本根。
        if (idle(p))
        {
            return t[p].l;
        }
        int m = (l + r) / 2;
        T mul = t[p].mul, add = t[p].add;
        return copyApply(t[p].l, l, m, mul, add);
    }

    int srcR(int p, int l, int r)
    {
        // p 是源版本节点，[l,r) 是其内部区间；返回已落实 p 标记的右儿子版本根。
        if (idle(p))
        {
            return t[p].r;
        }
        int m = (l + r) / 2;
        T mul = t[p].mul, add = t[p].add;
        return copyApply(t[p].r, m, r, mul, add);
    }

    int build(const vector<T> &a, int l, int r)
    {
        // a 是初始数组，[l,r) 是当前建树区间；返回该区间的初始版本根。
        int p = newNode();
        if (r - l == 1)
        {
            t[p].sum = a[l];
            return p;
        }
        int m = (l + r) / 2;
        t[p].l = build(a, l, m);
        t[p].r = build(a, m, r);
        pull(p);
        return p;
    }

    int affine(int p, int l, int r, int ql, int qr, const T &mul, const T &add)
    {
        // p 是旧版本节点，[l,r) 是其区间；对交集施加仿射变换并返回新节点。
        if (qr <= l || r <= ql)
        {
            return p;
        }
        int q = clone(p);
        if (ql <= l && r <= qr)
        {
            applyAt(q, l, r, mul, add);
            return q;
        }
        push(q, l, r);
        int m = (l + r) / 2;
        t[q].l = affine(t[q].l, l, m, ql, qr, mul, add);
        t[q].r = affine(t[q].r, m, r, ql, qr, mul, add);
        pull(q);
        return q;
    }

    int splice(int target, int source, int l, int r, int ql, int qr)
    {
        // target、source 是两个版本节点；把 source 在交集的位置覆盖到 target 并返回新节点。
        if (qr <= l || r <= ql)
        {
            return target;
        }
        if (ql <= l && r <= qr)
        {
            return source;
        }
        int q = clone(target);
        push(q, l, r);
        int m = (l + r) / 2;
        if (ql < m)
        {
            int x = srcL(source, l, r);
            t[q].l = splice(t[q].l, x, l, m, ql, qr);
        }
        if (m < qr)
        {
            int x = srcR(source, l, r);
            t[q].r = splice(t[q].r, x, m, r, ql, qr);
        }
        pull(q);
        return q;
    }

    T sum(int p, int l, int r, int ql, int qr, const T &outerMul, const T &outerAdd) const
    {
        // p 是版本节点，[l,r) 是其区间；outer 表示祖先尚未落实的仿射变换，返回交集和。
        if (qr <= l || r <= ql)
        {
            return T{};
        }
        if (ql <= l && r <= qr)
        {
            return t[p].sum * outerMul + T(r - l) * outerAdd;
        }
        T mul = t[p].mul * outerMul;
        T add = t[p].add * outerMul + outerAdd;
        int m = (l + r) / 2;
        return sum(t[p].l, l, m, ql, qr, mul, add)
             + sum(t[p].r, m, r, ql, qr, mul, add);
    }

  public:
    explicit PerAffSeg(const vector<T> &a) : n((int)a.size()), one(T(1)), t(1)
    {
        // a 是非空初始数组；T 需支持 0、1、加法、乘法、相等比较和由长度构造。
        assert(n > 0); // 调试检查，可删
        t.reserve(2 * n + 1);
        t[0].mul = one;
        base = build(a, 0, n);
    }

    int root() const
    {
        // 返回初始数组对应的版本根；此根可与后续版本根一起长期保存。
        return base;
    }

    int affine(int root, int l, int r, const T &mul, const T &add)
    {
        // root 是旧版本根，[l,r) 是修改区间；返回执行 x->mul*x+add 后的新版本根。
        assert(0 < root && root < (int)t.size()); // 调试检查，可删
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        if (l == r || (mul == one && add == T{}))
        {
            return root;
        }
        return affine(root, 0, n, l, r, mul, add);
    }

    int splice(int target, int source, int l, int r)
    {
        // target、source 是旧版本根；返回 target 的 [l,r) 被 source 同位置覆盖后的新版本根。
        assert(0 < target && target < (int)t.size()); // 调试检查，可删
        assert(0 < source && source < (int)t.size()); // 调试检查，可删
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        if (target == source || l == r)
        {
            return target;
        }
        return splice(target, source, 0, n, l, r);
    }

    T sum(int root, int l, int r) const
    {
        // root 是任意历史版本根，[l,r) 是查询区间；返回该区间当前元素和，空区间返回零。
        assert(0 < root && root < (int)t.size()); // 调试检查，可删
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        return sum(root, 0, n, l, r, one, T{});
    }
};
