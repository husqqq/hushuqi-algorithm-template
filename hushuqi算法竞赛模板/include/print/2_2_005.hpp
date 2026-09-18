#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "support/topic_common.hpp"

// Point3[0..3] 依次是 x、y、z 坐标与原输入编号。
using Point3 = array<int, 4>;

// ans[i] 是三维坐标均不大于第 i 个点的点数。
inline vector<int> dom3(vector<Point3> points)
{
    // points 的每项为 {x,y,z,id}；返回每个原编号的三维非严格支配计数。
    int n = points.size();
    vector<int> coord;
    for (auto point : points)
    {
        coord.push_back(point[2]);
    }
    ranges::sort(coord);
    coord.erase(unique(coord.begin(), coord.end()), coord.end());
    ranges::sort(points,
                 [](const Point3 &a, const Point3 &b)
                 {
                     return tie(a[0], a[1], a[2]) < tie(b[0], b[1], b[2]);
                 });
    struct Node
    {
        // x、y 是原坐标，z 是离散编号，weight 是该坐标的重复点数。
        int x, y, z, weight;
        // ids 保存该组重复点的原编号。
        vector<int> ids;
        // ans 保存当前累计的非严格支配计数。
        long long ans;
    };
    vector<Node> nodes;
    for (auto point : points)
    {
        int z = lower_bound(coord.begin(), coord.end(), point[2]) - coord.begin();
        if (!nodes.empty() && tie(nodes.back().x, nodes.back().y, nodes.back().z) == tie(point[0], point[1], z))
        {
            ++nodes.back().weight;
            ++nodes.back().ans;
            nodes.back().ids.push_back(point[3]);
        }
        else
        {
            nodes.push_back({point[0], point[1], z, 1, {point[3]}, 1});
        }
    }
    vector<int> bit(coord.size() + 1), order(nodes.size()), buffer(nodes.size()), ans(n);
    iota(order.begin(), order.end(), 0);
    auto add = [&](int x, int value)
    {
        for (++x; x < bit.size(); x += x & -x)
        {
            bit[x] += value;
        }
    };
    auto sum = [&](int x)
    {
        int res = 0;
        for (++x; x > 0; x -= x & -x)
        {
            res += bit[x];
        }
        return res;
    };
    function<void(int, int)> cdq = [&](int left, int right)
    {
        if (right - left <= 1)
        {
            return;
        }
        int middle = midpoint(left, right);
        cdq(left, middle);
        cdq(middle, right);
        int i = left, j = middle, output = left;
        while (j < right)
        {
            while (i < middle && nodes[order[i]].y <= nodes[order[j]].y)
            {
                add(nodes[order[i]].z, nodes[order[i]].weight);
                buffer[output++] = order[i++];
            }
            nodes[order[j]].ans += sum(nodes[order[j]].z);
            buffer[output++] = order[j++];
        }
        auto used = i;
        while (i < middle)
        {
            buffer[output++] = order[i++];
        }
        for (int k = left; k < used; ++k)
        {
            add(nodes[order[k]].z, -nodes[order[k]].weight);
        }
        copy(buffer.begin() + left, buffer.begin() + right, order.begin() + left);
    };
    cdq(0, nodes.size());
    for (auto &node : nodes)
    {
        for (int id : node.ids)
        {
            ans[id] = node.ans;
        }
    }
    return ans;
}

struct Point4
{
    // x、y、z、w 是四维坐标。
    int x, y, z, w;
};

struct Dom4Event
{
    // y、z、w 是后三维坐标；src 表示该点来自较小 x 的左半部。
    int y, z, w;
    bool src;
};

inline long long dom4Cross(vector<Dom4Event> a, int m)
{
    // a 是一次跨段统计的事件，m 是离散后第四维大小；返回后三维均严格递增的跨段点对数。
    sort(a.begin(), a.end(), [](const auto &u, const auto &v)
    {
        return tie(u.y, u.src) < tie(v.y, v.src);
    });
    vector<Dom4Event> buf(a.size());
    vector<int> bit(m + 1);
    auto add = [&](int p, int v)
    {
        for (++p; p <= m; p += p & -p) bit[p] += v;
    };
    auto sum = [&](int p)
    {
        int ans = 0;
        for (; p; p -= p & -p) ans += bit[p];
        return ans;
    };
    long long ans = 0;
    auto cdq = [&](auto &&self, int l, int r) -> void
    {
        if (r - l <= 1) return;
        int mid = (l + r) / 2;
        self(self, l, mid);
        self(self, mid, r);
        int i = l;
        for (int j = mid; j < r; j++)
        {
            while (i < mid && a[i].z < a[j].z)
            {
                if (a[i].src) add(a[i].w, 1);
                i++;
            }
            if (!a[j].src) ans += sum(a[j].w);
        }
        for (int k = l; k < i; k++) if (a[k].src) add(a[k].w, -1);
        merge(a.begin() + l, a.begin() + mid, a.begin() + mid, a.begin() + r,
              buf.begin() + l, [](const auto &u, const auto &v)
        {
            return u.z < v.z;
        });
        copy(buf.begin() + l, buf.begin() + r, a.begin() + l);
    };
    cdq(cdq, 0, a.size());
    return ans;
}

inline long long countFourDimensional(vector<Point4> a)
{
    // a 是四维点集；返回两点四维坐标均严格递增的有序点对数量。
    vector<int> ws;
    for (auto p : a) ws.push_back(p.w);
    sort(ws.begin(), ws.end());
    ws.erase(unique(ws.begin(), ws.end()), ws.end());
    for (auto &p : a) p.w = lower_bound(ws.begin(), ws.end(), p.w) - ws.begin();
    sort(a.begin(), a.end(), [](const auto &u, const auto &v)
    {
        return tie(u.x, u.y, u.z, u.w) < tie(v.x, v.y, v.z, v.w);
    });
    long long ans = 0;
    auto solve = [&](auto &&self, int l, int r) -> void
    {
        if (r - l <= 1) return;
        int mid = (l + r) / 2;
        while (mid < r && a[mid - 1].x == a[mid].x) mid++;
        if (mid == r)
        {
            mid = (l + r) / 2;
            while (mid > l && a[mid - 1].x == a[mid].x) mid--;
            if (mid == l) return;
        }
        self(self, l, mid);
        self(self, mid, r);
        vector<Dom4Event> e;
        e.reserve(r - l);
        for (int i = l; i < mid; i++) e.push_back({a[i].y, a[i].z, a[i].w, true});
        for (int i = mid; i < r; i++) e.push_back({a[i].y, a[i].z, a[i].w, false});
        ans += dom4Cross(move(e), ws.size());
    };
    solve(solve, 0, a.size());
    return ans;
}
