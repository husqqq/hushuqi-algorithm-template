
// TreePathSum 的完整实现与动态树公共模块共用，避免重复维护。


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

template <class Acc = long long>
class TreePathSum
{
    // n 保存点数，ti 保存 DFS 序计数器。
    int n, ti = 0;
    // g 保存静态树邻接表。
    vector<vector<int>> g;
    // fa、dep、siz、son、top、in 依次保存父节点、深度、子树大小、重儿子、链顶和 DFS 序。
    vector<int> fa, dep, siz, son, top, in;
    // val 保存各点当前权值，bit 保存 DFS 序上的树状数组。
    vector<Acc> val, bit;

    void dfs1(int u)
    {
        // u 是当前树节点；计算父子关系、深度、子树大小和重儿子，无返回值。
        siz[u] = 1;
        for (auto v : g[u])
        {
            if (v == fa[u])
            {
                continue;
            }
            fa[v] = u;
            dep[v] = dep[u] + 1;
            dfs1(v);
            siz[u] += siz[v];
            if (son[u] == -1 || siz[v] > siz[son[u]])
            {
                son[u] = v;
            }
        }
    }

    void dfs2(int u, int h)
    {
        // u 是当前点，h 是所在重链链顶；分配 DFS 序并继续剖分，无返回值。
        top[u] = h;
        in[u] = ti++;
        if (son[u] != -1)
        {
            dfs2(son[u], h);
        }
        for (auto v : g[u])
        {
            if (v != fa[u] && v != son[u])
            {
                dfs2(v, v);
            }
        }
    }

    void addBit(int p, Acc v)
    {
        // p 是 0 下标 DFS 序位置，v 是增量；更新树状数组，无返回值。
        for (p++; p <= n; p += p & -p)
        {
            bit[p] += v;
        }
    }

    Acc sumBit(int p) const
    {
        // p 是右端点；返回半开前缀 [0,p) 的点权和。
        Acc ans = 0;
        for (; p; p -= p & -p)
        {
            ans += bit[p];
        }
        return ans;
    }

  public:
    TreePathSum(const vector<vector<int>> &g, const vector<Acc> &a, int root = 0)
        : n(g.size()), g(g), fa(n, -1), dep(n), siz(n), son(n, -1), top(n), in(n), val(a), bit(n + 1)
    {
        // g 是静态树，a 是各点初值，root 是根；完成重链剖分与树状数组初始化，无返回值。
        assert(n > 0 && (int)a.size() == n && 0 <= root && root < n);
        dfs1(root), dfs2(root, root);
        for (int u = 0; u < n; u++)
        {
            addBit(in[u], val[u]);
        }
    }

    void set(int u, Acc v)
    {
        // u 是节点编号，v 是新点权；执行单点赋值，无返回值。
        assert(0 <= u && u < n);
        addBit(in[u], v - val[u]);
        val[u] = v;
    }

    Acc pathSum(int u, int v) const
    {
        // u、v 是路径两端；返回简单路径上全部点权之和。
        assert(0 <= u && u < n && 0 <= v && v < n);
        Acc ans = 0;
        while (top[u] != top[v])
        {
            if (dep[top[u]] < dep[top[v]])
            {
                swap(u, v);
            }
            ans += sumBit(in[u] + 1) - sumBit(in[top[u]]);
            u = fa[top[u]];
        }
        if (dep[u] > dep[v])
        {
            swap(u, v);
        }
        return ans + sumBit(in[v] + 1) - sumBit(in[u]);
    }

    Acc subtreeSum(int u) const
    {
        // u 是子树根；返回固定构造根下 u 的整棵子树点权和。
        assert(0 <= u && u < n);
        return sumBit(in[u] + siz[u]) - sumBit(in[u]);
    }
};

template <class Acc = long long>
class LinkCutTree
{
    struct Node
    {
        // ch[0..1] 依次保存辅助树左右儿子编号。
        array<int, 2> ch{};
        // fa 保存辅助树父节点，rev 标记辅助子树翻转。
        int fa = 0;
        bool rev = false;
        // val 保存当前点权，sum 保存辅助树中路径节点的聚合和。
        Acc val = 0, sum = 0;
    };

    // t[0] 是空节点，t[u+1] 对应外部 0 下标节点 u。
    vector<Node> t;

    bool isRoot(int x) const
    {
        // x 是内部节点编号；返回 x 是否为当前辅助树根。
        int f = t[x].fa;
        return !f || (t[f].ch[0] != x && t[f].ch[1] != x);
    }

    void pull(int x)
    {
        // x 是内部节点编号；由两个儿子重新计算路径聚合和，无返回值。
        t[x].sum = t[t[x].ch[0]].sum + t[x].val + t[t[x].ch[1]].sum;
    }

    void reverseNode(int x)
    {
        // x 是内部节点编号；翻转辅助子树并记录懒标记，无返回值。
        if (!x)
        {
            return;
        }
        swap(t[x].ch[0], t[x].ch[1]);
        t[x].rev ^= 1;
    }

    void push(int x)
    {
        // x 是内部节点编号；下传翻转标记，无返回值。
        if (!t[x].rev)
        {
            return;
        }
        reverseNode(t[x].ch[0]);
        reverseNode(t[x].ch[1]);
        t[x].rev = false;
    }

    void rotate(int x)
    {
        // x 是要上旋的内部节点；执行一次 Splay 旋转并维护聚合，无返回值。
        int y = t[x].fa, z = t[y].fa, k = t[y].ch[1] == x, w = t[x].ch[k ^ 1];
        if (!isRoot(y))
        {
            t[z].ch[t[z].ch[1] == y] = x;
        }
        t[x].fa = z;
        t[x].ch[k ^ 1] = y;
        t[y].fa = x;
        t[y].ch[k] = w;
        if (w)
        {
            t[w].fa = y;
        }
        pull(y), pull(x);
    }

    void splay(int x)
    {
        // x 是内部节点；把它旋到当前辅助树根，无返回值。
        vector<int> stk{x};
        for (int y = x; !isRoot(y); y = t[y].fa)
        {
            stk.push_back(t[y].fa);
        }
        while (!stk.empty())
        {
            push(stk.back());
            stk.pop_back();
        }
        while (!isRoot(x))
        {
            int y = t[x].fa, z = t[y].fa;
            if (!isRoot(y))
            {
                rotate((t[y].ch[1] == x) == (t[z].ch[1] == y) ? y : x);
            }
            rotate(x);
        }
    }

    void access(int x)
    {
        // x 是内部节点；把动态树根到 x 的路径改为首选路径，无返回值。
        for (int y = 0; x; x = t[y = x].fa)
        {
            splay(x);
            t[x].ch[1] = y;
            pull(x);
        }
    }

    void makeRootNode(int x)
    {
        // x 是内部节点；把它改为所在动态树的根，无返回值。
        access(x), splay(x), reverseNode(x);
    }

    int findRootNode(int x)
    {
        // x 是内部节点；返回所在动态树根的内部编号。
        access(x), splay(x);
        while (push(x), t[x].ch[0])
        {
            x = t[x].ch[0];
        }
        splay(x);
        return x;
    }

  public:
    LinkCutTree(const vector<Acc> &a = {}) : t(a.size() + 1)
    {
        // a 是各节点初始点权；建立 n 个互不连通的节点，无返回值。
        for (int i = 0; i < (int)a.size(); i++)
        {
            t[i + 1].val = t[i + 1].sum = a[i];
        }
    }

    bool same(int u, int v)
    {
        // u、v 是外部节点编号；返回两点是否连通。
        return findRootNode(u + 1) == findRootNode(v + 1);
    }

    bool link(int u, int v)
    {
        // u、v 是外部节点编号；不连通时连边并返回 true，否则不修改并返回 false。
        u++, v++;
        makeRootNode(u);
        if (findRootNode(v) == u)
        {
            return false;
        }
        t[u].fa = v;
        return true;
    }

    bool cut(int u, int v)
    {
        // u、v 是外部节点编号；边存在时删除并返回 true，否则返回 false。
        u++, v++;
        makeRootNode(u);
        access(v), splay(v);
        if (t[v].ch[0] != u || t[u].ch[1])
        {
            return false;
        }
        t[v].ch[0] = t[u].fa = 0;
        pull(v);
        return true;
    }

    bool connected(int u, int v)
    {
        // u、v 是外部节点编号；返回两点当前是否属于同一棵动态树。
        return findRootNode(u + 1) == findRootNode(v + 1);
    }

    void set(int u, Acc v)
    {
        // u 是外部节点编号，v 是新点权；执行单点赋值，无返回值。
        u++;
        access(u), splay(u);
        t[u].val = v;
        pull(u);
    }

    Acc pathSum(int u, int v)
    {
        // u、v 必须连通；返回两点简单路径上的点权和。
        u++, v++;
        makeRootNode(u);
        access(v), splay(v);
        return t[v].sum;
    }
};

template <class Acc = long long>
class SubtreeLCT
{
    struct Node
    {
        // ch[0..1] 依次保存辅助树左右儿子编号。
        array<int, 2> ch{};
        // fa 保存辅助树父节点，rev 标记辅助子树翻转。
        int fa = 0;
        bool rev = false;
        // val、path、vir、sum 依次保存点权、首选路径和、虚儿子贡献、整棵辅助子树贡献。
        Acc val = 0, path = 0, vir = 0, sum = 0;
    };

    // t[0] 是空节点，t[u+1] 对应外部节点 u。
    vector<Node> t;

    bool isRoot(int x) const
    {
        // x 是内部节点编号；返回 x 是否为当前辅助树根。
        int f = t[x].fa;
        return !f || (t[f].ch[0] != x && t[f].ch[1] != x);
    }

    void pull(int x)
    {
        // x 是内部节点；重算首选路径和与包含虚儿子的总和，无返回值。
        auto [l, r] = t[x].ch;
        t[x].path = t[l].path + t[x].val + t[r].path;
        t[x].sum = t[l].sum + t[x].val + t[r].sum + t[x].vir;
    }

    void reverseNode(int x)
    {
        // x 是内部节点；翻转辅助子树并记录标记，无返回值。
        if (!x)
        {
            return;
        }
        swap(t[x].ch[0], t[x].ch[1]);
        t[x].rev ^= 1;
    }

    void push(int x)
    {
        // x 是内部节点；下传翻转标记，无返回值。
        if (!t[x].rev)
        {
            return;
        }
        reverseNode(t[x].ch[0]), reverseNode(t[x].ch[1]);
        t[x].rev = false;
    }

    void rotate(int x)
    {
        // x 是要上旋的内部节点；执行一次 Splay 旋转并维护信息，无返回值。
        int y = t[x].fa, z = t[y].fa, k = t[y].ch[1] == x, w = t[x].ch[k ^ 1];
        if (!isRoot(y))
        {
            t[z].ch[t[z].ch[1] == y] = x;
        }
        t[x].fa = z;
        t[x].ch[k ^ 1] = y;
        t[y].fa = x;
        t[y].ch[k] = w;
        if (w)
        {
            t[w].fa = y;
        }
        pull(y), pull(x);
    }

    void splay(int x)
    {
        // x 是内部节点；把它旋到辅助树根，无返回值。
        vector<int> stk{x};
        for (int y = x; !isRoot(y); y = t[y].fa)
        {
            stk.push_back(t[y].fa);
        }
        while (!stk.empty())
        {
            push(stk.back());
            stk.pop_back();
        }
        while (!isRoot(x))
        {
            int y = t[x].fa, z = t[y].fa;
            if (!isRoot(y))
            {
                rotate((t[y].ch[1] == x) == (t[z].ch[1] == y) ? y : x);
            }
            rotate(x);
        }
    }

    void access(int x)
    {
        // x 是内部节点；切换首选儿子并同步虚儿子贡献，无返回值。
        for (int y = 0, z = x; z; z = t[y = z].fa)
        {
            splay(z);
            t[z].vir += t[t[z].ch[1]].sum - t[y].sum;
            t[z].ch[1] = y;
            pull(z);
        }
        splay(x);
    }

    void makeRootNode(int x)
    {
        // x 是内部节点；把它改为所在动态树的根，无返回值。
        access(x), reverseNode(x);
    }

    int findRootNode(int x)
    {
        // x 是内部节点；返回所在动态树根的内部编号。
        access(x);
        while (push(x), t[x].ch[0])
        {
            x = t[x].ch[0];
        }
        splay(x);
        return x;
    }

  public:
    SubtreeLCT(const vector<Acc> &a = {}) : t(a.size() + 1)
    {
        // a 是各点初始权值；建立 n 个互不连通节点，无返回值。
        for (int i = 0; i < (int)a.size(); i++)
        {
            t[i + 1].val = t[i + 1].path = t[i + 1].sum = a[i];
        }
    }

    bool link(int u, int v)
    {
        // u、v 是外部节点编号；不连通时连边并返回 true，否则返回 false。
        u++, v++;
        makeRootNode(u);
        if (findRootNode(v) == u)
        {
            return false;
        }
        access(v);
        t[u].fa = v;
        t[v].vir += t[u].sum;
        pull(v);
        return true;
    }

    bool cut(int u, int v)
    {
        // u、v 是外部节点编号；边存在时删除并返回 true，否则返回 false。
        u++, v++;
        makeRootNode(u), access(v);
        if (t[v].ch[0] != u || t[u].ch[1])
        {
            return false;
        }
        t[v].ch[0] = t[u].fa = 0;
        pull(v);
        return true;
    }

    bool connected(int u, int v)
    {
        // u、v 是外部节点编号；返回两点当前是否属于同一棵动态树。
        return findRootNode(u + 1) == findRootNode(v + 1);
    }

    void set(int u, Acc v)
    {
        // u 是外部节点编号，v 是新点权；执行单点赋值，无返回值。
        u++;
        access(u);
        t[u].val = v;
        pull(u);
    }

    Acc pathSum(int u, int v)
    {
        // u、v 必须连通；返回两点简单路径上的点权和。
        u++, v++;
        makeRootNode(u), access(v);
        return t[v].path;
    }

    Acc subtreeSum(int root, int u)
    {
        // root 是有根树根，u 是子树根；返回该根方向下 u 的子树点权和。
        root++, u++;
        makeRootNode(root), access(u);
        return t[u].val + t[u].vir;
    }
};

class OnlineBridge
{
    // d2、dc 分别保存边双代表元和连通块代表元。
    vector<int> d2, dc;
    // fa 保存当前生成森林父节点，siz 保存连通块大小。
    vector<int> fa, siz;
    // vis 保存最近一次 mergePath 的访问轮次。
    vector<int> vis;
    // ti 是访问轮次，cnt 是当前桥数。
    int ti = 0, cnt = 0;

    int find2(int x)
    {
        // x 是节点编号；返回其当前边双连通分量代表元。
        if (x == -1)
        {
            return -1;
        }
        return d2[x] == x ? x : d2[x] = find2(d2[x]);
    }

    int findC(int x)
    {
        // x 是节点编号；返回其当前连通块代表元。
        if (x == -1)
        {
            return -1;
        }
        x = find2(x);
        return dc[x] == x ? x : dc[x] = findC(dc[x]);
    }

    void makeRoot(int x)
    {
        // x 是节点编号；翻转生成森林中 x 到原根的父边，无返回值。
        x = find2(x);
        int root = x, pre = -1;
        while (x != -1)
        {
            int p = find2(fa[x]);
            fa[x] = pre;
            dc[x] = root;
            pre = x;
            x = p;
        }
        siz[root] = siz[pre];
    }

    void mergePath(int a, int b)
    {
        // a、b 已在同一连通块；压缩新环上的全部桥，无返回值。
        ti++;
        vector<int> pa, pb;
        int lca = -1;
        while (lca == -1)
        {
            if (a != -1)
            {
                a = find2(a);
                pa.push_back(a);
                if (vis[a] == ti)
                {
                    lca = a;
                    break;
                }
                vis[a] = ti;
                a = fa[a];
            }
            swap(a, b), swap(pa, pb);
        }
        for (auto x : pa)
        {
            d2[x] = lca;
            if (x == lca)
            {
                break;
            }
            cnt--;
        }
        for (auto x : pb)
        {
            d2[x] = lca;
            if (x == lca)
            {
                break;
            }
            cnt--;
        }
    }

  public:
    OnlineBridge(int n) : d2(n), dc(n), fa(n, -1), siz(n, 1), vis(n)
    {
        // n 是固定点数；建立没有边的无向图，无返回值。
        iota(d2.begin(), d2.end(), 0);
        iota(dc.begin(), dc.end(), 0);
    }

    void addEdge(int a, int b)
    {
        // a、b 是无向边两端；加入边并更新当前桥数，无返回值，允许重边和自环。
        a = find2(a), b = find2(b);
        if (a == b)
        {
            return;
        }
        int ca = findC(a), cb = findC(b);
        if (ca != cb)
        {
            cnt++;
            if (siz[ca] > siz[cb])
            {
                swap(a, b), swap(ca, cb);
            }
            makeRoot(a);
            fa[a] = dc[a] = b;
            siz[cb] += siz[a];
        }
        else
        {
            mergePath(a, b);
        }
    }

    int bridges() const
    {
        // 无参数；返回当前无向图中的桥数。
        return cnt;
    }
};

class DynamicCut
{
    // n 保存固定点数。
    int n;
    // e[id][0..1] 依次保存第 id 条边的两个端点，on 标记边当前是否存在。
    vector<array<int, 2>> e;
    vector<char> on;

  public:
    DynamicCut(int n) : n(n)
    {
        // n 是固定点数；建立空无向图，无返回值。
    }

    int addEdge(int u, int v)
    {
        // u、v 是边的两个端点；加入边并返回稳定边编号。
        assert(0 <= u && u < n && 0 <= v && v < n);
        e.push_back({u, v});
        on.push_back(1);
        return e.size() - 1;
    }

    bool eraseEdge(int id)
    {
        // id 是 addEdge 返回的边编号；边存在时删除并返回 true，否则返回 false。
        if (id < 0 || id >= (int)e.size() || !on[id])
        {
            return false;
        }
        on[id] = 0;
        return true;
    }

    pair<vector<int>, vector<int>> analyze() const
    {
        // 无参数；返回 {全部桥的边编号,全部割点编号}，两部分都按编号升序排列。
        vector<vector<array<int, 2>>> g(n);
        for (int id = 0; id < (int)e.size(); id++)
        {
            if (!on[id])
            {
                continue;
            }
            auto [u, v] = e[id];
            g[u].push_back({v, id});
            g[v].push_back({u, id});
        }
        vector<int> dfn(n), low(n), cut(n), bridge;
        int ti = 0;
        auto dfs = [&](auto &&self, int u, int pe) -> void
        {
            dfn[u] = low[u] = ++ti;
            int child = 0;
            for (auto [v, id] : g[u])
            {
                if (id == pe)
                {
                    continue;
                }
                if (!dfn[v])
                {
                    child++;
                    self(self, v, id);
                    chmin(low[u], low[v]);
                    if (low[v] > dfn[u])
                    {
                        bridge.push_back(id);
                    }
                    if (pe != -1 && low[v] >= dfn[u])
                    {
                        cut[u] = 1;
                    }
                }
                else
                {
                    chmin(low[u], dfn[v]);
                }
            }
            if (pe == -1 && child > 1)
            {
                cut[u] = 1;
            }
        };
        for (int u = 0; u < n; u++)
        {
            if (!dfn[u])
            {
                dfs(dfs, u, -1);
            }
        }
        vector<int> point;
        for (int u = 0; u < n; u++)
        {
            if (cut[u])
            {
                point.push_back(u);
            }
        }
        sort(bridge.begin(), bridge.end());
        return {bridge, point};
    }
};

class GlobalTree
{
    // n 保存点数，root 保存全局平衡树根。
    int n, root;
    // g 保存原树邻接表。
    vector<vector<int>> g;
    // pa、siz、son 依次保存原树父节点、子树大小和重儿子。
    vector<int> pa, siz, son;
    // ch[u][0..1] 保存全局平衡树左右儿子，fa 保存平衡树父边或轻边父亲。
    vector<array<int, 2>> ch;
    vector<int> fa;

    void dfs(int u)
    {
        // u 是当前原树节点；计算子树大小和重儿子，无返回值。
        siz[u] = 1;
        for (auto v : g[u])
        {
            if (v == pa[u])
            {
                continue;
            }
            pa[v] = u;
            dfs(v);
            siz[u] += siz[v];
            if (son[u] == -1 || siz[v] > siz[son[u]])
            {
                son[u] = v;
            }
        }
    }

    int buildSeq(const vector<int> &v, const vector<int> &pre, int l, int r)
    {
        // v 是一条重链，pre 是轻子树质量前缀和，l、r 是当前闭区间；返回加权中点根。
        if (l > r)
        {
            return -1;
        }
        int need = (pre[l] + pre[r + 1]) / 2;
        int m = lower_bound(pre.begin() + l + 1, pre.begin() + r + 2, need) - pre.begin() - 1;
        m = clamp(m, l, r);
        int u = v[m];
        ch[u][0] = buildSeq(v, pre, l, m - 1);
        ch[u][1] = buildSeq(v, pre, m + 1, r);
        if (ch[u][0] != -1)
        {
            fa[ch[u][0]] = u;
        }
        if (ch[u][1] != -1)
        {
            fa[ch[u][1]] = u;
        }
        return u;
    }

    int buildChain(int x)
    {
        // x 是一条重链链顶；递归建立轻链并返回该链的平衡树根。
        vector<int> v;
        for (int u = x; u != -1; u = son[u])
        {
            v.push_back(u);
        }
        for (auto u : v)
        {
            for (auto w : g[u])
            {
                if (w == pa[u] || w == son[u])
                {
                    continue;
                }
                int z = buildChain(w);
                fa[z] = u;
            }
        }
        vector<int> pre(v.size() + 1);
        for (int i = 0; i < (int)v.size(); i++)
        {
            pre[i + 1] = pre[i] + siz[v[i]] - (son[v[i]] == -1 ? 0 : siz[son[v[i]]]);
        }
        return buildSeq(v, pre, 0, v.size() - 1);
    }

  public:
    GlobalTree(const vector<vector<int>> &g, int rt = 0)
        : n(g.size()), root(-1), g(g), pa(n, -1), siz(n), son(n, -1), ch(n, array<int, 2>{-1, -1}), fa(n, -1)
    {
        // g 是静态树，rt 是原树根；建立全局平衡二叉树，无返回值。
        assert(n > 0 && 0 <= rt && rt < n);
        dfs(rt);
        root = buildChain(rt);
    }

    int getRoot() const
    {
        // 无参数；返回全局平衡二叉树根节点编号。
        return root;
    }

    const vector<array<int, 2>> &children() const
    {
        // 无参数；返回每个节点在平衡二叉树中的左右儿子。
        return ch;
    }

    const vector<int> &parents() const
    {
        // 无参数；返回平衡树父边与轻边共同组成的父指针。
        return fa;
    }

    const vector<int> &heavy() const
    {
        // 无参数；返回原树中每个节点的重儿子。
        return son;
    }

    const vector<int> &parentTree() const
    {
        // 无参数；返回原树父节点数组。
        return pa;
    }
};

template <class Acc = long long>
class DynamicMWIS
{
    struct Mat
    {
        // nullopt 表示不可达，其余项保存 Acc 类型的 max-plus 值。
        array<array<optional<Acc>, 2>, 2> a{};
    };

    // tr 保存全局平衡二叉树拓扑。
    GlobalTree tr;
    // ch、fa、son、pa 分别引用平衡树儿子、混合父指针、重儿子和原树父节点。
    vector<array<int, 2>> ch;
    vector<int> fa, son, pa;
    // w 保存点权，g[u][0..1] 保存不含重儿子的“不选 u / 选 u”贡献。
    vector<Acc> w;
    vector<array<Acc, 2>> g;
    // s 保存每个平衡树子树合并后的 max-plus 矩阵。
    vector<Mat> s;
    // tree 保存原树邻接表。
    vector<vector<int>> tree;

    static Mat mul(const Mat &x, const Mat &y)
    {
        // x、y 是两个 max-plus 矩阵；返回按顺序合并后的矩阵。
        Mat z;
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                for (int k = 0; k < 2; k++)
                {
                    if (!x.a[i][k] || !y.a[k][j])
                    {
                        continue;
                    }
                    Acc v = *x.a[i][k] + *y.a[k][j];
                    if (!z.a[i][j] || *z.a[i][j] < v)
                    {
                        z.a[i][j] = v;
                    }
                }
            }
        }
        return z;
    }

    bool isChild(int x) const
    {
        // x 是节点编号；返回 x 到 fa[x] 是否为平衡二叉树实儿子边。
        return fa[x] != -1 && (ch[fa[x]][0] == x || ch[fa[x]][1] == x);
    }

    void pull(int x)
    {
        // x 是节点编号；由自身转移和左右儿子重算矩阵，无返回值。
        Mat cur;
        cur.a[0][0] = g[x][0];
        cur.a[0][1] = g[x][0];
        cur.a[1][0] = g[x][1];
        if (ch[x][0] != -1)
        {
            cur = mul(s[ch[x][0]], cur);
        }
        if (ch[x][1] != -1)
        {
            cur = mul(cur, s[ch[x][1]]);
        }
        s[x] = cur;
    }

    array<Acc, 2> dfs(int u)
    {
        // u 是原树节点；返回静态 MWIS 的 {不选 u,选 u}，并初始化轻儿子贡献。
        array<Acc, 2> f{0, w[u]};
        for (auto v : tree[u])
        {
            if (v == pa[u])
            {
                continue;
            }
            auto h = dfs(v);
            f[0] += max(h[0], h[1]);
            f[1] += h[0];
            if (v != son[u])
            {
                g[u][0] += max(h[0], h[1]);
                g[u][1] += h[0];
            }
        }
        g[u][1] += w[u];
        return f;
    }

    void buildMat(int u)
    {
        // u 是平衡树节点；后序建立当前实儿子子树矩阵，无返回值。
        for (auto v : ch[u])
        {
            if (v != -1)
            {
                buildMat(v);
            }
        }
        pull(u);
    }

  public:
    DynamicMWIS(const vector<vector<int>> &tree, const vector<Acc> &w, int root = 0)
        : tr(tree, root), ch(tr.children()), fa(tr.parents()), son(tr.heavy()), pa(tr.parentTree()), w(w), g(w.size()),
          s(w.size()), tree(tree)
    {
        // tree 是静态树，w 是可修改点权，root 是固定根；初始化动态最大权独立集，无返回值。
        assert(tree.size() == w.size());
        dfs(root);
        for (int u = 0; u < (int)w.size(); u++)
        {
            if (!isChild(u))
            {
                buildMat(u);
            }
        }
    }

    void set(int x, Acc v)
    {
        // x 是节点编号，v 是新点权；更新点权并维护全树最大权独立集，无返回值。
        g[x][1] += v - w[x];
        w[x] = v;
        while (fa[x] != -1)
        {
            if (isChild(x))
            {
                pull(x);
            }
            else
            {
                int p = fa[x];
                Acc old0 = max(*s[x].a[0][0], *s[x].a[1][0]);
                Acc old1 = *s[x].a[0][0];
                g[p][0] -= old0;
                g[p][1] -= old1;
                pull(x);
                g[p][0] += max(*s[x].a[0][0], *s[x].a[1][0]);
                g[p][1] += *s[x].a[0][0];
            }
            x = fa[x];
        }
        pull(x);
    }

    Acc answer() const
    {
        // 无参数；返回当前整棵树最大权独立集的权值，允许选择空集。
        int r = tr.getRoot();
        return max(*s[r].a[0][0], *s[r].a[1][0]);
    }
};

template <long long P> class TreePathComposite
{
public:
    struct Func
    {
        long long a = 1, b = 0; // 表示一次函数 a*x+b（模 P）。
    };

private:
    vector<int> fa, dep, siz, son, top, in;
    int n = 1;
    vector<Func> fw, bw; // fw、bw 分别保存区间正序与逆序复合。

    static Func compose(Func first, Func second)
    {
        // first、second 按调用先后给出；返回 second(first(x))。
        return {second.a * first.a % P, (second.a * first.b + second.b) % P};
    }

    pair<Func, Func> range(int l, int r) const
    {
        // l、r 是 HLD 序半开区间；返回正序和逆序复合。
        Func lf, rf, lb, rb;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1)
        {
            if (l & 1)
            {
                lf = compose(lf, fw[l]);
                lb = compose(bw[l], lb);
                l++;
            }
            if (r & 1)
            {
                --r;
                rf = compose(fw[r], rf);
                rb = compose(rb, bw[r]);
            }
        }
        return {compose(lf, rf), compose(rb, lb)};
    }

public:
    TreePathComposite(const vector<vector<int>> &g, const vector<Func> &f, int root = 0)
        : fa(g.size(), -1), dep(g.size()), siz(g.size(), 1), son(g.size(), -1),
          top(g.size()), in(g.size())
    {
        // g 是非空树，f[x] 是点 x 的一次函数，root 是 HLD 根。
        assert(g.size() == f.size());
        vector<int> order{root};
        for (int i = 0; i < (int)order.size(); i++)
        {
            int u = order[i];
            for (int v : g[u]) if (v != fa[u]) fa[v] = u, dep[v] = dep[u] + 1, order.push_back(v);
        }
        for (int i = (int)order.size() - 1; i > 0; i--)
        {
            int u = order[i], p = fa[u];
            siz[p] += siz[u];
            if (son[p] < 0 || siz[u] > siz[son[p]]) son[p] = u;
        }
        int timer = 0;
        for (int u : order)
        {
            if (u != root && son[fa[u]] == u) continue;
            for (int v = u; v >= 0; v = son[v]) top[v] = u, in[v] = timer++;
        }
        while (n < (int)g.size()) n <<= 1;
        fw.assign(2 * n, {});
        bw.assign(2 * n, {});
        for (int u = 0; u < (int)g.size(); u++) fw[n + in[u]] = bw[n + in[u]] = f[u];
        for (int i = n - 1; i; i--)
        {
            fw[i] = compose(fw[2 * i], fw[2 * i + 1]);
            bw[i] = compose(bw[2 * i + 1], bw[2 * i]);
        }
    }

    void set(int u, Func f)
    {
        // u 是点编号，f 是新函数；替换点函数，无返回值。
        int p = n + in[u];
        fw[p] = bw[p] = f;
        while (p >>= 1)
        {
            fw[p] = compose(fw[2 * p], fw[2 * p + 1]);
            bw[p] = compose(bw[2 * p + 1], bw[2 * p]);
        }
    }

    Func path(int u, int v) const
    {
        // u、v 是路径端点；返回从 u 到 v 依次调用点函数的复合函数。
        Func left, right;
        while (top[u] != top[v])
        {
            if (dep[top[u]] >= dep[top[v]])
            {
                auto [forward, backward] = range(in[top[u]], in[u] + 1);
                left = compose(left, backward);
                u = fa[top[u]];
            }
            else
            {
                auto [forward, backward] = range(in[top[v]], in[v] + 1);
                right = compose(forward, right);
                v = fa[top[v]];
            }
        }
        if (dep[u] >= dep[v])
        {
            auto [forward, backward] = range(in[v], in[u] + 1);
            left = compose(left, backward);
        }
        else
        {
            auto [forward, backward] = range(in[u], in[v] + 1);
            right = compose(forward, right);
        }
        return compose(left, right);
    }

    long long apply(int u, int v, long long x) const
    {
        // u、v 是路径端点，x 是初值；返回路径复合在 x 处的值。
        Func f = path(u, v);
        return (f.a * x + f.b) % P;
    }
};

signed main()
{
    int n, q; cin >> n >> q;
    vector<long long> a(n);
    for (long long &x : a) cin >> x;
    vector<vector<int>> g(n);
    for (int v = 1; v < n; v++)
    {
        int p; cin >> p;
        g[p].push_back(v);
        g[v].push_back(p);
    }
    TreePathSum sum(g, a);
    while (q--)
    {
        int op, u; cin >> op >> u;
        if (op == 0)
        {
            long long x; cin >> x;
            a[u] += x;
            sum.set(u, a[u]);
        }
        else cout << sum.subtreeSum(u) << '\n';
    }
}
