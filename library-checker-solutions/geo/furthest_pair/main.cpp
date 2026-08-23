#include <bits/stdc++.h>

using namespace std;

using i128 = __int128_t;

struct Point {
    long long x, y;
    int id;
};

i128 cross(const Point& a, const Point& b, const Point& c) {
    return i128(b.x - a.x) * (c.y - a.y) - i128(b.y - a.y) * (c.x - a.x);
}

i128 distance2(const Point& a, const Point& b) {
    i128 x = i128(a.x) - b.x;
    i128 y = i128(a.y) - b.y;
    return x * x + y * y;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tests;
    cin >> tests;
    while (tests--) {
        int n;
        cin >> n;
        vector<Point> p(n);
        for (int i = 0; i < n; i++) {
            cin >> p[i].x >> p[i].y;
            p[i].id = i;
        }
        pair<int, int> answer = {0, 1};
        i128 best = distance2(p[0], p[1]);
        auto update = [&](const Point& a, const Point& b) {
            i128 value = distance2(a, b);
            if (value > best) {
                best = value;
                answer = {a.id, b.id};
            }
        };

        sort(p.begin(), p.end(),
             [&](const Point& a, const Point& b) { return tie(a.x, a.y, a.id) < tie(b.x, b.y, b.id); });
        p.erase(unique(p.begin(), p.end(), [&](const Point& a, const Point& b) { return a.x == b.x && a.y == b.y; }),
                p.end());
        if (p.size() >= 2) {
            vector<Point> hull;
            for (int pass = 0; pass < 2; pass++) {
                int start = hull.size();
                for (auto point : p) {
                    while (int(hull.size()) >= start + 2 && cross(hull[hull.size() - 2], hull.back(), point) <= 0) {
                        hull.pop_back();
                    }
                    hull.push_back(point);
                }
                hull.pop_back();
                reverse(p.begin(), p.end());
            }
            int h = hull.size();
            if (h == 2) {
                update(hull[0], hull[1]);
            } else {
                int j = 1;
                for (int i = 0; i < h; i++) {
                    int ni = (i + 1) % h;
                    while (cross(hull[i], hull[ni], hull[(j + 1) % h]) > cross(hull[i], hull[ni], hull[j])) {
                        j = (j + 1) % h;
                    }
                    update(hull[i], hull[j]);
                    update(hull[ni], hull[j]);
                }
            }
        }
        cout << answer.first << ' ' << answer.second << '\n';
    }
    return 0;
}
