



#include <bits/stdc++.h>

using namespace std;

#define int long long

constexpr long long INF = numeric_limits<long long>::max() / 4;
constexpr long double EPS = 1E-12L;

template <class T> bool chmin(T &x, const T &y)
{
    // y<x 时把 x 降到 y 并返回 true
    if (y < x)
    {
        x = y;
        return true;
    }
    return false;
}

template <class T> bool chmax(T &x, const T &y)
{
    // x<y 时把 x 升到 y 并返回 true
    if (x < y)
    {
        x = y;
        return true;
    }
    return false;
}

int sgn(long double x, long double eps = EPS)
{
    // 返回 x 按绝对容差 eps 与零比较的符号
    return (x > eps) - (x < -eps);
}

struct FastInput
{
    static constexpr int S = 1 << 16; // 缓冲区容量
    array<char, S> buf{};             // 输入字节
    FILE *src = stdin;                // 当前读取的 C 输入流
    int p = 0;                        // 下一个未读位置
    int n = 0;                        // 当前有效字节数

    // 参数 src 为读取来源，默认从标准输入读取。
    FastInput(FILE *src = stdin) : src(src) {}

    char get()
    {
        // 返回下一个输入字节，输入结束时返回 0
        if (p == n)
        {
            n = fread(buf.data(), 1, S, src);
            p = 0;
            if (!n)
            {
                return 0;
            }
        }
        return buf[p++];
    }

    template <class T> bool read(T &x)
    {
        // 读一个 T 范围内的整数，成功返回 true
        static_assert(is_integral_v<T> && !is_same_v<remove_cv_t<T>, bool>);
        using U = make_unsigned_t<T>;
        auto c = get();
        while (c && c <= ' ')
        {
            c = get();
        }
        if (!c)
        {
            return false;
        }
        bool neg = c == '-';
        if (neg)
        {
            c = get();
        }
        if (c < '0' || c > '9')
        {
            return false;
        }
        U lim = numeric_limits<U>::max();
        if constexpr (is_signed_v<T>)
        {
            lim = (U)numeric_limits<T>::max() + (neg ? 1 : 0);
        }
        bool fit = true;
        U v = 0;
        while ('0' <= c && c <= '9')
        {
            U d = (U)(c - '0');
            if (fit && v > (lim - d) / 10)
            {
                fit = false;
            }
            if (fit)
            {
                v = v * 10 + d;
            }
            c = get();
        }
        if (!fit)
        {
            return false;
        }
        if constexpr (is_unsigned_v<T>)
        {
            if (neg)
            {
                return false;
            }
            x = (T)v;
        }
        else if (neg)
        {
            U absMin = (U)numeric_limits<T>::max() + 1;
            if (v == absMin)
            {
                x = numeric_limits<T>::min();
            }
            else
            {
                x = -((T)v);
            }
        }
        else
        {
            x = (T)v;
        }
        return true;
    }

    bool read(string &s)
    {
        // 读下一个非空白字符串，成功返回 true
        auto c = get();
        while (c && c <= ' ')
        {
            c = get();
        }
        if (!c)
        {
            return false;
        }
        s.clear();
        while (c > ' ')
        {
            s.push_back(c);
            c = get();
        }
        return true;
    }
};

string toString(__int128 x)
{
    // 返回 x 的十进制字符串
    if (!x)
    {
        return "0";
    }
    auto neg = x < 0;
    auto y = neg ? 0 - (unsigned __int128)x : (unsigned __int128)x;
    string s;
    while (y)
    {
        s.push_back(char('0' + y % 10));
        y /= 10;
    }
    if (neg)
    {
        s.push_back('-');
    }
    reverse(s.begin(), s.end());
    return s;
}

optional<__int128> parseInt128(const string &s)
{
    // 严格解析十进制串，非法或越界时返回空
    if (s.empty())
    {
        return nullopt;
    }
    bool neg = s[0] == '-';
    int p = neg || s[0] == '+';
    if (p == (int)s.size())
    {
        return nullopt;
    }
    unsigned __int128 lim = (unsigned __int128)1 << 127;
    if (!neg)
    {
        lim--;
    }
    unsigned __int128 x = 0;
    for (; p < (int)s.size(); p++)
    {
        if (s[p] < '0' || s[p] > '9')
        {
            return nullopt;
        }
        unsigned d = (unsigned)(s[p] - '0');
        if (x > (lim - d) / 10)
        {
            return nullopt;
        }
        x = x * 10 + d;
    }
    if (!neg)
    {
        return (__int128)x;
    }
    if (x == ((unsigned __int128)1 << 127))
    {
        return -(__int128)(x - 1) - 1;
    }
    return -(__int128)x;
}

struct i128
{
    unsigned long long hi = 0; // 补码的高 64 位
    unsigned long long lo = 0; // 补码的低 64 位

    i128(int64_t x = 0) : hi(x < 0 ? ~0ULL : 0), lo((unsigned long long)x)
    {
        // 把 64 位有符号整数按补码符号扩展到 128 位
    }

    explicit operator bool() const
    {
        // 返回当前 128 位整数是否非零
        return hi || lo;
    }

private:
    i128(unsigned long long h, unsigned long long l) : hi(h), lo(l)
    {
        // h、l 是已经计算好的高、低 64 位补码
    }

    static bool lessU(i128 a, i128 b)
    {
        // 把 a、b 当作无符号 128 位整数，返回 a 是否小于 b
        return a.hi != b.hi ? a.hi < b.hi : a.lo < b.lo;
    }

    static unsigned long long mulHi(unsigned long long a, unsigned long long b)
    {
        // 返回 64 位无符号乘积的高 64 位
        constexpr unsigned long long MASK = 0xffffffffULL;
        unsigned long long a0 = a & MASK;
        unsigned long long a1 = a >> 32;
        unsigned long long b0 = b & MASK;
        unsigned long long b1 = b >> 32;
        unsigned long long p = a0 * b0;
        unsigned long long x = a0 * b1;
        unsigned long long y = a1 * b0;
        unsigned long long mid = (p >> 32) + (x & MASK) + (y & MASK);
        return a1 * b1 + (x >> 32) + (y >> 32) + (mid >> 32);
    }

public:
    friend i128 operator-(i128 x)
    {
        // 返回 x 的 128 位补码相反数
        auto lo = ~x.lo + 1;
        return i128(~x.hi + (lo == 0), lo);
    }

    friend i128 operator+(i128 a, i128 b)
    {
        // 返回 a+b 的低 128 位，低位溢出时向高位进位
        auto lo = a.lo + b.lo;
        return i128(a.hi + b.hi + (lo < a.lo), lo);
    }

    friend i128 operator-(i128 a, i128 b)
    {
        // 返回 a-b 的低 128 位，低位借位计入高位
        auto lo = a.lo - b.lo;
        return i128(a.hi - b.hi - (a.lo < b.lo), lo);
    }

    friend i128 operator*(i128 a, i128 b)
    {
        // 返回 a*b 的低 128 位
        return i128(mulHi(a.lo, b.lo) + a.hi * b.lo + a.lo * b.hi,
                    a.lo * b.lo);
    }

private:
    static pair<i128, i128> divU(i128 n, i128 d)
    {
        // n、d 按无符号解释且 d 非零；返回商与余数
        i128 q;
        i128 r;
        for (int i = 127; i >= 0; i--)
        {
            auto carry = r.hi >> 63;
            auto bit = i < 64 ? n.lo >> i & 1ULL : n.hi >> (i - 64) & 1ULL;
            r = i128(r.hi << 1 | r.lo >> 63, r.lo << 1 | bit);
            if (carry || !lessU(r, d))
            {
                r = r - d;
                if (i < 64)
                {
                    q.lo |= 1ULL << i;
                }
                else
                {
                    q.hi |= 1ULL << (i - 64);
                }
            }
        }
        return {q, r};
    }

    static pair<i128, i128> divS(i128 a, i128 b)
    {
        // b 非零且商可表示；返回向零取整的商和与 a 同号的余数
        auto na = a.hi >> 63;
        auto nb = b.hi >> 63;
        auto [q, r] = divU(na ? -a : a, nb ? -b : b);
        if (na != nb)
        {
            q = -q;
        }
        if (na)
        {
            r = -r;
        }
        return {q, r};
    }

public:
    friend i128 operator/(i128 a, i128 b)
    {
        // 返回 a/b 向零取整的商
        return divS(a, b).first;
    }

    friend i128 operator%(i128 a, i128 b)
    {
        // 返回 a%b，非零余数与 a 同号
        return divS(a, b).second;
    }

    string toString() const
    {
        // 返回当前值的十进制表示
        auto neg = hi >> 63;
        i128 x = neg ? -*this : *this;
        string s;
        do
        {
            auto [q, r] = divU(x, i128(10));
            s.push_back(char('0' + r.lo));
            x = q;
        } while (x);
        if (neg)
        {
            s.push_back('-');
        }
        reverse(s.begin(), s.end());
        return s;
    }

    friend ostream &operator<<(ostream &out, const i128 &x)
    {
        // 把 x 的十进制表示写入 out，并返回 out
        return out << x.toString();
    }
};

template <class T> istream &operator>>(istream &in, vector<T> &a)
{
    // 按 a 的既有长度从 in 依次读入，返回 in
    for (auto &x : a)
    {
        in >> x;
    }
    return in;
}

template <class T> ostream &operator<<(ostream &out, const vector<T> &a)
{
    // 把 a 以空格分隔写入 out，返回 out
    for (int i = 0; i < (int)a.size(); i++)
    {
        if (i)
        {
            out << ' ';
        }
        out << a[i];
    }
    return out;
}

vector<string> split(const string &s, char sep = ' ')
{
    // 按 sep 切分 s，保留连续分隔符产生的空字段
    vector<string> ans;
    string cur;
    for (auto c : s)
    {
        if (c == sep)
        {
            ans.push_back(cur);
            cur.clear();
        }
        else
        {
            cur.push_back(c);
        }
    }
    ans.push_back(cur);
    return ans;
}

struct CustomHash
{
    static uint64_t mix(uint64_t x)
    {
        // 返回 x 经 SplitMix64 finalizer 扰动后的值
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }

    size_t operator()(uint64_t x) const
    {
        // 返回混入进程随机种子的整数哈希值
        static const uint64_t seed =
            (uint64_t)chrono::steady_clock::now().time_since_epoch().count();
        return mix(x + seed);
    }

    size_t operator()(const pair<int, int> &x) const
    {
        // 只接受两个不超过 64 位的整数分量
        static const uint64_t seed =
            (uint64_t)chrono::steady_clock::now().time_since_epoch().count();
        uint64_t a = mix((uint64_t)x.first + seed);
        uint64_t b = mix((uint64_t)x.second + seed + 0x9e3779b97f4a7c15ULL);
        return mix(a ^ rotl(b, 32));
    }
};

struct DynamicBitset
{
    int n = 0;                    // 有效位数
    vector<unsigned long long> a; // 按 64 位机器字打包

    DynamicBitset(int m = 0)
    {
        // 构造 m 位的全零集合，要求 m>=0
        init(m);
    }

    void init(int m)
    {
        // 把集合重设为 m 位并清零
        assert(m >= 0); // 调试检查，可删
        n = m;
        a.assign((n + 63) / 64, 0);
    }

    void set(int p, bool v = true)
    {
        // 第 p 位写成 v
        assert(0 <= p && p < n); // 调试检查，可删
        auto b = 1ULL << (p & 63);
        if (v)
        {
            a[p >> 6] |= b;
        }
        else
        {
            a[p >> 6] &= ~b;
        }
    }

    void reset()
    {
        // 全部有效位清零
        fill(a.begin(), a.end(), 0);
    }

    bool test(int p) const
    {
        // 返回第 p 位
        assert(0 <= p && p < n); // 调试检查，可删
        return a[p >> 6] >> (p & 63) & 1ULL;
    }

    int count() const
    {
        // 返回 1 的总数
        int ans = 0;
        for (auto x : a)
        {
            ans += popcount(x);
        }
        return ans;
    }

    DynamicBitset &operator|=(const DynamicBitset &o)
    {
        // 逐位或入 o，返回当前集合
        assert(n == o.n); // 调试检查，可删
        for (int i = 0; i < (int)a.size(); i++)
        {
            a[i] |= o.a[i];
        }
        return *this;
    }

    DynamicBitset &operator&=(const DynamicBitset &o)
    {
        // 逐位与入 o，返回当前集合
        assert(n == o.n); // 调试检查，可删
        for (int i = 0; i < (int)a.size(); i++)
        {
            a[i] &= o.a[i];
        }
        return *this;
    }
};

template <class T, class Cmp = less<T>> void mergeSort(vector<T> &a, Cmp cmp = {})
{
    // a 是待排序数组，cmp 是严格弱序比较器；排序结果直接写回 a。
    vector<T> b;
    b.reserve(a.size());
    auto dfs = [&](auto &&self, int l, int r) -> void
    {
        if (r - l <= 1)
        {
            return;
        }
        auto m = midpoint(l, r);
        self(self, l, m);
        self(self, m, r);
        auto i = l;
        auto j = m;
        b.clear();
        while (i < m || j < r)
        {
            if (j == r || (i < m && !cmp(a[j], a[i])))
            {
                b.push_back(move(a[i++]));
            }
            else
            {
                b.push_back(move(a[j++]));
            }
        }
        move(b.begin(), b.end(), a.begin() + l);
    };
    dfs(dfs, 0, a.size());
}

void radixSort(vector<int> &a)
{
    // a 是待排序的有符号 64 位整数数组；排序结果直接写回 a。
    static_assert(numeric_limits<int>::digits == 63);
    vector<unsigned long long> x(a.size()), b(a.size());
    for (int i = 0; i < (int)a.size(); i++)
    {
        x[i] = (unsigned long long)a[i] ^ (1ULL << 63);
    }
    constexpr int B = 1 << 16;
    vector<int> cnt(B);
    for (int sh = 0; sh < 64; sh += 16)
    {
        fill(cnt.begin(), cnt.end(), 0);
        for (auto v : x)
        {
            cnt[v >> sh & (B - 1)]++;
        }
        partial_sum(cnt.begin(), cnt.end(), cnt.begin());
        for (auto it = x.rbegin(); it != x.rend(); it++)
        {
            auto p = *it >> sh & (B - 1);
            b[--cnt[p]] = *it;
        }
        x.swap(b);
    }
    for (int i = 0; i < (int)a.size(); i++)
    {
        a[i] = bit_cast<int64_t>(x[i] ^ (1ULL << 63));
    }
}

template <class T> T nthValue(vector<T> a, int k)
{
    // a 是按值复制的数组，k 是从 0 开始的排名；返回第 k 小值。
    assert(0 <= k && k < (int)a.size()); // 调试检查，可删
    nth_element(a.begin(), a.begin() + k, a.end());
    return a[k];
}

template <class T, class Rng> T quickSelect(vector<T> a, int k, Rng &rng)
{
    // a 是按值复制的数组，k 是从 0 开始的排名，rng 是随机源；返回第 k 小值。
    assert(0 <= k && k < (int)a.size()); // 调试检查，可删
    int l = 0, r = a.size();
    while (r - l > 1)
    {
        auto p = a[uniform_int_distribution<int>(l, r - 1)(rng)];
        auto m1 = partition(a.begin() + l,
                            a.begin() + r,
                            [&](const T &x)
                            {
                                return x < p;
                            });
        auto m2 = partition(m1,
                            a.begin() + r,
                            [&](const T &x)
                            {
                                return !(p < x);
                            });
        auto x = m1 - a.begin();
        auto y = m2 - a.begin();
        if (k < x)
        {
            r = x;
        }
        else if (k >= y)
        {
            l = y;
        }
        else
        {
            return p;
        }
    }
    return a[l];
}

template <class T> struct Discrete
{
    vector<T> a; // a 保存升序且去重后的原值。

    Discrete() = default;

    explicit Discrete(vector<T> v) : a(move(v))
    {
        // v 收集所有可能出现的值。
        sort(a.begin(), a.end());
        a.erase(unique(a.begin(), a.end()), a.end());
    }

    int id(const T &x) const
    {
        // x 必须已存在；返回 x 从 0 开始的离散编号。
        auto it = lower_bound(a.begin(), a.end(), x);
        assert(it != a.end() && *it == x); // 调试检查，可删
        return it - a.begin();
    }

    int lower(const T &x) const
    {
        // x 是查询值；返回第一个不小于 x 的离散位置。
        return lower_bound(a.begin(), a.end(), x) - a.begin();
    }

    const T &value(int p) const
    {
        // p 是合法离散编号；返回对应原值的常量引用。
        return a[p];
    }
};

template <class T> int invMerge(vector<T> a)
{
    // a 是按值复制的序列；返回严格逆序对数量。
    vector<T> b;
    b.reserve(a.size());
    int ans = 0;
    auto dfs = [&](auto &&self, int l, int r) -> void
    {
        if (r - l <= 1)
        {
            return;
        }
        auto m = midpoint(l, r);
        self(self, l, m);
        self(self, m, r);
        auto i = l;
        auto j = m;
        b.clear();
        while (i < m || j < r)
        {
            if (j == r || (i < m && !(a[j] < a[i])))
            {
                b.push_back(move(a[i++]));
            }
            else
            {
                ans += m - i;
                b.push_back(move(a[j++]));
            }
        }
        move(b.begin(), b.end(), a.begin() + l);
    };
    dfs(dfs, 0, a.size());
    return ans;
}

template <class T> int invBit(const vector<T> &a)
{
    // a 是原序列；返回用离散化和 Fenwick 统计的严格逆序对数量。
    Discrete<T> dis(a);
    vector<int> bit(dis.a.size() + 1);
    auto add = [&](int p)
    {
        for (p++; p < (int)bit.size(); p += p & -p)
        {
            bit[p]++;
        }
    };
    auto sum = [&](int p)
    {
        int ans = 0;
        for (; p > 0; p -= p & -p)
        {
            ans += bit[p];
        }
        return ans;
    };
    int ans = 0;
    for (auto it = a.rbegin(); it != a.rend(); it++)
    {
        auto p = dis.id(*it);
        ans += sum(p);
        add(p);
    }
    return ans;
}

struct Fenwick
{
    int n = 0;     // n 是下标范围长度。
    vector<int> a; // a 保存从 1 开始的树状数组节点和。

    Fenwick(int m = 0)
    {
        // m 是初始下标范围长度。
        init(m);
    }

    void init(int m)
    {
        // m 是新的下标范围长度；调用后全部频率清零。
        assert(m >= 0); // 调试检查，可删
        n = m;
        a.assign(n + 1, 0);
    }

    void add(int p, int v)
    {
        // p 是从 0 开始的位置，v 是增量。
        assert(0 <= p && p < n); // 调试检查，可删
        for (p++; p <= n; p += p & -p)
        {
            a[p] += v;
        }
    }

    int sum(int r) const
    {
        // r 是右端点；返回半开前缀 [0,r) 的和。
        assert(0 <= r && r <= n); // 调试检查，可删
        int ans = 0;
        for (; r > 0; r -= r & -r)
        {
            ans += a[r];
        }
        return ans;
    }

    int sum(int l, int r) const
    {
        // l、r 是半开区间端点；返回 [l,r) 的和。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        return sum(r) - sum(l);
    }
};

// Point3[0..3] 依次表示 x、y、z、原下标。
using Point3 = array<int, 4>;

vector<int> dom3(vector<Point3> p)
{
    // p 保存 {x,y,z,id} 点集；返回每个原下标对应的三维非严格支配计数。
    auto n = (int)p.size();
    vector<int> seen(n);
    for (auto q : p)
    {
        assert(0 <= q[3] && q[3] < n && !seen[q[3]]); // 调试检查，可删
        seen[q[3]] = 1;
    }
    vector<int> zs;
    for (auto q : p)
    {
        zs.push_back(q[2]);
    }
    sort(zs.begin(), zs.end());
    zs.erase(unique(zs.begin(), zs.end()), zs.end());
    sort(p.begin(),
         p.end(),
         [](const auto &a, const auto &b)
         {
             return tie(a[0], a[1], a[2]) < tie(b[0], b[1], b[2]);
         });
    struct Node
    {
        int x, y, z;        // x、y 保留原坐标，z 是离散编号。
        int w = 1, ans = 1; // w 是重数，ans 是当前支配计数。
        vector<int> ids;    // ids 保存这一组重复点的原下标。
    };
    vector<Node> a;
    for (auto q : p)
    {
        auto [x, y, rawZ, id] = q;
        auto z = lower_bound(zs.begin(), zs.end(), rawZ) - zs.begin();
        if (!a.empty() && tie(a.back().x, a.back().y, a.back().z) == tuple(x, y, z))
        {
            a.back().w++;
            a.back().ans++;
            a.back().ids.push_back(id);
        }
        else
        {
            a.push_back({x, y, z, 1, 1, {id}});
        }
    }
    Fenwick bit(zs.size());
    vector<int> ord(a.size()), buf(a.size()), ans(n);
    iota(ord.begin(), ord.end(), 0);
    auto cdq = [&](auto &&self, int l, int r) -> void
    {
        if (r - l <= 1)
        {
            return;
        }
        auto m = midpoint(l, r);
        self(self, l, m);
        self(self, m, r);
        auto i = l, j = m, k = l;
        while (j < r)
        {
            while (i < m && a[ord[i]].y <= a[ord[j]].y)
            {
                bit.add(a[ord[i]].z, a[ord[i]].w);
                buf[k++] = ord[i++];
            }
            a[ord[j]].ans += bit.sum(a[ord[j]].z + 1);
            buf[k++] = ord[j++];
        }
        auto used = i;
        while (i < m)
        {
            buf[k++] = ord[i++];
        }
        for (auto q = l; q < used; q++)
        {
            bit.add(a[ord[q]].z, -a[ord[q]].w);
        }
        copy(buf.begin() + l, buf.begin() + r, ord.begin() + l);
    };
    cdq(cdq, 0, a.size());
    for (auto &q : a)
    {
        for (auto id : q.ids)
        {
            ans[id] = q.ans;
        }
    }
    return ans;
}

template <class T> struct FracCascade
{
    struct Item
    {
        T x;           // x 是增强表中的键值。
        int own, down; // own 指向本层原表下界，down 指向下一层增强表下界。
    };

    vector<vector<T>> a;    // a 保存各层原始有序表。
    vector<vector<Item>> b; // b 保存采样合并后的增强表和桥指针。

    explicit FracCascade(vector<vector<T>> src) : a(move(src)), b(a.size())
    {
        // src 是若干个按同一规则升序排列的静态表。
        for (auto &x : a)
        {
            assert(is_sorted(x.begin(), x.end())); // 调试检查，可删
        }
        for (int i = (int)a.size() - 1; i >= 0; i--)
        {
            auto v = a[i];
            if (i + 1 < (int)b.size())
            {
                for (int j = 1; j < (int)b[i + 1].size(); j += 2)
                {
                    v.push_back(b[i + 1][j].x);
                }
            }
            sort(v.begin(), v.end());
            v.erase(unique(v.begin(), v.end()), v.end());
            for (auto x : v)
            {
                auto p = lower_bound(a[i].begin(), a[i].end(), x) - a[i].begin();
                int q = 0;
                if (i + 1 < (int)b.size())
                {
                    q = lower_bound(b[i + 1].begin(),
                                    b[i + 1].end(),
                                    x,
                                    [](const Item &u, const T &rhs)
                                    {
                                        return u.x < rhs;
                                    }) -
                        b[i + 1].begin();
                }
                b[i].push_back({x, p, q});
            }
        }
    }

    vector<int> lowerBound(const T &x) const
    {
        // x 是所有表共用的查询值；返回每个原表中的 lower_bound 位置。
        vector<int> ans(a.size());
        int p = 0;
        if (!b.empty())
        {
            p = lower_bound(b[0].begin(),
                            b[0].end(),
                            x,
                            [](const Item &u, const T &rhs)
                            {
                                return u.x < rhs;
                            }) -
                b[0].begin();
        }
        for (int i = 0; i < (int)a.size(); i++)
        {
            ans[i] = p == (int)b[i].size() ? (int)a[i].size() : b[i][p].own;
            if (i + 1 == (int)a.size())
            {
                break;
            }
            p = p == (int)b[i].size() ? (int)b[i + 1].size() : b[i][p].down;
            if (p && b[i + 1][p - 1].x >= x)
            {
                p--;
            }
        }
        return ans;
    }
};

// Rect[0..3] 依次表示左横坐标、下纵坐标、右横坐标、上纵坐标。
using Rect = array<int, 4>;

int rectArea(const vector<Rect> &rs)
{
    // rs 中每项是 {左、下、右、上}；返回所有半开矩形的面积并。
    // Event[0..3] 依次表示横坐标、下纵坐标、上纵坐标、覆盖次数变化量。
    using Event = array<int, 4>;
    vector<Event> es;
    vector<int> ys;
    for (auto [xl, yl, xr, yr] : rs)
    {
        if (xl >= xr || yl >= yr)
        {
            continue;
        }
        es.push_back({xl, yl, yr, 1});
        es.push_back({xr, yl, yr, -1});
        ys.push_back(yl);
        ys.push_back(yr);
    }
    if (es.empty())
    {
        return 0;
    }
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());
    sort(es.begin(),
         es.end(),
         [](const auto &a, const auto &b)
         {
             return a[0] < b[0];
         });
    vector<int> cov(ys.size() * 4);
    vector<int> len(ys.size() * 4);
    auto add = [&](auto &&self, int p, int l, int r, int ql, int qr, int d) -> void
    {
        if (ql <= l && r <= qr)
        {
            cov[p] += d;
        }
        else
        {
            auto m = midpoint(l, r);
            if (ql < m)
            {
                self(self, p * 2, l, m, ql, qr, d);
            }
            if (m < qr)
            {
                self(self, p * 2 + 1, m, r, ql, qr, d);
            }
        }
        if (cov[p])
        {
            len[p] = ys[r] - ys[l];
        }
        else if (r - l == 1)
        {
            len[p] = 0;
        }
        else
        {
            len[p] = len[p * 2] + len[p * 2 + 1];
        }
    };
    int ans = 0;
    auto pre = es[0][0];
    for (int i = 0; i < (int)es.size();)
    {
        auto width = es[i][0] - pre;
        ans += len[1] * width;
        auto j = i;
        while (j < (int)es.size() && es[j][0] == es[i][0])
        {
            auto [x, yl, yr, d] = es[j];
            auto l = lower_bound(ys.begin(), ys.end(), yl) - ys.begin();
            auto r = lower_bound(ys.begin(), ys.end(), yr) - ys.begin();
            add(add, 1, 0, ys.size() - 1, l, r, d);
            j++;
        }
        pre = es[i][0];
        i = j;
    }
    return ans;
}

template <class T> vector<int> prevLess(const vector<T> &a)
{
    // a 是原序列；返回每个位置左侧最近严格更小元素的下标，不存在时为 -1。
    vector<int> stk, ans(a.size(), -1);
    for (int i = 0; i < (int)a.size(); i++)
    {
        while (!stk.empty() && a[stk.back()] >= a[i])
        {
            stk.pop_back();
        }
        if (!stk.empty())
        {
            ans[i] = stk.back();
        }
        stk.push_back(i);
    }
    return ans;
}

template <class T> vector<T> windowMin(const vector<T> &a, int k)
{
    // a 是原序列，k 是固定窗口长度；返回每个连续窗口的最小值。
    assert(1 <= k && k <= (int)a.size()); // 调试检查，可删
    deque<int> q;
    vector<T> ans;
    for (int i = 0; i < (int)a.size(); i++)
    {
        while (!q.empty() && q.front() <= i - k)
        {
            q.pop_front();
        }
        while (!q.empty() && a[q.back()] >= a[i])
        {
            q.pop_back();
        }
        q.push_back(i);
        if (i + 1 >= k)
        {
            ans.push_back(a[q.front()]);
        }
    }
    return ans;
}

template <class T> vector<T> windowMax(const vector<T> &a, int k)
{
    // a 是原序列，k 是固定窗口长度；返回每个连续窗口的最大值。
    assert(1 <= k && k <= (int)a.size()); // 调试检查，可删
    deque<int> q;
    vector<T> ans;
    for (int i = 0; i < (int)a.size(); i++)
    {
        while (!q.empty() && q.front() <= i - k)
        {
            q.pop_front();
        }
        while (!q.empty() && a[q.back()] <= a[i])
        {
            q.pop_back();
        }
        q.push_back(i);
        if (i + 1 >= k)
        {
            ans.push_back(a[q.front()]);
        }
    }
    return ans;
}

vector<int> lis(const vector<int> &a)
{
    // a 是原序列；返回一组严格上升子序列的原数组下标。
    vector<int> val, pos, pre(a.size(), -1);
    for (int i = 0; i < (int)a.size(); i++)
    {
        auto p = lower_bound(val.begin(), val.end(), a[i]) - val.begin();
        if (p == (int)val.size())
        {
            val.push_back(a[i]);
            pos.push_back(i);
        }
        else
        {
            val[p] = a[i];
            pos[p] = i;
        }
        if (p)
        {
            pre[i] = pos[p - 1];
        }
    }
    vector<int> ans;
    for (auto x = pos.empty() ? -1 : pos.back(); x >= 0; x = pre[x])
    {
        ans.push_back(x);
    }
    reverse(ans.begin(), ans.end());
    return ans;
}

int lis2D(vector<pair<int, int>> a)
{
    // a 保存二维点并按值复制；返回两维都严格增加的最长链长度。
    sort(a.begin(),
         a.end(),
         [](const auto &x, const auto &y)
         {
             if (x.first != y.first)
             {
                 return x.first < y.first;
             }
             return x.second > y.second;
         });
    vector<int> d;
    for (auto [x, y] : a)
    {
        auto it = lower_bound(d.begin(), d.end(), y);
        if (it == d.end())
        {
            d.push_back(y);
        }
        else
        {
            *it = y;
        }
    }
    return d.size();
}

int lcis(const vector<int> &a, const vector<int> &b)
{
    // a、b 是两个原序列；返回最长公共严格上升子序列长度。
    vector<int> f(b.size());
    for (auto x : a)
    {
        int best = 0;
        for (int j = 0; j < (int)b.size(); j++)
        {
            if (b[j] < x)
            {
                chmax(best, f[j]);
            }
            else if (b[j] == x)
            {
                chmax(f[j], best + 1);
            }
        }
    }
    return f.empty() ? 0 : *max_element(f.begin(), f.end());
}

int maxRect(const vector<int> &h)
{
    // h 是非负直方图柱高；返回由 Acc 可表示的最大矩形面积。
    vector<int> stk;
    int ans = 0;
    for (int i = 0; i <= (int)h.size(); i++)
    {
        auto x = i == (int)h.size() ? 0 : h[i];
        while (!stk.empty() && h[stk.back()] >= x)
        {
            auto p = stk.back();
            stk.pop_back();
            auto l = stk.empty() ? 0 : stk.back() + 1;
            chmax(ans, h[p] * (i - l));
        }
        stk.push_back(i);
    }
    return ans;
}

int maxRect(const vector<vector<int>> &a)
{
    // a 是规则矩阵，非零元素视为 1；返回由 Acc 可表示的最大全 1 子矩形面积。
    if (a.empty() || a[0].empty())
    {
        return 0;
    }
    vector<int> h(a[0].size());
    int ans = 0;
    for (auto &row : a)
    {
        assert(row.size() == h.size()); // 调试检查，可删
        for (int j = 0; j < (int)h.size(); j++)
        {
            h[j] = row[j] ? h[j] + 1 : 0;
        }
        chmax(ans, maxRect(h));
    }
    return ans;
}

// MoQuery[0..2] 依次表示左端点、右端点、答案编号。
using MoQuery = array<int, 3>;

template <class Ans, class AddL, class AddR, class DelL, class DelR, class Get>
vector<Ans> mo(int n, vector<MoQuery> qs, AddL addL, AddR addR, DelL delL, DelR delR, Get get)
{
    // n 是数组长度，qs 是询问；四个端点回调维护状态，get 返回当前答案。
    auto z = max<int>(1, (int)(n / sqrtl(max<int>(1, qs.size()))));
    sort(qs.begin(),
         qs.end(),
         [&](const auto &a, const auto &b)
         {
             auto x = a[0] / z, y = b[0] / z;
             if (x != y)
             {
                 return x < y;
             }
             return x & 1 ? a[1] > b[1] : a[1] < b[1];
         });
    vector<Ans> ans(qs.size());
    int l = 0, r = 0;
    for (auto q : qs)
    {
        while (l > q[0])
        {
            addL(--l);
        }
        while (r < q[1])
        {
            addR(r++);
        }
        while (l < q[0])
        {
            delL(l++);
        }
        while (r > q[1])
        {
            delR(--r);
        }
        ans[q[2]] = get();
    }
    return ans;
}

// ModifyQuery[0..3] 依次表示左端点、右端点、答案编号、修改时间。
using ModifyQuery = array<int, 4>;

template <class Ans, class Add, class Del, class Change, class Get>
vector<Ans> moModify(int n, vector<ModifyQuery> qs, Add add, Del del, Change change, Get get)
{
    // n 是数组长度，qs 含修改时间；add/del 维护端点，change(p,forward,l,r) 切换第 p 次修改，get 返回答案。
    auto z = max<int>(1, (int)powl(max<int>(1, n), 2.0L / 3));
    sort(qs.begin(),
         qs.end(),
         [&](const auto &a, const auto &b)
         {
             auto al = a[0] / z, bl = b[0] / z;
             if (al != bl)
             {
                 return al < bl;
             }
             auto ar = a[1] / z, br = b[1] / z;
             if (ar != br)
             {
                 return al & 1 ? ar > br : ar < br;
             }
             return ar & 1 ? a[3] > b[3] : a[3] < b[3];
         });
    vector<Ans> ans(qs.size());
    int l = 0, r = 0, t = 0;
    for (auto q : qs)
    {
        while (t < q[3])
        {
            change(t++, true, l, r);
        }
        while (t > q[3])
        {
            change(--t, false, l, r);
        }
        while (l > q[0])
        {
            add(--l);
        }
        while (r < q[1])
        {
            add(r++);
        }
        while (l < q[0])
        {
            del(l++);
        }
        while (r > q[1])
        {
            del(--r);
        }
        ans[q[2]] = get();
    }
    return ans;
}

template <class Ans, class Add, class Snap, class Undo, class Get>
vector<Ans> rollbackMo(int n, vector<MoQuery> qs, Add add, Snap snap, Undo undo, Get get)
{
    // n 是数组长度，qs 是询问；add 加入位置，snap/undo 管理快照，get 返回当前答案。
    auto z = max<int>(1, (int)sqrtl(max<int>(1, n)));
    sort(qs.begin(),
         qs.end(),
         [&](const auto &a, const auto &b)
         {
             auto x = a[0] / z, y = b[0] / z;
             return x != y ? x < y : a[1] < b[1];
         });
    vector<Ans> ans(qs.size());
    auto empty = snap();
    for (int i = 0; i < (int)qs.size();)
    {
        auto k = qs[i][0] / z;
        auto j = i;
        while (j < (int)qs.size() && qs[j][0] / z == k)
        {
            j++;
        }
        auto mid = min<int>(n, (k + 1) * z);
        auto r = mid;
        for (auto p = i; p < j; p++)
        {
            auto q = qs[p];
            if (q[1] <= mid)
            {
                auto s = snap();
                for (auto x = q[0]; x < q[1]; x++)
                {
                    add(x);
                }
                ans[q[2]] = get();
                undo(s);
                continue;
            }
            while (r < q[1])
            {
                add(r++);
            }
            auto s = snap();
            for (auto x = mid - 1; x >= q[0]; x--)
            {
                add(x);
            }
            ans[q[2]] = get();
            undo(s);
        }
        undo(empty);
        i = j;
    }
    return ans;
}

vector<int> xorPairs(const vector<int> &a, const vector<pair<int, int>> &qs, int k, int bits)
{
    // a 是 bits 位非负整数，qs 是半开区间，k 是目标异或位数；返回各区间点对数。
    assert(0 <= bits && bits <= 20); // 调试检查，可删
    auto n = (int)a.size();
    auto lim = 1LL << bits;
    for (auto x : a)
    {
        assert(0 <= x && x < lim); // 调试检查，可删
        (void)x;
    }
    vector<int> ans(qs.size());
    if (k < 0 || k > bits)
    {
        return ans;
    }

    // Query[0..2] 依次表示从 1 开始的左端点、右端点、原询问编号。
    using Query = array<int, 3>;
    vector<Query> ord;
    for (int id = 0; id < (int)qs.size(); id++)
    {
        auto [l, r] = qs[id];
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        if (l < r)
        {
            ord.push_back({l + 1, r, id});
        }
    }
    if (ord.empty())
    {
        return ans;
    }

    auto z = max<int>(1, (int)(n / sqrtl((long double)ord.size())));
    sort(ord.begin(),
         ord.end(),
         [&](const auto &x, const auto &y)
         {
             auto bx = (x[0] - 1) / z, by = (y[0] - 1) / z;
             if (bx != by)
             {
                 return bx < by;
             }
             return bx & 1 ? x[1] > y[1] : x[1] < y[1];
         });

    vector<int> masks;
    for (int s = 0; s < lim; s++)
    {
        if (popcount((unsigned long long)s) == k)
        {
            masks.push_back(s);
        }
    }
    vector<int> v(n + 1), cnt(lim), pre(n + 1);
    copy(a.begin(), a.end(), v.begin() + 1);
    for (int i = 1; i <= n; i++)
    {
        pre[i] = pre[i - 1];
        for (auto s : masks)
        {
            pre[i] += cnt[v[i] ^ s];
        }
        cnt[v[i]]++;
    }

    // Event[0..3] 依次表示左端点、右端点、贡献符号、排序后询问编号。
    using Event = array<int, 4>;
    vector<vector<Event>> ev(n + 1);
    vector<int> cur(ord.size());
    int l = 1, r = 0;
    for (int id = 0; id < (int)ord.size(); id++)
    {
        auto q = ord[id];
        cur[id] = pre[q[1]] - pre[r] + pre[q[0] - 1] - pre[l - 1];
        if (!k)
        {
            cur[id] += q[0] - l;
        }
        if (r < q[1])
        {
            ev[l - 1].push_back({r + 1, q[1], -1, id});
            r = q[1];
        }
        if (l > q[0])
        {
            ev[r].push_back({q[0], l - 1, 1, id});
            l = q[0];
        }
        if (r > q[1])
        {
            ev[l - 1].push_back({q[1] + 1, r, 1, id});
            r = q[1];
        }
        if (l < q[0])
        {
            ev[r].push_back({l, q[0] - 1, -1, id});
            l = q[0];
        }
    }

    fill(cnt.begin(), cnt.end(), 0);
    for (int i = 1; i <= n; i++)
    {
        for (auto s : masks)
        {
            cnt[v[i] ^ s]++;
        }
        for (auto e : ev[i])
        {
            int val = 0;
            for (int j = e[0]; j <= e[1]; j++)
            {
                val += cnt[v[j]];
            }
            cur[e[3]] += e[2] * val;
        }
    }
    for (int i = 1; i < (int)cur.size(); i++)
    {
        cur[i] += cur[i - 1];
    }
    for (int i = 0; i < (int)ord.size(); i++)
    {
        ans[ord[i][2]] = cur[i];
    }
    return ans;
}

vector<int> distinct(const vector<int> &a, const vector<pair<int, int>> &qs)
{
    // a 是静态数组，qs 是半开区间；返回每个区间中的不同值数量。
    // 只有下标、排名和频次的热点数据固定用 32 位，避免全局 int 宏加倍工作集。
    vector<int> val = a;
    sort(val.begin(), val.end());
    val.erase(unique(val.begin(), val.end()), val.end());
    vector<int32_t> rank(a.size());
    for (int i = 0; i < (int)a.size(); i++)
    {
        rank[i] = (int32_t)(lower_bound(val.begin(), val.end(), a[i]) - val.begin());
    }
    // Ask[0..1] 依次表示左端点与答案编号，按右端点分桶。
    using Ask = array<int32_t, 2>;
    vector<vector<Ask>> ask(a.size() + 1);
    for (int i = 0; i < (int)qs.size(); i++)
    {
        auto [l, r] = qs[i];
        assert(0 <= l && l <= r && r <= (int)a.size()); // 调试检查，可删
        assert(r <= numeric_limits<int32_t>::max());    // 调试检查，可删
        ask[r].push_back({(int32_t)l, (int32_t)i});
    }
    vector<int32_t> bit(a.size() + 1), last(val.size(), -1);
    auto add = [&](int p, int delta)
    {
        // p 是从 0 开始的位置，delta 是出现位置标记的变化。
        for (p++; p < (int)bit.size(); p += p & -p)
        {
            bit[p] += (int32_t)delta;
        }
    };
    auto sum = [&](int p)
    {
        // p 是半开前缀右端点；返回 [0,p) 的最后出现位置标记数。
        int ans = 0;
        for (; p; p -= p & -p)
        {
            ans += bit[p];
        }
        return ans;
    };
    vector<int> ans(qs.size());
    for (int r = 0; r <= (int)a.size(); r++)
    {
        if (r)
        {
            auto x = rank[r - 1];
            if (last[x] >= 0)
            {
                add(last[x], -1);
            }
            add(r - 1, 1);
            last[x] = r - 1;
        }
        for (auto [l, id] : ask[r])
        {
            ans[id] = sum(r) - sum(l);
        }
    }
    return ans;
}

// KthQuery[0..2] 依次表示左端点、右端点、从 1 开始的排名。
using KthQuery = array<int, 3>;

vector<int> rangeKth(const vector<int> &a, vector<KthQuery> qs)
{
    // a 是静态数组，qs 中每项是 {l,r,k}；返回各半开区间第 k 小值。
    if (qs.empty())
    {
        return {};
    }
    assert(!a.empty()); // 调试检查，可删
    for (auto q : qs)
    {
        assert(0 <= q[0] && q[0] < q[1] && q[1] <= (int)a.size()); // 调试检查，可删
        assert(1 <= q[2] && q[2] <= q[1] - q[0]);                  // 调试检查，可删
    }
    Discrete<int> dis(a);
    vector<int> rk(a.size());
    for (int i = 0; i < (int)a.size(); i++)
    {
        rk[i] = dis.id(a[i]);
    }
    vector<int> es(a.size()), ids(qs.size()), ans(qs.size());
    iota(es.begin(), es.end(), 0);
    iota(ids.begin(), ids.end(), 0);
    Fenwick bit(a.size());
    auto solve = [&](auto &&self, int vl, int vr, vector<int> ps, vector<int> qi) -> void
    {
        if (qi.empty())
        {
            return;
        }
        if (vr - vl == 1)
        {
            for (auto id : qi)
            {
                ans[id] = dis.value(vl);
            }
            return;
        }
        auto vm = midpoint(vl, vr);
        vector<int> pl, pr, ql, qr;
        for (auto p : ps)
        {
            if (rk[p] < vm)
            {
                bit.add(p, 1);
                pl.push_back(p);
            }
            else
            {
                pr.push_back(p);
            }
        }
        for (auto id : qi)
        {
            auto &q = qs[id];
            auto c = bit.sum(q[0], q[1]);
            if (q[2] <= c)
            {
                ql.push_back(id);
            }
            else
            {
                q[2] -= c;
                qr.push_back(id);
            }
        }
        for (auto p : pl)
        {
            bit.add(p, -1);
        }
        self(self, vl, vm, move(pl), move(ql));
        self(self, vm, vr, move(pr), move(qr));
    };
    solve(solve, 0, dis.a.size(), move(es), move(ids));
    return ans;
}

struct TreeMo
{
    int n = 0;                     // n 是树的顶点数。
    int lg = 1;                    // lg 是倍增祖先表的层数。
    int blk = 1;                   // blk 是当前询问排序的块长。
    vector<vector<int>> g;         // g 是树的邻接表。
    vector<int> col;               // col[u] 是顶点 u 离散到 [0,n) 的权值。
    vector<int> st;                // st[u] 是顶点 u 进入欧拉序的位置。
    vector<int> ed;                // ed[u] 是顶点 u 离开欧拉序的位置。
    vector<int> seq;               // seq 是每个顶点进出各出现一次的欧拉序。
    vector<int> dep;               // dep[u] 是顶点 u 到当前根的边数。
    vector<vector<int>> up;        // up[k][u] 是顶点 u 的第 2^k 级祖先。
    vector<array<int, 4>> qs;      // 每项是 {欧拉左端、欧拉右端、额外 LCA、答案编号}。

    explicit TreeMo(int m) : n(m), g(m), col(m), st(m), ed(m), dep(m)
    {
        // m 是非空树的顶点数。
        assert(m > 0); // 调试检查，可删
    }

    void addTreeEdge(int u, int v)
    {
        // u、v 是无向树边的两个端点；调用后把边加入邻接表。
        assert(0 <= u && u < n && 0 <= v && v < n); // 调试检查，可删
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void build(int root = 0)
    {
        // root 是本次选定的根；重建欧拉序和祖先表，并清空依赖旧序的询问。
        assert(0 <= root && root < n); // 调试检查，可删
        lg = 1;
        while ((1ULL << lg) < (unsigned long long)n)
        {
            lg++;
        }
        up.assign(lg, vector<int>(n, root));
        fill(dep.begin(), dep.end(), 0);
        seq.clear();
        seq.reserve((size_t)n * 2);
        qs.clear();

        st[root] = 0;
        seq.push_back(root);
        auto dfs = [&](auto &&self, int u, int p) -> void
        {
            for (auto v : g[u])
            {
                if (v == p)
                {
                    continue;
                }
                up[0][v] = u;
                dep[v] = dep[u] + 1;
                st[v] = seq.size();
                seq.push_back(v);
                self(self, v, u);
            }
            ed[u] = seq.size();
            seq.push_back(u);
        };
        dfs(dfs, root, -1);
        for (int k = 1; k < lg; k++)
        {
            for (int u = 0; u < n; u++)
            {
                up[k][u] = up[k - 1][up[k - 1][u]];
            }
        }
    }

    int lca(int u, int v) const
    {
        // u、v 是树上顶点；返回它们在当前根下的最近公共祖先。
        assert(!up.empty() && 0 <= u && u < n && 0 <= v && v < n); // 调试检查，可删
        if (dep[u] < dep[v])
        {
            swap(u, v);
        }
        auto d = dep[u] - dep[v];
        for (int k = 0; k < lg; k++)
        {
            if (d >> k & 1)
            {
                u = up[k][u];
            }
        }
        if (u == v)
        {
            return u;
        }
        for (int k = lg - 1; k >= 0; k--)
        {
            if (up[k][u] != up[k][v])
            {
                u = up[k][u];
                v = up[k][v];
            }
        }
        return up[0][u];
    }

    void addQuery(int u, int v)
    {
        // u、v 是路径两端；把该路径转成半开欧拉区间并按加入顺序编号。
        assert(!up.empty() && 0 <= u && u < n && 0 <= v && v < n); // 调试检查，可删
        if (st[u] > st[v])
        {
            swap(u, v);
        }
        auto w = lca(u, v);
        auto id = (int)qs.size();
        if (w == u)
        {
            qs.push_back({st[u], st[v] + 1, -1, id});
        }
        else
        {
            qs.push_back({ed[u], st[v] + 1, w, id});
        }
    }

    vector<int> solve()
    {
        // 返回每条已加入路径上的不同权值数，顺序与 addQuery 一致。
        if (qs.empty())
        {
            return {};
        }
        for (auto x : col)
        {
            assert(0 <= x && x < n); // 调试检查，可删
        }
        auto m = (int)qs.size();
        blk = max<int>(1, (int)((long double)seq.size() / sqrtl(m)));
        sort(qs.begin(),
             qs.end(),
             [&](const auto &a, const auto &b)
             {
                 auto x = a[0] / blk;
                 auto y = b[0] / blk;
                 if (x != y)
                 {
                     return x < y;
                 }
                 return x & 1 ? a[1] > b[1] : a[1] < b[1];
             });
        vector<int> cnt(n);
        vector<int> used(n);
        vector<int> ans(m);
        int cur = 0;
        auto work = [&](int u)
        {
            // u 是本次切换的顶点；奇数次出现计入路径，偶数次出现移出路径。
            used[u] ^= 1;
            if (used[u])
            {
                if (cnt[col[u]]++ == 0)
                {
                    cur++;
                }
            }
            else
            {
                if (--cnt[col[u]] == 0)
                {
                    cur--;
                }
            }
        };
        int l = 0;
        int r = 0;
        for (auto q : qs)
        {
            auto ql = q[0];
            auto qr = q[1];
            auto w = q[2];
            auto id = q[3];
            while (l > ql)
            {
                work(seq[--l]);
            }
            while (r < qr)
            {
                work(seq[r++]);
            }
            while (l < ql)
            {
                work(seq[l++]);
            }
            while (r > qr)
            {
                work(seq[--r]);
            }
            if (w >= 0)
            {
                work(w);
            }
            ans[id] = cur;
            if (w >= 0)
            {
                work(w);
            }
        }
        return ans;
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
    vector<pair<int, int>> qs(q);
    for (auto &[l, r] : qs)
    {
        cin >> l >> r;
    }
    for (auto x : distinct(a, qs))
    {
        cout << x << '\n';
    }
    return 0;
}
