#include <bits/stdc++.h>

// Library Checker official reference implementation, adapted to this template's style.
// Problem: chromatic_polynomial
using namespace std;
using ll = long long;
using u32 = unsigned int;
using u64 = unsigned long long;
using pi = pair<ll, ll>;
using vi = vector<ll>;
template <class T> using vc = vector<T>;
template <class T> using vvc = vector<vc<T>>;
// https://trap.jp/post/1224/
int popcnt(int x) {
    return __builtin_popcount(x);
}
int popcnt(u32 x) {
    return __builtin_popcount(x);
}
int popcnt(ll x) {
    return __builtin_popcountll(x);
}
int popcnt(u64 x) {
    return __builtin_popcountll(x);
}
// (0, 1, 2, 3, 4) -> (-1, 0, 1, 1, 2)
int topbit(int x) {
    return (x == 0 ? -1 : 31 - __builtin_clz(x));
}
int topbit(u32 x) {
    return (x == 0 ? -1 : 31 - __builtin_clz(x));
}
int topbit(ll x) {
    return (x == 0 ? -1 : 63 - __builtin_clzll(x));
}
int topbit(u64 x) {
    return (x == 0 ? -1 : 63 - __builtin_clzll(x));
}
// (0, 1, 2, 3, 4) -> (-1, 0, 1, 0, 2)
int lowbit(int x) {
    return (x == 0 ? -1 : __builtin_ctz(x));
}
int lowbit(u32 x) {
    return (x == 0 ? -1 : __builtin_ctz(x));
}
int lowbit(ll x) {
    return (x == 0 ? -1 : __builtin_ctzll(x));
}
int lowbit(u64 x) {
    return (x == 0 ? -1 : __builtin_ctzll(x));
}
template <typename T> T floor(T a, T b) {
    return a / b - (a % b && (a ^ b) < 0);
}
template <typename T> T ceil(T x, T y) {
    return floor(x + y - 1, y);
}
template <typename T> T bmod(T x, T y) {
    return x - y * floor(x, y);
}
template <typename T> pair<T, T> divmod(T x, T y) {
    T q = floor(x, y);
    return {q, x - q * y};
}
template <typename T, typename U> T SUM(const vector<U>& A) {
    T sm = 0;
    for (auto&& a : A) {
        sm += a;
    }
    return sm;
}
template <typename mint> mint inv(int n) {
    static const int mod = mint::get_mod();
    static vector<mint> dat = {0, 1};
    assert(0 <= n);
    if (n >= mod) {
        n %= mod;
    }
    while (ll(dat.size()) <= n) {
        int k = ll(dat.size());
        int q = (mod + k - 1) / k;
        dat.emplace_back(dat[k * q - mod] * mint::raw(q));
    }
    return dat[n];
}
template <typename mint> mint fact(int n) {
    static const int mod = mint::get_mod();
    assert(0 <= n && n < mod);
    static vector<mint> dat = {1, 1};
    while (ll(dat.size()) <= n) {
        dat.emplace_back(dat[ll(dat.size()) - 1] * mint::raw(ll(dat.size())));
    }
    return dat[n];
}
template <typename mint> mint fact_inv(int n) {
    static vector<mint> dat = {1, 1};
    if (n < 0) {
        return mint(0);
    }
    while (ll(dat.size()) <= n) {
        dat.emplace_back(dat[ll(dat.size()) - 1] * inv<mint>(ll(dat.size())));
    }
    return dat[n];
}
template <class mint, class... Ts> mint fact_invs(Ts... xs) {
    return (mint(1) * ... * fact_inv<mint>(xs));
}
template <typename mint, class Head, class... Tail> mint multinomial(Head&& head, Tail&&... tail) {
    return fact<mint>(head) * fact_invs<mint>(forward<Tail>(tail)...);
}
template <typename mint> mint C_dense(int n, int k) {
    static vvc<mint> C;
    static int H = 0, W = 0;
    auto calc = [&](int i, int j) -> mint {
        if (i == 0) {
            return (j == 0 ? mint(1) : mint(0));
        }
        return C[i - 1][j] + (j ? C[i - 1][j - 1] : 0);
    };
    if (W <= k) {
        for (ll i = 0; i < ll(H); ++i) {
            C[i].resize(k + 1);
            for (ll j = W; j < ll(k + 1); ++j) {
                C[i][j] = calc(i, j);
            }
        }
        W = k + 1;
    }
    if (H <= n) {
        C.resize(n + 1);
        for (ll i = H; i < ll(n + 1); ++i) {
            C[i].resize(W);
            for (ll j = 0; j < ll(W); ++j) {
                C[i][j] = calc(i, j);
            }
        }
        H = n + 1;
    }
    return C[n][k];
}
template <typename mint, bool large = false, bool dense = false> mint C(ll n, ll k) {
    assert(n >= 0);
    if (k < 0 || n < k) {
        return 0;
    }
    if constexpr (dense) {
        return C_dense<mint>(n, k);
    }
    if constexpr (!large) {
        return multinomial<mint>(n, k, n - k);
    }
    k = min(k, n - k);
    mint x(1);
    for (ll i = 0; i < ll(k); ++i) {
        x *= mint(n - i);
    }
    return x * fact_inv<mint>(k);
}
template <typename mint, bool large = false> mint C_inv(ll n, ll k) {
    assert(n >= 0);
    assert(0 <= k && k <= n);
    if (!large) {
        return fact_inv<mint>(n) * fact<mint>(k) * fact<mint>(n - k);
    }
    return mint(1) / C<mint, 1>(n, k);
}
// [x^d](1-x)^{-n}
template <typename mint, bool large = false, bool dense = false> mint C_negative(ll n, ll d) {
    assert(n >= 0);
    if (d < 0) {
        return mint(0);
    }
    if (n == 0) {
        return (d == 0 ? mint(1) : mint(0));
    }
    return C<mint, large, dense>(n + d - 1, d);
}
template <int mod> struct modint {
    static constexpr u32 umod = u32(mod);
    static_assert(umod < u32(1) << 31);
    u32 val;
    static modint raw(u32 v) {
        modint x;
        x.val = v;
        return x;
    }
    constexpr modint() : val(0) {}
    constexpr modint(u32 x) : val(x % umod) {}
    constexpr modint(u64 x) : val(x % umod) {}
    constexpr modint(int x) : val((x %= mod) < 0 ? x + mod : x) {};
    constexpr modint(ll x) : val((x %= mod) < 0 ? x + mod : x) {};
    bool operator<(const modint& other) const { return val < other.val; }
    modint& operator+=(const modint& p) {
        if ((val += p.val) >= umod) {
            val -= umod;
        }
        return *this;
    }
    modint& operator-=(const modint& p) {
        if ((val += umod - p.val) >= umod) {
            val -= umod;
        }
        return *this;
    }
    modint& operator*=(const modint& p) {
        val = u64(val) * p.val % umod;
        return *this;
    }
    modint& operator/=(const modint& p) {
        *this *= p.inverse();
        return *this;
    }
    modint operator-() const { return modint::raw(val ? mod - val : u32(0)); }
    modint operator+(const modint& p) const { return modint(*this) += p; }
    modint operator-(const modint& p) const { return modint(*this) -= p; }
    modint operator*(const modint& p) const { return modint(*this) *= p; }
    modint operator/(const modint& p) const { return modint(*this) /= p; }
    bool operator==(const modint& p) const { return val == p.val; }
    bool operator!=(const modint& p) const { return val != p.val; }
    modint inverse() const {
        int a = val, b = mod, u = 1, v = 0, t;
        while (b > 0) {
            t = a / b;
            swap(a -= t * b, b), swap(u -= t * v, v);
        }
        return modint(u);
    }
    modint pow(ll n) const {
        assert(n >= 0);
        modint ret(1), mul(val);
        while (n > 0) {
            if (n & 1) {
                ret *= mul;
            }
            mul *= mul;
            n >>= 1;
        }
        return ret;
    }
    static constexpr int get_mod() { return mod; }
};
using modint998 = modint<998244353>;
template <typename T> struct Edge {
    int frm, to;
    T cost;
    int id;
};
template <typename T = int, bool directed = false> struct Graph {
    static constexpr bool is_directed = directed;
    int N, M;
    using cost_type = T;
    using edge_type = Edge<T>;
    vector<edge_type> edges;
    vector<int> indptr;
    vector<edge_type> csr_edges;
    bool prepared;
    class OutgoingEdges {
      public:
        OutgoingEdges(const Graph* G, int l, int r) : G(G), l(l), r(r) {}
        const edge_type* begin() const {
            if (l == r) {
                return 0;
            }
            return &G->csr_edges[l];
        }
        const edge_type* end() const {
            if (l == r) {
                return 0;
            }
            return &G->csr_edges[r];
        }

      private:
        const Graph* G;
        int l, r;
    };
    bool is_prepared() { return prepared; }
    Graph() : N(0), M(0), prepared(0) {}
    Graph(int N) : N(N), M(0), prepared(0) {}
    void build(int n) {
        N = n, M = 0;
        prepared = 0;
        edges.clear();
        indptr.clear();
        csr_edges.clear();
    }
    void add(int frm, int to, T cost = 1, int i = -1) {
        assert(!prepared);
        assert(0 <= frm && 0 <= to && to < N);
        if (i == -1) {
            i = M;
        }
        auto e = edge_type({frm, to, cost, i});
        edges.emplace_back(e);
        ++M;
    }
    void build() {
        assert(!prepared);
        prepared = true;
        indptr.assign(N + 1, 0);
        for (auto&& e : edges) {
            indptr[e.frm + 1]++;
            if (!directed) {
                indptr[e.to + 1]++;
            }
        }
        for (int v = 0; v < N; ++v) {
            indptr[v + 1] += indptr[v];
        }
        auto counter = indptr;
        csr_edges.resize(indptr.back() + 1);
        for (auto&& e : edges) {
            csr_edges[counter[e.frm]++] = e;
            if (!directed) {
                csr_edges[counter[e.to]++] = edge_type({e.to, e.frm, e.cost, e.id});
            }
        }
    }
    OutgoingEdges operator[](int v) const {
        assert(prepared);
        return {this, indptr[v], indptr[v + 1]};
    }
};
template <class T> vc<T> convolution_naive(const vc<T>& a, const vc<T>& b) {
    int n = int(a.size()), m = int(b.size());
    if (n > m) {
        return convolution_naive<T>(b, a);
    }
    if (n == 0) {
        return {};
    }
    vector<T> ans(n + m - 1);
    for (ll i = 0; i < ll(n); ++i) {
        for (ll j = 0; j < ll(m); ++j) {
            ans[i + j] += a[i] * b[j];
        }
    }
    return ans;
}
// 任意の環でできる
template <typename T> vc<T> convolution_karatsuba(const vc<T>& f, const vc<T>& g) {
    const int thresh = 30;
    if (min(ll(f.size()), ll(g.size())) <= thresh) {
        return convolution_naive(f, g);
    }
    int n = max(ll(f.size()), ll(g.size()));
    int m = ceil(n, 2);
    vc<T> f1, f2, g1, g2;
    if (ll(f.size()) < m) {
        f1 = f;
    }
    if (ll(f.size()) >= m) {
        f1 = {f.begin(), f.begin() + m};
    }
    if (ll(f.size()) >= m) {
        f2 = {f.begin() + m, f.end()};
    }
    if (ll(g.size()) < m) {
        g1 = g;
    }
    if (ll(g.size()) >= m) {
        g1 = {g.begin(), g.begin() + m};
    }
    if (ll(g.size()) >= m) {
        g2 = {g.begin() + m, g.end()};
    }
    vc<T> a = convolution_karatsuba(f1, g1);
    vc<T> b = convolution_karatsuba(f2, g2);
    for (ll i = 0; i < ll(ll(f2.size())); ++i) {
        f1[i] += f2[i];
    }
    for (ll i = 0; i < ll(ll(g2.size())); ++i) {
        g1[i] += g2[i];
    }
    vc<T> c = convolution_karatsuba(f1, g1);
    vc<T> F(ll(f.size()) + ll(g.size()) - 1);
    assert(2 * m + ll(b.size()) <= ll(F.size()));
    for (ll i = 0; i < ll(ll(a.size())); ++i) {
        F[i] += a[i], c[i] -= a[i];
    }
    for (ll i = 0; i < ll(ll(b.size())); ++i) {
        F[2 * m + i] += b[i], c[i] -= b[i];
    }
    if (c.back() == T(0)) {
        c.pop_back();
    }
    for (ll i = 0; i < ll(ll(c.size())); ++i) {
        if (c[i] != T(0)) {
            F[m + i] += c[i];
        }
    }
    return F;
}
template <typename mint> vc<mint> convolution(const vc<mint>& a, const vc<mint>& b) {
    return convolution_karatsuba<mint>(a, b);
}
template <typename mint> vc<mint> fps_inv_sparse(const vc<mint>& f) {
    int N = ll(f.size());
    vc<pair<int, mint>> dat;
    for (ll i = 1; i < ll(N); ++i) {
        if (f[i] != mint(0)) {
            dat.emplace_back(i, f[i]);
        }
    }
    vc<mint> g(N);
    mint g0 = mint(1) / f[0];
    g[0] = g0;
    for (ll n = 1; n < ll(N); ++n) {
        mint rhs = 0;
        for (auto&& [k, fk] : dat) {
            if (k > n) {
                break;
            }
            rhs -= fk * g[n - k];
        }
        g[n] = rhs * g0;
    }
    return g;
}
template <typename mint> vc<mint> fps_inv(const vc<mint>& f) {
    assert(f[0] != mint(0));
    return fps_inv_sparse<mint>(f);
}
// n, m 次多項式 (n>=m) a, b → n-m 次多項式 c
// c[i] = sum_j b[j]a[i+j]
template <typename mint> vc<mint> middle_product(vc<mint>& a, vc<mint>& b) {
    return middle_product_naive(a, b);
}
template <typename mint> vc<mint> middle_product_naive(vc<mint>& a, vc<mint>& b) {
    vc<mint> res(ll(a.size()) - ll(b.size()) + 1);
    for (ll i = 0; i < ll(ll(res.size())); ++i) {
        for (ll j = 0; j < ll(ll(b.size())); ++j) {
            res[i] += b[j] * a[i + j];
        }
    }
    return res;
}
template <typename mint> vc<mint> all_inverse(vc<mint>& X) {
    for (auto&& x : X) {
        assert(x != mint(0));
    }
    int N = ll(X.size());
    vc<mint> res(N + 1);
    res[0] = mint(1);
    for (ll i = 0; i < ll(N); ++i) {
        res[i + 1] = res[i] * X[i];
    }
    mint t = res.back().inverse();
    res.pop_back();
    for (ll i = (N)-1; i >= ll(0); --i) {
        res[i] *= t;
        t *= X[i];
    }
    return res;
}
template <typename mint> struct SubproductTree {
    int m;
    int sz;
    vc<vc<mint>> T;
    SubproductTree(const vc<mint>& x) {
        m = ll(x.size());
        sz = 1;
        while (sz < m) {
            sz *= 2;
        }
        T.resize(2 * sz);
        for (ll i = 0; i < ll(sz); ++i) {
            T[sz + i] = {1, (i < m ? -x[i] : 0)};
        }
        for (ll i = (sz)-1; i >= ll(1); --i) {
            T[i] = convolution(T[2 * i], T[2 * i + 1]);
        }
    }
    vc<mint> evaluation(vc<mint> f) {
        int n = ll(f.size());
        if (n == 0) {
            return vc<mint>(m, mint(0));
        }
        f.resize(2 * n - 1);
        vc<vc<mint>> g(2 * sz);
        g[1] = T[1];
        g[1].resize(n);
        g[1] = fps_inv(g[1]);
        g[1] = middle_product(f, g[1]);
        g[1].resize(sz);
        for (ll i = 1; i < ll(sz); ++i) {
            g[2 * i] = middle_product(g[i], T[2 * i + 1]);
            g[2 * i + 1] = middle_product(g[i], T[2 * i]);
        }
        vc<mint> vals(m);
        for (ll i = 0; i < ll(m); ++i) {
            vals[i] = g[sz + i][0];
        }
        return vals;
    }
    vc<mint> interpolation(vc<mint>& y) {
        assert(ll(y.size()) == m);
        vc<mint> a(m);
        for (ll i = 0; i < ll(m); ++i) {
            a[i] = T[1][m - i - 1] * (i + 1);
        }
        a = evaluation(a);
        vc<vc<mint>> t(2 * sz);
        for (ll i = 0; i < ll(sz); ++i) {
            t[sz + i] = {(i < m ? y[i] / a[i] : 0)};
        }
        for (ll i = (sz)-1; i >= ll(1); --i) {
            t[i] = convolution(t[2 * i], T[2 * i + 1]);
            auto tt = convolution(t[2 * i + 1], T[2 * i]);
            for (ll k = 0; k < ll(ll(t[i].size())); ++k) {
                t[i][k] += tt[k];
            }
        }
        t[1].resize(m);
        reverse(t[1].begin(), t[1].end());
        return t[1];
    }
};
template <typename mint> vc<mint> multipoint_eval(vc<mint>& f, vc<mint>& x) {
    if (x.empty()) {
        return {};
    }
    SubproductTree<mint> F(x);
    return F.evaluation(f);
}
template <typename mint> vc<mint> multipoint_interpolate(vc<mint>& x, vc<mint>& y) {
    if (x.empty()) {
        return {};
    }
    SubproductTree<mint> F(x);
    return F.interpolation(y);
}
template <typename T, int LIM> vc<array<T, LIM + 1>> ranked_zeta(const vc<T>& f) {
    int n = topbit(ll(f.size()));
    assert(n <= LIM);
    assert(ll(f.size()) == 1 << n);
    vc<array<T, LIM + 1>> Rf(1 << n);
    for (int s = 0; s < (1 << n); ++s) {
        Rf[s][popcnt(s)] = f[s];
    }
    for (int i = 0; i < n; ++i) {
        int w = 1 << i;
        for (int p = 0; p < (1 << n); p += 2 * w) {
            for (int s = p; s < p + w; ++s) {
                int t = s | 1 << i;
                for (int d = 0; d <= n; ++d) {
                    Rf[t][d] += Rf[s][d];
                }
            }
        }
    }
    return Rf;
}
template <typename T, int LIM> vc<T> ranked_mobius(vc<array<T, LIM + 1>>& Rf) {
    int n = topbit(ll(Rf.size()));
    assert(ll(Rf.size()) == 1 << n);
    for (int i = 0; i < n; ++i) {
        int w = 1 << i;
        for (int p = 0; p < (1 << n); p += 2 * w) {
            for (int s = p; s < p + w; ++s) {
                int t = s | 1 << i;
                for (int d = 0; d <= n; ++d) {
                    Rf[t][d] -= Rf[s][d];
                }
            }
        }
    }
    vc<T> f(1 << n);
    for (int s = 0; s < (1 << n); ++s) {
        f[s] = Rf[s][popcnt(s)];
    }
    return f;
}
template <typename T, int LIM = 20> vc<T> subset_convolution(const vc<T>& A, const vc<T>& B) {
    auto RA = ranked_zeta<T, LIM>(A);
    auto RB = ranked_zeta<T, LIM>(B);
    int n = topbit(ll(RA.size()));
    for (ll s = 0; s < ll(ll(RA.size())); ++s) {
        auto &f = RA[s], &g = RB[s];
        for (ll d = (n + 1) - 1; d >= ll(0); --d) {
            T x = 0;
            for (ll i = 0; i < ll(d + 1); ++i) {
                x += f[i] * g[d - i];
            }
            f[d] = x;
        }
    }
    return ranked_mobius<T, LIM>(RA);
}
// for fixed sps s, consider linear map F:a->b = subset-conv(a,s)
// given x, calculate transpose(F)(x)
template <typename mint, int LIM> vc<mint> transposed_subset_convolution(vc<mint> s, vc<mint> x) {
    /*
    sum_{j}x_jb_j = sum_{i subset j}x_ja_is_{j-i} = sum_{i}y_ia_i.
    y_i = sum_{j supset i}x_js_{j-i}
    (rev y)_i = sum_{j subset i}(rev x)_js_{i-j}
    y = rev(conv(rev x), s)
    */
    reverse(x.begin(), x.end());
    x = subset_convolution<mint, LIM>(x, s);
    reverse(x.begin(), x.end());
    return x;
}
// assume s[0]==0
// calculate sum_i wt_i(s^k/k!)_i for k=0,1,...,N
template <typename mint, int LIM> vc<mint> power_projection_of_sps_egf(vc<mint> wt, vc<mint>& s) {
    const int N = topbit(ll(s.size()));
    assert(ll(s.size()) == (1 << N) && ll(wt.size()) == (1 << N) && s[0] == mint(0));
    vc<mint> y(N + 1);
    y[0] = wt[0];
    auto& dp = wt;
    for (ll i = 0; i < ll(N); ++i) {
        vc<mint> newdp(1 << (N - 1 - i));
        for (ll j = 0; j < ll(N - i); ++j) {
            vc<mint> a = {s.begin() + (1 << j), s.begin() + (2 << j)};
            vc<mint> b = {dp.begin() + (1 << j), dp.begin() + (2 << j)};
            b = transposed_subset_convolution<mint, LIM>(a, b);
            for (ll k = 0; k < ll(ll(b.size())); ++k) {
                newdp[k] += b[k];
            }
        }
        swap(dp, newdp);
        y[1 + i] = dp[0];
    }
    return y;
}
// calculate sum_i x_i(s^k)_i for k=0,1,...,M-1
template <typename mint, int LIM> vc<mint> power_projection_of_sps(vc<mint> wt, vc<mint> s, int M) {
    const int N = topbit(ll(s.size()));
    assert(ll(s.size()) == (1 << N) && ll(wt.size()) == (1 << N));
    mint c = s[0];
    s[0] -= c;
    vc<mint> x = power_projection_of_sps_egf<mint, LIM>(wt, s);
    vc<mint> g(M);
    mint pow = 1;
    for (ll i = 0; i < ll(M); ++i) {
        g[i] = pow * fact_inv<mint>(i), pow *= c;
    }
    x = convolution<mint>(x, g);
    x.resize(M);
    for (ll i = 0; i < ll(M); ++i) {
        x[i] *= fact<mint>(i);
    }
    return x;
}
// O(N^22^N)
template <typename mint, int MAX_N> vc<mint> chromatic_polynomial(Graph<int, 0> G) {
    int N = G.N;
    assert(N <= MAX_N);
    vc<int> ng(1 << N);
    for (auto& e : G.edges) {
        int i = e.frm, j = e.to;
        ng[(1 << i) | (1 << j)] = 1;
    }
    for (ll s = 0; s < ll(1 << N); ++s) {
        if (ng[s]) {
            for (ll i = 0; i < ll(N); ++i) {
                ng[s | 1 << i] = 1;
            }
        }
    }
    vc<mint> f(1 << N);
    for (ll s = 0; s < ll(1 << N); ++s) {
        if (!ng[s]) {
            f[s] = 1;
        }
    }
    vc<mint> wt(1 << N);
    wt.back() = 1;
    vc<mint> Y = power_projection_of_sps<mint, MAX_N>(wt, f, N + 1);
    vc<mint> X(N + 1);
    for (ll i = 0; i < ll(N + 1); ++i) {
        X[i] = i;
    }
    return multipoint_interpolate<mint>(X, Y);
}
using mint = modint998;
void solve() {
    int N, M;
    scanf("%d %d", &N, &M);
    Graph<int, 0> G(N);
    for (int i = 0; i < M; ++i) {
        int a, b;
        scanf("%d %d", &a, &b);
        G.add(a, b);
    }
    G.build();
    vc<mint> f = chromatic_polynomial<mint, 20>(G);
    f.resize(N + 1);
    for (int i = 0; i <= N; ++i) {
        if (i) {
            printf(" ");
        }
        printf("%d", f[i].val);
    }
    printf("\n");
}
signed main() {
    solve();
    return 0;
}
