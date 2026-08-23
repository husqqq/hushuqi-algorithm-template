#include <bits/stdc++.h>

using namespace std;

constexpr long long P = 998244353;

struct Matrix {
    long long a = 1, b = 0, c = 0, d = 1;
    bool operator==(const Matrix&) const = default;
};

Matrix multiply(Matrix x, Matrix y) {
    return {(x.a * y.a + x.b * y.c) % P, (x.a * y.b + x.b * y.d) % P, (x.c * y.a + x.d * y.c) % P,
            (x.c * y.b + x.d * y.d) % P};
}

Matrix inverse(Matrix x) {
    return {x.d, (P - x.b) % P, (P - x.c) % P, x.a};
}

struct DSU {
    vector<int> parent;
    vector<Matrix> potential;

    DSU(int n) : parent(n, -1), potential(n) {}

    int find(int u) {
        if (parent[u] < 0) {
            return u;
        }
        int p = parent[u];
        parent[u] = find(p);
        potential[u] = multiply(potential[p], potential[u]);
        return parent[u];
    }

    bool unite(int first, int second, Matrix difference) {
        int rootFirst = find(first), rootSecond = find(second);
        if (rootFirst == rootSecond) {
            return multiply(inverse(potential[first]), potential[second]) == difference;
        }
        if (parent[rootFirst] < parent[rootSecond]) {
            swap(rootFirst, rootSecond);
            swap(first, second);
            difference = inverse(difference);
        }
        parent[rootSecond] += parent[rootFirst];
        parent[rootFirst] = rootSecond;
        potential[rootFirst] = multiply(multiply(potential[second], inverse(difference)), inverse(potential[first]));
        return true;
    }

    Matrix difference(int first, int second) {
        find(first);
        find(second);
        return multiply(inverse(potential[first]), potential[second]);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    DSU dsu(n);
    while (q--) {
        int type, u, v;
        cin >> type >> u >> v;
        if (type == 0) {
            Matrix value;
            cin >> value.a >> value.b >> value.c >> value.d;
            cout << dsu.unite(v, u, value) << '\n';
        } else if (dsu.find(u) != dsu.find(v)) {
            cout << -1 << '\n';
        } else {
            auto value = dsu.difference(v, u);
            cout << value.a << ' ' << value.b << ' ' << value.c << ' ' << value.d << '\n';
        }
    }
    return 0;
}
