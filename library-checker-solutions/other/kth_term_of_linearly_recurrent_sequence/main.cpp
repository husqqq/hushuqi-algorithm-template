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
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        while (j & bit) j ^= bit, bit >>= 1;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int length = 2; length <= n; length *= 2) {
        int root = power(G, (P - 1) / length);
        if (inverse) root = power(root, P - 2);
        for (int start = 0; start < n; start += length) {
            long long value = 1;
            for (int i = 0; i < length / 2; i++) {
                int even = a[start + i];
                int odd = value * a[start + i + length / 2] % P;
                a[start + i] = even + odd;
                if (a[start + i] >= P) a[start + i] -= P;
                a[start + i + length / 2] = even - odd;
                if (a[start + i + length / 2] < 0) a[start + i + length / 2] += P;
                value = value * root % P;
            }
        }
    }
    if (inverse) {
        long long inverseN = power(n, P - 2);
        for (int& value : a) value = value * inverseN % P;
    }
}

vector<int> convolution(const vector<int>& first, const vector<int>& second) {
    if (first.empty() || second.empty()) return {};
    int wanted = first.size() + second.size() - 1;
    vector<int> a = first, b = second;
    if (min(a.size(), b.size()) <= 32) {
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
    for (int size = 2; size / 2 < n; size *= 2) {
        int length = min(size, n);
        vector<int> prefix(f.begin(), f.begin() + min<int>(f.size(), length));
        auto product = convolution(prefix, result);
        product.resize(length);
        for (int& value : product) value = value ? P - value : 0;
        product[0] = (product[0] + 2) % P;
        result = convolution(result, product);
        result.resize(length);
    }
    result.resize(n);
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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int degree;
    long long index;
    cin >> degree >> index;
    vector<int> initial(degree), denominator(degree + 1);
    for (int& x : initial) cin >> x;
    denominator[0] = 1;
    for (int i = 1; i <= degree; i++) {
        cin >> denominator[i];
        if (denominator[i]) denominator[i] = P - denominator[i];
    }
    auto numerator = convolution(initial, denominator);
    numerator.resize(degree);
    cout << rationalCoefficient(numerator, denominator, index) << '\n';
}
