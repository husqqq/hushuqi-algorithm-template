#include <bits/stdc++.h>

using namespace std;

using i128 = __int128_t;

struct Gaussian {
    long long x, y;

    Gaussian operator-(const Gaussian& other) const { return {x - other.x, y - other.y}; }

    Gaussian operator*(const Gaussian& other) const { return {x * other.x - y * other.y, x * other.y + y * other.x}; }
};

long long roundedDivision(i128 value, i128 divisor) {
    if (value >= 0) {
        return (value + divisor / 2) / divisor;
    }
    return -((-value + divisor / 2) / divisor);
}

Gaussian quotient(const Gaussian& a, const Gaussian& b) {
    i128 norm = i128(b.x) * b.x + i128(b.y) * b.y;
    i128 real = i128(a.x) * b.x + i128(a.y) * b.y;
    i128 imaginary = i128(a.y) * b.x - i128(a.x) * b.y;
    return {roundedDivision(real, norm), roundedDivision(imaginary, norm)};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tests;
    cin >> tests;
    while (tests--) {
        Gaussian a, b;
        cin >> a.x >> a.y >> b.x >> b.y;
        while (b.x || b.y) {
            Gaussian remainder = a - quotient(a, b) * b;
            a = b;
            b = remainder;
        }
        cout << a.x << ' ' << a.y << '\n';
    }
    return 0;
}
