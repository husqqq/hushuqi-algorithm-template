#include <bits/stdc++.h>
using namespace std;
#define int long long

struct DivBlock
{
    long long l; // 极大块左端点
    long long r; // 极大块右端点
    long long q; // 块内公共商 floor(n/l)
    bool operator==(const DivBlock &) const = default;
};

vector<DivBlock> divBlocks(long long n)
{
    // n 是正整数；返回 floor(n/i) 相同的全部极大闭区间。
    assert(n > 0); // 调试检查，可删
    vector<DivBlock> a;
    long long l = 1;
    while (l <= n)
    {
        long long q = n / l;
        long long r = n / q;
        a.push_back({l, r, q});
        if (r == n)
        {
            break;
        }
        l = r + 1;
    }
    return a;
}

vector<long long> quotients(long long n)
{
    // n 是正整数；返回 floor(n/i) 的全部不同取值并按数值升序排列。
    vector<long long> a;
    for (const auto &block : divBlocks(n))
    {
        a.push_back(block.q);
    }
    reverse(a.begin(), a.end());
    return a;
}

signed main()
{
    long long n; cin >> n;
    auto a = quotients(n);
    cout << a.size() << '\n';
    for (int i = 0; i < (int)a.size(); i++) cout << a[i] << " \n"[i + 1 == (int)a.size()];
}
