#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int inf = 1E9;
constexpr long long INF = 4E18;
constexpr long double eps = 1E-12L;

template <class T> bool chmin(T &a, const T &b)
{
    // a 是待更新值，b 是候选上界；若 b 更小则写入 a 并返回 true。
    return b < a ? a = b, true : false;
}
template <class T> bool chmax(T &a, const T &b)
{
    // a 是待更新值，b 是候选下界；若 b 更大则写入 a 并返回 true。
    return a < b ? a = b, true : false;
}

struct Cart
{
    // root 是树根；parent、left、right 分别保存父亲与左右儿子编号。
    int root = -1;
    vector<int> parent, left, right;
};

// 最小笛卡尔树；中序遍历为原下标，相等时靠前元素在上方。
template <class T> Cart cartesian(const vector<T> &a)
{
    // a 是静态数组；返回其最小笛卡尔树的根、父亲和左右儿子。
    int n = a.size();
    Cart c{-1, vector<int>(n, -1), vector<int>(n, -1), vector<int>(n, -1)};
    vector<int> s;
    for (int i = 0; i < n; i++)
    {
        int last = -1;
        while (!s.empty() && a[i] < a[s.back()])
        {
            last = s.back();
            s.pop_back();
        }
        if (!s.empty())
        {
            c.parent[i] = s.back();
            c.right[s.back()] = i;
        }
        if (last != -1)
        {
            c.parent[last] = i;
            c.left[i] = last;
        }
        s.push_back(i);
    }
    if (n)
    {
        c.root = s[0];
    }
    return c;
}

signed main()
{
    int n; cin >> n;
    vector<int> a(n);
    for (int &x : a) cin >> x;
    auto c = cartesian(a);
    c.parent[c.root] = c.root;
    for (int i = 0; i < n; i++) cout << c.parent[i] << " \n"[i + 1 == n];
}
