#pragma once

#include "3_4_005.hpp"

template <int LOG = 17> class StaticRngLIS
{
    static constexpr int32_t none = -1;

    // n 是原排列长度；wt 保存海草边界编码后的静态顺序统计结构。
    int n = 0;
    Wavelet<LOG> wt;

    static vector<int32_t> inverse(const vector<int32_t> &p)
    {
        // p 是允许 none 的部分排列；返回其反向部分排列，none 保持为 none。
        int n = p.size();
        vector<int32_t> q(n, none);
        for (int i = 0; i < n; i++)
        {
            if (p[i] == none)
            {
                continue;
            }
            assert(0 <= p[i] && p[i] < n && q[p[i]] == none); // 调试检查，可删
            q[p[i]] = (int32_t)i;
        }
        return q;
    }

    static int workSize(int n)
    {
        // n 是单位 Monge 排列阶数；返回一次递归乘法所需的 int32_t 工作区长度。
        int ans = 1;
        while (n > 1)
        {
            ans += 2 * n;
            n = (n + 1) / 2;
            ans += 4 * n;
        }
        return ans;
    }

    static void unitMongeMul(int n, int32_t *work, const int32_t *a, const int32_t *b)
    {
        // n 是排列阶数，work 是足够大的工作区，a、b 是两个单位 Monge 排列；把乘积行映射写入 work[0,n)。
        if (n == 1)
        {
            work[0] = 0;
            return;
        }
        int32_t *rowToCol = work;
        work += n;
        int32_t *colToRow = work;
        work += n;
        int mid = n / 2;

        auto solveHalf = [&](int len, auto inHalf, auto shift)
        {
            // len 是当前值域半边长度；inHalf 判定元素是否属于该半边；shift 把值压到半边内。
            int32_t *aHead = work;
            int32_t *aWhere = work + len;
            int32_t *bHead = work + 2 * len;
            int32_t *bWhere = work + 3 * len;
            auto split = [&](const int32_t *src, int32_t *head, int32_t *where)
            {
                // src 是完整排列；head 写压缩后的子排列，where 写对应原下标。
                for (int i = 0; i < n; i++)
                {
                    if (inHalf(src[i]))
                    {
                        *head++ = (int32_t)shift(src[i]);
                        *where++ = (int32_t)i;
                    }
                }
            };
            split(a, aHead, aWhere);
            split(b, bHead, bWhere);
            unitMongeMul(len, work + 4 * len, aHead, bHead);
            int32_t *part = work + 4 * len;
            for (int i = 0; i < len; i++)
            {
                int row = aWhere[i];
                int col = bWhere[part[i]];
                rowToCol[row] = (int32_t)col;
                colToRow[col] = (int32_t)row;
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

        struct Cursor
        {
            // delta 是当前 Monge 边界相对平衡，col 是当前列位置。
            int delta = 0, col = 0;
        };
        int row = n;
        auto goRight = [&](Cursor &it)
        {
            // it 是当前边界游标；向右越过一列并维护其平衡量。
            if (b[it.col] < mid)
            {
                if (colToRow[it.col] >= row)
                {
                    it.delta++;
                }
            }
            else if (colToRow[it.col] < row)
            {
                it.delta++;
            }
            it.col++;
        };
        auto goUp = [&](Cursor &it)
        {
            // it 是当前边界游标；把行边界上移一格并维护其平衡量。
            if (a[row] < mid)
            {
                if (rowToCol[row] >= it.col)
                {
                    it.delta--;
                }
            }
            else if (rowToCol[row] < it.col)
            {
                it.delta--;
            }
        };
        Cursor neg, pos;
        while (row)
        {
            while (pos.col != n)
            {
                Cursor next = pos;
                goRight(next);
                if (next.delta != 0)
                {
                    break;
                }
                pos = next;
            }
            row--;
            goUp(neg);
            goUp(pos);
            while (neg.delta != 0)
            {
                goRight(neg);
            }
            if (neg.col > pos.col)
            {
                rowToCol[row] = (int32_t)pos.col;
            }
        }
    }

    static vector<int32_t> partMongeMul(vector<int32_t> a, vector<int32_t> b)
    {
        // a、b 是同阶部分 Monge 排列；返回它们复合后的部分排列。
        int n = a.size();
        vector<int32_t> aInv = inverse(a), bInv = inverse(b);
        swap(b, bInv);
        vector<int32_t> aMap, bMap;
        for (int i = n - 1; i >= 0; i--)
        {
            if (a[i] != none)
            {
                aMap.push_back((int32_t)i);
                a[n - (int)aMap.size()] = a[i];
            }
        }
        reverse(aMap.begin(), aMap.end());
        int ptr = 0;
        for (int i = 0; i < n; i++)
        {
            if (aInv[i] == none)
            {
                a[ptr++] = (int32_t)i;
            }
        }
        for (int i = 0; i < n; i++)
        {
            if (b[i] != none)
            {
                b[(int)bMap.size()] = b[i];
                bMap.push_back((int32_t)i);
            }
        }
        ptr = bMap.size();
        for (int i = 0; i < n; i++)
        {
            if (bInv[i] == none)
            {
                b[ptr++] = (int32_t)i;
            }
        }

        vector<int32_t> work(workSize(n));
        unitMongeMul(n, work.data(), a.data(), b.data());
        vector<int32_t> c(n, none);
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

    static vector<int32_t> seaweed(const vector<int32_t> &p)
    {
        // p 是值域 [0,n) 的排列；返回其半局部 LIS 的 seaweed 部分排列编码。
        int n = p.size();
        if (n <= 1)
        {
            return vector<int32_t>(n, none);
        }
        int mid = n / 2;
        vector<int32_t> low, high, lowMap, highMap;
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
                lowMap.push_back((int32_t)i);
            }
            else
            {
                high.push_back((int32_t)(p[i] - mid));
                highMap.push_back((int32_t)i);
            }
        }
        low = seaweed(low);
        high = seaweed(high);
        vector<int32_t> lowPad(n), highPad(n);
        iota(lowPad.begin(), lowPad.end(), int32_t(0));
        iota(highPad.begin(), highPad.end(), int32_t(0));
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
    explicit StaticRngLIS(vector<int> p = {})
    {
        // p 是值域 [0,n) 的静态排列；构造其区间严格 LIS 查询结构。
        init(move(p));
    }

    void init(vector<int> p)
    {
        // p 是值域 [0,n) 的新静态排列；重建结构，不保留旧查询状态。
        n = p.size();
        assert(n <= numeric_limits<int32_t>::max()); // 调试检查，可删
        assert((unsigned long long)n < (1ULL << LOG)); // 调试检查，可删
        vector<char> used(n);
        vector<int32_t> q(n);
        for (int i = 0; i < n; i++)
        {
            assert(0 <= p[i] && p[i] < n && !used[p[i]]); // 调试检查，可删
            used[p[i]] = true;
            q[i] = (int32_t)p[i];
        }
        vector<int32_t> edge = seaweed(q);
        vector<int> code(n);
        for (int i = 0; i < n; i++)
        {
            code[i] = edge[i] == none ? n : edge[i];
        }
        wt.init(move(code));
    }

    int query(int l, int r) const
    {
        // l、r 是半开区间端点；返回排列 p[l,r) 的严格最长递增子序列长度。
        assert(0 <= l && l <= r && r <= n); // 调试检查，可删
        return r - l - wt.countLess(l, n, r);
    }
};
