#include <bits/stdc++.h>

using namespace std;

using Path = vector<pair<char, long long>>;

struct Fraction {
    long long numerator, denominator;
};

Path encode(Fraction value) {
    Path path;
    while (value.numerator != 1 || value.denominator != 1) {
        if (value.numerator > value.denominator) {
            long long count = (value.numerator - 1) / value.denominator;
            path.push_back({'R', count});
            value.numerator -= count * value.denominator;
        } else {
            long long count = (value.denominator - 1) / value.numerator;
            path.push_back({'L', count});
            value.denominator -= count * value.numerator;
        }
    }
    return path;
}

Fraction decode(Path path, Fraction value = {1, 1}) {
    reverse(path.begin(), path.end());
    for (auto [direction, count] : path) {
        if (direction == 'L') {
            value.denominator += value.numerator * count;
        } else {
            value.numerator += value.denominator * count;
        }
    }
    return value;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int queries;
    cin >> queries;
    while (queries--) {
        string operation;
        cin >> operation;
        if (operation == "ENCODE_PATH") {
            Fraction value;
            cin >> value.numerator >> value.denominator;
            auto path = encode(value);
            cout << path.size();
            for (auto [direction, count] : path) {
                cout << ' ' << direction << ' ' << count;
            }
            cout << '\n';
        } else if (operation == "DECODE_PATH") {
            int size;
            cin >> size;
            Path path(size);
            for (auto& [direction, count] : path) {
                cin >> direction >> count;
            }
            auto value = decode(path);
            cout << value.numerator << ' ' << value.denominator << '\n';
        } else if (operation == "LCA") {
            Fraction a, b;
            cin >> a.numerator >> a.denominator >> b.numerator >> b.denominator;
            auto first = encode(a);
            auto second = encode(b);
            Path common;
            for (int i = 0; i < int(min(first.size(), second.size())); i++) {
                if (first[i].first != second[i].first) {
                    break;
                }
                common.push_back({first[i].first, min(first[i].second, second[i].second)});
                if (first[i].second != second[i].second) {
                    break;
                }
            }
            auto answer = decode(common);
            cout << answer.numerator << ' ' << answer.denominator << '\n';
        } else if (operation == "ANCESTOR") {
            long long depth;
            Fraction value;
            cin >> depth >> value.numerator >> value.denominator;
            Path prefix;
            bool found = false;
            for (auto [direction, count] : encode(value)) {
                if (depth <= count) {
                    if (depth) {
                        prefix.push_back({direction, depth});
                    }
                    found = true;
                    break;
                }
                prefix.push_back({direction, count});
                depth -= count;
            }
            if (!found) {
                cout << -1 << '\n';
            } else {
                auto answer = decode(prefix);
                cout << answer.numerator << ' ' << answer.denominator << '\n';
            }
        } else if (operation == "RANGE") {
            Fraction value;
            cin >> value.numerator >> value.denominator;
            auto path = encode(value);
            auto lower = decode(path, {0, 1});
            auto upper = decode(path, {1, 0});
            cout << lower.numerator << ' ' << lower.denominator << ' ' << upper.numerator << ' ' << upper.denominator
                 << '\n';
        }
    }
    return 0;
}
