#include <bits/stdc++.h>

using namespace std;

using i64 = long long;
using i128 = __int128_t;

struct Point {
    i64 x, y;
};

int region(const Point& p) {
    if (p.y < 0) {
        return 0;
    }
    if (p.y == 0 && p.x >= 0) {
        return 1;
    }
    return 2;
}

i128 cross(const Point& a, const Point& b) {
    return i128(a.x) * b.y - i128(a.y) * b.x;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<Point> points(n);
    for (auto& [x, y] : points) {
        cin >> x >> y;
    }
    stable_sort(points.begin(), points.end(), [&](const Point& a, const Point& b) {
        int x = region(a), y = region(b);
        if (x != y) {
            return x < y;
        }
        return cross(a, b) > 0;
    });
    for (auto [x, y] : points) {
        cout << x << ' ' << y << '\n';
    }
    return 0;
}
