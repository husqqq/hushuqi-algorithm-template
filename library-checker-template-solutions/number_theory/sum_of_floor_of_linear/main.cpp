#include <bits/stdc++.h>
using namespace std;
#define int long long

__int128 floorSum(long long n, long long m, long long a, long long b)
{
    // n 是项数，m 是正除数，a、b 是线性系数；返回取整和，要求精确中间量与答案可由 __int128 表示。
    assert(n >= 0 && m > 0); // 调试检查，可删
    __int128 N = n;
    __int128 M = m;
    __int128 A = a;
    __int128 B = b;
    __int128 ans = 0;
    auto divFloor = [](__int128 x, __int128 y)
    {
        __int128 q = x / y;
        __int128 r = x % y;
        return q - (r < 0);
    };
    __int128 q = divFloor(A, M);
    ans += N * (N - 1) / 2 * q;
    A -= q * M;
    q = divFloor(B, M);
    ans += N * q;
    B -= q * M;
    while (true)
    {
        if (A >= M)
        {
            ans += N * (N - 1) / 2 * (A / M);
            A %= M;
        }
        if (B >= M)
        {
            ans += N * (B / M);
            B %= M;
        }
        __int128 y = A * N + B;
        if (y < M)
        {
            break;
        }
        N = y / M;
        B = y % M;
        swap(A, M);
    }
    return ans;
}

signed main()
{
    int t; cin >> t;
    while (t--)
    {
        long long n, m, a, b; cin >> n >> m >> a >> b;
        cout << (long long)floorSum(n, m, a, b) << '\n';
    }
}
