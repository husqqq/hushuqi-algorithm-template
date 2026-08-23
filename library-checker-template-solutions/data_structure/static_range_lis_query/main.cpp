
#include <bits/stdc++.h>
using namespace std;
#define int long long

template <int LOG = 30> struct Wavelet
{
    static_assert(1 <= LOG && LOG <= 62);

    // size 是序列长度，middle[bit] 是该层零段长度。
    int size;
    array<int, LOG> middle;
    // prefixOne[bit][i] 是该层前 i 位的一数；pre0 是零分支原值和。
    array<vector<int>, LOG> prefixOne;
    array<vector<int>, LOG> pre0;
    // prefixSum[i] 是原序列前 i 项的值和。
    vector<int> prefixSum;

    explicit Wavelet(vector<int> values = {})
    {
        // values 是位于 [0,2^LOG) 的初始序列。
        init(move(values));
    }

    void init(vector<int> values)
    {
        // values 是新的静态序列；清空旧层并重新稳定划分。
        size = values.size();
        prefixSum.assign(size + 1, 0);
        constexpr unsigned long long limit = 1ULL << LOG;
        for (int i = 0; i < size; i++)
        {
            assert(0 <= values[i] && (unsigned long long)values[i] < limit); // 调试检查，可删
            prefixSum[i + 1] = prefixSum[i] + values[i];
        }
        vector<int> next(size);
        for (int bit = LOG - 1; bit >= 0; --bit)
        {
            prefixOne[bit].assign(size + 1, 0);
            pre0[bit].assign(size + 1, 0);
            for (int i = 0; i < size; ++i)
            {
                bool one = values[i] >> bit & 1;
                prefixOne[bit][i + 1] = prefixOne[bit][i] + one;
                pre0[bit][i + 1] = pre0[bit][i] + (one ? 0 : values[i]);
            }
            middle[bit] = size - prefixOne[bit][size];
            int zero = 0, one = middle[bit];
            for (int value : values)
            {
                if (value >> bit & 1)
                {
                    next[one++] = value;
                }
                else
                {
                    next[zero++] = value;
                }
            }
            values.swap(next);
        }
    }

    int kth(int left, int right, int rank) const
    {
        // left、right 是半开区间端点，rank 从 0 开始；返回第 rank 小的原值。
        assert(0 <= left && left < right && right <= size); // 调试检查，可删
        assert(0 <= rank && rank < right - left); // 调试检查，可删
        int answer = 0;
        for (int bit = LOG - 1; bit >= 0; --bit)
        {
            int onesLeft = prefixOne[bit][left];
            int onesR = prefixOne[bit][right];
            int zeros = right - left - onesR + onesLeft;
            if (rank < zeros)
            {
                left -= onesLeft;
                right -= onesR;
            }
            else
            {
                answer |= 1LL << bit;
                rank -= zeros;
                left = middle[bit] + onesLeft;
                right = middle[bit] + onesR;
            }
        }
        return answer;
    }

    pair<int, int> countSumLT(int left, int right, int hi) const
    {
        // 返回 [left,right) 内严格小于 hi 的元素个数与元素值和。
        assert(0 <= left && left <= right && right <= size); // 调试检查，可删
        if (hi <= 0)
        {
            return {0, 0};
        }
        constexpr unsigned long long limit = 1ULL << LOG;
        if ((unsigned long long)hi >= limit)
        {
            return {right - left, prefixSum[right] - prefixSum[left]};
        }
        int count = 0;
        int sum = 0;
        for (int bit = LOG - 1; bit >= 0; --bit)
        {
            int onesLeft = prefixOne[bit][left];
            int onesR = prefixOne[bit][right];
            int zeros = right - left - onesR + onesLeft;
            if (hi >> bit & 1)
            {
                count += zeros;
                sum += pre0[bit][right] - pre0[bit][left];
                left = middle[bit] + onesLeft;
                right = middle[bit] + onesR;
            }
            else
            {
                left -= onesLeft;
                right -= onesR;
            }
        }
        return {count, sum};
    }

    int countLess(int left, int right, int hi) const
    {
        // 返回 [left,right) 内严格小于 hi 的元素个数。
        return countSumLT(left, right, hi).first;
    }

    int frequency(int left, int right, int value) const
    {
        // 返回 [left,right) 内 value 的出现次数，值域外返回 0。
        assert(0 <= left && left <= right && right <= size); // 调试检查，可删
        constexpr unsigned long long limit = 1ULL << LOG;
        if (value < 0 || (unsigned long long)value >= limit)
        {
            return 0;
        }
        return countLess(left, right, value + 1) - countLess(left, right, value);
    }

    pair<int, int> countSumLE(int left, int right, int hi) const
    {
        // 返回 [left,right) 内不大于 hi 的元素个数与元素值和。
        assert(0 <= left && left <= right && right <= size); // 调试检查，可删
        if (hi < 0)
        {
            return {0, 0};
        }
        constexpr unsigned long long limit = 1ULL << LOG;
        if ((unsigned long long)hi >= limit - 1)
        {
            return {right - left, prefixSum[right] - prefixSum[left]};
        }
        return countSumLT(left, right, hi + 1);
    }
};

template <int LOG = 30> struct KthWavelet
{
    static_assert(1 <= LOG && LOG <= 62);

    // size 是序列长度，middle[bit] 是该层零段长度；固定 32 位以节省只求第 k 小时的常驻空间。
    int32_t size = 0;
    array<int32_t, LOG> middle;
    // prefixOne[bit][i] 是该层前 i 位的一数。
    array<vector<int32_t>, LOG> prefixOne;

    explicit KthWavelet(vector<int> values = {})
    {
        // values 是位于 [0,2^LOG) 的初始序列。
        init(move(values));
    }

    void init(vector<int> values)
    {
        // values 是新的静态序列；清空旧层并重新稳定划分。
        assert(values.size() <= INT32_MAX); // 调试检查，可删
        size = (int32_t)values.size();
        constexpr unsigned long long limit = 1ULL << LOG;
        for (int value : values)
        {
            assert(0 <= value && (unsigned long long)value < limit); // 调试检查，可删
        }
        vector<int> next(size);
        for (int bit = LOG - 1; bit >= 0; --bit)
        {
            prefixOne[bit].assign(size + 1, 0);
            for (int i = 0; i < size; ++i)
            {
                prefixOne[bit][i + 1] = prefixOne[bit][i] + (values[i] >> bit & 1);
            }
            middle[bit] = size - prefixOne[bit][size];
            int zero = 0, one = middle[bit];
            for (int value : values)
            {
                if (value >> bit & 1)
                {
                    next[one++] = value;
                }
                else
                {
                    next[zero++] = value;
                }
            }
            values.swap(next);
        }
    }

    int kth(int left, int right, int rank) const
    {
        // left、right 是半开区间端点，rank 从 0 开始；返回第 rank 小的原值。
        assert(0 <= left && left < right && right <= size); // 调试检查，可删
        assert(0 <= rank && rank < right - left); // 调试检查，可删
        int answer = 0;
        for (int bit = LOG - 1; bit >= 0; --bit)
        {
            int onesLeft = prefixOne[bit][left];
            int onesR = prefixOne[bit][right];
            int zeros = right - left - onesR + onesLeft;
            if (rank < zeros)
            {
                left -= onesLeft;
                right -= onesR;
            }
            else
            {
                answer |= 1LL << bit;
                rank -= zeros;
                left = middle[bit] + onesLeft;
                right = middle[bit] + onesR;
            }
        }
        return answer;
    }
};

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

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> p(n);
    for (auto &x : p)
    {
        cin >> x;
    }
    StaticRngLIS<> st(p);
    while (q--)
    {
        int l, r;
        cin >> l >> r;
        cout << st.query(l, r) << '\n';
    }
    return 0;
}
