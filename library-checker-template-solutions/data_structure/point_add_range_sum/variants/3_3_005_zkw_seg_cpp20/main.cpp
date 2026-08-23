#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class T, class F> struct ZkwSeg
{
    // n 保存补到二次幂后的叶子数；e 保存幺元；op 保存满足结合律的合并运算；tr 保存连续线段树节点。
    int n = 1;
    T e;
    F op;
    vector<T> tr;

    ZkwSeg(const vector<T> &a, T e, F op) : e(e), op(op)
    {
        // a 表示初始序列，e 表示幺元，op 表示区间合并运算；构造固定规模的迭代线段树。
        while (n < (int)a.size())
        {
            n *= 2;
        }
        tr.assign(2 * n, e);
        copy(a.begin(), a.end(), tr.begin() + n);
        for (int p = n - 1; p; p--)
        {
            tr[p] = op(tr[2 * p], tr[2 * p + 1]);
        }
    }

    void set(int x, T v)
    {
        // x 表示 0 下标位置，v 表示新值；修改叶子并沿父链重算。
        for (tr[x += n] = v; x >>= 1;)
        {
            tr[x] = op(tr[2 * x], tr[2 * x + 1]);
        }
    }

    T prod(int l, int r) const
    {
        // l、r 表示半开区间 [l,r)；返回保持原顺序合并后的区间信息。
        T a = e, b = e;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1)
        {
            if (l & 1)
            {
                a = op(a, tr[l++]);
            }
            if (r & 1)
            {
                b = op(tr[--r], b);
            }
        }
        return op(a, b);
    }
};

signed main(){ios::sync_with_stdio(false);cin.tie(nullptr);int n,q;cin>>n>>q;vector<int>a(n);for(int&x:a)cin>>x;ZkwSeg<int,plus<int>>s(a,0,plus<int>{});while(q--){int t,x,y;cin>>t>>x>>y;if(t==0){a[x]+=y;s.set(x,a[x]);}else cout<<s.prod(x,y)<<'\n';}}
