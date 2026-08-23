#include <bits/stdc++.h>

using namespace std;

using i64 = long long;
using i128 = __int128_t;

struct Point {
    i64 x, y;

    auto operator<=>(const Point&) const = default;
};

i128 cross(const Point& a, const Point& b, const Point& c) {
    return i128(b.x - a.x) * (c.y - a.y) - i128(b.y - a.y) * (c.x - a.x);
}

vector<Point> convexHull(vector<Point> points) {
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());
    if (points.size() <= 1) {
        return points;
    }

    vector<Point> hull;
    for (auto point : points) {
        while (hull.size() >= 2 && cross(hull[hull.size() - 2], hull.back(), point) <= 0) {
            hull.pop_back();
        }
        hull.push_back(point);
    }
    int lower = hull.size();
    for (int i = (int)points.size() - 2; i >= 0; i--) {
        while ((int)hull.size() > lower && cross(hull[hull.size() - 2], hull.back(), points[i]) <= 0) {
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }
    hull.pop_back();
    return hull;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<Point> points(n);
        for (auto& [x, y] : points) {
            cin >> x >> y;
        }
        auto hull = convexHull(move(points));
        cout << hull.size() << '\n';
        for (auto [x, y] : hull) {
            cout << x << ' ' << y << '\n';
        }
    }
    return 0;
}
