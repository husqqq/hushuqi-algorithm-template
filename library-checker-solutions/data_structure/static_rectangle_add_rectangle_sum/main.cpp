#include <bits/stdc++.h>

using namespace std;

constexpr long long P = 998244353;

struct Corner {
    long long x, y, value;
};

struct PrefixQuery {
    long long x, y;
    int id, sign;
};

struct Fenwick {
    vector<long long> tree;
    Fenwick(int n) : tree(n + 1) {}
    void add(int index, long long value) {
        value %= P;
        for (index++; index < int(tree.size()); index += index & -index) {
            tree[index] += value;
            tree[index] %= P;
        }
    }
    long long sum(int index) const {
        long long result = 0;
        for (; index; index -= index & -index) result += tree[index];
        return result % P;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<Corner> corners;
    vector<long long> yCoordinates;
    auto addCorner = [&](long long x, long long y, long long value) {
        corners.push_back({x, y, (value % P + P) % P});
        yCoordinates.push_back(y);
    };
    for (int i = 0; i < n; i++) {
        long long left, lower, right, upper, weight;
        cin >> left >> lower >> right >> upper >> weight;
        addCorner(left, lower, weight);
        addCorner(left, upper, -weight);
        addCorner(right, lower, -weight);
        addCorner(right, upper, weight);
    }
    vector<PrefixQuery> queries;
    auto addPrefix = [&](long long x, long long y, int id, int sign) {
        queries.push_back({x, y, id, sign});
        yCoordinates.push_back(y);
    };
    for (int id = 0; id < q; id++) {
        long long left, lower, right, upper;
        cin >> left >> lower >> right >> upper;
        addPrefix(right, upper, id, 1);
        addPrefix(left, upper, id, -1);
        addPrefix(right, lower, id, -1);
        addPrefix(left, lower, id, 1);
    }
    sort(corners.begin(), corners.end(), [&](const Corner& a, const Corner& b) { return a.x < b.x; });
    sort(queries.begin(), queries.end(), [&](const PrefixQuery& a, const PrefixQuery& b) { return a.x < b.x; });
    sort(yCoordinates.begin(), yCoordinates.end());
    yCoordinates.erase(unique(yCoordinates.begin(), yCoordinates.end()), yCoordinates.end());
    array<Fenwick, 4> fenwick = {Fenwick(yCoordinates.size()), Fenwick(yCoordinates.size()),
                                 Fenwick(yCoordinates.size()), Fenwick(yCoordinates.size())};
    vector<long long> answer(q);
    int pointer = 0;
    for (auto query : queries) {
        while (pointer < int(corners.size()) && corners[pointer].x < query.x) {
            auto corner = corners[pointer++];
            int y = lower_bound(yCoordinates.begin(), yCoordinates.end(), corner.y) - yCoordinates.begin();
            fenwick[0].add(y, corner.value);
            fenwick[1].add(y, corner.value * (corner.y % P));
            fenwick[2].add(y, corner.value * (corner.x % P));
            fenwick[3].add(y, corner.value * (corner.x % P) % P * (corner.y % P));
        }
        int y = lower_bound(yCoordinates.begin(), yCoordinates.end(), query.y) - yCoordinates.begin();
        long long xValue = query.x % P, yValue = query.y % P;
        long long value = xValue * yValue % P * fenwick[0].sum(y) % P;
        value = (value - xValue * fenwick[1].sum(y)) % P;
        value = (value - yValue * fenwick[2].sum(y)) % P;
        value = (value + fenwick[3].sum(y)) % P;
        answer[query.id] = (answer[query.id] + query.sign * value) % P;
    }
    for (long long value : answer) {
        value %= P;
        if (value < 0) value += P;
        cout << value << '\n';
    }
    return 0;
}
