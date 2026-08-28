
#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int mod = 1000000007, deg = 10;
using Table = array<array<int, deg + 1>, deg + 1>; // 第一维为 floor 幂，第二维为下标幂。

int bin[deg + 2][deg + 2]; // bin[n][k] 是组合数。
array<vector<int>, deg + 1> pc; // pc[p] 是 sum(i^p) 的 Newton 插值系数。

int power(int a, int b)
{
    // a、b 是模幂底数和非负指数；返回 a^b mod mod。
    int r = 1;
    a %= mod;
    while (b)
    {
        if (b & 1)
        {
            r = r * a % mod;
        }
        a = a * a % mod;
        b >>= 1;
    }
    return r;
}

int eval(const vector<int> &f, int x)
{
    // f 是低次到高次多项式系数；x 是代入值；返回模值。
    int r = 0;
    x %= mod;
    for (int i = f.size() - 1; i >= 0; i--)
    {
        r = (r * x + f[i]) % mod;
    }
    return r;
}

void initFloor()
{
    // 无参数；预处理 0..deg 次幂和插值系数。
    for (int n = 0; n <= deg + 1; n++)
    {
        bin[n][0] = bin[n][n] = 1;
        for (int k = 1; k < n; k++)
        {
            bin[n][k] = (bin[n - 1][k - 1] + bin[n - 1][k]) % mod;
        }
    }
    for (int p = 0; p <= deg; p++)
    {
        int d = p + 1, sum = 0;
        vector<int> dif(d + 1), f{1}, g(d + 1);
        for (int x = 0; x <= d; x++)
        {
            sum = (sum + power(x, p)) % mod;
            dif[x] = sum;
        }
        for (int k = 0; k <= d; k++)
        {
            for (int i = 0; i < (int)f.size(); i++)
            {
                g[i] = (g[i] + dif[0] * f[i]) % mod;
            }
            for (int i = 0; i + 1 < (int)dif.size(); i++)
            {
                dif[i] = (dif[i + 1] - dif[i] + mod) % mod;
            }
            dif.pop_back();
            if (k == d)
            {
                break;
            }
            vector<int> h(f.size() + 1);
            for (int i = 0; i < (int)f.size(); i++)
            {
                h[i] = (h[i] - f[i] * k) % mod;
                h[i + 1] = (h[i + 1] + f[i]) % mod;
            }
            int iv = power(k + 1, mod - 2);
            for (int &x : h)
            {
                x = (x * iv % mod + mod) % mod;
            }
            f.swap(h);
        }
        pc[p] = move(g);
    }
}

Table floorMoment(int n, int a, int b, int c, int d)
{
    // n、a、b、c 定义 floor((a*i+b)/c)，d 是总次数上界；返回混合幂和表。
    assert(n >= 0 && a >= 0 && b >= 0 && c > 0 && d <= deg);
    static bool ok = false;
    if (!ok)
    {
        initFloor();
        ok = true;
    }
    Table res{};
    if (!a)
    {
        int v = b / c;
        for (int p = 0; p <= d; p++)
        {
            int s = eval(pc[p], n);
            for (int q = 0; p + q <= d; q++)
            {
                res[p][q] = s * power(v, q) % mod;
            }
        }
        return res;
    }
    int qa = a / c, qb = b / c;
    if (qa || qb)
    {
        auto sub = floorMoment(n, a % c, b % c, c, d);
        vector<int> pa(d + 1, 1), pb(d + 1, 1);
        for (int i = 1; i <= d; i++)
        {
            pa[i] = pa[i - 1] * (qa % mod) % mod;
            pb[i] = pb[i - 1] * (qb % mod) % mod;
        }
        for (int p = 0; p <= d; p++)
        {
            for (int q = 0; p + q <= d; q++)
            {
                for (int j = 0; j <= q; j++)
                {
                    for (int t = 0; t + j <= q; t++)
                    {
                        int w = bin[q][j] * bin[q - j][t] % mod;
                        w = w * pa[t] % mod * pb[q - j - t] % mod;
                        res[p][q] = (res[p][q] + w * sub[p + t][j]) % mod;
                    }
                }
            }
        }
        return res;
    }
    int h = ((__int128)a * n + b) / c;
    for (int p = 0; p <= d; p++)
    {
        res[p][0] = eval(pc[p], n);
    }
    if (!h)
    {
        return res;
    }
    auto sub = floorMoment(h - 1, c, c - b - 1, a, d);
    for (int p = 0; p <= d; p++)
    {
        for (int q = 1; p + q <= d; q++)
        {
            int w = res[p][0] * power(h, q) % mod;
            for (int u = 0; u < q; u++)
            {
                for (int r = 0; r <= p + 1; r++)
                {
                    w = (w - bin[q][u] * pc[p][r] % mod * sub[u][r]) % mod;
                }
            }
            res[p][q] = (w + mod) % mod;
        }
    }
    return res;
}

vector<int> floorMoments(const vector<array<int, 6>> &qs)
{
    // qs 每项为 n,a,b,c,p,q；返回对应混合幂和，要求 p+q<=deg。
    vector<int> ans;
    ans.reserve(qs.size());
    for (auto x : qs)
    {
        assert(x[4] >= 0 && x[5] >= 0 && x[4] + x[5] <= deg);
        auto t = floorMoment(x[0], x[1], x[2], x[3], x[4] + x[5]);
        ans.push_back(t[x[4]][x[5]]);
    }
    return ans;
}

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin >> T;
    vector<array<int, 6>> qs(T);
    for (auto &q : qs)
    {
        for (int &x : q)
        {
            cin >> x;
        }
    }
    for (int x : floorMoments(qs))
    {
        cout << x << '\n';
    }
}

