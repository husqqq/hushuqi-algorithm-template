#include <bits/stdc++.h>
using namespace std;
#include "../include/full/2_2_015.hpp"

void checkWitness(const vector<int> &a, const optional<mitm::Result> &res)
{
    if (!res)
    {
        return;
    }
    __int128 sum = 0;
    int last = -1;
    for (int i : res->indices)
    {
        assert(last < i && i < (int)a.size());
        sum += a[i];
        last = i;
    }
    assert(sum == res->sum);
}

signed main()
{
    mt19937_64 rng(20261021);
    for (int tc = 0; tc < 3000; tc++)
    {
        int n = rng() % 15;
        vector<int> a(n);
        for (int &x : a)
        {
            x = (int)(rng() % 41) - 20;
        }
        int target = (int)(rng() % 201) - 100;
        optional<__int128> low, high;
        __int128 closest = 0;
        unsigned __int128 bestGap = 0;
        unsigned long long count = 1ULL << n;
        for (unsigned long long mask = 0; mask < count; mask++)
        {
            __int128 sum = 0;
            for (int i = 0; i < n; i++)
            {
                if (mask >> i & 1)
                {
                    sum += a[i];
                }
            }
            if (sum <= target && (!low || sum > *low))
            {
                low = sum;
            }
            if (sum >= target && (!high || sum < *high))
            {
                high = sum;
            }
            unsigned __int128 gap = sum >= target ? sum - target : target - sum;
            if (mask == 0 || gap < bestGap || (gap == bestGap && sum < closest))
            {
                closest = sum;
                bestGap = gap;
            }
        }
        auto gotLow = mitm::bound(a, target);
        auto gotHigh = mitm::bound(a, target, true);
        auto gotClosest = mitm::closest(a, target);
        assert((bool)gotLow == (bool)low);
        assert((bool)gotHigh == (bool)high);
        if (gotLow)
        {
            assert(gotLow->sum == *low);
        }
        if (gotHigh)
        {
            assert(gotHigh->sum == *high);
        }
        assert(gotClosest.sum == closest);
        checkWitness(a, gotLow);
        checkWitness(a, gotHigh);
        checkWitness(a, optional<mitm::Result>(gotClosest));
    }

    auto x = mitm::closest({numeric_limits<int>::max(), numeric_limits<int>::max(),
                            numeric_limits<int>::lowest(), numeric_limits<int>::lowest()}, 0);
    assert(x.sum == 0 && x.indices.empty());
    auto wide = mitm::bound({numeric_limits<int>::max(), numeric_limits<int>::max()},
                            (__int128)numeric_limits<int>::max() * 2);
    assert(wide && wide->sum == (__int128)numeric_limits<int>::max() * 2
           && wide->indices == vector<int>({0, 1}));
    bool rejected = false;
    try
    {
        mitm::bound(vector<int>(47), 0);
    }
    catch (const length_error &)
    {
        rejected = true;
    }
    assert(rejected);
}
