#include <bits/stdc++.h>

using namespace std;

struct Event {
    int x, lower, upper, delta;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<Event> events;
    vector<int> coordinates;
    events.reserve(2 * n);
    coordinates.reserve(2 * n);
    for (int i = 0; i < n; i++) {
        int left, lower, right, upper;
        cin >> left >> lower >> right >> upper;
        events.push_back({left, lower, upper, 1});
        events.push_back({right, lower, upper, -1});
        coordinates.push_back(lower);
        coordinates.push_back(upper);
    }
    if (n == 0) {
        cout << 0 << '\n';
        return 0;
    }
    sort(coordinates.begin(), coordinates.end());
    coordinates.erase(unique(coordinates.begin(), coordinates.end()), coordinates.end());
    sort(events.begin(), events.end(), [&](const Event& a, const Event& b) { return a.x < b.x; });

    int segments = coordinates.size() - 1;
    vector<int> cover(4 * max(1, segments));
    vector<long long> length(4 * max(1, segments));
    auto pull = [&](int node, int l, int r) {
        if (cover[node]) {
            length[node] = coordinates[r] - coordinates[l];
        } else if (r - l == 1) {
            length[node] = 0;
        } else {
            length[node] = length[node * 2] + length[node * 2 + 1];
        }
    };
    auto update = [&](auto&& self, int node, int l, int r, int ql, int qr, int value) -> void {
        if (qr <= l || r <= ql) {
            return;
        }
        if (ql <= l && r <= qr) {
            cover[node] += value;
            pull(node, l, r);
            return;
        }
        int mid = (l + r) / 2;
        self(self, node * 2, l, mid, ql, qr, value);
        self(self, node * 2 + 1, mid, r, ql, qr, value);
        pull(node, l, r);
    };

    long long answer = 0;
    int previousX = events[0].x;
    for (int i = 0; i < int(events.size());) {
        int x = events[i].x;
        answer += length[1] * (x - previousX);
        while (i < int(events.size()) && events[i].x == x) {
            int lower = lower_bound(coordinates.begin(), coordinates.end(), events[i].lower) - coordinates.begin();
            int upper = lower_bound(coordinates.begin(), coordinates.end(), events[i].upper) - coordinates.begin();
            update(update, 1, 0, segments, lower, upper, events[i].delta);
            i++;
        }
        previousX = x;
    }
    cout << answer << '\n';
    return 0;
}
