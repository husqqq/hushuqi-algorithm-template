#include <bits/stdc++.h>

using namespace std;

struct Event {
    long long x, y, value;
    int type, query = -1, sign = 0;
};

struct Fenwick {
    vector<long long> tree;
    Fenwick(int n) : tree(n + 1) {}
    void add(int index, long long value) {
        for (index++; index < int(tree.size()); index += index & -index) tree[index] += value;
    }
    long long sum(int index) const {
        long long result = 0;
        for (; index; index -= index & -index) result += tree[index];
        return result;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<Event> events;
    vector<long long> yCoordinates;
    auto addPoint = [&](long long x, long long y, long long weight) {
        events.push_back({x, y, weight, 0});
        yCoordinates.push_back(y);
    };
    for (int i = 0; i < n; i++) {
        long long x, y, weight;
        cin >> x >> y >> weight;
        addPoint(x, y, weight);
    }
    vector<long long> answer;
    auto addPrefixQuery = [&](long long x, long long y, int id, int sign) {
        events.push_back({x, y, 0, 1, id, sign});
        yCoordinates.push_back(y);
    };
    for (int i = 0; i < q; i++) {
        int type;
        cin >> type;
        if (type == 0) {
            long long x, y, weight;
            cin >> x >> y >> weight;
            addPoint(x, y, weight);
        } else {
            long long left, lower, right, upper;
            cin >> left >> lower >> right >> upper;
            int id = answer.size();
            answer.push_back(0);
            addPrefixQuery(right, upper, id, 1);
            addPrefixQuery(left, upper, id, -1);
            addPrefixQuery(right, lower, id, -1);
            addPrefixQuery(left, lower, id, 1);
        }
    }
    sort(yCoordinates.begin(), yCoordinates.end());
    yCoordinates.erase(unique(yCoordinates.begin(), yCoordinates.end()), yCoordinates.end());
    Fenwick fenwick(yCoordinates.size());
    auto divide = [&](auto&& self, int l, int r) -> vector<int> {
        if (r - l == 1) return {l};
        int mid = (l + r) / 2;
        auto left = self(self, l, mid);
        auto right = self(self, mid, r);
        int pointer = 0;
        for (int index : right) {
            if (events[index].type != 1) continue;
            while (pointer < int(left.size()) && events[left[pointer]].x < events[index].x) {
                int update = left[pointer++];
                if (events[update].type == 0) {
                    int y =
                        lower_bound(yCoordinates.begin(), yCoordinates.end(), events[update].y) - yCoordinates.begin();
                    fenwick.add(y, events[update].value);
                }
            }
            int y = lower_bound(yCoordinates.begin(), yCoordinates.end(), events[index].y) - yCoordinates.begin();
            answer[events[index].query] += events[index].sign * fenwick.sum(y);
        }
        for (int i = 0; i < pointer; i++) {
            int update = left[i];
            if (events[update].type == 0) {
                int y = lower_bound(yCoordinates.begin(), yCoordinates.end(), events[update].y) - yCoordinates.begin();
                fenwick.add(y, -events[update].value);
            }
        }
        vector<int> merged;
        merged.reserve(r - l);
        merge(left.begin(), left.end(), right.begin(), right.end(), back_inserter(merged),
              [&](int a, int b) { return events[a].x < events[b].x; });
        return merged;
    };
    divide(divide, 0, events.size());
    for (long long value : answer) cout << value << '\n';
    return 0;
}
