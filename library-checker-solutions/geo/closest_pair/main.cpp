#include <bits/stdc++.h>

using namespace std;

using i128 = __int128_t;

struct Point {
    long long x, y;
    int id;
};

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
        vector<Point> p(n), buffer(n);
        for (int i = 0; i < n; i++) {
            cin >> p[i].x >> p[i].y;
            p[i].id = i;
        }
        sort(p.begin(), p.end(),
             [&](const Point& a, const Point& b) { return tie(a.x, a.y, a.id) < tie(b.x, b.y, b.id); });
        bool duplicate = false;
        for (int i = 1; i < n; i++) {
            if (p[i - 1].x == p[i].x && p[i - 1].y == p[i].y) {
                cout << p[i - 1].id << ' ' << p[i].id << '\n';
                duplicate = true;
                break;
            }
        }
        if (duplicate) continue;

        i128 best = distance2(p[0], p[1]);
        pair<int, int> answer = {p[0].id, p[1].id};
        auto update = [&](const Point& a, const Point& b) {
            i128 value = distance2(a, b);
            if (value < best) {
                best = value;
                answer = {a.id, b.id};
            }
        };

        auto solve = [&](auto&& self, int l, int r) -> void {
            if (r - l <= 3) {
                for (int i = l; i < r; i++) {
                    for (int j = l; j < i; j++) {
                        update(p[i], p[j]);
                    }
                }
                sort(p.begin() + l, p.begin() + r,
                     [&](const Point& a, const Point& b) { return tie(a.y, a.x) < tie(b.y, b.x); });
                return;
            }
            int mid = (l + r) / 2;
            long long middleX = p[mid].x;
            self(self, l, mid);
            self(self, mid, r);
            merge(p.begin() + l, p.begin() + mid, p.begin() + mid, p.begin() + r, buffer.begin() + l,
                  [&](const Point& a, const Point& b) { return tie(a.y, a.x) < tie(b.y, b.x); });
            copy(buffer.begin() + l, buffer.begin() + r, p.begin() + l);
            vector<Point> strip;
            strip.reserve(r - l);
            for (int i = l; i < r; i++) {
                i128 dx = i128(p[i].x) - middleX;
                if (dx * dx > best) {
                    continue;
                }
                for (int j = int(strip.size()) - 1; j >= 0; j--) {
                    i128 dy = i128(p[i].y) - strip[j].y;
                    if (dy * dy > best) {
                        break;
                    }
                    update(p[i], strip[j]);
                }
                strip.push_back(p[i]);
            }
        };
        solve(solve, 0, n);
        cout << answer.first << ' ' << answer.second << '\n';
    }
    return 0;
}
