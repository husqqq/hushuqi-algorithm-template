#include <bits/stdc++.h>
using namespace std;
#define int long long

vector<int> lyndon(const string &s)
{
    // s 是待分解字符串；返回各段左端点，并在末尾附加 s.size()。
    int n = s.size();
    vector<int> a;
    for (int i = 0; i < n;)
    {
        int j = i + 1, k = i;
        while (j < n && (unsigned char)s[k] <= (unsigned char)s[j])
        {
            if ((unsigned char)s[k] < (unsigned char)s[j])
            {
                k = i;
            }
            else
            {
                k++;
            }
            j++;
        }
        while (i <= k)
        {
            a.push_back(i);
            i += j - k;
        }
    }
    a.push_back(n);
    return a;
}

signed main()
{
    // 无参数；输出 Lyndon 分解左端点及末尾哨兵，成功返回 0。
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    cin >> s;
    auto a = lyndon(s);
    for (int i = 0; i < (int)a.size(); i++) cout << (i ? " " : "") << a[i];
    cout << '\n';
}
