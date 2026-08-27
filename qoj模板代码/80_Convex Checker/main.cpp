#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
using i128 = __int128_t;

struct Point
{
    i64 x;
    i64 y;

    friend bool operator<(const Point &a, const Point &b)
    {
        return tie(a.x, a.y) < tie(b.x, b.y);
    }

    friend bool operator==(const Point &a, const Point &b)
    {
        return a.x == b.x && a.y == b.y;
    }
};

i128 cross(const Point &a, const Point &b, const Point &c)
{
    return (i128)(b.x - a.x) * (c.y - a.y) -
           (i128)(b.y - a.y) * (c.x - a.x);
}

vector<Point> convexHull(vector<Point> points)
{
    // 与正式板子 11.3.001 一致：Andrew 严格凸包，删除共线中间点。
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());
    if (points.size() <= 1)
    {
        return points;
    }

    vector<Point> hull;
    for (int pass = 0; pass < 2; pass++)
    {
        int base = (int)hull.size();
        for (const Point &point : points)
        {
            while ((int)hull.size() >= base + 2 &&
                   cross(hull[hull.size() - 2], hull.back(), point) <= 0)
            {
                hull.pop_back();
            }
            hull.push_back(point);
        }
        hull.pop_back();
        reverse(points.begin(), points.end());
    }
    return hull;
}

bool sameCyclicOrder(const vector<Point> &polygon,
                     const vector<Point> &hull)
{
    int n = (int)polygon.size();
    int start = -1;
    for (int i = 0; i < n; i++)
    {
        if (polygon[i] == hull[0])
        {
            start = i;
            break;
        }
    }
    if (start == -1)
    {
        return false;
    }

    for (int i = 0; i < n; i++)
    {
        if (!(polygon[(start + i) % n] == hull[i]))
        {
            return false;
        }
    }
    return true;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<Point> polygon(n);
    for (Point &point : polygon)
    {
        cin >> point.x >> point.y;
    }

    vector<Point> hull = convexHull(polygon);
    bool convex = (int)hull.size() == n;

    if (convex && !sameCyclicOrder(polygon, hull))
    {
        reverse(polygon.begin(), polygon.end());
        convex = sameCyclicOrder(polygon, hull);
    }

    cout << (convex ? "Yes" : "No") << '\n';
    return 0;
}
