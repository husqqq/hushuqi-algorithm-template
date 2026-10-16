#include <bits/stdc++.h>
using namespace std;
#define int long long



#include <limits>

using namespace std;

constexpr long long INF = numeric_limits<long long>::max() / 4;

template <class T> bool chmin(T &a, const T &b)
{
    // a 是待更新值，b 是候选值；若 b 更小则更新 a 并返回 true。
    if (b < a)
    {
        a = b;
        return true;
    }
    return false;
}

template <class T> bool chmax(T &a, const T &b)
{
    // a 是待更新值，b 是候选值；若 b 更大则更新 a 并返回 true。
    if (a < b)
    {
        a = b;
        return true;
    }
    return false;
}

constexpr int inf = 1E9;
constexpr long double eps = 1E-12L;

using OrderedSet = set<int>;
using OrderedMSet = multiset<int>;

// 只需要按键插删、前驱和后继时直接使用上述标准容器；第 k 小与排名接口见 3.9.007、3.9.008。

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    OrderedMSet st;
    while (n--)
    {
        int x;
        cin >> x;
        st.insert(x);
    }
    while (q--)
    {
        int type;
        cin >> type;
        if (type == 0)
        {
            int x;
            cin >> x;
            st.insert(x);
        }
        else if (type == 1)
        {
            auto it = st.begin();
            cout << *it << '\n';
            st.erase(it);
        }
        else
        {
            auto it = prev(st.end());
            cout << *it << '\n';
            st.erase(it);
        }
    }
    return 0;
}
