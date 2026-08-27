// Generated from hushuqi算法竞赛模板. Do not edit by hand.

// QOJ contest 3936: 51 四维偏序

#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int inf = 1E9;
constexpr long long INF = 4E18;
constexpr long double eps = 1E-12L;

template <class T> bool chmin(T &a, const T &b)
{
    // 若 b 更小则以 b 更新 a；返回是否发生更新。
    return b < a ? a = b, true : false;
}
template <class T> bool chmax(T &a, const T &b)
{
    // 若 b 更大则以 b 更新 a；返回是否发生更新。
    return a < b ? a = b, true : false;
}

// Point3[0..3] 依次是 x、y、z 坐标与原输入编号。
using Point3 = array<int, 4>;

// answer[i] 是三维坐标均不大于第 i 个点的点数。
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
        // answer 保存当前累计的非严格支配计数。
        long long answer;
    };
    vector<Node> nodes;
    for (auto point : points)
    {
        int z = lower_bound(coord.begin(), coord.end(), point[2]) - coord.begin();
        if (!nodes.empty() && tie(nodes.back().x, nodes.back().y, nodes.back().z) == tie(point[0], point[1], z))
        {
            ++nodes.back().weight;
            ++nodes.back().answer;
            nodes.back().ids.push_back(point[3]);
        }
        else
        {
            nodes.push_back({point[0], point[1], z, 1, {point[3]}, 1});
        }
    }
    vector<int> bit(coord.size() + 1), order(nodes.size()), buffer(nodes.size()), answer(n);
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
        int result = 0;
        for (++x; x > 0; x -= x & -x)
        {
            result += bit[x];
        }
        return result;
    };
    function<void(int, int)> cdq = [&](int left, int right)
    {
        if (right - left == 1)
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
            nodes[order[j]].answer += sum(nodes[order[j]].z);
            buffer[output++] = order[j++];
        }
        while (i < middle)
        {
            buffer[output++] = order[i++];
        }
        for (int k = left; k < middle; ++k)
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
            answer[id] = node.answer;
        }
    }
    return answer;
}
