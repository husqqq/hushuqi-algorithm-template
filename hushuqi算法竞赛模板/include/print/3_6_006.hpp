#include "support/base.hpp"

template <class K, class V> using HashMap = unordered_map<K, V, CustomHash>;
template <class K> using HashSet = unordered_set<K, CustomHash>;

template <class K, class V, int N, class H = CustomHash> class FixedHashMap
{
    static_assert(1 <= N && N <= 30);
    static constexpr int CAP = 1U << N;
    static constexpr int MASK = CAP - 1;

    int cnt = 0;
    unique_ptr<K[]> key{new K[CAP]};
    unique_ptr<V[]> val{new V[CAP]};
    vector<unsigned long long> used = vector<unsigned long long>((CAP + 63) / 64);
    H hs;

    bool test(int p) const
    {
        // p 是槽编号；返回该槽是否已被占用。
        return used[p >> 6] >> (p & 63) & 1ULL;
    }

    void set(int p)
    {
        // p 是空槽编号；把该槽标记为占用。
        used[p >> 6] |= 1ULL << (p & 63);
    }

    int slot(const K &x) const
    {
        // x 是待查键；返回经过随机哈希与乘法混合后的初始槽。
        unsigned long long h = (unsigned long long)hs(x);
        h *= 11400714819323198485ULL;
        return h >> (64 - N);
    }

    int findPos(const K &x) const
    {
        // x 是待查键；返回保存 x 的槽，或其探测链上的第一个空槽。
        int p = slot(x);
        while (test(p) && key[p] != x)
        {
            p = (p + 1) & MASK;
        }
        return p;
    }

  public:
    V &operator[](const K &x)
    {
        assert(cnt < CAP); // 调试检查，可删；容量必须严格大于不同键数。
        int p = findPos(x);
        if (!test(p))
        {
            set(p);
            key[p] = x;
            val[p] = V{};
            cnt++;
        }
        return val[p];
    }

    V *find(const K &x)
    {
        int p = findPos(x);
        return test(p) ? &val[p] : nullptr;
    }

    const V *find(const K &x) const
    {
        int p = findPos(x);
        return test(p) ? &val[p] : nullptr;
    }

    bool contains(const K &x) const
    {
        return find(x) != nullptr;
    }

    V get(const K &x) const
    {
        auto p = find(x);
        return p ? *p : V{};
    }

    int size() const
    {
        return cnt;
    }

    void clear()
    {
        fill(used.begin(), used.end(), 0);
        cnt = 0;
    }
};

template <class K, int N, class H = CustomHash> class FixedHashSet
{
    static_assert(1 <= N && N <= 30);
    static constexpr int CAP = 1U << N;
    static constexpr int MASK = CAP - 1;

    // cnt 是当前元素数，key 保存槽内键，used 标记槽是否占用。
    int cnt = 0;
    unique_ptr<K[]> key{new K[CAP]};
    vector<unsigned long long> used = vector<unsigned long long>((CAP + 63) / 64);
    H hs;

    bool test(int p) const
    {
        // p 是槽编号；返回该槽是否已被占用。
        return used[p >> 6] >> (p & 63) & 1ULL;
    }

    void set(int p)
    {
        // p 是空槽编号；把该槽标记为占用。
        used[p >> 6] |= 1ULL << (p & 63);
    }

    int slot(const K &x) const
    {
        // x 是待查键；返回经过随机哈希与乘法混合后的初始槽。
        unsigned long long h = (unsigned long long)hs(x);
        h *= 11400714819323198485ULL;
        return h >> (64 - N);
    }

    int findPos(const K &x) const
    {
        // x 是待查键；返回保存 x 的槽，或其探测链上的第一个空槽。
        int p = slot(x);
        while (test(p) && key[p] != x)
        {
            p = (p + 1) & MASK;
        }
        return p;
    }

  public:
    bool insert(const K &x)
    {
        // x 是待插入键；首次插入返回 true，已经存在返回 false。
        assert(cnt < CAP); // 调试检查，可删；容量必须严格大于不同键数。
        int p = findPos(x);
        if (test(p))
        {
            return false;
        }
        set(p);
        key[p] = x;
        cnt++;
        return true;
    }

    bool contains(const K &x) const
    {
        // x 是待查询键；返回集合是否包含 x。
        return test(findPos(x));
    }

    int size() const
    {
        // 返回当前不同键的数量。
        return cnt;
    }

    void clear()
    {
        // 清空全部键，保留已经分配的固定容量。
        fill(used.begin(), used.end(), 0);
        cnt = 0;
    }
};
