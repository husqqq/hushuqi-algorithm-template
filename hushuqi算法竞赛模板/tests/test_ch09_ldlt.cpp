#include "../include/full/9_2_009.hpp"

vector<vector<long double>> makeMatrix(const vector<vector<long double>> &l, const vector<long double> &d)
{
    // l 是单位下三角矩阵，d 是对角向量；返回 l*diag(d)*l^T。
    int n = d.size();
    vector a(n, vector<long double>(n));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n; k++)
            {
                a[i][j] += l[i][k] * d[k] * l[j][k];
            }
        }
    }
    return a;
}

signed main()
{
    vector<vector<long double>> l{{1.0L, 0.0L, 0.0L}, {2.0L, 1.0L, 0.0L}, {-1.0L, 3.0L, 1.0L}};
    vector<long double> d{4, 5, -2};
    auto a = makeMatrix(l, d);
    LDLT fact(a);
    assert(fact.valid);
    for (int i = 0; i < 3; i++)
    {
        assert(fabsl(fact.d[i] - d[i]) < 1E-10L);
        for (int j = 0; j <= i; j++)
        {
            assert(fabsl(fact.l[i][j] - l[i][j]) < 1E-10L);
        }
    }
    vector<long double> x{1, -2, 3};
    vector<long double> b(3);
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            b[i] += a[i][j] * x[j];
        }
    }
    auto got = fact.solve(b);
    assert(got);
    for (int i = 0; i < 3; i++)
    {
        assert(fabsl((*got)[i] - x[i]) < 1E-10L);
    }

    LDLT empty({});
    auto emptySol = empty.solve({});
    assert(empty.valid && emptySol && emptySol->empty());
    LDLT singular({{1, 1}, {1, 1}});
    assert(!singular.valid && !singular.solve({1, 2}));
    LDLT zeroPivot({{0, 1}, {1, 0}});
    assert(!zeroPivot.valid);

    mt19937_64 rng(0);
    for (int tc = 0; tc < 200; tc++)
    {
        int n = 1 + rng() % 7;
        vector lower(n, vector<long double>(n));
        for (int i = 0; i < n; i++)
        {
            lower[i][i] = 1;
            for (int j = 0; j < i; j++)
            {
                lower[i][j] = (int)(rng() % 7) - 3;
            }
        }
        vector<long double> dd(n);
        for (auto &v : dd)
        {
            v = (int)(rng() % 9) - 4;
            if (v == 0)
            {
                v = 1;
            }
        }
        auto mat = makeMatrix(lower, dd);
        LDLT f(mat);
        assert(f.valid);
        vector<long double> rhs(n);
        for (auto &v : rhs)
        {
            v = (int)(rng() % 21) - 10;
        }
        auto sol = f.solve(rhs);
        assert(sol);
        for (int i = 0; i < n; i++)
        {
            long double check = 0;
            for (int j = 0; j < n; j++)
            {
                check += mat[i][j] * (*sol)[j];
            }
            assert(fabsl(check - rhs[i]) < 1E-8L);
        }
    }
    cout << "LDLT OK\n";
    return 0;
}
