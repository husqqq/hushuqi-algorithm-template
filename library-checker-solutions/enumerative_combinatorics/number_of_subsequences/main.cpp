#ifndef TEMPLATE_INLINE_22FD82FDAAAA7108A788
#define TEMPLATE_INLINE_22FD82FDAAAA7108A788

#include <bits/stdc++.h>

using namespace std;

class DistinctSubsequenceCounter {
  public:
    static int countNonempty(const vector<long long>& values, int modulus = 998244353) {
        unordered_map<long long, int> previous;
        previous.reserve(values.size() * 2 + 1);
        long long total = 1;
        for (long long value : values) {
            long long next = (2 * total - previous[value] + modulus) % modulus;
            previous[value] = total;
            total = next;
        }
        return (total + modulus - 1) % modulus;
    }
};
#endif


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> values(n);
    for (auto& value : values) cin >> value;
    cout << DistinctSubsequenceCounter::countNonempty(values) << '\n';
}
