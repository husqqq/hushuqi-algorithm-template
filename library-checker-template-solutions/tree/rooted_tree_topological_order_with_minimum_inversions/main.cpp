
#include <bits/stdc++.h>
using namespace std;
#define int long long

vector<int> minInversionTreeOrder(const vector<int> &par, vector<long long> c,
                                  vector<long long> d, int root = 0)
{
    int n = par.size();
    assert((int)c.size() == n && (int)d.size() == n && par[root] == -1);

    struct DSU
    {
        vector<int> p, id;

        DSU(int n) : p(n, -1), id(n)
        {
            iota(id.begin(), id.end(), 0);
        }

        int find(int x)
        {
            return p[x] < 0 ? x : p[x] = find(p[x]);
        }

        int operator[](int x)
        {
            return id[find(x)];
        }

        void merge(int x, int y, int z)
        {
            x = find(x);
            y = find(y);
            if (x == y)
            {
                id[x] = z;
                return;
            }
            if (-p[x] < -p[y])
            {
                swap(x, y);
            }
            p[x] += p[y];
            p[y] = x;
            id[x] = z;
        }
    };

    struct Block
    {
        __int128 d, c;
        int root, ver;

        static bool ratioLess(const Block &x, const Block &y)
        {
            if (y.c == 0 && y.d == 0)
            {
                return false;
            }
            if (x.c == 0 && x.d == 0)
            {
                return true;
            }
            return (__int128)x.c * y.d < (__int128)x.d * y.c;
        }

        bool operator<(const Block &o) const
        {
            if (ratioLess(*this, o))
            {
                return true;
            }
            if (ratioLess(o, *this))
            {
                return false;
            }
            return pair(root, ver) < pair(o.root, o.ver);
        }
    };

    DSU dsu(n);
    priority_queue<Block> q;
    vector<int> nxt(n), ver(n);
    vector<__int128> sumC(n), sumD(n);
    iota(nxt.begin(), nxt.end(), 0);
    for (int u = 0; u < n; u++)
    {
        sumC[u] = c[u];
        sumD[u] = d[u];
        if (u != root)
        {
            assert(0 <= par[u] && par[u] < n);
            q.push({sumD[u], sumC[u], u, 0});
        }
    }
    while (!q.empty())
    {
        auto x = q.top();
        q.pop();
        int u = x.root;
        if (ver[u] != x.ver)
        {
            continue;
        }
        int v = dsu[par[u]];
        sumD[v] += sumD[u];
        sumC[v] += sumC[u];
        dsu.merge(u, v, v);
        if (v != root)
        {
            q.push({sumD[v], sumC[v], v, ++ver[v]});
        }
        swap(nxt[u], nxt[v]);
    }

    vector<int> ans{nxt[root]};
    while (ans.back() != root)
    {
        ans.push_back(nxt[ans.back()]);
    }
    reverse(ans.begin(), ans.end());
    return ans;
}

signed main()
{
    int n; cin >> n;
    vector<int> p(n, -1);
    for (int i = 1; i < n; i++) cin >> p[i];
    vector<long long> c(n), d(n);
    for (long long &x : c) cin >> x;
    for (long long &x : d) cin >> x;
    auto order = minInversionTreeOrder(p, c, d);
    long long ans = 0, sum = 0;
    for (int u : order)
    {
        ans += sum * c[u];
        sum += d[u];
    }
    cout << ans << '\n';
    for (int i = 0; i < n; i++) cout << order[i] << " \n"[i + 1 == n];
}
