
#include <bits/stdc++.h>
using namespace std;
#define int long long

template<class Acc = long long>
struct Assignment
{
    // cost 是最优匹配总权。
    Acc cost;
    // col[i] 是第 i 行匹配的列编号。
    vector<int> col;
};

template<class Acc = long long>
optional<Assignment<Acc>> assignment(const vector<vector<long long>> &a, bool mx = false)
{
    // a 是权值矩阵，mx 表示求最大权；返回最优完备匹配，不存在时返回空。
    static_assert(numeric_limits<Acc>::is_integer && numeric_limits<Acc>::is_signed &&
                  sizeof(Acc) >= sizeof(long long));
    int n = a.size(), m = n ? a[0].size() : 0;
    for (const auto &row : a)
    {
        if ((int)row.size() != m)
        {
            return nullopt;
        }
    }
    if (n > m)
    {
        return nullopt;
    }
    auto add = []<class T>(T x, T y) -> optional<T>
    {
        // x、y 是同类型整数；和超出该类型范围时返回空。
        if ((y > 0 && x > numeric_limits<T>::max() - y) ||
            (y < 0 && x < numeric_limits<T>::min() - y))
        {
            return nullopt;
        }
        return x + y;
    };
    auto sub = []<class T>(T x, T y) -> optional<T>
    {
        // x、y 是同类型整数；差超出该类型范围时返回空。
        if ((y > 0 && x < numeric_limits<T>::min() + y) ||
            (y < 0 && x > numeric_limits<T>::max() + y))
        {
            return nullopt;
        }
        return x - y;
    };
    vector<Acc> u(n + 1), v(m + 1);
    vector<int> p(m + 1), way(m + 1);
    for (int i = 1; i <= n; i++)
    {
        p[0] = i;
        int j0 = 0;
        vector<optional<Acc>> d(m + 1);
        vector<bool> used(m + 1);
        do
        {
            used[j0] = true;
            int i0 = p[j0], j1 = 0;
            optional<Acc> delta;
            for (int j = 1; j <= m; j++)
            {
                if (used[j])
                {
                    continue;
                }
                Acc cur = (Acc)a[i0 - 1][j - 1];
                if (mx)
                {
                    auto z = sub((Acc)0, cur);
                    if (!z)
                    {
                        return nullopt;
                    }
                    cur = *z;
                }
                auto z = sub(cur, u[i0]);
                if (!z)
                {
                    return nullopt;
                }
                z = sub(*z, v[j]);
                if (!z)
                {
                    return nullopt;
                }
                cur = *z;
                if (!d[j] || cur < *d[j])
                {
                    d[j] = cur;
                    way[j] = j0;
                }
                if (!delta || *d[j] < *delta)
                {
                    delta = d[j];
                    j1 = j;
                }
            }
            assert(delta); // 调试检查，可删：n<=m 时总有未访问列。
            for (int j = 0; j <= m; j++)
            {
                if (used[j])
                {
                    auto x = add(u[p[j]], *delta);
                    if (!x)
                    {
                        return nullopt;
                    }
                    u[p[j]] = *x;
                    if (j)
                    {
                        x = sub(v[j], *delta);
                        if (!x)
                        {
                            return nullopt;
                        }
                        v[j] = *x;
                    }
                }
            }
            for (int j = 1; j <= m; j++)
            {
                if (!used[j] && d[j])
                {
                    auto x = sub(*d[j], *delta);
                    if (!x)
                    {
                        return nullopt;
                    }
                    d[j] = *x;
                }
            }
            j0 = j1;
        } while (p[j0]);
        do
        {
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0);
    }
    vector<int> col(n);
    for (int j = 1; j <= m; j++)
    {
        if (p[j])
        {
            col[p[j] - 1] = j - 1;
        }
    }
    Acc cost = 0;
    for (int i = 0; i < n; i++)
    {
        auto x = add(cost, (Acc)a[i][col[i]]);
        if (!x)
        {
            return nullopt;
        }
        cost = *x;
    }
    return Assignment<Acc>{cost, col};
}

signed main()
{
    int n; cin >> n;
    vector<vector<long long>> a(n, vector<long long>(n));
    for (auto &row : a)
    {
        for (long long &x : row) cin >> x;
    }
    auto ans = assignment(a);
    assert(ans.has_value());
    cout << ans->cost << '\n';
    for (int i = 0; i < n; i++) cout << ans->col[i] << " \n"[i + 1 == n];
}
