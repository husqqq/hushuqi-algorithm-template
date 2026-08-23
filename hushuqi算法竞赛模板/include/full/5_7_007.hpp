#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

struct StableEdge
{
    int left, right;           // left、right 是可接受关系的左右端点。
    int leftRank, rightRank;   // 两个排名分别表示双方对该关系的偏好，0 最优。
};

struct SparseMatch
{
    vector<int> left, right; // 两个数组分别记录左右侧配偶；未匹配位置为 -1。
};

SparseMatch sparseMatch(int nLeft, int nRight,
                                    const vector<StableEdge> &edges)
{
    // nLeft、nRight 是两侧人数，edges 给出全部可接受关系及双方严格排名；返回左侧最优稳定匹配。
    assert(nLeft >= 0 && nRight >= 0);
    vector<int> beginLeft(nLeft + 1), beginR(nRight + 1);
    for (auto [x, y, rx, ry] : edges)
    {
        assert(0 <= x && x < nLeft && 0 <= y && y < nRight);
        ++beginLeft[x + 1];
        ++beginR[y + 1];
    }
    partial_sum(beginLeft.begin(), beginLeft.end(), beginLeft.begin());
    partial_sum(beginR.begin(), beginR.end(), beginR.begin());

    // pref 按左侧排名连续存边；usedRight 检查每个右侧排名恰好出现一次。
    vector<pair<int, int>> pref(edges.size(), {-1, -1});
    vector<char> usedRight(edges.size());
    for (auto [x, y, rx, ry] : edges)
    {
        assert(0 <= rx && rx < beginLeft[x + 1] - beginLeft[x]);
        assert(0 <= ry && ry < beginR[y + 1] - beginR[y]);
        int px = beginLeft[x] + rx, py = beginR[y] + ry;
        assert(pref[px].first == -1 && !usedRight[py]);
        pref[px] = {y, ry};
        usedRight[py] = true;
    }

    vector<int> left(nLeft, -1), right(nRight, -1), heldRank(nRight, -1);
    vector<int> next = beginLeft, q;
    q.reserve(nLeft + edges.size());
    for (int x = 0; x < nLeft; ++x)
    {
        if (next[x] < beginLeft[x + 1])
        {
            q.push_back(x);
        }
    }
    for (int head = 0; head < (int)q.size(); ++head)
    {
        int x = q[head];
        auto [y, rank] = pref[next[x]++];
        if (right[y] == -1 || rank < heldRank[y])
        {
            int old = right[y];
            if (old != -1)
            {
                left[old] = -1;
                if (next[old] < beginLeft[old + 1])
                {
                    q.push_back(old);
                }
            }
            left[x] = y;
            right[y] = x;
            heldRank[y] = rank;
        }
        else if (next[x] < beginLeft[x + 1])
        {
            q.push_back(x);
        }
    }
    return {left, right};
}
