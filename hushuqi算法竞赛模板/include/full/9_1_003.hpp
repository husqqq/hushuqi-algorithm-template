#pragma once
#include <bits/stdc++.h>
using namespace std;

template <class T> struct NaivePolyMul
{
    vector<T> operator()(const vector<T> &a, const vector<T> &b) const
    {
        // a、b 是系数从低次到高次排列的多项式；返回普通卷积。
        if (a.empty() || b.empty())
        {
            return {};
        }
        vector<T> c(a.size() + b.size() - 1);
        for (int i = 0; i < (int)a.size(); i++)
        {
            for (int j = 0; j < (int)b.size(); j++)
            {
                c[i + j] += a[i] * b[j];
            }
        }
        return c;
    }
};

template <class T> struct FieldBasis
{
    // n 是向量维数，rk 是当前秩。
    int n, rk = 0;
    // basis 保存插入的原向量；row 是按首个非零位置存放的阶梯基，way 给出 row 关于 basis 的系数。
    vector<vector<T>> basis, row, way;

    FieldBasis(int n) : n(n), row(n, vector<T>(n)), way(n, vector<T>(n))
    {
        // 建立 n 维空线性基。
    }

    pair<bool, vector<T>> solveOrAdd(vector<T> x)
    {
        // x 是待查询向量；可表示时返回 true 及其关于 basis 的系数，否则插入 x 并返回 false。
        vector<T> original = x;
        vector<T> coef(n);
        for (int i = 0; i < n; i++)
        {
            if (row[i][i] == T(1))
            {
                coef[i] = x[i];
                for (int j = i; j < n; j++)
                {
                    x[j] -= coef[i] * row[i][j];
                }
            }
        }
        int p = 0;
        while (p < n && x[p] == T(0))
        {
            p++;
        }
        if (p == n)
        {
            vector<T> ans(rk);
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < rk; j++)
                {
                    ans[j] += coef[i] * way[i][j];
                }
            }
            return {true, ans};
        }
        T z = T(1) / x[p];
        for (int j = p; j < n; j++)
        {
            x[j] *= z;
        }
        for (auto &v : coef)
        {
            v *= z;
        }
        basis.push_back(move(original));
        row[p] = move(x);
        way[p][rk] = z;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < rk; j++)
            {
                way[p][j] -= coef[i] * way[i][j];
            }
        }
        rk++;
        return {false, {}};
    }

    void rollback(int r)
    {
        // r 是此前的秩；撤销最近插入的向量，直到秩恢复为 r。
        while (rk > r)
        {
            rk--;
            basis.pop_back();
            for (int i = 0; i < n; i++)
            {
                if (way[i][rk] != T(0))
                {
                    fill(row[i].begin(), row[i].end(), T(0));
                    fill(way[i].begin(), way[i].end(), T(0));
                }
            }
        }
    }
};

template <class T, class Conv = NaivePolyMul<T>> struct FrobPower
{
    using Poly = vector<T>;
    using Matrix = vector<vector<T>>;

    // n 是矩阵阶数；a 是原矩阵，p 的列是 Krylov 基，ip 是 p 的逆矩阵。
    int n;
    Matrix a, p, ip;
    // start 保存各循环子空间的起始向量；relation 保存对应伴随块的递推系数。
    vector<Poly> start, relation;
    // conv 计算多项式卷积，rng 产生试探向量，trials 记录完整重建次数。
    Conv conv;
    mt19937_64 rng;
    int trials = 0;

    FrobPower(Matrix a, Conv conv = Conv(), unsigned long long seed = 0)
        : n(a.size()), a(move(a)), conv(move(conv)), rng(seed)
    {
        // a 是域上的方阵，conv 是卷积器，seed 是固定随机种子；构造时完成 Frobenius 分解。
        // 调试检查，可删。
        for (const auto &r : this->a)
        {
            assert((int)r.size() == n);
        }
        do
        {
            trials++;
        } while (!trial());
    }

    Matrix pow(unsigned long long b)
    {
        // b 是非负指数；返回原矩阵的 b 次幂。
        Matrix x(n, vector<T>(n));
        int s = 0;
        for (const auto &g : relation)
        {
            int d = g.size();
            Poly f = polyPowMod(g, b);
            for (int j = 0; j < d; j++)
            {
                for (int i = 0; i < (int)f.size(); i++)
                {
                    x[s + i][s + j] = f[i];
                }
                if (j + 1 < d)
                {
                    f.insert(f.begin(), T(0));
                    divmod(f, g);
                }
            }
            s += d;
        }
        return matrixMul(matrixMul(p, x), ip);
    }

private:
    static void trim(Poly &f)
    {
        // f 的系数按低次到高次排列；原地删除高次零项。
        while (!f.empty() && f.back() == T(0))
        {
            f.pop_back();
        }
    }

    Poly divmod(Poly &f, const Poly &g)
    {
        // f 是被除式，g 表示 x^d-sum(g[i]x^i)；原地留下余数并返回商。
        trim(f);
        int d = g.size();
        Poly q;
        for (int i = (int)f.size() - 1; i >= d; i--)
        {
            q.push_back(f[i]);
            for (int j = 0; j < d; j++)
            {
                f[i - d + j] += f[i] * g[j];
            }
            f[i] = T(0);
        }
        trim(f);
        reverse(q.begin(), q.end());
        return q;
    }

    Poly polyMulMod(const Poly &x, const Poly &y, const Poly &g)
    {
        // x、y 是被乘多项式，g 表示模多项式；返回乘积的余式。
        Poly z = conv(x, y);
        divmod(z, g);
        return z;
    }

    Poly polyPowMod(const Poly &g, unsigned long long b)
    {
        // g 表示模多项式，b 是非负指数；返回 x^b 的余式。
        Poly ans{T(1)};
        Poly x{T(0), T(1)};
        divmod(x, g);
        while (b)
        {
            if (b & 1)
            {
                ans = polyMulMod(ans, x, g);
            }
            b >>= 1;
            if (b)
            {
                x = polyMulMod(x, x, g);
            }
        }
        return ans;
    }

    Matrix matrixMul(const Matrix &x, const Matrix &y)
    {
        // x、y 是 n 阶方阵；返回普通矩阵乘积。
        Matrix z(n, vector<T>(n));
        for (int i = 0; i < n; i++)
        {
            for (int k = 0; k < n; k++)
            {
                for (int j = 0; j < n; j++)
                {
                    z[i][j] += x[i][k] * y[k][j];
                }
            }
        }
        return z;
    }

    optional<Matrix> matrixInv(Matrix x)
    {
        // x 是 n 阶方阵；可逆时返回逆矩阵，否则返回空。
        Matrix y(n, vector<T>(n));
        for (int i = 0; i < n; i++)
        {
            y[i][i] = T(1);
        }
        for (int col = 0; col < n; col++)
        {
            int row = col;
            while (row < n && x[row][col] == T(0))
            {
                row++;
            }
            if (row == n)
            {
                return nullopt;
            }
            swap(x[row], x[col]);
            swap(y[row], y[col]);
            T z = T(1) / x[col][col];
            for (int j = 0; j < n; j++)
            {
                x[col][j] *= z;
                y[col][j] *= z;
            }
            for (int i = 0; i < n; i++)
            {
                if (i == col)
                {
                    continue;
                }
                T q = x[i][col];
                for (int j = 0; j < n; j++)
                {
                    x[i][j] -= q * x[col][j];
                    y[i][j] -= q * y[col][j];
                }
            }
        }
        return y;
    }

    vector<T> apply(const vector<T> &v)
    {
        // v 是 n 维列向量；返回 a*v。
        vector<T> w(n);
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                w[i] += a[i][j] * v[j];
            }
        }
        return w;
    }

    vector<T> randomVector()
    {
        // 返回由 rng 生成的 n 维试探向量。
        vector<T> v(n);
        for (auto &x : v)
        {
            x = T((long long)(rng() & 0x7FFFFFFFFFFFFFFFULL));
        }
        return v;
    }

    bool trial()
    {
        // 从空基底尝试构造一次 Frobenius 分解；成功时写入 p、ip 与 relation。
        start.clear();
        relation.clear();
        FieldBasis<T> s(n);
        while (s.rk < n)
        {
            int oldRank = s.rk;
            vector<T> v = randomVector();
            start.push_back(v);
            while (true)
            {
                auto [solved, coef] = s.solveOrAdd(v);
                if (!solved)
                {
                    v = apply(v);
                    continue;
                }
                Poly f(coef.begin() + oldRank, coef.end());
                if (f.empty())
                {
                    return false;
                }
                relation.push_back(f);
                if (start.size() == 1)
                {
                    break;
                }
                v = start.back();
                int pos = 0;
                for (int k = 0; k + 1 < (int)start.size(); k++)
                {
                    int d = relation[k].size();
                    Poly rem(coef.begin() + pos, coef.begin() + pos + d);
                    Poly q = divmod(rem, f);
                    if (!rem.empty())
                    {
                        return false;
                    }
                    for (int i = 0; i < (int)q.size(); i++)
                    {
                        for (int j = 0; j < n; j++)
                        {
                            v[j] -= q[i] * s.basis[pos + i][j];
                        }
                    }
                    pos += d;
                }
                s.rollback(oldRank);
                start.back() = v;
                for (int i = 0; i < (int)f.size(); i++)
                {
                    if (s.solveOrAdd(v).first)
                    {
                        return false;
                    }
                    if (i + 1 < (int)f.size())
                    {
                        v = apply(v);
                    }
                }
                break;
            }
        }
        p.assign(n, vector<T>(n));
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                p[i][j] = s.basis[j][i];
            }
        }
        auto inv = matrixInv(p);
        if (!inv)
        {
            return false;
        }
        ip = move(*inv);
        return true;
    }
};
