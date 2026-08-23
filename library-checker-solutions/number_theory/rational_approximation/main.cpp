#include <bits/stdc++.h>

using namespace std;

struct Fraction {
    long long numerator, denominator;
};

bool equal(Fraction a, Fraction b) {
    return a.numerator * b.denominator == a.denominator * b.numerator;
}

bool lessEqual(Fraction a, Fraction b) {
    return a.numerator * b.denominator <= a.denominator * b.numerator;
}

Fraction addMultiple(Fraction a, long long count, Fraction b) {
    return {a.numerator * count + b.numerator, a.denominator * count + b.denominator};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tests;
    cin >> tests;
    while (tests--) {
        long long limit, x, y;
        cin >> limit >> x >> y;
        Fraction target{x, y};
        Fraction lower{0, 1}, upper{1, 0};
        while (true) {
            auto search = [&](auto predicate) {
                long long low = 0, high = 1;
                while (predicate(high)) {
                    high *= 2;
                }
                while (high - low > 1) {
                    long long mid = (low + high) / 2;
                    if (predicate(mid)) {
                        low = mid;
                    } else {
                        high = mid;
                    }
                }
                return low;
            };
            long long leftSteps = search([&](long long count) {
                auto value = addMultiple(upper, count, lower);
                return value.numerator <= limit && value.denominator <= limit && lessEqual(value, target);
            });
            lower = addMultiple(upper, leftSteps, lower);
            if (equal(lower, target)) {
                upper = lower;
                break;
            }
            long long rightSteps = search([&](long long count) {
                auto value = addMultiple(lower, count, upper);
                return value.numerator <= limit && value.denominator <= limit && lessEqual(target, value);
            });
            upper = addMultiple(lower, rightSteps, upper);
            if (equal(upper, target)) {
                lower = upper;
                break;
            }
            if (leftSteps == 0 && rightSteps == 0) {
                break;
            }
        }
        cout << lower.numerator << ' ' << lower.denominator << ' ' << upper.numerator << ' ' << upper.denominator
             << '\n';
    }
    return 0;
}
