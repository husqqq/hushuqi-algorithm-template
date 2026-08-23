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
using OrderedSet = set<int>;
using OrderedMSet = multiset<int>;

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
