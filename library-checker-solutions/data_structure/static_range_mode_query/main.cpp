#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> a(n), values;
    for (int& x : a) {
        cin >> x;
    }
    values = a;
    sort(values.begin(), values.end());
    values.erase(unique(values.begin(), values.end()), values.end());
    vector<vector<int>> positions(values.size());
    for (int i = 0; i < n; i++) {
        a[i] = lower_bound(values.begin(), values.end(), a[i]) - values.begin();
        positions[a[i]].push_back(i);
    }

    int blockSize = max(1, int(sqrt(n)));
    int blocks = (n + blockSize - 1) / blockSize;
    vector<vector<int>> mode(blocks, vector<int>(blocks, -1));
    vector<int> frequency(values.size());
    for (int leftBlock = 0; leftBlock < blocks; leftBlock++) {
        fill(frequency.begin(), frequency.end(), 0);
        int best = -1;
        for (int i = leftBlock * blockSize; i < n; i++) {
            int x = a[i];
            frequency[x]++;
            if (best == -1 || frequency[x] > frequency[best]) {
                best = x;
            }
            if ((i + 1) % blockSize == 0 || i + 1 == n) {
                mode[leftBlock][i / blockSize] = best;
            }
        }
    }

    auto countInRange = [&](int value, int l, int r) {
        auto& list = positions[value];
        return int(lower_bound(list.begin(), list.end(), r) - lower_bound(list.begin(), list.end(), l));
    };
    while (q--) {
        int l, r;
        cin >> l >> r;
        int firstFull = (l + blockSize - 1) / blockSize;
        int afterFull = r / blockSize;
        int best = -1, bestCount = 0;
        if (firstFull < afterFull) {
            best = mode[firstFull][afterFull - 1];
            bestCount = countInRange(best, l, r);
        }
        auto consider = [&](int index) {
            int value = a[index];
            int count = countInRange(value, l, r);
            if (count > bestCount) {
                best = value;
                bestCount = count;
            }
        };
        int leftEnd = min(r, firstFull * blockSize);
        for (int i = l; i < leftEnd; i++) {
            consider(i);
        }
        int rightStart = max(l, afterFull * blockSize);
        for (int i = rightStart; i < r; i++) {
            consider(i);
        }
        cout << values[best] << ' ' << bestCount << '\n';
    }
    return 0;
}
