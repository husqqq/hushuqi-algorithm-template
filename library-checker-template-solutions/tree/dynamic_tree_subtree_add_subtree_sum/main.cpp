

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

template <class Acc = long long> class DynamicSubtreeAddSum
{
    struct Node
    {
        int p = 0, ch[2]{};
        bool rev = false;
        int size = 1, virtualSize = 0;
        Acc value = 0, sum = 0, virtualSum = 0;
        Acc added = 0, parentAdded = 0;
    };
    vector<Node> t;

    bool auxiliaryRoot(int x) const
    {
        int p = t[x].p;
        return !p || (t[p].ch[0] != x && t[p].ch[1] != x);
    }
    void applyNode(int x, Acc delta)
    {
        if (!x) return;
        t[x].value += delta;
        t[x].sum += delta * t[x].size;
        t[x].virtualSum += delta * t[x].virtualSize;
        t[x].added += delta;
    }
    void push(int x)
    {
        if (!x) return;
        if (t[x].p)
        {
            applyNode(x, t[t[x].p].added - t[x].parentAdded);
            t[x].parentAdded = t[t[x].p].added;
        }
        if (t[x].rev)
        {
            if (t[x].ch[0]) t[t[x].ch[0]].rev = !t[t[x].ch[0]].rev;
            if (t[x].ch[1]) t[t[x].ch[1]].rev = !t[t[x].ch[1]].rev;
            swap(t[x].ch[0], t[x].ch[1]);
            t[x].rev = false;
        }
    }
    void pull(int x)
    {
        int l = t[x].ch[0], r = t[x].ch[1];
        push(l), push(r);
        t[x].size = 1 + t[l].size + t[r].size + t[x].virtualSize;
        t[x].sum = t[x].value + t[l].sum + t[r].sum + t[x].virtualSum;
    }
    void rotate(int x)
    {
        int y = t[x].p, z = t[y].p, d = t[y].ch[1] == x;
        int middle = t[x].ch[d ^ 1];
        push(middle);
        t[y].ch[d] = middle;
        if (middle)
        {
            t[middle].p = y;
            t[middle].parentAdded = t[y].added;
        }
        t[x].ch[d ^ 1] = y;
        t[x].p = z;
        t[x].parentAdded = t[z].added;
        if (z)
        {
            if (t[z].ch[0] == y) t[z].ch[0] = x;
            if (t[z].ch[1] == y) t[z].ch[1] = x;
        }
        t[y].p = x;
        t[y].parentAdded = t[x].added;
        pull(y), pull(x);
    }
    void splay(int x)
    {
        while (!auxiliaryRoot(x))
        {
            int y = t[x].p;
            if (auxiliaryRoot(y))
            {
                push(y), push(x);
                rotate(x);
            }
            else
            {
                int z = t[y].p;
                push(z), push(y), push(x);
                bool same = (t[z].ch[0] == y) == (t[y].ch[0] == x);
                if (same) rotate(y), rotate(x);
                else rotate(x), rotate(x);
            }
        }
        push(x);
    }
    void access(int x)
    {
        int previous = 0;
        for (int current = x; current; current = t[current].p)
        {
            splay(current);
            int old = t[current].ch[1];
            if (old)
            {
                push(old);
                t[current].virtualSize += t[old].size;
                t[current].virtualSum += t[old].sum;
            }
            t[current].ch[1] = previous;
            if (previous)
            {
                push(previous);
                t[current].virtualSize -= t[previous].size;
                t[current].virtualSum -= t[previous].sum;
            }
            pull(current);
            previous = current;
        }
        splay(x);
    }
    void makeRootInternal(int x)
    {
        access(x);
        t[x].rev = !t[x].rev;
        push(x);
    }

public:
    explicit DynamicSubtreeAddSum(const vector<Acc> &value = {}) : t(value.size() + 1)
    {
        t[0].size = 0;
        for (int i = 0; i < (int)value.size(); i++) t[i + 1].value = t[i + 1].sum = value[i];
    }
    bool link(int child, int parent)
    {
        // child、parent 属于不同树；连边并返回是否成功。
        child++, parent++;
        access(child);
        access(parent);
        t[parent].ch[1] = child;
        t[child].p = parent;
        t[child].parentAdded = t[parent].added;
        pull(parent);
        return true;
    }
    bool cut(int u, int v)
    {
        // u、v 是直接边端点；删除该边并返回是否成功。
        u++, v++;
        makeRootInternal(u);
        access(v);
        if (t[v].ch[0] != u || t[u].ch[1]) return false;
        t[v].ch[0] = 0;
        t[u].p = 0;
        t[u].parentAdded = 0;
        pull(v);
        return true;
    }
    void addTree(int root, Acc delta)
    {
        // root 所在连通树当前已按 root 暴露；给整棵树加 delta。
        root++;
        access(root);
        applyNode(root, delta);
    }
    Acc treeSum(int root)
    {
        // 返回 root 所在整棵树的点权和。
        root++;
        access(root);
        return t[root].sum;
    }
};

template <class Acc = long long> class DynamicSubtreeAddSumExact
{
    struct Node
    {
        Node *p = nullptr, *ch[2]{nullptr, nullptr};
        Acc value = 0, sum = 0, added = 0, cancel = 0;
        int size = 0, lightSize = 0;
        Acc lightSum = 0;
        bool rev = false;
        void apply(Acc delta)
        {
            value += delta;
            sum += (Acc)size * delta;
            lightSum += (Acc)lightSize * delta;
            added += delta;
        }
    } none;
    vector<Node> nodes;

    void flush(Node *x)
    {
        if (x->p != &none)
        {
            x->apply(x->p->added - x->cancel);
            x->cancel = x->p->added;
        }
        if (x->rev)
        {
            x->ch[0]->rev = !x->ch[0]->rev;
            x->ch[1]->rev = !x->ch[1]->rev;
            swap(x->ch[0], x->ch[1]);
            x->rev = false;
        }
    }
    void fetch(Node *x)
    {
        flush(x->ch[0]);
        flush(x->ch[1]);
        x->size = 1 + x->ch[0]->size + x->ch[1]->size + x->lightSize;
        x->sum = x->value + x->ch[0]->sum + x->ch[1]->sum + x->lightSum;
    }
    bool isRoot(Node *x) const
    {
        return x->p == &none || (x->p->ch[0] != x && x->p->ch[1] != x);
    }
    void rotate(Node *x, int direction)
    {
        Node *y = x->p, *z = y->p, *w = x->ch[direction];
        flush(w);
        y->ch[!direction] = w;
        w->p = y;
        w->cancel = y->added;
        x->ch[direction] = y;
        x->p = z;
        x->cancel = z->added;
        if (z->ch[0] == y) z->ch[0] = x;
        if (z->ch[1] == y) z->ch[1] = x;
        y->p = x;
        y->cancel = x->added;
        fetch(y), fetch(x);
    }
    void splay(Node *x)
    {
        while (!isRoot(x))
        {
            Node *y = x->p;
            if (isRoot(y))
            {
                flush(y), flush(x);
                rotate(x, y->ch[0] == x);
            }
            else
            {
                Node *z = y->p;
                flush(z), flush(y), flush(x);
                bool same = (z->ch[0] == y) == (y->ch[0] == x);
                if (same) rotate(y, z->ch[0] == y), rotate(x, y->ch[0] == x);
                else rotate(x, y->ch[0] == x), rotate(x, z->ch[0] == x);
            }
        }
        flush(x);
    }
    void expose(Node *x)
    {
        Node *previous = &none;
        for (Node *current = x; current != &none; current = current->p)
        {
            splay(current);
            if (current->ch[1] != &none)
            {
                flush(current->ch[1]);
                current->lightSize += current->ch[1]->size;
                current->lightSum += current->ch[1]->sum;
            }
            current->ch[1] = previous;
            if (current->ch[1] != &none)
            {
                flush(current->ch[1]);
                current->lightSize -= current->ch[1]->size;
                current->lightSum -= current->ch[1]->sum;
            }
            fetch(current);
            previous = current;
        }
        splay(x);
    }
    void evert(Node *x)
    {
        expose(x);
        x->rev = !x->rev;
        flush(x);
    }
    void linkNode(Node *child, Node *parent)
    {
        expose(child);
        expose(parent);
        parent->ch[1] = child;
        child->p = parent;
        child->cancel = parent->added;
        fetch(parent);
    }
    void cutNode(Node *x)
    {
        expose(x);
        flush(x->ch[0]);
        x->ch[0]->p = &none;
        x->ch[0] = &none;
        fetch(x);
    }

public:
    explicit DynamicSubtreeAddSumExact(const vector<Acc> &value = {}) : nodes(value.size())
    {
        none.p = &none;
        none.ch[0] = none.ch[1] = &none;
        none.size = none.lightSize = 0;
        for (int i = 0; i < (int)value.size(); i++)
        {
            nodes[i].p = &none;
            nodes[i].ch[0] = nodes[i].ch[1] = &none;
            nodes[i].value = nodes[i].sum = value[i];
            fetch(&nodes[i]);
        }
    }
    bool link(int u, int v)
    {
        evert(&nodes[u]);
        linkNode(&nodes[u], &nodes[v]);
        return true;
    }
    bool cut(int u, int v)
    {
        evert(&nodes[u]);
        expose(&nodes[v]);
        if (nodes[v].ch[0] != &nodes[u] || nodes[u].ch[1] != &none) return false;
        cutNode(&nodes[v]);
        return true;
    }
    void add(int u, Acc delta)
    {
        evert(&nodes[u]);
        nodes[u].apply(delta);
    }
    Acc componentSum(int u)
    {
        evert(&nodes[u]);
        return nodes[u].sum;
    }
};

signed main()
{
    int n, q; cin >> n >> q;
    vector<long long> a(n);
    for (auto &x : a) cin >> x;
    DynamicSubtreeAddSumExact<long long> tree(a);
    for (int i = 1; i < n; i++)
    {
        int u, v; cin >> u >> v;
        tree.link(u, v);
    }
    while (q--)
    {
        int type; cin >> type;
        if (type == 0)
        {
            int u, v, w, x; cin >> u >> v >> w >> x;
            tree.cut(u, v);
            tree.link(w, x);
        }
        else
        {
            int v, p; cin >> v >> p;
            tree.cut(v, p);
            if (type == 1)
            {
                long long x; cin >> x;
                tree.add(v, x);
            }
            else cout << tree.componentSum(v) << '\n';
            tree.link(v, p);
        }
    }
}
