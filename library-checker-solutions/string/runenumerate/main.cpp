#include <bits/stdc++.h>

using namespace std;

vector<int> zAlgorithm(const vector<int>& sequence) {
    int n = sequence.size();
    vector<int> z(n + 1);
    for (int i = 1, left = 0; i <= n; i++) {
        z[i] = left + z[left] <= i ? 0 : min(left + z[left] - i, z[i - left]);
        while (i + z[i] < n && sequence[z[i]] == sequence[i + z[i]]) z[i]++;
        if (left + z[left] < i + z[i]) left = i;
    }
    z[0] = n;
    return z;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string text;
    cin >> text;
    int n = text.size();
    vector<int> sequence(text.begin(), text.end());
    vector<vector<pair<int, int>>> runs(n / 2 + 1);

    auto enumerateDirection = [&](vector<int>& value, int rounding) {
        auto solve = [&](auto&& self, int l, int r) -> void {
            if (r - l == 1) return;
            int mid = (l + r + rounding) / 2;
            self(self, l, mid);
            self(self, mid, r);
            vector<int> left(value.begin() + l, value.begin() + mid);
            reverse(left.begin(), left.end());
            vector<int> right(value.begin() + mid, value.begin() + r);
            right.insert(right.end(), value.begin() + l, value.begin() + r);
            auto leftZ = zAlgorithm(left);
            auto rightZ = zAlgorithm(right);
            for (int start = mid - 1; start >= l; start--) {
                int leftExtension = min(start - l, leftZ[mid - start]);
                int rightExtension = min(r - mid, rightZ[(r - l) - (mid - start)]);
                int period = mid - start;
                int runLeft = start - leftExtension;
                int runRight = mid + rightExtension;
                if (runRight - runLeft >= 2 * period) runs[period].push_back({runLeft, runRight});
            }
        };
        solve(solve, 0, n);
    };

    reverse(sequence.begin(), sequence.end());
    enumerateDirection(sequence, 0);
    for (auto& list : runs) {
        for (auto& [l, r] : list) tie(l, r) = pair{n - r, n - l};
    }
    reverse(sequence.begin(), sequence.end());
    enumerateDirection(sequence, 1);

    set<pair<int, int>> intervalSeen;
    for (int period = 1; period <= n / 2; period++) {
        auto& list = runs[period];
        sort(list.begin(), list.end(),
             [&](auto a, auto b) { return a.first != b.first ? a.first < b.first : a.second > b.second; });
        vector<pair<int, int>> filtered;
        for (auto interval : list) {
            if (!filtered.empty() && interval.second <= filtered.back().second) continue;
            filtered.push_back(interval);
        }
        list.clear();
        for (auto interval : filtered) {
            if (intervalSeen.insert(interval).second) list.push_back(interval);
        }
        sort(list.begin(), list.end());
    }
    int count = 0;
    for (auto& list : runs) count += list.size();
    cout << count << '\n';
    for (int period = 1; period <= n / 2; period++) {
        for (auto [l, r] : runs[period]) cout << period << ' ' << l << ' ' << r << '\n';
    }
    return 0;
}
