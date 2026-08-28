#pragma once

// QOJ candidate template extracted from the verified standalone solver.
// Keep this file outside the formal template until it is approved.

#include <bits/stdc++.h>
using namespace std;
#define int long long

struct Point
{
    int x; // 第一维坐标。
    int y; // 第二维坐标。
    int z; // 第三维坐标。
    int w; // 第四维坐标。
};

struct Event
{
    int y; // 第二维坐标。
    int z; // 第三维坐标。
    int w; // 第四维坐标。
    bool source; // 是否为左半部分的来源点。
};

class Fenwick
{
    vector<int> tree;

public:
    // n 是 Fenwick 下标的最大值；初始所有位置的计数为 0。
    explicit Fenwick(int n) : tree(n + 1) {}

    // position 是 1 下标位置，value 是增量；执行单点加法。
    void add(int position, int value)
    {
        for (; position < (int)tree.size(); position += position & -position)
        {
            tree[position] += value;
        }
    }

    // position 是前缀右端点；返回 [1,position] 的计数和。
    int prefix(int position) const
    {
        int result = 0;
        for (; position; position -= position & -position)
        {
            result += tree[position];
        }
        return result;
    }
};

long long countThreeDimensional(vector<Event> events, int coordinateLimit)
{
    // events 是按第二维排序的三维偏序事件；返回来源点严格小于查询点的数量。
    assert(coordinateLimit >= 0);
    sort(events.begin(), events.end(), [](const Event &a, const Event &b)
    {
        return a.y < b.y;
    });
    vector<Event> buffer(events.size());
    Fenwick fenwick(coordinateLimit);
    long long answer = 0;
    auto solve = [&](auto &&self, int left, int right) -> void
    {
        if (right - left <= 1)
        {
            return;
        }
        int middle = (left + right) / 2;
        self(self, left, middle);
        self(self, middle, right);
        int i = left;
        for (int j = middle; j < right; j++)
        {
            while (i < middle && events[i].z < events[j].z)
            {
                if (events[i].source)
                {
                    fenwick.add(events[i].w, 1);
                }
                i++;
            }
            if (!events[j].source)
            {
                answer += fenwick.prefix(events[j].w - 1);
            }
        }
        for (int k = left; k < i; k++)
        {
            if (events[k].source)
            {
                fenwick.add(events[k].w, -1);
            }
        }
        merge(events.begin() + left, events.begin() + middle,
              events.begin() + middle, events.begin() + right,
              buffer.begin() + left, [](const Event &a, const Event &b)
        {
            return a.z < b.z;
        });
        copy(buffer.begin() + left, buffer.begin() + right, events.begin() + left);
    };
    solve(solve, 0, events.size());
    return answer;
}

long long countFourDimensional(vector<Point> points)
{
    // points 是四维点集；返回两点四维坐标均严格递增的有序点对数量。
    sort(points.begin(), points.end(), [](const Point &a, const Point &b)
    {
        return a.x < b.x;
    });
    int n = points.size();
    long long answer = 0;
    auto solve = [&](auto &&self, int left, int right) -> void
    {
        if (right - left <= 1)
        {
            return;
        }
        int middle = (left + right) / 2;
        self(self, left, middle);
        self(self, middle, right);
        vector<Event> events;
        events.reserve(right - left);
        for (int i = left; i < middle; i++)
        {
            events.push_back({points[i].y, points[i].z, points[i].w, true});
        }
        for (int i = middle; i < right; i++)
        {
            events.push_back({points[i].y, points[i].z, points[i].w, false});
        }
        answer += countThreeDimensional(move(events), n);
    };
    solve(solve, 0, n);
    return answer;
}
