#include <bits/stdc++.h>
using namespace std;
#define int long long

class RangeMode
{
    int n, B, m;
    // a 是离散值，val 是原值，rk[i] 是 i 在 pos[a[i]] 中的秩。
    vector<int> a, val, rk;
    vector<vector<int>> pos;
    // mode[l][r]、mc[l][r] 是第 l 到第 r 块合并后的众数及次数。
    vector<vector<int>> mode, mc;
    vector<unsigned long long> seen;
    unsigned long long stamp = 0;

    void nextStamp()
    {
        // 开始一次新查询；极端回绕时清空访问标记。
        stamp++;
        if (!stamp)
        {
            fill(seen.begin(), seen.end(), 0);
            stamp = 1;
        }
    }

  public:
    explicit RangeMode(vector<int> x) : n(x.size())
    {
        // x 是静态数组；完成离散化、出现秩与整块众数预处理。
        val = x;
        sort(val.begin(), val.end());
        val.erase(unique(val.begin(), val.end()), val.end());
        a.resize(n);
        rk.resize(n);
        pos.resize(val.size());
        for (int i = 0; i < n; i++)
        {
            a[i] = lower_bound(val.begin(), val.end(), x[i]) - val.begin();
            rk[i] = pos[a[i]].size();
            pos[a[i]].push_back(i);
        }

        B = max<int>(1, sqrt((long double)max<int>(1, n)));
        m = (n + B - 1) / B;
        mode.assign(m, vector<int>(m, -1));
        mc.assign(m, vector<int>(m));
        for (int l = 0; l < m; l++)
        {
            vector<int> f(val.size());
            int best = -1, c = 0;
            for (int i = l * B; i < n; i++)
            {
                int v = a[i];
                f[v]++;
                if (f[v] > c)
                {
                    best = v;
                    c = f[v];
                }
                if ((i + 1) % B == 0 || i + 1 == n)
                {
                    mode[l][i / B] = best;
                    mc[l][i / B] = c;
                }
            }
        }
        seen.assign(val.size(), 0);
    }

    optional<pair<int, int>> query(int l, int r)
    {
        // l、r 是半开区间端点；返回 {一个众数,出现次数}，空区间返回空。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        if (l == r)
        {
            return nullopt;
        }
        nextStamp();
        auto use = [&](int v)
        {
            if (seen[v] == stamp)
            {
                return false;
            }
            seen[v] = stamp;
            return true;
        };

        int L = (l + B - 1) / B;
        int R = r / B;
        int best = -1, c = 0;
        if (L < R)
        {
            best = mode[L][R - 1];
            c = mc[L][R - 1];
            for (int i = l; i < L * B; i++)
            {
                c += a[i] == best;
            }
            for (int i = R * B; i < r; i++)
            {
                c += a[i] == best;
            }
            use(best);
        }

        int le = min(r, L * B);
        for (int i = l; i < le; i++)
        {
            int v = a[i];
            if (!use(v))
            {
                continue;
            }
            int first = rk[i], e = first + c;
            if (e < (int)pos[v].size() && pos[v][e] < r)
            {
                e++;
                while (e < (int)pos[v].size() && pos[v][e] < r)
                {
                    e++;
                }
                best = v;
                c = e - first;
            }
        }

        int rs = max(l, R * B);
        for (int i = r - 1; i >= rs; i--)
        {
            int v = a[i];
            if (!use(v))
            {
                continue;
            }
            int last = rk[i], e = last - c;
            if (e >= 0 && pos[v][e] >= l)
            {
                e--;
                while (e >= 0 && pos[v][e] >= l)
                {
                    e--;
                }
                best = v;
                c = last - e;
            }
        }
        return pair{val[best], c};
    }
};

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (auto &x : a)
    {
        cin >> x;
    }
    RangeMode st(a);
    while (q--)
    {
        int l, r;
        cin >> l >> r;
        auto [x, cnt] = *st.query(l, r);
        cout << x << ' ' << cnt << '\n';
    }
    return 0;
}
