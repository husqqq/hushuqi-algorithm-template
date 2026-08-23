#ifndef TEMPLATE_INLINE_2E55E36A5446511A938B
#define TEMPLATE_INLINE_2E55E36A5446511A938B

#include <bits/stdc++.h>

using namespace std;

template <class Select> vector<int> monotoneMinima(int rows, int columns, Select&& select) {
    vector<int> minimumColumn(rows);
    auto solve = [&](auto&& self, int top, int bottom, int left, int right) -> void {
        if (top == bottom) return;
        int row = midpoint(top, bottom), best = left;
        for (int column = left + 1; column < right; ++column) {
            if (select(row, best, column)) best = column;
        }
        minimumColumn[row] = best;
        self(self, top, row, left, best + 1);
        self(self, row + 1, bottom, best, right);
    };
    solve(solve, 0, rows, 0, columns);
    return minimumColumn;
}

inline vector<long long> minPlusConvexConvex(const vector<long long>& a, const vector<long long>& b) {
    vector<long long> result{a[0] + b[0]};
    int i = 1, j = 1;
    while (i < a.size() || j < b.size()) {
        if (j == b.size() || i < a.size() && a[i] - a[i - 1] <= b[j] - b[j - 1]) {
            result.push_back(result.back() + a[i] - a[i - 1]);
            ++i;
        } else {
            result.push_back(result.back() + b[j] - b[j - 1]);
            ++j;
        }
    }
    return result;
}

inline vector<long long> minPlusConvexArbitrary(const vector<long long>& convex, const vector<long long>& arbitrary) {
    int n = arbitrary.size(), m = convex.size();
    auto select = [&](int sum, int first, int second) {
        if (sum < second) return false;
        if (sum - first >= m) return true;
        return arbitrary[first] + convex[sum - first] >= arbitrary[second] + convex[sum - second];
    };
    auto choice = monotoneMinima(n + m - 1, n, select);
    vector<long long> result(n + m - 1);
    for (int sum = 0; sum < result.size(); ++sum) {
        int first = choice[sum];
        result[sum] = arbitrary[first] + convex[sum - first];
    }
    return result;
}

inline vector<long long> minPlusConcaveArbitrary(const vector<long long>& concave, const vector<long long>& arbitrary) {
    int n = arbitrary.size(), m = concave.size(), height = n + m - 1;
    vector<int> minimumFirst(height), maximumFirst(height, n - 1);
    for (int sum = m; sum < height; ++sum) minimumFirst[sum] = sum - m + 1;
    for (int sum = 0; sum <= height - m; ++sum) maximumFirst[sum] = sum;
    vector<int> minimumSum(n), maximumSum(n);
    for (int first = 0; first < n; ++first) minimumSum[first] = first, maximumSum[first] = first + m - 1;
    vector<long long> result(height, numeric_limits<long long>::max());

    function<void(int, int, int, int)> solve = [&](int leftSum, int rightSum, int leftFirst, int rightFirst) {
        if (maximumFirst[leftSum] >= rightFirst && leftFirst >= minimumFirst[rightSum]) {
            auto value = [&](int row, int reversedColumn) {
                int first = rightFirst - reversedColumn;
                int sum = leftSum + row;
                return arbitrary[first] + concave[sum - first];
            };
            auto choice =
                monotoneMinima(rightSum - leftSum + 1, rightFirst - leftFirst + 1,
                               [&](int row, int first, int second) { return value(row, first) >= value(row, second); });
            for (int sum = leftSum; sum <= rightSum; ++sum) {
                result[sum] = min(result[sum], value(sum - leftSum, choice[sum - leftSum]));
            }
            return;
        }
        if (1LL * (rightSum - leftSum) * (rightFirst - leftFirst) < 1000) {
            for (int sum = leftSum; sum <= rightSum; ++sum) {
                int begin = max(minimumFirst[sum], leftFirst);
                int end = min(maximumFirst[sum], rightFirst);
                for (int first = begin; first <= end; ++first) {
                    result[sum] = min(result[sum], arbitrary[first] + concave[sum - first]);
                }
            }
            return;
        }
        if (rightSum - leftSum > rightFirst - leftFirst) {
            int middle = midpoint(leftSum, rightSum);
            int nextRight = min(maximumFirst[middle], rightFirst);
            if (leftFirst <= nextRight) solve(leftSum, middle, leftFirst, nextRight);
            int nextLeft = max(minimumFirst[middle + 1], leftFirst);
            if (nextLeft <= rightFirst) solve(middle + 1, rightSum, nextLeft, rightFirst);
        } else {
            int middle = midpoint(leftFirst, rightFirst);
            int nextRight = min(maximumSum[middle], rightSum);
            if (leftSum <= nextRight) solve(leftSum, nextRight, leftFirst, middle);
            int nextLeft = max(minimumSum[middle + 1], leftSum);
            if (nextLeft <= rightSum) solve(nextLeft, rightSum, middle + 1, rightFirst);
        }
    };
    solve(0, height - 1, 0, n - 1);
    return result;
}
#endif


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<long long> a(n), b(m);
    for (auto& value : a) cin >> value;
    for (auto& value : b) cin >> value;
    auto result = minPlusConvexConvex(a, b);
    for (int i = 0; i < result.size(); ++i) cout << result[i] << " \n"[i + 1 == result.size()];
}
