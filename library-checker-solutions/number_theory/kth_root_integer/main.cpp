#include <bits/stdc++.h>

using namespace std;

using u64 = unsigned long long;

bool powerAtMost(u64 base, int exponent, u64 limit) {
    __uint128_t result = 1;
    for (int i = 0; i < exponent; ++i) {
        result *= base;
        if (result > limit) return false;
    }
    return true;
}

u64 kthRoot(u64 value, int exponent) {
    if (exponent == 1) return value;
    u64 low = 0, high = u64{1} << 32;
    while (high - low > 1) {
        u64 middle = midpoint(low, high);
        if (powerAtMost(middle, exponent, value)) low = middle;
        else high = middle;
    }
    return low;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tests;
    cin >> tests;
    while (tests--) {
        u64 value;
        int exponent;
        cin >> value >> exponent;
        cout << kthRoot(value, exponent) << '\n';
    }
}
