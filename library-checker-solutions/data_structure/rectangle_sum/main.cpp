#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

struct Fenwick {
    vector<i64> a;

    Fenwick(int n) : a(n + 1) {}

    void add(int x, i64 value) {
        for (x++; x < (int)a.size(); x += x & -x) {
            a[x] += value;
        }
    }

    i64 sum(int x) const {
        i64 answer = 0;
        for (; x > 0; x -= x & -x) {
            answer += a[x];
        }
        return answer;
    }
};

struct Point {
    int x, y;
    i64 weight;
};

struct Event {
    int x, d, u, id, sign;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<Point> points(n);
    vector<int> ys;
    for (auto& [x, y, weight] : points) {
        cin >> x >> y >> weight;
        ys.push_back(y);
    }
    vector<Event> events;
    for (int id = 0; id < q; id++) {
        int l, d, r, u;
        cin >> l >> d >> r >> u;
        events.push_back({l, d, u, id, -1});
        events.push_back({r, d, u, id, 1});
    }
    sort(points.begin(), points.end(), [&](const Point& a, const Point& b) { return a.x < b.x; });
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());
    sort(events.begin(), events.end(), [&](const Event& a, const Event& b) { return a.x < b.x; });

    Fenwick fenwick(ys.size());
    vector<i64> answer(q);
    int index = 0;
    for (auto [x, d, u, id, sign] : events) {
        while (index < n && points[index].x < x) {
            int y = lower_bound(ys.begin(), ys.end(), points[index].y) - ys.begin();
            fenwick.add(y, points[index].weight);
            index++;
        }
        int left = lower_bound(ys.begin(), ys.end(), d) - ys.begin();
        int right = lower_bound(ys.begin(), ys.end(), u) - ys.begin();
        answer[id] += sign * (fenwick.sum(right) - fenwick.sum(left));
    }
    for (i64 value : answer) {
        cout << value << '\n';
    }
    return 0;
}
