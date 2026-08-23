// Adapted from yosupo06/library-checker-problems (Apache-2.0).
// Problem: point_set_range_sort_range_composite; upstream commit: 04c8de378bab67be926325de2871f0babb8e6451.
#include <cstdio>
#include <vector>
#include <tuple>
#include <cassert>
#include <algorithm>

using namespace std;

#ifndef LC_INLINE_398DB3E6C1F1F17B935B
#define LC_INLINE_398DB3E6C1F1F17B935B

template <class Monoid>
struct SegTree {
  using X = typename Monoid::value_type;
  using value_type = X;
  vector<X> dat;
  int n, log, size;

  SegTree() : SegTree(0) {}
  SegTree(int n) : SegTree(vector<X>(n, Monoid::unit())) {}
  SegTree(vector<X> v) : n(v.size()) {
    log = 1;
    while ((1 << log) < n) ++log;
    size = 1 << log;
    dat.assign(size << 1, Monoid::unit());
    for (int i = 0; i < n; ++i) dat[size + i] = v[i];
    for (int i = size - 1; i >= 1; --i) update(i);
  }

  void reset() { fill(all(dat), Monoid::unit()); }

  void set_all(const vector<X>& v) {
    dat.assign(size << 1, Monoid::unit());
    for (int i = 0; i < n; ++i) dat[size + i] = v[i];
    for (int i = size - 1; i >= 1; --i) update(i);
  }

  X operator[](int i) { return dat[size + i]; }

  void update(int i) { dat[i] = Monoid::op(dat[2 * i], dat[2 * i + 1]); }

  void set(int i, const X& x) {
    assert(i < n);
    dat[i += size] = x;
    while (i >>= 1) update(i);
  }

  void multiply(int i, const X& x) {
    assert(i < n);
    i += size;
    dat[i] = Monoid::op(dat[i], x);
    while (i >>= 1) update(i);
  }

  X prod(int L, int R) {
    assert(L <= R);
    assert(R <= n);
    X vl = Monoid::unit(), vr = Monoid::unit();
    L += size, R += size;
    while (L < R) {
      if (L & 1) vl = Monoid::op(vl, dat[L++]);
      if (R & 1) vr = Monoid::op(dat[--R], vr);
      L >>= 1, R >>= 1;
    }
    return Monoid::op(vl, vr);
  }

  X prod_all() { return dat[1]; }

  template <class F>
  int max_right(F& check, int L) {
    assert(0 <= L && L <= n && check(Monoid::unit()));
    if (L == n) return n;
    L += size;
    X sm = Monoid::unit();
    do {
      while (L % 2 == 0) L >>= 1;
      if (!check(Monoid::op(sm, dat[L]))) {
        while (L < size) {
          L = 2 * L;
          if (check(Monoid::op(sm, dat[L]))) {
            sm = Monoid::op(sm, dat[L]);
            L++;
          }
        }
        return L - size;
      }
      sm = Monoid::op(sm, dat[L]);
      L++;
    } while ((L & -L) != L);
    return n;
  }

  template <class F>
  int min_left(F& check, int R) {
    assert(0 <= R && R <= n && check(Monoid::unit()));
    if (R == 0) return 0;
    R += size;
    X sm = Monoid::unit();
    do {
      --R;
      while (R > 1 && (R % 2)) R >>= 1;
      if (!check(Monoid::op(dat[R], sm))) {
        while (R < size) {
          R = 2 * R + 1;
          if (check(Monoid::op(dat[R], sm))) {
            sm = Monoid::op(dat[R], sm);
            R--;
          }
        }
        return R + 1 - size;
      }
      sm = Monoid::op(dat[R], sm);
    } while ((R & -R) != R);
    return 0;
  }

  // モノイドが可換なら、prod_{l<=i<r}A[i^x] が計算可能
  // https://codeforces.com/contest/1401/problem/F
  X Xor_prod(int l, int r, int xor_val) {
    assert(Monoid::commute);
    X x = Monoid::unit();
    for (int k = 0; k < log + 1; ++k) {
      if (l >= r) break;
      if (l & 1) { x = Monoid::op(x, dat[(size >> k) + ((l++) ^ xor_val)]); }
      if (r & 1) { x = Monoid::op(x, dat[(size >> k) + ((--r) ^ xor_val)]); }
      l /= 2, r /= 2, xor_val /= 2;
    }
    return x;
  }
};
#endif

/* 64分木。
insert, erase
[]での存在判定
next, prev
*/
struct FastSet {
  using uint = unsigned;
  using ull = unsigned long long;

  int bsr(ull x) { return 63 - __builtin_clzll(x); }
  int bsf(ull x) { return __builtin_ctzll(x); }

  static constexpr uint B = 64;
  int n, lg;
  vector<vector<ull>> seg;
  FastSet(int _n) : n(_n) {
    do {
      seg.push_back(vector<ull>((_n + B - 1) / B));
      _n = (_n + B - 1) / B;
    } while (_n > 1);
    lg = int(seg.size());
  }
  bool operator[](int i) const { return (seg[0][i / B] >> (i % B) & 1) != 0; }
  void insert(int i) {
    for (int h = 0; h < lg; h++) {
      seg[h][i / B] |= 1ULL << (i % B);
      i /= B;
    }
  }
  void erase(int i) {
    for (int h = 0; h < lg; h++) {
      seg[h][i / B] &= ~(1ULL << (i % B));
      if (seg[h][i / B]) break;
      i /= B;
    }
  }

  // x以上最小の要素を返す。存在しなければ n。
  int next(int i) {
    for (int h = 0; h < lg; h++) {
      if (i / B == seg[h].size()) break;
      ull d = seg[h][i / B] >> (i % B);
      if (!d) {
        i = i / B + 1;
        continue;
      }
      // find
      i += bsf(d);
      for (int g = h - 1; g >= 0; g--) {
        i *= B;
        i += bsf(seg[g][i / B]);
      }
      return i;
    }
    return n;
  }

  // x以下最大の要素を返す。存在しなければ -1。
  int prev(int i) {
    if (i < 0) return -1;
    if (i >= n) i = n - 1;
    for (int h = 0; h < lg; h++) {
      if (i == -1) break;
      ull d = seg[h][i / B] << (63 - i % 64);
      if (!d) {
        i = i / B - 1;
        continue;
      }
      // find
      i += bsr(d) - (B - 1);
      for (int g = h - 1; g >= 0; g--) {
        i *= B;
        i += bsr(seg[g][i / B]);
      }
      return i;
    }
    return -1;
  }

  // [l, r) 内の要素を全部集める
  vector<int> collect(int l, int r) {
    vector<int> res;
    int x = l - 1;
    while (1) {
      x = next(x + 1);
      if (x >= r) break;
      res.emplace_back(x);
    }
    return res;
  }
};


template <typename Monoid, int NODES = 4'000'000>
struct Sortable_SegTree {
  using X = typename Monoid::value_type;
  const int N, KEY_MAX;

  struct Node {
    X x, rev_x;
    int size;
    Node *l, *r;
    Node() {}
  };

  Node* pool;
  int pid;

  // 区間の左端全体を表す fastset
  FastSet ss;
  // 区間を集約した値を区間の左端にのせた segtree
  SegTree<Monoid> seg;
  // 区間の左端に、dynamic segtree の node を乗せる
  vector<bool> rev;
  vector<Node*> root;
  Sortable_SegTree(int KEY_MAX, vector<int> key, vector<X> dat)
      : N(key.size()), KEY_MAX(KEY_MAX), pid(0), ss(key.size()), seg(dat) {
    pool = new Node[NODES];
    init(key, dat);
  }

  void set(int i, int key, const X& x) {
    assert(key < KEY_MAX);
    split_at(i);
    split_at(i + 1);
    rev[i] = 0;
    root[i] = new_node();
    set_rec_key(root[i], 0, KEY_MAX, key, x);
    seg.set(i, x);
  }

  X prod_all() { return seg.prod_all(); }

  X prod(int l, int r) {
    if (pid > NODES * 0.9) rebuild();
    split_at(l), split_at(r);
    return seg.prod(l, r);
  }

  void sort_inc(int l, int r) {
    split_at(l), split_at(r);
    while (1) {
      if (pid > NODES * 0.9) rebuild();
      Node* n = root[l];
      int i = ss.next(l + 1);
      if (i == r) break;
      root[l] = merge(n, root[i]);
      ss.erase(i);
      seg.set(i, Monoid::unit());
    }
    rev[l] = 0;
    seg.set(l, root[l]->x);
  };

  void sort_dec(int l, int r) {
    if (pid > NODES * 0.9) rebuild();
    sort_inc(l, r);
    rev[l] = 1;
    seg.set(l, root[l]->rev_x);
  };

private:
  void init(vector<int>& key, vector<X>& dat) {
    rev.assign(N, 0);
    root.clear();
    seg.set_all(dat);
    for (int i = 0; i < N; ++i) ss.insert(i);
    for (int i = 0; i < N; ++i) root.emplace_back(new_node(Monoid::unit()));
    for (int i = 0; i < N; ++i) {
      assert(key[i] < KEY_MAX);
      set_rec_key(root[i], 0, KEY_MAX, key[i], dat[i]);
    }
  }

  void split_at(int x) {
    if (x == N || ss[x]) return;
    int a = ss.prev(x);
    int b = ss.next(a + 1);
    ss.insert(x);
    if (!rev[a]) {
      auto [nl, nr] = split(root[a], x - a);
      root[a] = nl;
      root[x] = nr;
      rev[a] = rev[x] = 0;
      seg.set(a, root[a]->x);
      seg.set(x, root[x]->x);
    } else {
      auto [nl, nr] = split(root[a], b - x);
      root[a] = nr;
      root[x] = nl;
      rev[a] = rev[x] = 1;
      seg.set(a, root[a]->rev_x);
      seg.set(x, root[x]->rev_x);
    }
  }

  void rebuild() {
    vector<int> key;
    vector<X> dat;
    key.reserve(N);
    dat.reserve(N);
    auto dfs
        = [&](auto& dfs, Node* n, int node_l, int node_r, bool rev) -> void {
      if (!n) return;
      if (node_r == node_l + 1) {
        key.emplace_back(node_l);
        dat.emplace_back(n->x);
        return;
      }
      int node_m = (node_l + node_r) / 2;
      if (!rev) {
        dfs(dfs, n->l, node_l, node_m, rev);
        dfs(dfs, n->r, node_m, node_r, rev);
      }
      if (rev) {
        dfs(dfs, n->r, node_m, node_r, rev);
        dfs(dfs, n->l, node_l, node_m, rev);
      }
    };
    for (int i = 0; i < N; ++i) {
      if (ss[i]) dfs(dfs, root[i], 0, KEY_MAX, rev[i]);
    }
    assert(int(key.size()) == N);

    pid = 0;
    init(key, dat);
  }

  Node* new_node(X x = Monoid::unit()) {
    assert(pid < NODES);
    pool[pid].x = pool[pid].rev_x = x;
    pool[pid].l = pool[pid].r = nullptr;
    pool[pid].size = 1;
    return &(pool[pid++]);
  }

  pair<Node*, Node*> split(Node* n, int k) {
    if (k == 0) { return {nullptr, n}; }
    if (k == n->size) { return {n, nullptr}; }
    int s = (n->l ? n->l->size : 0);
    Node* b = new_node();
    if (k <= s) {
      auto [nl, nr] = split(n->l, k);
      b->l = nr;
      b->r = n->r;
      n->l = nl;
      n->r = nullptr;
    }
    if (k > s) {
      auto [nl, nr] = split(n->r, k - s);
      n->l = n->l;
      n->r = nl;
      b->l = nullptr;
      b->r = nr;
    }
    update(n);
    update(b);
    return {n, b};
  }

  Node* merge(Node* a, Node* b) {
    if (!a) return b;
    if (!b) return a;
    a->l = merge(a->l, b->l);
    a->r = merge(a->r, b->r);
    update(a);
    return a;
  }

  void update(Node* n) {
    if (!(n->l) && !(n->r)) { return; }
    if (!(n->l)) {
      n->x = n->r->x, n->rev_x = n->r->rev_x;
      n->size = n->r->size;
      return;
    }
    if (!(n->r)) {
      n->x = n->l->x, n->rev_x = n->l->rev_x;
      n->size = n->l->size;
      return;
    }
    n->x = Monoid::op(n->l->x, n->r->x);
    n->rev_x = Monoid::op(n->r->rev_x, n->l->rev_x);
    n->size = n->l->size + n->r->size;
  }

  void set_rec_key(Node* n, int node_l, int node_r, int k, const X& x) {
    if (node_r == node_l + 1) {
      n->x = n->rev_x = x;
      return;
    }
    int node_m = (node_l + node_r) / 2;
    if (k < node_m) {
      if (!(n->l)) n->l = new_node();
      set_rec_key(n->l, node_l, node_m, k, x);
    }
    if (node_m <= k) {
      if (!(n->r)) n->r = new_node();
      set_rec_key(n->r, node_m, node_r, k, x);
    }
    update(n);
  }

  void set_rec_idx(Node* n, int node_l, int node_r, int idx, const X& x) {
    if (node_r == node_l + 1) {
      n->x = n->rev_x = x;
      return;
    }
    int node_m = (node_l + node_r) / 2;
    int s = (n->l ? n->l->size : 0);
    if (idx < s) { set_rec_idx(n->l, node_l, node_m, idx, x); }
    if (idx >= s) { set_rec_idx(n->r, node_m, node_r, idx - s, x); }
    update(n);
  }
};

#ifndef LC_INLINE_92537CF31D79B2277561
#define LC_INLINE_92537CF31D79B2277561

template <unsigned int mod>
struct modint {
  static constexpr bool is_modint = true;
  unsigned int val;
  constexpr modint(const long long val = 0) noexcept
      : val(val >= 0 ? val % mod : (mod - (-val) % mod) % mod) {}
  bool operator<(const modint &other) const {
    return val < other.val;
  } // To use std::map
  modint &operator+=(const modint &p) {
    if ((val += p.val) >= mod) val -= mod;
    return *this;
  }
  modint &operator-=(const modint &p) {
    if ((val += mod - p.val) >= mod) val -= mod;
    return *this;
  }
  modint &operator*=(const modint &p) {
    val = (unsigned int)(1LL * val * p.val % mod);
    return *this;
  }
  modint &operator/=(const modint &p) {
    *this *= p.inverse();
    return *this;
  }
  modint operator-() const { return modint(get_mod() - val); }
  modint operator+(const modint &p) const { return modint(*this) += p; }
  modint operator-(const modint &p) const { return modint(*this) -= p; }
  modint operator*(const modint &p) const { return modint(*this) *= p; }
  modint operator/(const modint &p) const { return modint(*this) /= p; }
  bool operator==(const modint &p) const { return val == p.val; }
  bool operator!=(const modint &p) const { return val != p.val; }
  modint inverse() const {
    int a = val, b = mod, u = 1, v = 0, t;
    while (b > 0) {
      t = a / b;
      swap(a -= t * b, b), swap(u -= t * v, v);
    }
    return modint(u);
  }
  modint pow(long long n) const {
    modint ret(1), mul(val);
    while (n > 0) {
      if (n & 1) ret *= mul;
      mul *= mul;
      n >>= 1;
    }
    return ret;
  }
  static constexpr unsigned int get_mod() { return mod; }
};

using modint107 = modint<1000000007>;
using modint998 = modint<998244353>;
#endif

template <typename K>
struct Group_Affine {
  using F = pair<K, K>;
  using value_type = F;
  static constexpr F op(const F &x, const F &y) noexcept {
    return F({x.first * y.first, x.second * y.first + y.second});
  }
  static constexpr F inversecond(const F &x) {
    auto [a, b] = x;
    a = K(1) / a;
    return {a, a * (-b)};
  }
  static constexpr K eval(const F &f, K x) noexcept { return f.first * x + f.second; }
  static constexpr F unit() { return {K(1), K(0)}; }
  static constexpr bool commute = false;
};


using mint = modint998;
using Mono = Group_Affine<mint>;
using X = typename Mono::value_type;

int main() {
  int N, Q;
  scanf("%d %d", &N, &Q);

  vector<int> all_key;
  using QT = tuple<int, int, int, int, int>;
  vector<QT> query(Q);

  vector<int> key(N);
  vector<X> dat(N);
  for (int n = 0; n < N; ++n) {
    int p, a, b;
    scanf("%d %d %d", &p, &a, &b);
    key[n] = p;
    dat[n] = {mint(a), mint(b)};
    all_key.emplace_back(p);
  }

  for (int q = 0; q < Q; ++q) {
    int t;
    scanf("%d", &t);
    if (t == 0) {
      int i, p, a, b;
      scanf("%d %d %d %d", &i, &p, &a, &b);
      query[q] = {t, i, p, a, b};
      all_key.emplace_back(p);
    }
    if (t == 1) {
      int l, r, x;
      scanf("%d %d %d", &l, &r, &x);
      query[q] = {t, l, r, x, 0};
    }
    if (t == 2) {
      int l, r;
      scanf("%d %d", &l, &r);
      query[q] = {t, l, r, 0, 0};
    }
    if (t == 3) {
      int l, r;
      scanf("%d %d", &l, &r);
      query[q] = {t, l, r, 0, 0};
    }
  }

  sort(all_key.begin(), all_key.end());
  for (auto&& x: key)
    x = lower_bound(all_key.begin(), all_key.end(), x) - all_key.begin();

  Sortable_SegTree<Mono> seg(all_key.size(), key, dat);

  for (int q = 0; q < Q; ++q) {
    auto [t, x1, x2, x3, x4] = query[q];
    if (t == 0) {
      int i = x1;
      int p = lower_bound(all_key.begin(), all_key.end(), x2) - all_key.begin();
      int a = x3;
      int b = x4;
      seg.set(i, p, {mint(a), mint(b)});
    }
    if (t == 1) {
      int l = x1, r = x2, x = x3;
      auto f = seg.prod(l, r);
      mint ans = Mono::eval(f, mint(x));
      printf("%d\n", ans.val);
    }
    if (t == 2) {
      int l = x1, r = x2;
      seg.sort_inc(l, r);
    }
    if (t == 3) {
      int l = x1, r = x2;
      seg.sort_dec(l, r);
    }
  }

  return 0;
}
