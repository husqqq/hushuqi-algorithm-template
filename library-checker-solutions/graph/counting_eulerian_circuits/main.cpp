#include <bits/stdc++.h>

// Library Checker official reference implementation, adapted to this template's style.
// Problem: counting_eulerian_circuits
using namespace std;
using ll = long long;
using u32 = unsigned int;
using u64 = unsigned long long;
using vi = vector<ll>;
template <class T> using vc = vector<T>;
template <class T> using vvc = vector<vc<T>>;
template <typename T> T POP(vc<T>& que) {
    T a = que.back();
    que.pop_back();
    return a;
}
template <class T, class S> inline bool chmax(T& a, const S& b) {
    return (a < b ? a = b, 1 : 0);
}
template <class T, class S> inline bool chmin(T& a, const S& b) {
    return (a > b ? a = b, 1 : 0);
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
    vc<int> vc_deg, vc_indeg, vc_outdeg;
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
    vc<int> new_idx;
    vc<bool> used_e;
    // G における頂点 V[i] が、新しいグラフで i になるようにする
    // {G, es}
    // sum(deg(v)) の計算量になっていて、
    // 新しいグラフの n+m より大きい可能性があるので注意
    Graph<T, directed> rearrange(vc<int> V, bool keep_eid = 0) {
        if (ll(new_idx.size()) != N) {
            new_idx.assign(N, -1);
        }
        int n = ll(V.size());
        for (ll i = 0; i < ll(n); ++i) {
            new_idx[V[i]] = i;
        }
        Graph<T, directed> G(n);
        vc<int> history;
        for (ll i = 0; i < ll(n); ++i) {
            for (auto&& e : (*this)[V[i]]) {
                if (ll(used_e.size()) <= e.id) {
                    used_e.resize(e.id + 1);
                }
                if (used_e[e.id]) {
                    continue;
                }
                int a = e.frm, b = e.to;
                if (new_idx[a] != -1 && new_idx[b] != -1) {
                    history.emplace_back(e.id);
                    used_e[e.id] = 1;
                    int eid = (keep_eid ? e.id : -1);
                    G.add(new_idx[a], new_idx[b], e.cost, eid);
                }
            }
        }
        for (ll i = 0; i < ll(n); ++i) {
            new_idx[V[i]] = -1;
        }
        for (auto&& eid : history) {
            used_e[eid] = 0;
        }
        G.build();
        return G;
    }
};
int det_mod(vvc<int> A, int mod) {
    const int n = ll(A.size());
    ll det = 1;
    for (ll i = 0; i < ll(n); ++i) {
        for (ll j = i; j < ll(n); ++j) {
            if (A[j][i] == 0) {
                continue;
            }
            if (i != j) {
                swap(A[i], A[j]), det = mod - det;
            }
            break;
        }
        for (ll j = i + 1; j < ll(n); ++j) {
            while (A[i][i] != 0) {
                ll c = mod - A[j][i] / A[i][i];
                for (ll k = (n)-1; k >= ll(i); --k) {
                    A[j][k] = (A[j][k] + A[i][k] * c) % mod;
                }
                swap(A[i], A[j]), det = mod - det;
            }
            swap(A[i], A[j]), det = mod - det;
        }
    }
    for (ll i = 0; i < ll(n); ++i) {
        det = det * A[i][i] % mod;
    }
    return det % mod;
}
template <typename mint> mint det(vvc<mint>& A) {
    const int n = ll(A.size());
    vector<vector<int>> B(n, vector<int>(n));
    for (ll i = 0; i < ll(n); ++i) {
        for (ll j = 0; j < ll(n); ++j) {
            B[i][j] = A[i][j].val;
        }
    }
    return det_mod(B, mint::get_mod());
}
/*
ひとつ選んだ辺から始めて全ての辺を通る closed walk を数える.
多重辺は vc<int>(eid) で渡す，なければすべて 1. e.cost は参照しない.
辺はラベル付きで考える. 多重辺を同一視する場合などは後で階乗で割ること.
O(N^2+NM) （ + 最後に重複度の階乗をかける）．
*/
template <typename mint, typename GT> mint BEST_theorem(GT G, vc<int> edge_multiplicity = {}) {
    static_assert(GT::is_directed);
    int N = G.N, M = G.M;
    if (M == 0) {
        return 0;
    }
    if (edge_multiplicity.empty()) {
        edge_multiplicity.assign(M, 1);
    }
    vc<int> vs;
    for (auto& e : G.edges) {
        if (edge_multiplicity[e.id] == 0) {
            continue;
        }
        vs.emplace_back(e.frm), vs.emplace_back(e.to);
    }
    sort(vs.begin(), vs.end()), vs.erase(unique(vs.begin(), vs.end()), vs.end()), vs.shrink_to_fit();
    G = G.rearrange(vs, true);
    N = G.N;
    vc<int> indeg(N), outdeg(N);
    vector<vector<mint>> mat(N - 1, vector<mint>(N - 1));
    for (auto& e : G.edges) {
        int a = e.frm, b = e.to, x = edge_multiplicity[e.id];
        outdeg[a] += x, indeg[b] += x;
        if (a < N - 1 && b < N - 1) {
            mat[a][b] -= x;
        }
        if (a < N - 1) {
            mat[a][a] += x;
        }
    }
    for (ll v = 0; v < ll(N); ++v) {
        if (indeg[v] != outdeg[v]) {
            return 0;
        }
    }
    mint d = det<mint>(mat);
    for (auto& x : outdeg) {
        d *= fact<mint>(x - 1);
    }
    return d;
}
using mint = modint998;
void solve() {
    int N, M;
    scanf("%d %d", &N, &M);
    Graph<int, 1> G(N);
    for (ll _ = 0; _ < ll(M); ++_) {
        int a, b;
        scanf("%d %d", &a, &b);
        G.add(a, b);
    }
    G.build();
    mint ANS = BEST_theorem<mint>(G);
    printf("%d\n", ANS.val);
}
signed main() {
    solve();
}
