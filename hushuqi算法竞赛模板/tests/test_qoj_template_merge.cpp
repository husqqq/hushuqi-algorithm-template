#include <bits/stdc++.h>
using namespace std;
#include "../include/full/1_1_008.hpp"
#include "../include/full/7_1_001.hpp"
#include "../include/topic_common.hpp"
namespace d2 {
#include "../include/full/2_2_005.hpp"
}
#undef int
namespace s9 {
#include "../include/full/4_3_010.hpp"
}
#undef int
namespace s3 {
#include "../include/full/4_3_011.hpp"
}
#undef int
namespace gtri {
#include "../include/full/5_9_006.hpp"
}
#undef int
namespace chord {
#include "../include/full/5_10_001.hpp"
}
#undef int
namespace tutte {
using ::Z;
#include "../include/full/5_10_010.hpp"
}
#undef int
namespace floor_m {
#include "../include/full/7_5_003.hpp"
}
#undef int
namespace matrix_p {
#include "../include/full/9_1_007.hpp"
}
#undef int
namespace linear {
#include "../include/full/9_2_001.hpp"
}
#undef int

long long distinct(const string &s)
{
    set<string> a;
    for (int l = 0; l < (int)s.size(); l++) for (int r = l + 1; r <= (int)s.size(); r++) a.insert(s.substr(l, r - l));
    return a.size();
}

template <class T> T det(vector<vector<T>> a)
{
    T ans = 1;
    int n = a.size();
    for (int j = 0; j < n; j++)
    {
        int p = j;
        while (p < n && a[p][j] == T(0)) p++;
        if (p == n) return 0;
        if (p != j) swap(a[p], a[j]), ans = -ans;
        ans *= a[j][j];
        T inv = T(1) / a[j][j];
        for (int i = j + 1; i < n; i++)
        {
            T q = a[i][j] * inv;
            for (int k = j; k < n; k++) a[i][k] -= q * a[j][k];
        }
    }
    return ans;
}

signed main()
{
    mt19937 rng(123456);
    for (int n = 1; n <= 14; n++) for (int it = 0; it < 100; it++)
    {
        string s(n, 'a');
        for (char &c : s) c += rng() % 3;
        vector<pair<long long, long long>> qs;
        for (int q = 0; q < 30; q++)
        {
            int l = rng() % n, r = l + 1 + rng() % (n - l);
            qs.push_back({l, r});
        }
        auto got = s9::rangeSubs(s, qs);
        for (int q = 0; q < (int)qs.size(); q++) assert(got[q] == distinct(s.substr(qs[q].first, qs[q].second - qs[q].first)));
        vector<pair<long long, long long>> closed;
        for (auto [l, r] : qs) closed.push_back({l, r - 1});
        auto border = s3::basicDict(s, closed);
        for (int q = 0; q < (int)qs.size(); q++)
        {
            auto [l, r] = qs[q];
            vector<int> b;
            for (int len = 1; len < r - l; len++) if (s.substr(l, len) == s.substr(r - len, len)) b.push_back(len);
            if (border[q].cnt != (int)b.size())
            {
                cerr << s << ' ' << l << ' ' << r << " got " << border[q].cnt << " want " << b.size() << '\n';
                abort();
            }
            if (!b.empty()) assert(border[q].mn == b.front() && border[q].mx == b.back());
        }
    }
    for (int n = 0; n <= 30; n++) for (int it = 0; it < 100; it++)
    {
        vector<d2::Point4> a(n);
        for (auto &p : a) p = {(int)rng() % 7, (int)rng() % 7, (int)rng() % 7, (int)rng() % 7};
        long long want = 0;
        for (auto p : a) for (auto q : a) want += p.x < q.x && p.y < q.y && p.z < q.z && p.w < q.w;
        if (d2::countFourDimensional(a) != want)
        {
            cerr << "dom4 n=" << n << " got=" << d2::countFourDimensional(a) << " want=" << want << '\n';
            for (auto p : a) cerr << p.x << ',' << p.y << ',' << p.z << ',' << p.w << ' '; cerr << '\n';
            for (auto p : a) for (auto q : a) if (p.x < q.x && p.y < q.y && p.z < q.z && p.w < q.w) cerr << "pair\n";
            cerr << d2::dom4Cross({{0,0,0,true},{2,-5,1,true},{6,-3,2,false}}, 3) << '\n';
            abort();
        }
    }
    for (int n = 0; n <= 20; n++) for (int a = 0; a <= 8; a++) for (int b = 0; b <= 8; b++) for (int c = 1; c <= 8; c++)
    {
        auto t = floor_m::floorMoment(n, a, b, c, 4);
        for (int p = 0; p <= 4; p++) for (int q = 0; p + q <= 4; q++)
        {
            long long want = 0;
            for (int i = 0; i <= n; i++)
            {
                long long x = 1, y = 1;
                for (int k = 0; k < p; k++) x = x * i % floor_m::floorMomentMod;
                for (int k = 0; k < q; k++) y = y * ((a * i + b) / c) % floor_m::floorMomentMod;
                want = (want + x * y) % floor_m::floorMomentMod;
            }
            assert(t[p][q] == want);
        }
    }
    using M = matrix_p::LinearMint<998244353>;
    for (int n = 0; n <= 5; n++) for (int it = 0; it < 100; it++)
    {
        vector a(n, vector<M>(n)), b(n, vector<M>(n));
        for (auto &r : a) for (auto &x : r) x = rng() % 7;
        for (auto &r : b) for (auto &x : r) x = rng() % 7;
        auto p = matrix_p::detAPlusBz(a, b);
        for (int z = 0; z <= n; z++)
        {
            vector c = a;
            for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) c[i][j] += b[i][j] * M(z);
            M v = 0, pw = 1;
            for (M x : p) v += x * pw, pw *= M(z);
            assert(v == det(c));
        }
    }
    for (int vars = 0; vars <= 12; vars++) for (int rows = 0; rows <= 12; rows++) for (int it = 0; it < 50; it++)
    {
        vector a(rows, vector<int>(vars + 1));
        for (auto &r : a) for (int &x : r) x = rng() % 3;
        auto x = matrix_p::solveMod3(a, vars);
        bool possible = false;
        int all = 1;
        for (int i = 0; i < vars; i++) all *= 3;
        if (vars <= 8) for (int mask = 0; mask < all; mask++)
        {
            int z = mask;
            bool ok = true;
            for (auto r : a)
            {
                int v = 0, t = z;
                for (int j = 0; j < vars; j++) v += r[j] * (t % 3), t /= 3;
                ok &= v % 3 == r[vars];
            }
            possible |= ok;
        }
        if (vars <= 8) assert((bool)x == possible);
        if (x) for (auto r : a)
        {
            int v = 0;
            for (int j = 0; j < vars; j++) v += r[j] * (*x)[j];
            assert(v % 3 == r[vars]);
        }
    }
    assert(gtri::triWeight998(4, {2, 3, 5, 7}, {{0, 1}, {1, 2}, {2, 0}, {0, 3}}) == 30);
    for (int n = 1; n <= 10; n++) for (int it = 0; it < 300; it++)
    {
        vector<pair<long long, long long>> e;
        vector<vector<int>> adj(n, vector<int>(n));
        for (int u = 0; u < n; u++) for (int v = u + 1; v < n; v++) if (rng() & 1)
            e.push_back({u, v}), adj[u][v] = adj[v][u] = 1;
        auto slow = chord::chordalCert(n, e), fast = chord::chordalCertFast(n, e);
        assert(slow.ok == fast.ok);
        if (fast.ok)
        {
            vector<int> pos(n);
            for (int i = 0; i < n; i++) pos[fast.peo[i]] = i;
            for (int u = 0; u < n; u++)
            {
                int p = -1;
                for (int v = 0; v < n; v++) if (adj[u][v] && pos[v] > pos[u] && (p == -1 || pos[v] < pos[p])) p = v;
                if (p != -1) for (int v = 0; v < n; v++) if (v != p && adj[u][v] && pos[v] > pos[u]) assert(adj[p][v]);
            }
        }
        else if (fast.cycle.size() >= 4)
        {
            int k = fast.cycle.size();
            for (int i = 0; i < k; i++) for (int j = i + 1; j < k; j++)
            {
                bool side = j == i + 1 || (i == 0 && j == k - 1);
                assert(adj[fast.cycle[i]][fast.cycle[j]] == side);
            }
        }
    }
    for (int n = 1; n <= 6; n++) for (int it = 0; it < 30; it++)
    {
        vector<unsigned long long> g(n);
        vector<pair<int, int>> e;
        for (int u = 0; u < n; u++) for (int v = u + 1; v < n; v++) if (rng() & 1)
            g[u] |= 1ULL << v, g[v] |= 1ULL << u, e.push_back({u, v});
        for (int xv = 1; xv <= 3; xv++) for (int yv = 1; yv <= 3; yv++)
        {
            tutte::Z want = 0;
            for (int mask = 0; mask < (1 << e.size()); mask++)
            {
                vector<int> p(n), sz(n, 1);
                iota(p.begin(), p.end(), 0);
                auto find = [&](auto &&self, int x) -> int { return p[x] == x ? x : p[x] = self(self, p[x]); };
                int used = 0;
                for (int i = 0; i < (int)e.size(); i++) if (mask >> i & 1)
                {
                    used++;
                    auto [u, v] = e[i];
                    u = find(find, u), v = find(find, v);
                    if (u != v) p[v] = u, sz[u] += sz[v];
                }
                int comp = 0;
                for (int i = 0; i < n; i++) comp += find(find, i) == i;
                vector<int> all(n), allsz(n, 1);
                iota(all.begin(), all.end(), 0);
                auto fall = [&](auto &&self, int x) -> int { return all[x] == x ? x : all[x] = self(self, all[x]); };
                for (auto [u, v] : e) u = fall(fall, u), v = fall(fall, v), all[v] = u;
                int fullComp = 0;
                for (int i = 0; i < n; i++) fullComp += fall(fall, i) == i;
                auto pw = [](tutte::Z a, int b) { tutte::Z r = 1; while (b--) r *= a; return r; };
                want += pw(tutte::Z(xv - 1), comp - fullComp) * pw(tutte::Z(yv - 1), used - n + comp);
            }
            assert(tutte::tutteEval(g, tutte::Z(xv), tutte::Z(yv)) == want);
        }
    }
    cout << "OK\n";
}
