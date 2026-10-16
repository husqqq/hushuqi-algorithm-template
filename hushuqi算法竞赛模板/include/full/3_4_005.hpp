#include <bits/stdc++.h>
using namespace std;
#define int long long

template <int LOG = 30> struct Wavelet
{
    static_assert(1 <= LOG && LOG <= 62);
    // 逐位读取先转为 64 位，关闭整数宏且 LOG>31 时也不越位移位。

    // size 是序列长度，middle[bit] 是该层零段长度。
    int size;
    array<int, LOG> middle;
    // prefixOne[bit][i] 是该层前 i 位的一数。
    array<vector<int>, LOG> prefixOne;
    // pre0[bit][i] 是该层前 i 项中零分支的原值和；显式 64 位累加。
    array<vector<long long>, LOG> pre0;
    // prefixSum[i] 是原序列前 i 项的值和；关闭整数宏后仍用 64 位。
    vector<long long> prefixSum;

    explicit Wavelet(vector<int> values = {})
    {
        // values 是位于 [0,2^LOG) 的初始序列。
        init(move(values));
    }

    void init(vector<int> values)
    {
        // values 是新的静态序列；重建全部状态，无返回值；总和须能用 long long 表示。
        assert(values.size() < numeric_limits<int>::max()); // 调试检查，可删
        size = (int)values.size();
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
                bool one = (unsigned long long)values[i] >> bit & 1;
                prefixOne[bit][i + 1] = prefixOne[bit][i] + one;
                pre0[bit][i + 1] = pre0[bit][i] + (one ? 0 : values[i]);
            }
            middle[bit] = size - prefixOne[bit][size];
            int zero = 0, one = middle[bit];
            for (int value : values)
            {
                if ((unsigned long long)value >> bit & 1)
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
        int ans = 0;
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
                ans |= 1LL << bit;
                rank -= zeros;
                left = middle[bit] + onesLeft;
                right = middle[bit] + onesR;
            }
        }
        return ans;
    }

    pair<int, long long> countSumLT(int left, int right, int hi) const
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
        long long sum = 0; // 元素和可能超过 32 位。
        for (int bit = LOG - 1; bit >= 0; --bit)
        {
            int onesLeft = prefixOne[bit][left];
            int onesR = prefixOne[bit][right];
            int zeros = right - left - onesR + onesLeft;
            if ((unsigned long long)hi >> bit & 1)
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
        for (int bit = LOG - 1; bit >= 0; --bit)
        {
            int onesLeft = prefixOne[bit][left];
            int onesR = prefixOne[bit][right];
            if ((unsigned long long)value >> bit & 1)
            {
                left = middle[bit] + onesLeft;
                right = middle[bit] + onesR;
            }
            else
            {
                left -= onesLeft;
                right -= onesR;
            }
        }
        return right - left;
    }

    pair<int, long long> countSumLE(int left, int right, int hi) const
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
    // 逐位读取先转为 64 位，关闭整数宏且 LOG>31 时也不越位移位。

    // size 是序列长度，middle[bit] 是该层零段长度；只保存第 k 小所需的计数。
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
        // values 是新的静态序列；重建全部状态，无返回值；不要求元素总和可表示。
        assert(values.size() < (1ULL << 31)); // 调试检查，可删
        size = (int32_t)values.size();
        constexpr unsigned long long limit = 1ULL << LOG;
        for (int value : values)
        {
            assert(0 <= value && (unsigned long long)value < limit); // 调试检查，可删
        }
        vector<int> next(size);
        for (int bit = LOG - 1; bit >= 0; --bit)
        {
            prefixOne[bit].assign((size_t)size + 1, 0);
            for (int i = 0; i < size; ++i)
            {
                prefixOne[bit][i + 1] = prefixOne[bit][i] + ((unsigned long long)values[i] >> bit & 1);
            }
            middle[bit] = size - prefixOne[bit][size];
            int zero = 0, one = middle[bit];
            for (int value : values)
            {
                if ((unsigned long long)value >> bit & 1)
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
        int ans = 0;
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
                ans |= 1LL << bit;
                rank -= zeros;
                left = middle[bit] + onesLeft;
                right = middle[bit] + onesR;
            }
        }
        return ans;
    }
};
