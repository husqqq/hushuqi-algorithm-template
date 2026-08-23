// Library Checker official reference implementation, adapted to this template's style.
// Problem: common_interval_decomposition_tree
#include <bits/stdc++.h>
using namespace std;

// https://github.com/atcoder/ac-library

#ifdef _MSC_VER
#endif

namespace atcoder {

namespace internal {

// @param n `0 <= n`
// @return minimum non-negative `x` s.t. `n <= 2**x`
int ceil_pow2(int n) {
    int x = 0;
    while ((1U << x) < (unsigned int)(n)) x++;
    return x;
}

// @param n `1 <= n`
// @return minimum non-negative `x` s.t. `(n & (1 << x)) != 0`
int bsf(unsigned int n) {
#ifdef _MSC_VER
    unsigned long index;
    _BitScanForward(&index, n);
    return index;
#else
    return __builtin_ctz(n);
#endif
}

} // namespace internal

} // namespace atcoder

namespace atcoder {

template <class S, S (*op)(S, S), S (*e)(), class F, S (*mapping)(F, S), F (*composition)(F, F), F (*id)()>
struct lazy_segtree {
  public:
    lazy_segtree() : lazy_segtree(0) {}
    lazy_segtree(int n) : lazy_segtree(vector<S>(n, e())) {}
    lazy_segtree(const vector<S>& v) : _n(int(v.size())) {
        log = internal::ceil_pow2(_n);
        size = 1 << log;
        d = vector<S>(2 * size, e());
        lz = vector<F>(size, id());
        for (int i = 0; i < _n; i++) d[size + i] = v[i];
        for (int i = size - 1; i >= 1; i--) {
            update(i);
        }
    }

    void set(int p, S x) {
        assert(0 <= p && p < _n);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        d[p] = x;
        for (int i = 1; i <= log; i++) update(p >> i);
    }

    S get(int p) {
        assert(0 <= p && p < _n);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        return d[p];
    }

    S prod(int l, int r) {
        assert(0 <= l && l <= r && r <= _n);
        if (l == r) return e();

        l += size;
        r += size;

        for (int i = log; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push(r >> i);
        }

        S sml = e(), smr = e();
        while (l < r) {
            if (l & 1) sml = op(sml, d[l++]);
            if (r & 1) smr = op(d[--r], smr);
            l >>= 1;
            r >>= 1;
        }

        return op(sml, smr);
    }

    S all_prod() { return d[1]; }

    void apply(int p, F f) {
        assert(0 <= p && p < _n);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        d[p] = mapping(f, d[p]);
        for (int i = 1; i <= log; i++) update(p >> i);
    }
    void apply(int l, int r, F f) {
        assert(0 <= l && l <= r && r <= _n);
        if (l == r) return;

        l += size;
        r += size;

        for (int i = log; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }

        {
            int l2 = l, r2 = r;
            while (l < r) {
                if (l & 1) all_apply(l++, f);
                if (r & 1) all_apply(--r, f);
                l >>= 1;
                r >>= 1;
            }
            l = l2;
            r = r2;
        }

        for (int i = 1; i <= log; i++) {
            if (((l >> i) << i) != l) update(l >> i);
            if (((r >> i) << i) != r) update((r - 1) >> i);
        }
    }

    template <bool (*g)(S)> int max_right(int l) {
        return max_right(l, [](S x) { return g(x); });
    }
    template <class G> int max_right(int l, G g) {
        assert(0 <= l && l <= _n);
        assert(g(e()));
        if (l == _n) return _n;
        l += size;
        for (int i = log; i >= 1; i--) push(l >> i);
        S sm = e();
        do {
            while (l % 2 == 0) l >>= 1;
            if (!g(op(sm, d[l]))) {
                while (l < size) {
                    push(l);
                    l = (2 * l);
                    if (g(op(sm, d[l]))) {
                        sm = op(sm, d[l]);
                        l++;
                    }
                }
                return l - size;
            }
            sm = op(sm, d[l]);
            l++;
        } while ((l & -l) != l);
        return _n;
    }

    template <bool (*g)(S)> int min_left(int r) {
        return min_left(r, [](S x) { return g(x); });
    }
    template <class G> int min_left(int r, G g) {
        assert(0 <= r && r <= _n);
        assert(g(e()));
        if (r == 0) return 0;
        r += size;
        for (int i = log; i >= 1; i--) push((r - 1) >> i);
        S sm = e();
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!g(op(d[r], sm))) {
                while (r < size) {
                    push(r);
                    r = (2 * r + 1);
                    if (g(op(d[r], sm))) {
                        sm = op(d[r], sm);
                        r--;
                    }
                }
                return r + 1 - size;
            }
            sm = op(d[r], sm);
        } while ((r & -r) != r);
        return 0;
    }

  private:
    int _n, size, log;
    vector<S> d;
    vector<F> lz;

    void update(int k) { d[k] = op(d[2 * k], d[2 * k + 1]); }
    void all_apply(int k, F f) {
        d[k] = mapping(f, d[k]);
        if (k < size) lz[k] = composition(f, lz[k]);
    }
    void push(int k) {
        all_apply(2 * k, lz[k]);
        all_apply(2 * k + 1, lz[k]);
        lz[k] = id();
    }
};

} // namespace atcoder

namespace permutation_tree {
template <typename T> T op(T x, T y) {
    return max(x, y);
}
template <typename T> T e() {
    return -T(1 << 30);
}
template <typename T, typename F> T mapping(F x, T y) {
    return x + y;
}
template <typename F> F composition(F x, F y) {
    return x + y;
}
template <typename F> F id() {
    return F();
}
template <typename T> struct perm_tree {
    struct node;
    using np = node*;
    struct node {
        // 両方とも半開区間で保存
        int l, r;
        T mn, mx;
        bool is_join;
        vector<np> ch;
        node() {}
        node(int idx, T x) : l(idx), r(idx + 1), mn(x), mx(x + 1), is_join(1) {};
    };
    stack<int> mn_stk, mx_stk;
    vector<T> x;
    using F = T;
    atcoder::lazy_segtree<T, op, e, F, mapping, composition, id> seg;
    stack<np> stk;
    perm_tree(const vector<T>& x) : x(x), seg(x.size()) {
        for (int i = 0; i < (int)x.size(); ++i) insert(i);
    }
    np get() {
        assert((int)stk.size() == 1);
        return stk.top();
    }
    void insert(int idx) {
        {
            int r = idx;
            while (!mn_stk.empty() && x[mn_stk.top()] > x[idx]) {
                mn_stk.pop();
                int l = (mn_stk.empty() ? 0 : mn_stk.top() + 1);
                seg.apply(l, r, x[idx] - x[r - 1]);
                r = l;
            }
            mn_stk.emplace(idx);
        }
        {
            int r = idx;
            while (!mx_stk.empty() && x[mx_stk.top()] < x[idx]) {
                mx_stk.pop();
                int l = (mx_stk.empty() ? 0 : mx_stk.top() + 1);
                seg.apply(l, r, -x[idx] + x[r - 1]);
                r = l;
            }
            mx_stk.emplace(idx);
        }
        seg.apply(0, idx, 1);
        np nw = new node(idx, x[idx]);

        while (1) {
            if (!stk.empty() && !stk.top()->ch.empty() && stk.top()->is_join &&
                (nw->mn == stk.top()->ch.back()->mx || nw->mx == stk.top()->ch.back()->mn)) {
                stk.top()->ch.emplace_back(nw);
                stk.top()->mx = max(stk.top()->mx, nw->mx);
                stk.top()->mn = min(stk.top()->mn, nw->mn);
                stk.top()->r = idx + 1;
                nw = stk.top();
                stk.pop();
                continue;
            }
            if (!stk.empty() && (stk.top()->mx == nw->mn || stk.top()->mn == nw->mx)) {
                np nw2 = new node();
                auto top = stk.top();
                stk.pop();
                nw2->is_join = 1;
                nw2->l = top->l;
                nw2->r = nw->r;
                nw2->mn = min(top->mn, nw->mn);
                nw2->mx = max(top->mx, nw->mx);
                nw2->ch.emplace_back(top);
                nw2->ch.emplace_back(nw);
                nw = nw2;
                continue;
            }
            stk.emplace(nw);
            if ((int)stk.size() == 1) break;
            T mx = seg.prod(0, stk.top()->l);
            if (mx != 0) break;
            nw = new node();
            *nw = *stk.top();
            nw->is_join = 0;
            nw->ch.clear();
            nw->ch.emplace_back(stk.top());
            stk.pop();
            while (1) {
                auto top = stk.top();
                nw->ch.emplace_back(top);
                stk.pop();
                nw->l = min(nw->l, top->l);
                nw->r = max(nw->r, top->r);
                nw->mn = min(nw->mn, top->mn);
                nw->mx = max(nw->mx, top->mx);
                if (nw->r - nw->l == nw->mx - nw->mn) {
                    break;
                }
            }
            reverse(nw->ch.begin(), nw->ch.end());
        }
        seg.set(idx, T());
    }
};
}; // namespace permutation_tree
using namespace permutation_tree;

int main() {
    int n;
    scanf("%d", &n);
    vector<int> p(n);
    for (int i = 0; i < n; ++i) scanf("%d", &p[i]);
    perm_tree<int> pt(p);
    auto root = pt.get();
    using np = decltype(root);
    vector<tuple<int, int, int, string>> v;
    auto dfs = [&](auto dfs, np t, int par) -> void {
        int now = v.size();
        v.emplace_back(par, t->l, t->r, (t->is_join ? "linear" : "prime"));
        for (auto e : t->ch) {
            dfs(dfs, e, now);
        }
    };
    dfs(dfs, root, -1);
    printf("%zu\n", v.size());
    for (auto [par, l, r, s] : v) {
        printf("%d %d %d %s\n", par, l, r - 1, s.c_str());
    }
}
