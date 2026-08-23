
#include <bits/stdc++.h>
#if defined(__x86_64__) && defined(__GNUC__)
#pragma GCC push_options
#pragma GCC target("pclmul")
#endif
using namespace std;
#define int long long

unsigned __int128 carrylessMul(uint64_t a, uint64_t b)
{
    // a、b 是 64 位二进制多项式系数；返回不进位乘积的 128 位系数。
#if defined(__x86_64__) && defined(__GNUC__)
    typedef long long Vec128 __attribute__((vector_size(16)));
    Vec128 z = __builtin_ia32_pclmulqdq128(Vec128{(long long)a, 0},
                                           Vec128{(long long)b, 0}, 0);
    unsigned __int128 r;
    memcpy(&r, &z, sizeof(r));
    return r;
#else
    unsigned __int128 r = 0;
    while (a)
    {
        int k = countr_zero(a);
        r ^= (unsigned __int128)b << k;
        a &= a - 1;
    }
    return r;
#endif
}

constexpr unsigned __int128 carrylessMulConst(uint64_t a, uint64_t b)
{
    // a、b 是编译期 64 位二进制多项式系数；返回不进位乘积。
    unsigned __int128 r = 0;
    for (int i = 0; i < 64; i++)
    {
        if (a >> i & 1)
        {
            r ^= (unsigned __int128)b << i;
        }
    }
    return r;
}

struct GF64
{
    static constexpr unsigned __int128 poly = ((unsigned __int128)1 << 64) | 0b11011; // 定义域的 64 次不可约多项式。
    static constexpr uint64_t inv = [] // Montgomery 式约减使用的低半逆元。
    {
        uint64_t a = 1;
        for (int i = 0; i < 6; i++)
        {
            a = carrylessMulConst(a, (uint64_t)carrylessMulConst(a, (uint64_t)poly));
        }
        return a;
    }();
    static constexpr uint64_t r2 = [] // 普通表示转内部表示时乘入的 x^128 mod poly。
    {
        unsigned __int128 r = 1;
        for (int i = 0; i < 128; i++)
        {
            r <<= 1;
            if (r >> 64 & 1)
            {
                r ^= poly;
            }
        }
        return (uint64_t)r;
    }();

    uint64_t x = 0; // 当前域元素的内部 Montgomery 式表示。

    GF64() = default;
    explicit GF64(uint64_t v) : x(reduce(carrylessMul(v, r2)))
    {
        // v 是普通 64 位域元素表示；构造对应内部表示。
    }

    static GF64 bit(int x)
    {
        // x 是整数；返回其最低位对应的域元素 0 或 1。
        return GF64(x & 1);
    }

    static uint64_t reduce(unsigned __int128 x)
    {
        // x 是 128 位不进位乘积；返回除以 poly 后的内部表示余数。
        uint64_t f = carrylessMul((uint64_t)x, inv);
        return (uint64_t)(x >> 64) ^ (uint64_t)(carrylessMul(f, (uint64_t)poly) >> 64) ^ f;
    }

    GF64 &operator+=(GF64 o)
    {
        // o 是同域元素；原地相加并返回当前对象。
        x ^= o.x;
        return *this;
    }

    GF64 &operator-=(GF64 o)
    {
        // o 是同域元素；特征 2 下原地相减并返回当前对象。
        return *this += o;
    }

    GF64 &operator*=(GF64 o)
    {
        // o 是同域元素；原地相乘并返回当前对象。
        x = reduce(carrylessMul(x, o.x));
        return *this;
    }

    friend GF64 operator+(GF64 a, GF64 b)
    {
        // a、b 是同域元素；返回 a+b。
        return a += b;
    }

    friend GF64 operator-(GF64 a, GF64 b)
    {
        // a、b 是同域元素；返回 a-b。
        return a -= b;
    }

    friend GF64 operator*(GF64 a, GF64 b)
    {
        // a、b 是同域元素；返回 a*b。
        return a *= b;
    }

    GF64 pow(uint64_t n) const
    {
        // n 是非负指数；返回当前域元素的 n 次幂。
        GF64 a = *this, r = bit(1);
        while (n)
        {
            if (n & 1)
            {
                r *= a;
            }
            a *= a;
            n >>= 1;
        }
        return r;
    }

    GF64 invField() const
    {
        // 当前值非零；返回乘法逆元。
        assert(x != 0); // 调试检查，可删。
        return pow(~uint64_t(0) - 1);
    }

    uint64_t val() const
    {
        // 无参数；返回普通 64 位域元素表示。
        return reduce(x);
    }

    friend bool operator==(GF64 a, GF64 b)
    {
        // a、b 是同域元素；返回二者是否相等。
        return a.x == b.x;
    }
};

vector<GF64> gf64SubsetSums(const vector<GF64> &b)
{
    // b 是一组域元素；返回按二进制子集编号排列的元素和。
    vector<GF64> a(1LL << b.size());
    for (int k = 0; k < (int)b.size(); k++)
    {
        for (int s = 0; s < (1LL << k); s++)
        {
            a[(1LL << k) + s] = a[s] + b[k];
        }
    }
    return a;
}

struct GF64FFT
{
    struct Data
    {
        vector<GF64> basis; // 当前层的 Cantor 基。
        vector<GF64> next; // 下降一层后的 Cantor 基。
        vector<GF64> sums; // 归一化基的全部子集和。
        vector<GF64> aux; // 当前层递归拆分复用的工作区。

        void init()
        {
            // basis 已非空；预处理当前层变换数据，无返回值。
            int lg = basis.size();
            aux.resize(1LL << lg);
            GF64 b = basis.back(), ib = b.invField();
            next.resize(lg - 1);
            vector<GF64> g(lg - 1);
            for (int i = 0; i + 1 < lg; i++)
            {
                g[i] = basis[i] * ib;
                next[i] = g[i] * g[i] + g[i];
            }
            sums = gf64SubsetSums(g);
        }
    };

    vector<Data> d; // d[k] 保存长度 2^k 变换所需的数据。

    void prepare(int lg)
    {
        // lg 是所需最大长度的二进制指数；缺少对应层时建立一组 Cantor 基。
        if ((int)d.size() > lg)
        {
            return;
        }
        mt19937_64 rng;
        vector<GF64> basis;
        while ((int)basis.size() < lg)
        {
            basis.clear();
            for (GF64 x(rng()); !(x == GF64()); x = x * x + x)
            {
                basis.push_back(x);
            }
        }
        basis.erase(basis.begin(), basis.end() - lg);
        d.resize(lg + 1);
        d[lg].basis = basis;
        for (int k = lg; k > 0; k--)
        {
            d[k].init();
            d[k - 1].basis = d[k].next;
        }
    }

    template <bool inverse = false>
    void taylor(span<GF64> f)
    {
        // f 是长度为 2 的幂的连续切片；完成正向或逆向 Taylor 基变换，无返回值。
        for (int n = inverse ? 1 : f.size() / 4;
             inverse ? n * 4 <= (int)f.size() : n >= 1;
             inverse ? n *= 2 : n /= 2)
        {
            for (int s = 0; s < (int)f.size(); s += 4 * n)
            {
                for (int i = 0; i < n; i++)
                {
                    GF64 b = f[s + n + i], c = f[s + 2 * n + i], z = f[s + 3 * n + i];
                    f[s + n + i] = inverse ? b + c : b + c + z;
                    f[s + 2 * n + i] = c + z;
                }
            }
        }
    }

    template <bool inverse = false>
    void transform(span<GF64> f)
    {
        // f 是已由 prepare 覆盖的二次幂长度切片；原地完成加法 FFT 或逆变换。
        int n = f.size();
        if (n == 1)
        {
            return;
        }
        int lg = bit_width((unsigned long long)n) - 1;
        auto &cur = d[lg];
        if (n == 2)
        {
            f[1] = f[0] + f[1];
            return;
        }
        span<GF64> u(cur.aux.data(), n / 2);
        span<GF64> v(cur.aux.data() + n / 2, n / 2);
        if constexpr (!inverse)
        {
            taylor(f);
            for (int i = 0; i < n / 2; i++)
            {
                u[i] = f[2 * i];
                v[i] = f[2 * i + 1];
            }
            transform(u);
            transform(v);
            for (int i = 0; i < n / 2; i++)
            {
                GF64 a = u[i] + cur.sums[i] * v[i];
                f[i] = a;
                f[i + n / 2] = a + v[i];
            }
        }
        else
        {
            for (int i = 0; i < n / 2; i++)
            {
                v[i] = f[i] + f[n / 2 + i];
                u[i] = f[i] + cur.sums[i] * v[i];
            }
            transform<true>(u);
            transform<true>(v);
            for (int i = 0; i < n / 2; i++)
            {
                f[2 * i] = u[i];
                f[2 * i + 1] = v[i];
            }
            taylor<true>(f);
        }
    }
};

inline GF64FFT gf64FFT; // 全局复用的 GF(2^64) 加法 FFT 预处理。

vector<uint64_t> convGF64(const vector<uint64_t> &x, const vector<uint64_t> &y)
{
    // x、y 是 GF(2^64) 系数的普通表示；返回线性卷积，任一输入为空时返回空。
    if (x.empty() || y.empty())
    {
        return {};
    }
    int need = x.size() + y.size() - 1;
    int lg = bit_width((unsigned long long)(need - 1));
    int n = 1LL << lg;
    if ((long long)x.size() * (long long)y.size() <=
        (long long)n * (lg + 1) * (lg + 1))
    {
        vector<GF64> c(need);
        vector<uint64_t> ans(need);
        for (int i = 0; i < (int)x.size(); i++)
        {
            for (int j = 0; j < (int)y.size(); j++)
            {
                c[i + j] += GF64(x[i]) * GF64(y[j]);
            }
        }
        for (int i = 0; i < need; i++)
        {
            ans[i] = c[i].val();
        }
        return ans;
    }
    if (lg > 3 && need == (1LL << (lg - 1)) + 1)
    {
        vector<uint64_t> prefix(x.begin(), x.end() - 1);
        vector<uint64_t> ans = convGF64(prefix, y);
        ans.resize(need);
        GF64 last(x.back());
        for (int j = 0; j < (int)y.size(); j++)
        {
            ans[x.size() - 1 + j] ^= (last * GF64(y[j])).val();
        }
        return ans;
    }
    vector<GF64> a(n), b(n);
    for (int i = 0; i < (int)x.size(); i++)
    {
        a[i] = GF64(x[i]);
    }
    for (int i = 0; i < (int)y.size(); i++)
    {
        b[i] = GF64(y[i]);
    }
    gf64FFT.prepare(lg);
    gf64FFT.transform(span<GF64>(a));
    gf64FFT.transform(span<GF64>(b));
    for (int i = 0; i < n; i++)
    {
        a[i] *= b[i];
    }
    gf64FFT.transform<true>(span<GF64>(a));
    vector<uint64_t> ans(need);
    for (int i = 0; i < need; i++)
    {
        ans[i] = a[i].val();
    }
    return ans;
}

#if defined(__x86_64__) && defined(__GNUC__)
#pragma GCC pop_options
#endif

signed main(){ ios::sync_with_stdio(false);cin.tie(nullptr);int n,m;cin>>n>>m;vector<uint64_t>a(n),b(m);for(auto&x:a)cin>>x;for(auto&x:b)cin>>x;auto c=convGF64(a,b);for(int i=0;i<(int)c.size();i++)cout<<c[i]<<" \n"[i+1==(int)c.size()]; }
