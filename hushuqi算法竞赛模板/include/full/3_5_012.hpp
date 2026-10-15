#include <bits/stdc++.h>
using namespace std;
#define int long long

template <class T, int LOG = 20> class PersistQueue
{
    static_assert(1 <= LOG && LOG < 63);

    struct Node
    {
        // value 是本次入队的值；up[k] 是沿历史尾链向前 2^k 步的节点编号。
        T value{};
        array<int32_t, LOG> up{};
    };

    struct Version
    {
        // tail 是逻辑队尾节点，len 是该版本未被弹出的逻辑元素数。
        int32_t tail = 0, len = 0;
    };

    // nodes[0] 是空哨兵；versions[0] 是初始空队列。
    vector<Node> nodes{Node{}};
    vector<Version> versions{Version{}};

    int anc(int p, int distance) const
    {
        // p 是非空尾节点，distance 是向前跳的步数；返回对应祖先节点编号。
        assert(0 < p && p < (int)nodes.size()); // 调试检查，可删
        assert(0 <= distance && (unsigned long long)distance < (1ULL << LOG)); // 调试检查，可删
        for (int k = 0; distance; k++, distance >>= 1)
        {
            if (distance & 1)
            {
                p = nodes[p].up[k];
            }
        }
        return p;
    }

  public:
    explicit PersistQueue(int cap = 0)
    {
        // cap 是预计总操作数，仅用于预留节点和版本容量；默认 LOG=20 覆盖 Q<=5e5。
        assert(0 <= cap && (unsigned long long)cap < (1ULL << LOG)
               && cap <= numeric_limits<int32_t>::max()); // 调试检查，可删
        nodes.reserve(cap + 1);
        versions.reserve(cap + 1);
    }

    int emptyVer() const
    {
        // 返回初始空队列的版本编号。
        return 0;
    }

    int verCnt() const
    {
        // 返回当前已经创建的版本数，包含初始空版本。
        return (int)versions.size();
    }

    int size(int version) const
    {
        // version 是版本编号；返回该版本的逻辑队列长度。
        assert(0 <= version && version < (int)versions.size()); // 调试检查，可删
        return versions[version].len;
    }

    bool empty(int version) const
    {
        // version 是版本编号；返回该版本是否为空。
        return size(version) == 0;
    }

    int push(int version, const T &value)
    {
        // version 是旧版本，value 是新队尾元素；返回追加后的新版本编号。
        assert(0 <= version && version < (int)versions.size()); // 调试检查，可删
        int p = (int)nodes.size();
        assert((unsigned long long)p < (1ULL << LOG)
               && p <= numeric_limits<int32_t>::max()); // 调试检查，可删
        Node cur;
        cur.value = value;
        cur.up[0] = versions[version].len ? versions[version].tail : 0;
        for (int k = 1; k < LOG; k++)
        {
            cur.up[k] = nodes[cur.up[k - 1]].up[k - 1];
        }
        nodes.push_back(move(cur));
        int len = versions[version].len + 1;
        assert(len <= numeric_limits<int32_t>::max()); // 调试检查，可删
        versions.push_back({(int32_t)p, (int32_t)len});
        return (int)versions.size() - 1;
    }

    T front(int version) const
    {
        // version 是非空版本编号；返回该版本的队首值，不生成新版本。
        assert(0 <= version && version < (int)versions.size()); // 调试检查，可删
        assert(versions[version].len > 0); // 调试检查，可删
        int p = anc(versions[version].tail, versions[version].len - 1);
        return nodes[p].value;
    }

    int pop(int version)
    {
        // version 是非空版本编号；删除其队首并返回新版本编号。
        assert(0 <= version && version < (int)versions.size()); // 调试检查，可删
        assert(versions[version].len > 0); // 调试检查，可删
        versions.push_back({versions[version].tail, (int32_t)(versions[version].len - 1)});
        return (int)versions.size() - 1;
    }
};
