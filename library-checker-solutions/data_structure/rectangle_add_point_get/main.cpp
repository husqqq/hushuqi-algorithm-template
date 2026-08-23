#ifndef TEMPLATE_INLINE_AA3856B9A0CA8848B2BC
#define TEMPLATE_INLINE_AA3856B9A0CA8848B2BC

#include <bits/stdc++.h>

using namespace std;

class OfflineFenwick2D {
    vector<int> xCoordinates;
    vector<vector<int>> yCoordinates;
    vector<vector<long long>> data;

  public:
    explicit OfflineFenwick2D(const vector<pair<int, int>>& updatePoints = {}) { prepare(updatePoints); }

    void prepare(const vector<pair<int, int>>& updatePoints) {
        xCoordinates.clear();
        for (auto [x, y] : updatePoints) xCoordinates.push_back(x);
        sort(xCoordinates.begin(), xCoordinates.end());
        xCoordinates.erase(unique(xCoordinates.begin(), xCoordinates.end()), xCoordinates.end());
        int size = xCoordinates.size();
        yCoordinates.assign(size + 1, {});
        for (auto [x, y] : updatePoints) {
            int index = lower_bound(xCoordinates.begin(), xCoordinates.end(), x) - xCoordinates.begin() + 1;
            for (int i = index; i <= size; i += i & -i) yCoordinates[i].push_back(y);
        }
        data.resize(size + 1);
        for (int i = 1; i <= size; ++i) {
            auto& coordinates = yCoordinates[i];
            sort(coordinates.begin(), coordinates.end());
            coordinates.erase(unique(coordinates.begin(), coordinates.end()), coordinates.end());
            data[i].assign(coordinates.size() + 1, 0);
        }
    }

    void add(int x, int y, long long value) {
        int size = xCoordinates.size();
        int index = lower_bound(xCoordinates.begin(), xCoordinates.end(), x) - xCoordinates.begin() + 1;
        for (int i = index; i <= size; i += i & -i) {
            int position = lower_bound(yCoordinates[i].begin(), yCoordinates[i].end(), y) - yCoordinates[i].begin() + 1;
            for (int j = position; j < int(data[i].size()); j += j & -j) data[i][j] += value;
        }
    }

    long long prefixSum(int xUpperBound, int yUpperBound) const {
        int index = lower_bound(xCoordinates.begin(), xCoordinates.end(), xUpperBound) - xCoordinates.begin();
        long long answer = 0;
        for (int i = index; i > 0; i -= i & -i) {
            int position =
                lower_bound(yCoordinates[i].begin(), yCoordinates[i].end(), yUpperBound) - yCoordinates[i].begin();
            for (int j = position; j > 0; j -= j & -j) answer += data[i][j];
        }
        return answer;
    }
};
#endif


struct RectangleQuery {
    int type;
    int left, down, right, up;
    long long weight;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<RectangleQuery> queries;
    queries.reserve(n + q);
    for (int i = 0; i < n; ++i) {
        int left, down, right, up;
        long long weight;
        cin >> left >> down >> right >> up >> weight;
        queries.push_back({0, left, down, right, up, weight});
    }
    for (int i = 0; i < q; ++i) {
        int type;
        cin >> type;
        if (type == 0) {
            int left, down, right, up;
            long long weight;
            cin >> left >> down >> right >> up >> weight;
            queries.push_back({0, left, down, right, up, weight});
        } else {
            int x, y;
            cin >> x >> y;
            queries.push_back({1, x, y, 0, 0, 0});
        }
    }

    vector<pair<int, int>> updatePoints;
    for (const auto& query : queries) {
        if (query.type == 1) continue;
        updatePoints.emplace_back(query.left, query.down);
        updatePoints.emplace_back(query.left, query.up);
        updatePoints.emplace_back(query.right, query.down);
        updatePoints.emplace_back(query.right, query.up);
    }
    OfflineFenwick2D fenwick(updatePoints);
    for (const auto& query : queries) {
        if (query.type == 1) {
            cout << fenwick.prefixSum(query.left + 1, query.down + 1) << '\n';
            continue;
        }
        fenwick.add(query.left, query.down, query.weight);
        fenwick.add(query.left, query.up, -query.weight);
        fenwick.add(query.right, query.down, -query.weight);
        fenwick.add(query.right, query.up, query.weight);
    }
}
