#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int inf = 1E9;
constexpr long long INF = 4E18;
constexpr long double eps = 1E-12L;

template <class T> bool chmin(T &a, const T &b)
{
    // a 是待更新值，b 是候选值；若 a 变小则返回 true。
    if (b >= a)
    {
        return false;
    }
    a = b;
    return true;
}
template <class T> bool chmax(T &a, const T &b)
{
    // a 是待更新值，b 是候选值；若 a 变大则返回 true。
    if (a >= b)
    {
        return false;
    }
    a = b;
    return true;
}

struct Manacher
{
    // odd、even 分别保存奇回文与偶回文半径。
    vector<int> odd, even;

    Manacher(const string &s)
    {
        // s 是要预处理回文半径的原字符串。
        int n = s.size();
        odd.resize(n);
        for (int i = 0, l = 0, r = -1; i < n; i++)
        {
            int k = i > r ? 1 : min(odd[l + r - i], r - i + 1);
            while (i - k >= 0 && i + k < n && s[i - k] == s[i + k])
            {
                k++;
            }
            odd[i] = k--;
            if (i + k > r)
            {
                l = i - k;
                r = i + k;
            }
        }
        even.resize(n);
        for (int i = 0, l = 0, r = -1; i < n; i++)
        {
            int k = i > r ? 0 : min(even[l + r - i + 1], r - i + 1);
            while (i - k - 1 >= 0 && i + k < n && s[i - k - 1] == s[i + k])
            {
                k++;
            }
            even[i] = k--;
            if (i + k > r)
            {
                l = i - k - 1;
                r = i + k;
            }
        }
    }

    // odd[i] 对应长度 2*odd[i]-1；even[i] 对应中心缝 (i-1,i) 的半径。
    bool isPal(int l, int r) const
    {
        // l、r 是原串下标；返回半开区间 [l,r) 是否为回文串。
        assert(0 <= l && l <= r && r <= (int)odd.size()); // 调试检查，可删
        if (l == r)
        {
            return true;
        }
        int n = r - l;
        if (n & 1)
        {
            return odd[(l + r) / 2] > n / 2;
        }
        return even[(l + r) / 2] >= n / 2;
    }
};

signed main()
{
    string s; cin >> s; Manacher m(s);
    for (int i = 0; i < (int)s.size(); i++)
    {
        if (i) cout << ' ';
        cout << 2 * m.odd[i] - 1;
        if (i + 1 < (int)s.size()) cout << ' ' << 2 * m.even[i + 1];
    }
    cout << '\n';
}
