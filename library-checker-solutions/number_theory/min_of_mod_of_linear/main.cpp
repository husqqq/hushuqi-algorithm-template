#include <bits/stdc++.h>

using namespace std;

pair<vector<int>, vector<int>> minimumSegments(int a, int b, int mod) {
    vector<int> boundary = {0}, step;
    int divisor = gcd(a, mod);
    a /= divisor;
    b /= divisor;
    mod /= divisor;
    int leftNumerator = 0, leftDenominator = 1;
    int rightNumerator = 1, rightDenominator = 1;
    int leftDeterminant = mod - a, rightDeterminant = a;
    int x = 0, y = b;
    while (y) {
        int count = rightDeterminant / leftDeterminant;
        rightDeterminant %= leftDeterminant;
        if (rightDeterminant == 0) {
            count--;
            rightDeterminant = leftDeterminant;
        }
        rightNumerator += count * leftNumerator;
        rightDenominator += count * leftDenominator;
        while (true) {
            count = max(0, (leftDeterminant - y + rightDeterminant - 1) / rightDeterminant);
            if (leftDeterminant - count * rightDeterminant <= 0) {
                break;
            }
            leftDeterminant -= count * rightDeterminant;
            leftNumerator += count * rightNumerator;
            leftDenominator += count * rightDenominator;
            count = y / leftDeterminant;
            y -= count * leftDeterminant;
            x += leftDenominator * count;
            boundary.push_back(x);
            step.push_back(leftDenominator);
        }
        count = leftDeterminant / rightDeterminant;
        leftDeterminant -= count * rightDeterminant;
        leftNumerator += count * rightNumerator;
        leftDenominator += count * rightDenominator;
    }
    return {boundary, step};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tests;
    cin >> tests;
    while (tests--) {
        long long n;
        int mod, a, b;
        cin >> n >> mod >> a >> b;
        auto [boundary, step] = minimumSegments(a, b, mod);
        long long x = 0;
        for (int i = 0; i + 1 < int(boundary.size()); i++) {
            if (boundary[i + 1] < n) {
                x = boundary[i + 1];
            } else {
                x = boundary[i] + (n - 1 - boundary[i]) / step[i] * step[i];
                break;
            }
        }
        cout << (1LL * a * x + b) % mod << '\n';
    }
    return 0;
}
