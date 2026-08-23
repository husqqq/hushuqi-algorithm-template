#ifndef TEMPLATE_INLINE_808C12CA16F13BEC70A7
#define TEMPLATE_INLINE_808C12CA16F13BEC70A7

#include <bits/stdc++.h>

using namespace std;

template <int LOG = 30> class WaveletMatrix {
    int size;
    array<int, LOG> middle;
    array<vector<int>, LOG> prefixOne;
    array<vector<long long>, LOG> prefixZeroSum;

  public:
    explicit WaveletMatrix(vector<int> values = {}) { init(move(values)); }

    void init(vector<int> values) {
        size = values.size();
        vector<int> next(size);
        for (int bit = LOG - 1; bit >= 0; --bit) {
            prefixOne[bit].assign(size + 1, 0);
            prefixZeroSum[bit].assign(size + 1, 0);
            for (int i = 0; i < size; ++i) {
                bool one = values[i] >> bit & 1;
                prefixOne[bit][i + 1] = prefixOne[bit][i] + one;
                prefixZeroSum[bit][i + 1] = prefixZeroSum[bit][i] + (one ? 0 : values[i]);
            }
            middle[bit] = size - prefixOne[bit][size];
            int zero = 0, one = middle[bit];
            for (int value : values) {
                if (value >> bit & 1) next[one++] = value;
                else next[zero++] = value;
            }
            values.swap(next);
        }
    }

    int kth(int left, int right, int rank) const {
        int answer = 0;
        for (int bit = LOG - 1; bit >= 0; --bit) {
            int onesLeft = prefixOne[bit][left];
            int onesRight = prefixOne[bit][right];
            int zeros = right - left - onesRight + onesLeft;
            if (rank < zeros) {
                left -= onesLeft;
                right -= onesRight;
            } else {
                answer |= 1 << bit;
                rank -= zeros;
                left = middle[bit] + onesLeft;
                right = middle[bit] + onesRight;
            }
        }
        return answer;
    }

    pair<int, long long> countAndSumLess(int left, int right, int upperBound) const {
        int count = 0;
        long long sum = 0;
        for (int bit = LOG - 1; bit >= 0; --bit) {
            int onesLeft = prefixOne[bit][left];
            int onesRight = prefixOne[bit][right];
            int zeros = right - left - onesRight + onesLeft;
            if (upperBound >> bit & 1) {
                count += zeros;
                sum += prefixZeroSum[bit][right] - prefixZeroSum[bit][left];
                left = middle[bit] + onesLeft;
                right = middle[bit] + onesRight;
            } else {
                left -= onesLeft;
                right -= onesRight;
            }
        }
        return {count, sum};
    }

    int countLess(int left, int right, int upperBound) const { return countAndSumLess(left, right, upperBound).first; }

    int frequency(int left, int right, int value) const {
        return countLess(left, right, value + 1) - countLess(left, right, value);
    }

    pair<int, long long> countAndSumLessEqual(int left, int right, int upperBound) const {
        return countAndSumLess(left, right, upperBound + 1);
    }
};
#endif


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (auto& x : a) cin >> x;
    WaveletMatrix matrix(move(a));
    while (q--) {
        int left, right, value;
        cin >> left >> right >> value;
        cout << matrix.frequency(left, right, value) << '\n';
    }
}
