
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

signed main()
{
    int a, b;
    cin >> a >> b;
    cout << a + b << '\n';
}
