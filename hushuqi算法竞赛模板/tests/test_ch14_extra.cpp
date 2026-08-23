#include "../include/full/14_1_015.hpp"
#include "../include/full/14_1_016.hpp"
#include "../include/full/14_1_017.hpp"
#include "../include/full/14_1_018.hpp"

bool validFactor(const vector<vector<int>> &a, int m)
{
    if ((int)a.size() != m + 1)
    {
        return false;
    }
    vector<vector<char>> used(m + 1, vector<char>(m));
    for (int i = 0; i <= m; i++)
    {
        for (int j = i + 1; j <= m; j++)
        {
            int c = a[i][j];
            if (c < 0 || c >= m || a[j][i] != c || used[i][c] || used[j][c])
            {
                return false;
            }
            used[i][c] = true;
            used[j][c] = true;
        }
    }
    for (auto &row : used)
    {
        if (count(row.begin(), row.end(), true) != m)
        {
            return false;
        }
    }
    return true;
}

bool near(long double x, long double y)
{
    return fabsl(x - y) <= 1E-15L * max({1.0L, fabsl(x), fabsl(y)});
}

bool validQueens(const vector<int> &p)
{
    int n = p.size();
    set<int> a;
    set<int> b;
    set<int> c;
    for (int i = 0; i < n; i++)
    {
        a.insert(p[i]);
        b.insert(i - p[i]);
        c.insert(i + p[i]);
    }
    return (int)a.size() == n && (int)b.size() == n && (int)c.size() == n;
}

signed main()
{
    for (int m : {1, 3, 5})
    {
        auto a = factor_ext::extendFactor({}, m);
        assert(a && validFactor(*a, m));
    }
    vector<vector<int>> bad{
        {-1, 0, 1, 2},
        {0, -1, 2, 1},
        {1, 2, -1, 0},
        {2, 1, 0, -1}
    };
    assert(!factor_ext::extendFactor(bad, 5));

    string sq = "15241578753238836750495351562536198787501905199875019052100";
    assert(bigSqrt(sq) == "123456789012345678901234567890");
    assert(bigSqrt("15241578753238836750495351562536198787501905199875019052099")
           == "123456789012345678901234567889");
    assert(bigSqrt("15241578753238836750495351562536198787501905199875019052101")
           == "123456789012345678901234567890");
    assert(bigSqrt("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000")
           == "100000000000000000000000000000000000000000000000000");

    for (long double k : {1E-200L, 1.0L, 1E200L})
    {
        auto r = quadRoots(k, -3 * k, 2 * k);
        assert(r.size() == 2 && near(r[0], 1) && near(r[1], 2));
    }

    for (int n = 1; n <= 500; n++)
    {
        auto p = nQueens(n);
        if (n == 2 || n == 3)
        {
            assert(p.empty());
        }
        else
        {
            assert((int)p.size() == n && validQueens(p));
        }
    }
    cout << "OK\n";
    return 0;
}
