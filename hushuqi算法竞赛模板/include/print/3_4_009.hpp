#pragma once

#include "3_4_005.hpp"

template <int LOG = 17> class StaticRngLIS
{
    static constexpr int none = -1; // 部分排列中未定义的映射；性能版路径用 32 位节省工作区。

    // n 是序列长度；wt 保存海草边界编码，只需其 countLess 接口。
    int n = 0;
    Wavelet<LOG> wt;

    static vector<int> inverse(const vector<int> &p)
    {
        // p 是允许 none 的部分排列；返回其反向部分排列，none 保持为 none。
        int n = p.size();
        vector<int> q(n, none);
        for (int i = 0; i < n; i++)
        {
            if (p[i] == none)
            {
                continue;
            }
            assert(0 <= p[i] && p[i] < n && q[p[i]] == none); // 调试检查，可删
            q[p[i]] = (int)i;
        }
        return q;
    }

    static size_t workSize(int n)
    {
        // n 是正的排列阶数；返回递归乘法的工作区元素数，长度用 size_t 避免 32 位累加溢出。
        size_t ans = 1;
        while (n > 1)
        {
            ans += 2 * (size_t)n;
            n = n / 2 + n % 2;
            ans += 4 * (size_t)n;
        }
        return ans;
    }

    static void unitMongeMul(int n, int *work, const int *a, const int *b)
    {
        // n>0；a、b 是同阶排列；work 至少有 workSize(n) 项且与输入不重叠。
        // 把单位 Monge 的 min-plus 乘积对应的行映射写入 work[0,n)，无返回值。
        if (n == 1)
        {
            work[0] = 0;
            return;
        }
        int *rowToCol = work;
        work += n;
        int *colToRow = work;
        work += n;
        int mid = n / 2;

        auto solveHalf = [&](int len, auto inHalf, auto shift)
        {
            // len 是当前值域半边长度；inHalf 判定元素是否属于该半边；shift 把值压到半边内。
            int *aHead = work;
            int *aWhere = work + len;
            int *bHead = work + 2 * (size_t)len;
            int *bWhere = work + 3 * (size_t)len;
            auto split = [&](const int *src, int *head, int *where)
            {
                // src 是完整排列；head 写压缩后的子排列，where 写对应原下标。
                for (int i = 0; i < n; i++)
                {
                    if (inHalf(src[i]))
                    {
                        *head++ = (int)shift(src[i]);
                        *where++ = (int)i;
                    }
                }
            };
            split(a, aHead, aWhere);
            split(b, bHead, bWhere);
            unitMongeMul(len, work + 4 * (size_t)len, aHead, bHead);
            int *part = work + 4 * (size_t)len;
            for (int i = 0; i < len; i++)
            {
                int row = aWhere[i];
                int col = bWhere[part[i]];
                rowToCol[row] = (int)col;
                colToRow[col] = (int)row;
            }
        };
        solveHalf(mid, [=](int x)
        {
            return x < mid;
        }, [](int x)
        {
            return x;
        });
        solveHalf(n - mid, [=](int x)
        {
            return x >= mid;
        }, [=](int x)
        {
            return x - mid;
        });

        int row = n;
        auto goRight = [&](array<int, 2> &it)
        {
            // it 是当前边界游标；向右越过一列并维护其平衡量。
            if (b[it[1]] < mid)
            {
                if (colToRow[it[1]] >= row)
                {
                    it[0]++;
                }
            }
            else if (colToRow[it[1]] < row)
            {
                it[0]++;
            }
            it[1]++;
        };
        auto goUp = [&](array<int, 2> &it)
        {
            // it 是当前边界游标；把行边界上移一格并维护其平衡量。
            if (a[row] < mid)
            {
                if (rowToCol[row] >= it[1])
                {
                    it[0]--;
                }
            }
            else if (rowToCol[row] < it[1])
            {
                it[0]--;
            }
        };
        array<int, 2> neg{}, pos{}; // 两维依次为边界相对平衡量、当前列；保存两条单调边界。
        while (row)
        {
            while (pos[1] != n)
            {
                auto next = pos;
                goRight(next);
                if (next[0] != 0)
                {
                    break;
                }
                pos = next;
            }
            row--;
            goUp(neg);
            goUp(pos);
            while (neg[0] != 0)
            {
                goRight(neg);
            }
            if (neg[1] > pos[1])
            {
                rowToCol[row] = (int)pos[1];
            }
        }
    }

    static vector<int> partMongeMul(vector<int> a, vector<int> b)
    {
        // a、b 是同阶非空部分排列，空位为 none；返回对应 Monge 乘积的部分排列。
        int n = a.size();
        vector<int> aInv = inverse(a), bInv = inverse(b);
        swap(b, bInv);
        vector<int> aMap, bMap;
        for (int i = n - 1; i >= 0; i--)
        {
            if (a[i] != none)
            {
                aMap.push_back((int)i);
                a[n - (int)aMap.size()] = a[i];
            }
        }
        reverse(aMap.begin(), aMap.end());
        int ptr = 0;
        for (int i = 0; i < n; i++)
        {
            if (aInv[i] == none)
            {
                a[ptr++] = (int)i;
            }
        }
        for (int i = 0; i < n; i++)
        {
            if (b[i] != none)
            {
                b[(int)bMap.size()] = b[i];
                bMap.push_back((int)i);
            }
        }
        ptr = bMap.size();
        for (int i = 0; i < n; i++)
        {
            if (bInv[i] == none)
            {
                b[ptr++] = (int)i;
            }
        }

        vector<int> work(workSize(n));
        unitMongeMul(n, work.data(), a.data(), b.data());
        vector<int> c(n, none);
        int start = n - (int)aMap.size();
        for (int i = 0; i < (int)aMap.size(); i++)
        {
            int to = work[start + i];
            if (to < (int)bMap.size())
            {
                c[aMap[i]] = bMap[to];
            }
        }
        return c;
    }

    static vector<int> seaweed(const vector<int> &p)
    {
        // p 是值域 [0,n) 的排列；返回其半局部 LIS 的 seaweed 部分排列编码。
        int n = p.size();
        if (n <= 1)
        {
            return vector<int>(n, none);
        }
        int mid = n / 2;
        vector<int> low, high, lowMap, highMap;
        low.reserve(mid);
        high.reserve(n - mid);
        lowMap.reserve(mid);
        highMap.reserve(n - mid);
        for (int i = 0; i < n; i++)
        {
            assert(0 <= p[i] && p[i] < n); // 调试检查，可删
            if (p[i] < mid)
            {
                low.push_back(p[i]);
                lowMap.push_back((int)i);
            }
            else
            {
                high.push_back((int)(p[i] - mid));
                highMap.push_back((int)i);
            }
        }
        low = seaweed(low);
        high = seaweed(high);
        vector<int> lowPad(n), highPad(n);
        iota(lowPad.begin(), lowPad.end(), (int)0);
        iota(highPad.begin(), highPad.end(), (int)0);
        for (int i = 0; i < mid; i++)
        {
            if (low[i] == none)
            {
                lowPad[lowMap[i]] = none;
            }
            else
            {
                lowPad[lowMap[i]] = lowMap[low[i]];
            }
        }
        for (int i = 0; mid + i < n; i++)
        {
            if (high[i] == none)
            {
                highPad[highMap[i]] = none;
            }
            else
            {
                highPad[highMap[i]] = highMap[high[i]];
            }
        }
        return partMongeMul(move(lowPad), move(highPad));
    }

  public:
    explicit StaticRngLIS(vector<int> a = {}, bool strict = true)
    {
        // a 是任意静态整数序列；strict 为真查严格递增，为假查非下降长度。
        init(move(a), strict);
    }

    void init(vector<int> a, bool strict = true)
    {
        // a 是新静态序列；strict 选择严格递增或非下降；覆盖全部旧状态，无返回值。
        assert(a.size() < (1ULL << 31) - 1); // 调试检查，可删
        n = (int)a.size();
        assert((unsigned long long)n < (1ULL << LOG)); // 调试检查，可删
        vector<int> order(n);
        iota(order.begin(), order.end(), (int)0);
        sort(order.begin(), order.end(), [&](int i, int j)
        {
            // i、j 是原下标；返回排秩顺序，相等值在严格 / 非下降模式按下标降序 / 升序。
            if (a[i] != a[j]) return a[i] < a[j];
            return strict ? i > j : i < j;
        });
        vector<int> q(n);
        for (int i = 0; i < n; i++)
        {
            q[order[i]] = (int)i;
        }
        vector<int> edge = seaweed(q);
        vector<int> code(n);
        for (int i = 0; i < n; i++)
        {
            code[i] = edge[i] == none ? n : edge[i];
        }
        wt.init(move(code));
    }

    int query(int l, int r) const
    {
        // l、r 是半开区间端点；返回所选模式的最长子序列长度，空区间为 0。
        // 递减 / 非上升：先 reverse(a)，保持 strict 选择，对原区间改查 [n-r,n-l)。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        return r - l - wt.countLess(l, n, r);
    }
};
