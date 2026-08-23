// Adapted from yosupo06/library-checker-problems (Apache-2.0).
// Problem: dynamic_point_set_rectangle_affine_rectangle_sum; upstream commit: 04c8de378bab67be926325de2871f0babb8e6451.
// Offline solution

#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#ifndef ATCODER_MODINT_HPP
#define ATCODER_MODINT_HPP 1

#include <cassert>
#include <numeric>
#include <type_traits>

#ifdef _MSC_VER
#include <intrin.h>
#endif

#ifndef ATCODER_INTERNAL_MATH_HPP
#define ATCODER_INTERNAL_MATH_HPP 1

#include <utility>

#ifdef _MSC_VER
#include <intrin.h>
#endif

namespace atcoder {

namespace internal {

// @param m `1 <= m`
// @return x mod m
constexpr long long safe_mod(long long x, long long m) {
    x %= m;
    if (x < 0) x += m;
    return x;
}

// Fast modular multiplication by barrett reduction
// Reference: https://en.wikipedia.org/wiki/Barrett_reduction
// NOTE: reconsider after Ice Lake
struct barrett {
    unsigned int _m;
    unsigned long long im;

    // @param m `1 <= m`
    explicit barrett(unsigned int m) : _m(m), im((unsigned long long)(-1) / m + 1) {}

    // @return m
    unsigned int umod() const { return _m; }

    // @param a `0 <= a < m`
    // @param b `0 <= b < m`
    // @return `a * b % m`
    unsigned int mul(unsigned int a, unsigned int b) const {
        // [1] m = 1
        // a = b = im = 0, so okay

        // [2] m >= 2
        // im = ceil(2^64 / m)
        // -> im * m = 2^64 + r (0 <= r < m)
        // let z = a*b = c*m + d (0 <= c, d < m)
        // a*b * im = (c*m + d) * im = c*(im*m) + d*im = c*2^64 + c*r + d*im
        // c*r + d*im < m * m + m * im < m * m + 2^64 + m <= 2^64 + m * (m + 1) < 2^64 * 2
        // ((ab * im) >> 64) == c or c + 1
        unsigned long long z = a;
        z *= b;
#ifdef _MSC_VER
        unsigned long long x;
        _umul128(z, im, &x);
#else
        unsigned long long x =
            (unsigned long long)(((unsigned __int128)(z)*im) >> 64);
#endif
        unsigned long long y = x * _m;
        return (unsigned int)(z - y + (z < y ? _m : 0));
    }
};

// @param n `0 <= n`
// @param m `1 <= m`
// @return `(x ** n) % m`
constexpr long long pow_mod_constexpr(long long x, long long n, int m) {
    if (m == 1) return 0;
    unsigned int _m = (unsigned int)(m);
    unsigned long long r = 1;
    unsigned long long y = safe_mod(x, m);
    while (n) {
        if (n & 1) r = (r * y) % _m;
        y = (y * y) % _m;
        n >>= 1;
    }
    return r;
}

// Reference:
// M. Forisek and J. Jancina,
// Fast Primality Testing for Integers That Fit into a Machine Word
// @param n `0 <= n`
constexpr bool is_prime_constexpr(int n) {
    if (n <= 1) return false;
    if (n == 2 || n == 7 || n == 61) return true;
    if (n % 2 == 0) return false;
    long long d = n - 1;
    while (d % 2 == 0) d /= 2;
    constexpr long long bases[3] = {2, 7, 61};
    for (long long a : bases) {
        long long t = d;
        long long y = pow_mod_constexpr(a, t, n);
        while (t != n - 1 && y != 1 && y != n - 1) {
            y = y * y % n;
            t <<= 1;
        }
        if (y != n - 1 && t % 2 == 0) {
            return false;
        }
    }
    return true;
}
template <int n> constexpr bool is_prime = is_prime_constexpr(n);

// @param b `1 <= b`
// @return pair(g, x) s.t. g = gcd(a, b), xa = g (mod b), 0 <= x < b/g
constexpr std::pair<long long, long long> inv_gcd(long long a, long long b) {
    a = safe_mod(a, b);
    if (a == 0) return {b, 0};

    // Contracts:
    // [1] s - m0 * a = 0 (mod b)
    // [2] t - m1 * a = 0 (mod b)
    // [3] s * |m1| + t * |m0| <= b
    long long s = b, t = a;
    long long m0 = 0, m1 = 1;

    while (t) {
        long long u = s / t;
        s -= t * u;
        m0 -= m1 * u;  // |m1 * u| <= |m1| * s <= b

        // [3]:
        // (s - t * u) * |m1| + t * |m0 - m1 * u|
        // <= s * |m1| - t * u * |m1| + t * (|m0| + |m1| * u)
        // = s * |m1| + t * |m0| <= b

        auto tmp = s;
        s = t;
        t = tmp;
        tmp = m0;
        m0 = m1;
        m1 = tmp;
    }
    // by [3]: |m0| <= b/g
    // by g != b: |m0| < b/g
    if (m0 < 0) m0 += b / s;
    return {s, m0};
}

// Compile time primitive root
// @param m must be prime
// @return primitive root (and minimum in now)
constexpr int primitive_root_constexpr(int m) {
    if (m == 2) return 1;
    if (m == 167772161) return 3;
    if (m == 469762049) return 3;
    if (m == 754974721) return 11;
    if (m == 998244353) return 3;
    int divs[20] = {};
    divs[0] = 2;
    int cnt = 1;
    int x = (m - 1) / 2;
    while (x % 2 == 0) x /= 2;
    for (int i = 3; (long long)(i)*i <= x; i += 2) {
        if (x % i == 0) {
            divs[cnt++] = i;
            while (x % i == 0) {
                x /= i;
            }
        }
    }
    if (x > 1) {
        divs[cnt++] = x;
    }
    for (int g = 2;; g++) {
        bool ok = true;
        for (int i = 0; i < cnt; i++) {
            if (pow_mod_constexpr(g, (m - 1) / divs[i], m) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return g;
    }
}
template <int m> constexpr int primitive_root = primitive_root_constexpr(m);

// @param n `n < 2^32`
// @param m `1 <= m < 2^32`
// @return sum_{i=0}^{n-1} floor((ai + b) / m) (mod 2^64)
unsigned long long floor_sum_unsigned(unsigned long long n,
                                      unsigned long long m,
                                      unsigned long long a,
                                      unsigned long long b) {
    unsigned long long ans = 0;
    while (true) {
        if (a >= m) {
            ans += n * (n - 1) / 2 * (a / m);
            a %= m;
        }
        if (b >= m) {
            ans += n * (b / m);
            b %= m;
        }

        unsigned long long y_max = a * n + b;
        if (y_max < m) break;
        // y_max < m * (n + 1)
        // floor(y_max / m) <= n
        n = (unsigned long long)(y_max / m);
        b = (unsigned long long)(y_max % m);
        std::swap(m, a);
    }
    return ans;
}

}  // namespace internal

}  // namespace atcoder

#endif  // ATCODER_INTERNAL_MATH_HPP

#ifndef ATCODER_INTERNAL_TYPE_TRAITS_HPP
#define ATCODER_INTERNAL_TYPE_TRAITS_HPP 1

#include <cassert>
#include <numeric>
#include <type_traits>

namespace atcoder {

namespace internal {

#ifndef _MSC_VER
template <class T>
using is_signed_int128 =
    typename std::conditional<std::is_same<T, __int128_t>::value ||
                                  std::is_same<T, __int128>::value,
                              std::true_type,
                              std::false_type>::type;

template <class T>
using is_unsigned_int128 =
    typename std::conditional<std::is_same<T, __uint128_t>::value ||
                                  std::is_same<T, unsigned __int128>::value,
                              std::true_type,
                              std::false_type>::type;

template <class T>
using make_unsigned_int128 =
    typename std::conditional<std::is_same<T, __int128_t>::value,
                              __uint128_t,
                              unsigned __int128>;

template <class T>
using is_integral = typename std::conditional<std::is_integral<T>::value ||
                                                  is_signed_int128<T>::value ||
                                                  is_unsigned_int128<T>::value,
                                              std::true_type,
                                              std::false_type>::type;

template <class T>
using is_signed_int = typename std::conditional<(is_integral<T>::value &&
                                                 std::is_signed<T>::value) ||
                                                    is_signed_int128<T>::value,
                                                std::true_type,
                                                std::false_type>::type;

template <class T>
using is_unsigned_int =
    typename std::conditional<(is_integral<T>::value &&
                               std::is_unsigned<T>::value) ||
                                  is_unsigned_int128<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <class T>
using to_unsigned = typename std::conditional<
    is_signed_int128<T>::value,
    make_unsigned_int128<T>,
    typename std::conditional<std::is_signed<T>::value,
                              std::make_unsigned<T>,
                              std::common_type<T>>::type>::type;

#else

template <class T> using is_integral = typename std::is_integral<T>;

template <class T>
using is_signed_int =
    typename std::conditional<is_integral<T>::value && std::is_signed<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <class T>
using is_unsigned_int =
    typename std::conditional<is_integral<T>::value &&
                                  std::is_unsigned<T>::value,
                              std::true_type,
                              std::false_type>::type;

template <class T>
using to_unsigned = typename std::conditional<is_signed_int<T>::value,
                                              std::make_unsigned<T>,
                                              std::common_type<T>>::type;

#endif

template <class T>
using is_signed_int_t = std::enable_if_t<is_signed_int<T>::value>;

template <class T>
using is_unsigned_int_t = std::enable_if_t<is_unsigned_int<T>::value>;

template <class T> using to_unsigned_t = typename to_unsigned<T>::type;

}  // namespace internal

}  // namespace atcoder

#endif  // ATCODER_INTERNAL_TYPE_TRAITS_HPP


namespace atcoder {

namespace internal {

struct modint_base {};
struct static_modint_base : modint_base {};

template <class T> using is_modint = std::is_base_of<modint_base, T>;
template <class T> using is_modint_t = std::enable_if_t<is_modint<T>::value>;

}  // namespace internal

template <int m, std::enable_if_t<(1 <= m)>* = nullptr>
struct static_modint : internal::static_modint_base {
    using mint = static_modint;

  public:
    static constexpr int mod() { return m; }
    static mint raw(int v) {
        mint x;
        x._v = v;
        return x;
    }

    static_modint() : _v(0) {}
    template <class T, internal::is_signed_int_t<T>* = nullptr>
    static_modint(T v) {
        long long x = (long long)(v % (long long)(umod()));
        if (x < 0) x += umod();
        _v = (unsigned int)(x);
    }
    template <class T, internal::is_unsigned_int_t<T>* = nullptr>
    static_modint(T v) {
        _v = (unsigned int)(v % umod());
    }

    unsigned int val() const { return _v; }

    mint& operator++() {
        _v++;
        if (_v == umod()) _v = 0;
        return *this;
    }
    mint& operator--() {
        if (_v == 0) _v = umod();
        _v--;
        return *this;
    }
    mint operator++(int) {
        mint result = *this;
        ++*this;
        return result;
    }
    mint operator--(int) {
        mint result = *this;
        --*this;
        return result;
    }

    mint& operator+=(const mint& rhs) {
        _v += rhs._v;
        if (_v >= umod()) _v -= umod();
        return *this;
    }
    mint& operator-=(const mint& rhs) {
        _v -= rhs._v;
        if (_v >= umod()) _v += umod();
        return *this;
    }
    mint& operator*=(const mint& rhs) {
        unsigned long long z = _v;
        z *= rhs._v;
        _v = (unsigned int)(z % umod());
        return *this;
    }
    mint& operator/=(const mint& rhs) { return *this = *this * rhs.inv(); }

    mint operator+() const { return *this; }
    mint operator-() const { return mint() - *this; }

    mint pow(long long n) const {
        assert(0 <= n);
        mint x = *this, r = 1;
        while (n) {
            if (n & 1) r *= x;
            x *= x;
            n >>= 1;
        }
        return r;
    }
    mint inv() const {
        if (prime) {
            assert(_v);
            return pow(umod() - 2);
        } else {
            auto eg = internal::inv_gcd(_v, m);
            assert(eg.first == 1);
            return eg.second;
        }
    }

    friend mint operator+(const mint& lhs, const mint& rhs) {
        return mint(lhs) += rhs;
    }
    friend mint operator-(const mint& lhs, const mint& rhs) {
        return mint(lhs) -= rhs;
    }
    friend mint operator*(const mint& lhs, const mint& rhs) {
        return mint(lhs) *= rhs;
    }
    friend mint operator/(const mint& lhs, const mint& rhs) {
        return mint(lhs) /= rhs;
    }
    friend bool operator==(const mint& lhs, const mint& rhs) {
        return lhs._v == rhs._v;
    }
    friend bool operator!=(const mint& lhs, const mint& rhs) {
        return lhs._v != rhs._v;
    }

  private:
    unsigned int _v;
    static constexpr unsigned int umod() { return m; }
    static constexpr bool prime = internal::is_prime<m>;
};

template <int id> struct dynamic_modint : internal::modint_base {
    using mint = dynamic_modint;

  public:
    static int mod() { return (int)(bt.umod()); }
    static void set_mod(int m) {
        assert(1 <= m);
        bt = internal::barrett(m);
    }
    static mint raw(int v) {
        mint x;
        x._v = v;
        return x;
    }

    dynamic_modint() : _v(0) {}
    template <class T, internal::is_signed_int_t<T>* = nullptr>
    dynamic_modint(T v) {
        long long x = (long long)(v % (long long)(mod()));
        if (x < 0) x += mod();
        _v = (unsigned int)(x);
    }
    template <class T, internal::is_unsigned_int_t<T>* = nullptr>
    dynamic_modint(T v) {
        _v = (unsigned int)(v % mod());
    }

    unsigned int val() const { return _v; }

    mint& operator++() {
        _v++;
        if (_v == umod()) _v = 0;
        return *this;
    }
    mint& operator--() {
        if (_v == 0) _v = umod();
        _v--;
        return *this;
    }
    mint operator++(int) {
        mint result = *this;
        ++*this;
        return result;
    }
    mint operator--(int) {
        mint result = *this;
        --*this;
        return result;
    }

    mint& operator+=(const mint& rhs) {
        _v += rhs._v;
        if (_v >= umod()) _v -= umod();
        return *this;
    }
    mint& operator-=(const mint& rhs) {
        _v += mod() - rhs._v;
        if (_v >= umod()) _v -= umod();
        return *this;
    }
    mint& operator*=(const mint& rhs) {
        _v = bt.mul(_v, rhs._v);
        return *this;
    }
    mint& operator/=(const mint& rhs) { return *this = *this * rhs.inv(); }

    mint operator+() const { return *this; }
    mint operator-() const { return mint() - *this; }

    mint pow(long long n) const {
        assert(0 <= n);
        mint x = *this, r = 1;
        while (n) {
            if (n & 1) r *= x;
            x *= x;
            n >>= 1;
        }
        return r;
    }
    mint inv() const {
        auto eg = internal::inv_gcd(_v, mod());
        assert(eg.first == 1);
        return eg.second;
    }

    friend mint operator+(const mint& lhs, const mint& rhs) {
        return mint(lhs) += rhs;
    }
    friend mint operator-(const mint& lhs, const mint& rhs) {
        return mint(lhs) -= rhs;
    }
    friend mint operator*(const mint& lhs, const mint& rhs) {
        return mint(lhs) *= rhs;
    }
    friend mint operator/(const mint& lhs, const mint& rhs) {
        return mint(lhs) /= rhs;
    }
    friend bool operator==(const mint& lhs, const mint& rhs) {
        return lhs._v == rhs._v;
    }
    friend bool operator!=(const mint& lhs, const mint& rhs) {
        return lhs._v != rhs._v;
    }

  private:
    unsigned int _v;
    static internal::barrett bt;
    static unsigned int umod() { return bt.umod(); }
};
template <int id> internal::barrett dynamic_modint<id>::bt(998244353);

using modint998244353 = static_modint<998244353>;
using modint1000000007 = static_modint<1000000007>;
using modint = dynamic_modint<-1>;

namespace internal {

template <class T>
using is_static_modint = std::is_base_of<internal::static_modint_base, T>;

template <class T>
using is_static_modint_t = std::enable_if_t<is_static_modint<T>::value>;

template <class> struct is_dynamic_modint : public std::false_type {};
template <int id>
struct is_dynamic_modint<dynamic_modint<id>> : public std::true_type {};

template <class T>
using is_dynamic_modint_t = std::enable_if_t<is_dynamic_modint<T>::value>;

}  // namespace internal

}  // namespace atcoder

#endif  // ATCODER_MODINT_HPP

using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

template<typename PT, typename M, u32 K = tuple_size<PT>::value>
struct LazyKDTree
{

#define KDT_MIS make_index_sequence<K>{}
#define KDT_ARGS const PT& a, const PT& b, index_sequence<Is...>

    template <size_t... Is> 
    PT min_pt(KDT_ARGS) {return { min(std::get<Is>(a), std::get<Is>(b))... };}
    template <size_t... Is> 
    PT max_pt(KDT_ARGS) {return { max(std::get<Is>(a), std::get<Is>(b))... };}

    template <size_t... Is> 
    bool cle_pt_impl(KDT_ARGS) 
        { return ((std::get<Is>(a) <= std::get<Is>(b)) && ...); }
    bool cle_pt(const PT& a, const PT& b){ return cle_pt_impl(a, b, KDT_MIS); }

    using MS = typename M::MS;
    using MA = typename M::MA;
    using S = typename MS::S;
    using A = typename MA::S;

    struct Node
    {
        // Vector sp -> ep describes the hypercube.
        PT sp, ep;
        S val;
        Node(const PT &_sp, const PT &_tp, const S &s = M::un()): 
            sp(_sp), ep(_tp), val(s) { }
        Node() { }
    };

    u32 n;
    vec<A> t;
    vec<Node> d;
    vec<u32> rnk;

    LazyKDTree(): n() { }
    LazyKDTree(u32 _n, const vec<PT> &p, const vec<S> &s)
        : n(_n), t(), d(), rnk(n)
    {
        u32 m = 1;
        while (m < n) m += m;
        t.resize(m, MA::un());
        d.resize(m * 2);
        vec<u32> idx(n);
        for (u32 i = 0; i < n; i++) idx[i] = i;
        build_rec<0>(p, s, idx, 0, n, 1);
    }

    void apply_at(u32 x, const A& a, u32 len)
    {
        d[x].val = M::act(d[x].val, a, len);
        if (len > 1) t[x] = MA::op(t[x], a);
    }
    void push_down(u32 x, u32 l, u32 r)
    {
        if (t[x] == MA::un()) return ;
        apply_at(x * 2, t[x], l);
        apply_at(x * 2 + 1, t[x], r);
        t[x] = MA::un();
    }

    // Vector st -> ed describes a hypercube.
    S prod(const PT &st, const PT &ed) { return prod_rec(st, ed, 0, n, 1); }
    S get(u32 x) { return get_rec(x, 0, n, 1); }
    void set(u32 x, const S &v) { set_rec(rnk[x], v, 0, n, 1); }
    void apply(const PT &st, const PT &ed, const A &a) 
    { 
        apply_rec(st, ed, a, 0, n, 1);
    }
    template<typename RIt>
    void print_val(RIt o) 
    {
        vec<u32> idx(n);
        for (u32 i = 0; i < n; i++) idx[rnk[i]] = i;
        print_rec(idx, 0, n, 1, o);
    }

    template<u32 D>
    void build_rec(const vec<PT> &p, const vec<S> &s, 
        vec<u32> &idx, u32 l, u32 r, u32 x)
    {
        if (l + 1 == r) {
            u32 idn = idx[l];
            d[x] = {p[idn], p[idn], s[idn]};
            rnk[idn] = l;
            return ;
        }
        u32 mid = (l + r) >> 1;
        nth_element(idx.data() + l, idx.data() + mid, idx.data() + r, 
            [&p](u32 a, u32 b) 
            { return std::get<D>(p[a]) < std::get<D>(p[b]); });
        build_rec<(D + 1) % K>(p, s, idx, l, mid, x * 2);
        build_rec<(D + 1) % K>(p, s, idx, mid, r, x * 2 + 1);
        d[x].sp = min_pt(d[x * 2].sp, d[x * 2 + 1].sp, KDT_MIS);
        d[x].ep = max_pt(d[x * 2].ep, d[x * 2 + 1].ep, KDT_MIS);
        d[x].val = MS::op(d[x * 2].val, d[x * 2 + 1].val);
    }

private:

    S prod_rec(const PT &st, const PT &ed, u32 l, u32 r, u32 x)
    {
        if (cle_pt(st, d[x].sp) && cle_pt(d[x].ep, ed)) return d[x].val;
        if (!cle_pt(d[x].sp, ed) || !cle_pt(st, d[x].ep)) return MS::un();
        u32 mid = (l + r) >> 1;
        push_down(x, mid - l, r - mid);
        return MS::op(prod_rec(st, ed, l, mid, x * 2), 
                     prod_rec(st, ed, mid, r, x * 2 + 1));
    }

    void apply_rec(const PT &st, const PT &ed, const A &a, u32 l, u32 r, u32 x)
    {
        if (cle_pt(st, d[x].sp) && cle_pt(d[x].ep, ed)) {
            apply_at(x, a, r - l);
            return ;
        }
        if (!cle_pt(d[x].sp, ed) || !cle_pt(st, d[x].ep)) return ;
        u32 mid = (l + r) >> 1;
        push_down(x, mid - l, r - mid);
        apply_rec(st, ed, a, l, mid, x * 2);
        apply_rec(st, ed, a, mid, r, x * 2 + 1);
        d[x].val = MS::op(d[x * 2].val, d[x * 2 + 1].val);
    }

    void set_rec(u32 p, const S &v, u32 l, u32 r, u32 x)
    {
        if (l + 1 == r) {
            d[x].val = v;
            return ;
        }
        u32 mid = (l + r) >> 1;
        push_down(x, mid - l, r - mid);
        if (p < mid) set_rec(p, v, l, mid, x * 2);
        else set_rec(p, v, mid, r, x * 2 + 1);
        d[x].val = MS::op(d[x * 2].val, d[x * 2 + 1].val);
    }

    S get_rec(u32 p, u32 l, u32 r, u32 x)
    {
        if (l + 1 == r) return d[x].val;
        u32 mid = (l + r) >> 1;
        push_down(x, mid - l, r - mid);
        if (p < mid) return get_rec(p, l, mid, x * 2);
        return get_rec(p, mid, r, x * 2 + 1);
    }

    template<typename RIt>
    void print_rec(const vec<u32> &idx, u32 l, u32 r, u32 x, RIt o)
    {
        if (l + 1 == r) {
            o[idx[l]] = d[x].val;
            return ;
        }
        u32 mid = (l + r) >> 1;
        push_down(x, mid - l, r - mid);
        print_rec(idx, l, mid, x * 2, o);
        print_rec(idx, mid, r, x * 2 + 1, o);
    }

#undef KDT_MIS
#undef KDT_ARGS
};

using mint = atcoder::modint998244353;

struct MonoidSum
{
    using S = pair<mint, u32>;
    static S op(S a, S b) { return {a.first + b.first, a.second + b.second}; }
    static S un() { return S(); }
};
struct MonoidAffine
{
    using S = pair<mint, mint>;
    static S op(const S &a, const S &b)
    {
        return {a.first * b.first, a.second * b.first + b.second};
    }
    static S un() { return {1, 0}; }
};
struct ActedMonoid
{
    using MS = MonoidSum;
    using MA = MonoidAffine;
    static pair<mint, u32> 
        act(const pair<mint, u32> v, const pair<mint, mint> &a, u32)
    {
        return {v.first * a.first + v.second * a.second, v.second};
    }
};

struct Query
{
    u32 op, a, b, c, d, e, f;
};

signed main() 
{
    // ios::sync_with_stdio(false);
    // cin.tie(0), cout.tie(0);
    using Point = pair<u32, u32>;
    u32 n, q;
    // cin >> n >> q;
	scanf("%u %u", &n, &q);
    vec<Point> pt;
    vec<pair<mint, u32>> v;
    vec<Query> qr(q);
    for (u32 i = 0, x, y, z; i < n; i++) {
        // cin >> x >> y >> z;
		scanf("%u %u %u", &x, &y, &z);
        pt.emplace_back(x, y);
        v.emplace_back(z, 1);
    }
    for (auto &[op, a, b, c, d, e, f]: qr) {
        // cin >> op;
		scanf("%u", &op);
        if (op == 0) {
            // cin >> a >> b >> c;
			scanf("%u %u %u", &a, &b, &c);
            pt.emplace_back(a, b);
            v.emplace_back(0, 0);
            a = pt.size() - 1, b = c;
        }
        else if (op == 1) {
			// cin >> a >> b;
			scanf("%u %u", &a, &b);
		}
        else if (op == 2) {
			// cin >> a >> b >> c >> d;
			scanf("%u %u %u %u", &a, &b, &c, &d);
		}
        else {
			// cin >> a >> b >> c >> d >> e >> f;
			scanf("%u %u %u %u %u %u", &a, &b, &c, &d, &e, &f);
		}
    }
    LazyKDTree<Point, ActedMonoid> t(pt.size(), pt, v);
    for (auto [op, a, b, c, d, e, f]: qr) {
        if (op <= 1) t.set(a, {b, 1});
        else if (op == 2) {
            // cout << t.prod({a, b}, {c - 1, d - 1}).first.val() << '\n';
			printf("%u\n", t.prod({a, b}, {c - 1, d - 1}).first.val());
        }
        else t.apply({a, b}, {c - 1, d - 1}, {e, f});
    }
    return 0;
}
