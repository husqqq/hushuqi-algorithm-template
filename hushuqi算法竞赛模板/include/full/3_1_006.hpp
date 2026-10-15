#include "3_1_001.hpp"

struct RangeDSU
{
    struct D
    {
        // f[x]<0 表示 x 是块集合根且大小为 -f[x]；否则 f[x] 是父亲。
        vector<int> f;
        D(int n = 0) : f(n, -1)
        {
            // n 是本层可作为块起点的位置数。
        }
        int find(int x)
        {
            // x 是本层块起点；压缩父链并返回块集合根。
            return f[x] < 0 ? x : f[x] = find(f[x]);
        }
        pair<int, int> merge(int x, int y)
        {
            // x、y 是本层块起点；返回 {新根,被吸收根}，已经同集时返回 {-1,-1}。
            x = find(x);
            y = find(y);
            if (x == y)
            {
                return {-1, -1};
            }
            if (f[x] > f[y])
            {
                swap(x, y);
            }
            f[x] += f[y];
            f[y] = x;
            return {x, y};
        }
    };

    // n 是原序列长度；d[k] 维护所有长度为 2^k 的块之间的等价关系。
    int n;
    vector<D> d;

    RangeDSU(int n) : n(n)
    {
        // n 是原序列长度；预建每个倍增层的块并查集。
        assert(n >= 0); // 调试检查，可删
        int m = max<int>(1, bit_width((unsigned long long)n));
        for (int k = 0; k < m; k++)
        {
            d.emplace_back(n - (1LL << k) + 1);
        }
    }

    template <class F> void mergeBlock(int k, int x, int y, F &f)
    {
        // k 是块层级，x、y 是两个块起点；首次连通单点块时调用 f(新根,被吸收根)。
        auto [a, b] = d[k].merge(x, y);
        if (a < 0)
        {
            return;
        }
        if (!k)
        {
            return f(a, b);
        }
        int h = 1LL << (k - 1);
        mergeBlock(k - 1, x, y, f);
        mergeBlock(k - 1, x + h, y + h, f);
    }

    template <class F> void merge(int x, int y, int len, F f)
    {
        // 合并 [x,x+len) 与 [y,y+len)；每个新单点合并调用一次 f(新根,被吸收根)。
        assert(0 <= x && x <= n && 0 <= y && y <= n && 0 <= len && len <= n - x && len <= n - y); // 调试检查，可删
        if (!len)
        {
            return;
        }
        int k = bit_width((unsigned long long)len) - 1;
        int z = 1LL << k;
        mergeBlock(k, x, y, f);
        mergeBlock(k, x + len - z, y + len - z, f);
    }
};
