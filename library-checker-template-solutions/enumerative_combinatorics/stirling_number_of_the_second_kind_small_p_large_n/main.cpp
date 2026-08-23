

#include <bits/stdc++.h>
using namespace std;
#define int long long

class SmallPrimeComb
{
  protected:
    // p 是小素数，c[i][j] 是 C(i,j) mod p，其中 0<=j<=i<p。
    int p;
    vector<vector<int32_t>> c;

    int comb(int n, int k) const
    {
        // n、k 是非负 64 位组合数参数；返回 C(n,k) mod p，k 越界时返回 0。
        if (k < 0 || k > n)
        {
            return 0;
        }
        int ans = 1;
        while (n)
        {
            int a = n % p;
            int b = k % p;
            if (b > a)
            {
                return 0;
            }
            ans = ans * c[a][b] % p;
            n /= p;
            k /= p;
        }
        return ans;
    }

  public:
    explicit SmallPrimeComb(int p) : p(p)
    {
        // p 是可承担 O(p^2) 表的素数；建立一位 Lucas 所需的 Pascal 三角。
        assert(2 <= p && p <= numeric_limits<int32_t>::max()); // 调试检查，可删。
        c.resize(p);
        c[0] = {1};
        for (int i = 1; i < p; i++)
        {
            c[i].assign(i + 1, 0);
            for (int j = 0; j <= i; j++)
            {
                int x = 0;
                if (j < i)
                {
                    x += c[i - 1][j];
                }
                if (j)
                {
                    x += c[i - 1][j - 1];
                }
                c[i][j] = x % p;
            }
        }
    }
};

class Stirling1Prime : public SmallPrimeComb
{
    // s[i][j] 是有符号第一类 Stirling 数 s(i,j) mod p，其中 0<=j<=i<p。
    vector<vector<int32_t>> s;

  public:
    explicit Stirling1Prime(int p) : SmallPrimeComb(p), s(p)
    {
        // p 是小素数；建立第一类低位表与 Lucas 表。
        s[0] = {1};
        for (int i = 1; i < p; i++)
        {
            s[i].assign(i + 1, 0);
            for (int j = 0; j <= i; j++)
            {
                int x = 0;
                if (j)
                {
                    x += s[i - 1][j - 1];
                }
                if (j < i)
                {
                    x += s[i - 1][j] * (p - i + 1) % p;
                }
                s[i][j] = x % p;
            }
        }
    }

    int get(int n, int k) const
    {
        // n、k 是非负 64 位下标；返回有符号 s(n,k) mod p，k>n 时返回 0。
        assert(n >= 0); // 调试检查，可删。
        if (k < 0 || k > n)
        {
            return 0;
        }
        int i = n / p;
        int j = n % p;
        if (i > k)
        {
            return 0;
        }
        int a = (k - i) / (p - 1);
        int b = (k - i) % (p - 1);
        if (b == 0 && j > 0)
        {
            b += p - 1;
            a--;
        }
        if (a < 0 || i < a || b > j)
        {
            return 0;
        }
        int ans = comb(i, a) * s[j][b] % p;
        if (((i + a) & 1) && ans)
        {
            ans = p - ans;
        }
        return ans;
    }
};

class Stirling2Prime : public SmallPrimeComb
{
    // s[i][j] 是第二类 Stirling 数 S(i,j) mod p，其中 0<=j<=i<p。
    vector<vector<int32_t>> s;

  public:
    explicit Stirling2Prime(int p) : SmallPrimeComb(p), s(p)
    {
        // p 是小素数；建立第二类低位表与 Lucas 表。
        s[0] = {1};
        for (int i = 1; i < p; i++)
        {
            s[i].assign(i + 1, 0);
            for (int j = 0; j <= i; j++)
            {
                int x = 0;
                if (j)
                {
                    x += s[i - 1][j - 1];
                }
                if (j < i)
                {
                    x += s[i - 1][j] * j % p;
                }
                s[i][j] = x % p;
            }
        }
    }

    int get(int n, int k) const
    {
        // n、k 是非负 64 位下标；返回 S(n,k) mod p，k>n 时返回 0。
        assert(n >= 0); // 调试检查，可删。
        if (k < 0 || k > n)
        {
            return 0;
        }
        if (n < p)
        {
            return s[n][k];
        }
        int i = k / p;
        int j = k % p;
        int a = (n - i) / (p - 1);
        int b = (n - i) % (p - 1);
        if (b == 0)
        {
            b = p - 1;
            a--;
        }
        if (a < 0 || j > b)
        {
            return 0;
        }
        if (b < p - 1)
        {
            return comb(a, i) * s[b][j] % p;
        }
        if (j == 0)
        {
            return comb(a, i - 1);
        }
        return comb(a, i) * s[p - 1][j] % p;
    }
};

signed main()
{
    int t, p; cin >> t >> p;
    Stirling2Prime s(p);
    while (t--)
    {
        long long n, k; cin >> n >> k;
        cout << s.get(n, k) << '\n';
    }
}
