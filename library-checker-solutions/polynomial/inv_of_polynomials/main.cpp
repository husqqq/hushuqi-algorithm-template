#include <bits/stdc++.h>

using namespace std;

constexpr int P = 998244353;
constexpr int G = 3;

long long power(long long a, long long b) {
    long long result = 1;
    while (b) {
        if (b & 1) result = result * a % P;
        a = a * a % P;
        b >>= 1;
    }
    return result;
}

void ntt(vector<int>& a, bool inverse) {
    int n = a.size();
    int height = __builtin_ctz(n);
    static array<int, 24> root, inverseRoot, rate2, inverseRate2, rate3, inverseRate3;
    static bool prepared = false;
    if (!prepared) {
        prepared = true;
        root[23] = power(G, (P - 1) >> 23);
        inverseRoot[23] = power(root[23], P - 2);
        for (int i = 22; i >= 0; i--) {
            root[i] = 1LL * root[i + 1] * root[i + 1] % P;
            inverseRoot[i] = 1LL * inverseRoot[i + 1] * inverseRoot[i + 1] % P;
        }
        long long product = 1, inverseProduct = 1;
        for (int i = 0; i <= 21; i++) {
            rate2[i] = root[i + 2] * product % P;
            inverseRate2[i] = inverseRoot[i + 2] * inverseProduct % P;
            product = product * inverseRoot[i + 2] % P;
            inverseProduct = inverseProduct * root[i + 2] % P;
        }
        product = inverseProduct = 1;
        for (int i = 0; i <= 20; i++) {
            rate3[i] = root[i + 3] * product % P;
            inverseRate3[i] = inverseRoot[i + 3] * inverseProduct % P;
            product = product * inverseRoot[i + 3] % P;
            inverseProduct = inverseProduct * root[i + 3] % P;
        }
    }
    if (!inverse) {
        int level = 0;
        while (level < height) {
            if (height - level == 1) {
                int width = 1 << (height - level - 1);
                long long rotation = 1;
                for (int block = 0; block < (1 << level); block++) {
                    int offset = block << (height - level);
                    for (int i = 0; i < width; i++) {
                        int left = a[offset + i];
                        int right = a[offset + i + width] * rotation % P;
                        a[offset + i] = left + right;
                        if (a[offset + i] >= P) a[offset + i] -= P;
                        a[offset + i + width] = left - right;
                        if (a[offset + i + width] < 0) a[offset + i + width] += P;
                    }
                    rotation = rotation * rate2[__builtin_ctz(~block)] % P;
                }
                level++;
            } else {
                int width = 1 << (height - level - 2);
                long long rotation = 1;
                long long imaginary = root[2];
                long long squareMod = 1LL * P * P;
                for (int block = 0; block < (1 << level); block++) {
                    long long rotation2 = rotation * rotation % P;
                    long long rotation3 = rotation2 * rotation % P;
                    int offset = block << (height - level);
                    for (int i = 0; i < width; i++) {
                        long long a0 = a[offset + i];
                        long long a1 = 1LL * a[offset + i + width] * rotation;
                        long long a2 = 1LL * a[offset + i + 2 * width] * rotation2;
                        long long a3 = 1LL * a[offset + i + 3 * width] * rotation3;
                        long long odd = (a1 + squareMod - a3) % P * imaginary % P;
                        long long negativeA2 = squareMod - a2;
                        a[offset + i] = (a0 + a2 + a1 + a3) % P;
                        a[offset + i + width] = (a0 + a2 + 2 * squareMod - a1 - a3) % P;
                        a[offset + i + 2 * width] = (a0 + negativeA2 + odd) % P;
                        a[offset + i + 3 * width] = (a0 + negativeA2 + squareMod - odd) % P;
                    }
                    rotation = rotation * rate3[__builtin_ctz(~block)] % P;
                }
                level += 2;
            }
        }
    } else {
        long long inverseN = power(n, P - 2);
        for (int& value : a) value = value * inverseN % P;
        int level = height;
        while (level) {
            if (level == 1) {
                int width = 1 << (height - level);
                long long rotation = 1;
                for (int block = 0; block < (1 << (level - 1)); block++) {
                    int offset = block << (height - level + 1);
                    for (int i = 0; i < width; i++) {
                        long long left = a[offset + i];
                        long long right = a[offset + i + width];
                        a[offset + i] = (left + right) % P;
                        a[offset + i + width] = (P + left - right) * rotation % P;
                    }
                    rotation = rotation * inverseRate2[__builtin_ctz(~block)] % P;
                }
                level--;
            } else {
                int width = 1 << (height - level);
                long long rotation = 1;
                long long imaginary = inverseRoot[2];
                for (int block = 0; block < (1 << (level - 2)); block++) {
                    long long rotation2 = rotation * rotation % P;
                    long long rotation3 = rotation2 * rotation % P;
                    int offset = block << (height - level + 2);
                    for (int i = 0; i < width; i++) {
                        long long a0 = a[offset + i];
                        long long a1 = a[offset + i + width];
                        long long a2 = a[offset + i + 2 * width];
                        long long a3 = a[offset + i + 3 * width];
                        long long x = (P + a2 - a3) * imaginary % P;
                        a[offset + i] = (a0 + a1 + a2 + a3) % P;
                        a[offset + i + width] = (a0 + P - a1 + x) * rotation % P;
                        a[offset + i + 2 * width] = (a0 + a1 + 2LL * P - a2 - a3) * rotation2 % P;
                        a[offset + i + 3 * width] = (a0 + 2LL * P - a1 - x) * rotation3 % P;
                    }
                    rotation = rotation * inverseRate3[__builtin_ctz(~block)] % P;
                }
                level -= 2;
            }
        }
    }
}

vector<int> convolution(const vector<int>& first, const vector<int>& second) {
    if (first.empty() || second.empty()) return {};
    int wanted = first.size() + second.size() - 1;
    vector<int> a = first, b = second;
    if (min(a.size(), b.size()) <= 50) {
        vector<int> result(wanted);
        for (int i = 0; i < int(a.size()); i++) {
            for (int j = 0; j < int(b.size()); j++) {
                result[i + j] = (result[i + j] + 1LL * a[i] * b[j]) % P;
            }
        }
        return result;
    }
    int size = 1;
    while (size < wanted) size *= 2;
    if (int(a.size() + b.size()) - 3 <= size / 2) {
        int lastA = a.back(), lastB = b.back();
        a.pop_back();
        b.pop_back();
        auto result = convolution(a, b);
        result.resize(wanted);
        result[wanted - 1] = 1LL * lastA * lastB % P;
        for (int i = 0; i < int(a.size()); i++) {
            result[i + b.size()] = (result[i + b.size()] + 1LL * a[i] * lastB) % P;
        }
        for (int i = 0; i < int(b.size()); i++) {
            result[i + a.size()] = (result[i + a.size()] + 1LL * b[i] * lastA) % P;
        }
        return result;
    }
    a.resize(size);
    b.resize(size);
    ntt(a, false);
    ntt(b, false);
    for (int i = 0; i < size; i++) a[i] = 1LL * a[i] * b[i] % P;
    ntt(a, true);
    a.resize(wanted);
    return a;
}

void trim(vector<int>& a) {
    while (!a.empty() && a.back() == 0) a.pop_back();
}

vector<int> inverseSeries(const vector<int>& f, int n) {
    vector<int> result(1, power(f[0], P - 2));
    result.reserve(n);
    for (int size = 1; size < n; size *= 2) {
        vector<int> prefix(2 * size), current(2 * size);
        for (int i = 0; i < min<int>(f.size(), 2 * size); i++) prefix[i] = f[i];
        for (int i = 0; i < size; i++) current[i] = result[i];
        ntt(prefix, false);
        ntt(current, false);
        for (int i = 0; i < 2 * size; i++) prefix[i] = 1LL * prefix[i] * current[i] % P;
        ntt(prefix, true);
        fill(prefix.begin(), prefix.begin() + size, 0);
        ntt(prefix, false);
        for (int i = 0; i < 2 * size; i++) prefix[i] = 1LL * prefix[i] * current[i] % P;
        ntt(prefix, true);
        for (int i = size; i < min(n, 2 * size); i++) result.push_back(prefix[i] ? P - prefix[i] : 0);
    }
    return result;
}

vector<int> derivative(const vector<int>& f) {
    if (f.size() <= 1) return {};
    vector<int> result(f.size() - 1);
    for (int i = 1; i < int(f.size()); i++) result[i - 1] = 1LL * i * f[i] % P;
    return result;
}

vector<int> integralSeries(const vector<int>& f) {
    vector<int> result(f.size() + 1);
    for (int i = 0; i < int(f.size()); i++) result[i + 1] = 1LL * f[i] * power(i + 1, P - 2) % P;
    return result;
}

vector<int> logarithm(const vector<int>& f, int n) {
    auto result = convolution(derivative(f), inverseSeries(f, n));
    if (n > 0) result.resize(n - 1);
    result = integralSeries(result);
    result.resize(n);
    return result;
}

vector<int> exponential(const vector<int>& f, int n) {
    vector<int> result(1, 1);
    for (int size = 2; size / 2 < n; size *= 2) {
        int length = min(size, n);
        auto correction = logarithm(result, length);
        correction.resize(length);
        for (int i = 0; i < length; i++) {
            int target = i < int(f.size()) ? f[i] : 0;
            correction[i] = target - correction[i];
            if (correction[i] < 0) correction[i] += P;
        }
        correction[0] = (correction[0] + 1) % P;
        result = convolution(result, correction);
        result.resize(length);
    }
    result.resize(n);
    return result;
}

vector<int> powerSeries(const vector<int>& f, long long exponent, int n) {
    vector<int> answer(n);
    if (exponent == 0) {
        if (n) answer[0] = 1;
        return answer;
    }
    int leading = 0;
    while (leading < int(f.size()) && f[leading] == 0) leading++;
    if (leading == int(f.size()) || 1LL * leading * exponent >= n) return answer;
    int shift = leading * exponent;
    int length = n - shift;
    int constant = f[leading];
    long long inverseConstant = power(constant, P - 2);
    vector<int> normalized(length);
    for (int i = 0; i < length && leading + i < int(f.size()); i++) {
        normalized[i] = f[leading + i] * inverseConstant % P;
    }
    auto logarithmValue = logarithm(normalized, length);
    for (int& value : logarithmValue) value = value * (exponent % P) % P;
    auto result = exponential(logarithmValue, length);
    long long scale = power(constant, exponent);
    for (int i = 0; i < length; i++) answer[i + shift] = result[i] * scale % P;
    return answer;
}

int modularSqrt(int value) {
    if (value == 0) return 0;
    if (power(value, (P - 1) / 2) != 1) return -1;
    int odd = P - 1, exponent = 0;
    while (!(odd & 1)) odd >>= 1, exponent++;
    int nonResidue = 2;
    while (power(nonResidue, (P - 1) / 2) == 1) nonResidue++;
    long long c = power(nonResidue, odd);
    long long x = power(value, (odd + 1) / 2);
    long long t = power(value, odd);
    int m = exponent;
    while (t != 1) {
        int i = 1;
        long long current = t * t % P;
        while (current != 1) current = current * current % P, i++;
        long long b = power(c, 1LL << (m - i - 1));
        x = x * b % P;
        c = b * b % P;
        t = t * c % P;
        m = i;
    }
    return x;
}

optional<vector<int>> squareRootSeries(const vector<int>& f, int n) {
    int leading = 0;
    while (leading < n && f[leading] == 0) leading++;
    if (leading == n) return vector<int>(n);
    if (leading & 1) return nullopt;
    int shift = leading / 2;
    int constantRoot = modularSqrt(f[leading]);
    if (constantRoot == -1) return nullopt;
    int length = n - 2 * shift;
    vector<int> normalized(length);
    for (int i = 0; i < length && leading + i < int(f.size()); i++) normalized[i] = f[leading + i];
    vector<int> result(1, constantRoot);
    long long inverseTwo = (P + 1) / 2;
    for (int size = 2; size / 2 < length; size *= 2) {
        int currentLength = min(size, length);
        auto quotient = convolution(vector<int>(normalized.begin(), normalized.begin() + currentLength),
                                    inverseSeries(result, currentLength));
        quotient.resize(currentLength);
        result.resize(currentLength);
        for (int i = 0; i < currentLength; i++) {
            result[i] = (result[i] + quotient[i]) * inverseTwo % P;
        }
    }
    vector<int> answer(n);
    for (int i = 0; i < length; i++) answer[i + shift] = result[i];
    return answer;
}

pair<vector<int>, vector<int>> dividePolynomials(vector<int> f, vector<int> g) {
    trim(f);
    trim(g);
    if (f.size() < g.size()) return {{}, f};
    int quotientSize = f.size() - g.size() + 1;
    vector<int> reversedF = f, reversedG = g;
    reverse(reversedF.begin(), reversedF.end());
    reverse(reversedG.begin(), reversedG.end());
    reversedF.resize(quotientSize);
    auto quotient = convolution(reversedF, inverseSeries(reversedG, quotientSize));
    quotient.resize(quotientSize);
    reverse(quotient.begin(), quotient.end());
    auto product = convolution(quotient, g);
    f.resize(max(f.size(), product.size()));
    for (int i = 0; i < int(product.size()); i++) {
        f[i] -= product[i];
        if (f[i] < 0) f[i] += P;
    }
    f.resize(g.size() - 1);
    trim(f);
    trim(quotient);
    return {quotient, f};
}

vector<int> polynomialRemainder(vector<int> f, const vector<int>& g) {
    if (f.size() < g.size()) {
        trim(f);
        return f;
    }
    return dividePolynomials(move(f), g).second;
}

int rationalCoefficient(vector<int> numerator, vector<int> denominator, long long index) {
    while (index) {
        vector<int> negative = denominator;
        for (int i = 1; i < int(negative.size()); i += 2) {
            if (negative[i]) negative[i] = P - negative[i];
        }
        auto first = convolution(numerator, negative);
        auto second = convolution(denominator, negative);
        vector<int> nextNumerator, nextDenominator;
        for (int i = index & 1; i < int(first.size()); i += 2) nextNumerator.push_back(first[i]);
        for (int i = 0; i < int(second.size()); i += 2) nextDenominator.push_back(second[i]);
        numerator = move(nextNumerator);
        denominator = move(nextDenominator);
        index >>= 1;
    }
    return numerator.empty() ? 0 : numerator[0] * power(denominator[0], P - 2) % P;
}

struct ProductTree {
    int count, size = 1;
    vector<vector<int>> product;

    ProductTree(const vector<int>& points) : count(points.size()) {
        while (size < count) size *= 2;
        product.resize(2 * size);
        for (int i = 0; i < size; i++) {
            product[size + i] = i < count ? vector<int>{points[i] ? P - points[i] : 0, 1} : vector<int>{1};
        }
        for (int node = size - 1; node; node--) product[node] = convolution(product[node * 2], product[node * 2 + 1]);
    }

    vector<int> evaluate(const vector<int>& polynomial) const {
        vector<int> answer(count);
        auto dfs = [&](auto&& self, int node, vector<int> remainder) -> void {
            if (node >= size) {
                int index = node - size;
                if (index < count) answer[index] = remainder.empty() ? 0 : remainder[0];
                return;
            }
            self(self, node * 2, polynomialRemainder(remainder, product[node * 2]));
            self(self, node * 2 + 1, polynomialRemainder(move(remainder), product[node * 2 + 1]));
        };
        dfs(dfs, 1, polynomialRemainder(polynomial, product[1]));
        return answer;
    }

    vector<int> interpolate(const vector<int>& values) const {
        auto denominators = evaluate(derivative(product[1]));
        vector<int> weight(count);
        for (int i = 0; i < count; i++) weight[i] = 1LL * values[i] * power(denominators[i], P - 2) % P;
        auto dfs = [&](auto&& self, int node) -> vector<int> {
            if (node >= size) {
                int index = node - size;
                return index < count ? vector<int>{weight[index]} : vector<int>{0};
            }
            auto left = self(self, node * 2);
            auto right = self(self, node * 2 + 1);
            auto first = convolution(left, product[node * 2 + 1]);
            auto second = convolution(right, product[node * 2]);
            first.resize(max(first.size(), second.size()));
            for (int i = 0; i < int(second.size()); i++) {
                first[i] += second[i];
                if (first[i] >= P) first[i] -= P;
            }
            trim(first);
            return first;
        };
        auto answer = dfs(dfs, 1);
        answer.resize(count);
        return answer;
    }
};

void printPolynomial(const vector<int>& a) {
    for (int i = 0; i < int(a.size()); i++) cout << a[i] << " \n"[i + 1 == int(a.size())];
    if (a.empty()) cout << '\n';
}

namespace halfGcd {

using Poly = vector<int>;
using PolyPair = array<Poly, 2>;
using Matrix = array<Poly, 4>;

Poly add(const Poly& a, const Poly& b) {
    Poly result(max(a.size(), b.size()));
    for (int i = 0; i < int(a.size()); i++) result[i] = a[i];
    for (int i = 0; i < int(b.size()); i++) {
        result[i] += b[i];
        if (result[i] >= P) result[i] -= P;
    }
    trim(result);
    return result;
}

Poly subtract(const Poly& a, const Poly& b) {
    Poly result(max(a.size(), b.size()));
    for (int i = 0; i < int(a.size()); i++) result[i] = a[i];
    for (int i = 0; i < int(b.size()); i++) {
        result[i] -= b[i];
        if (result[i] < 0) result[i] += P;
    }
    trim(result);
    return result;
}

Matrix multiply(const Matrix& a, const Matrix& b) {
    return {
        add(convolution(a[0], b[0]), convolution(a[1], b[2])), add(convolution(a[0], b[1]), convolution(a[1], b[3])),
        add(convolution(a[2], b[0]), convolution(a[3], b[2])), add(convolution(a[2], b[1]), convolution(a[3], b[3]))};
}

PolyPair multiply(const Matrix& a, const PolyPair& b) {
    return {add(convolution(a[0], b[0]), convolution(a[1], b[1])),
            add(convolution(a[2], b[0]), convolution(a[3], b[1]))};
}

Matrix step(const Poly& quotient) {
    Matrix result;
    result[1] = {1};
    result[2] = {1};
    result[3] = subtract({}, quotient);
    return result;
}

Matrix reduceHalf(PolyPair a) {
    int middle = a[0].size() / 2;
    if (int(a[1].size()) <= middle) {
        Matrix identity;
        identity[0] = {1};
        identity[3] = {1};
        return identity;
    }
    Matrix result = reduceHalf({Poly(a[0].begin() + middle, a[0].end()), Poly(a[1].begin() + middle, a[1].end())});
    a = multiply(result, a);
    if (int(a[1].size()) <= middle) return result;
    Matrix quotient = step(dividePolynomials(a[0], a[1]).first);
    result = multiply(quotient, result);
    a = multiply(quotient, a);
    if (int(a[1].size()) <= middle) return result;
    int cut = 2 * middle + 1 - a[0].size();
    Matrix suffix = reduceHalf({Poly(a[0].begin() + cut, a[0].end()), Poly(a[1].begin() + cut, a[1].end())});
    return multiply(suffix, result);
}

Matrix reduceAll(PolyPair a) {
    Matrix result = reduceHalf(a);
    a = multiply(result, a);
    if (a[1].empty()) return result;
    Matrix quotient = step(dividePolynomials(a[0], a[1]).first);
    result = multiply(quotient, result);
    a = multiply(quotient, a);
    if (a[1].empty()) return result;
    return multiply(reduceAll(a), result);
}

tuple<Poly, Poly, Poly> extendedGcd(const Poly& f, const Poly& g) {
    Matrix result = step(dividePolynomials(f, g).first);
    PolyPair remainder = multiply(result, PolyPair{f, g});
    if (!remainder[1].empty()) result = multiply(reduceAll(remainder), result);
    return {add(convolution(f, result[0]), convolution(g, result[1])), result[0], result[1]};
}

} // namespace halfGcd

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<int> f(n), g(m);
    for (int& x : f) cin >> x;
    for (int& x : g) cin >> x;
    trim(f);
    trim(g);

    auto [gcd, inverse, coefficient] = halfGcd::extendedGcd(f, g);
    if (gcd.size() != 1) {
        cout << -1 << '\n';
        return 0;
    }
    long long scale = power(gcd[0], P - 2);
    for (int& value : inverse) value = value * scale % P;
    trim(inverse);
    cout << inverse.size() << '\n';
    printPolynomial(inverse);
}
