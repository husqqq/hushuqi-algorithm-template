#include "../include/full/3_1_008.hpp"

struct MatrixGroup
{
    using T = array<array<int, 2>, 2>; // T[i][j] 是模 998244353 的第 i 行第 j 列元素。
    static constexpr int mod = 998244353;

    static T id()
    {
        // 返回二维行列式为一矩阵群的单位矩阵。
        return {{{1, 0}, {0, 1}}};
    }

    static T op(const T &a, const T &b)
    {
        // a、b 是两个矩阵；返回按 a*b 顺序相乘的矩阵。
        T c{};
        for (int i = 0; i < 2; i++)
        {
            for (int k = 0; k < 2; k++)
            {
                for (int j = 0; j < 2; j++)
                {
                    c[i][j] = (c[i][j] + a[i][k] * b[k][j]) % mod;
                }
            }
        }
        return c;
    }

    static T inv(const T &a)
    {
        // a 是行列式为一的二维矩阵；返回它的逆矩阵。
        return {{{a[1][1], (mod - a[0][1]) % mod},
                 {(mod - a[1][0]) % mod, a[0][0]}}};
    }
};

struct PermGroup
{
    using T = array<int, 3>; // T[i] 是置换把 i 映到的像。

    static T id()
    {
        // 返回三元置换群的恒等置换。
        return {0, 1, 2};
    }

    static T op(const T &a, const T &b)
    {
        // a、b 是置换；返回先作用 b 再作用 a 的复合 a*b。
        T c{};
        for (int i = 0; i < 3; i++)
        {
            c[i] = a[b[i]];
        }
        return c;
    }

    static T inv(const T &a)
    {
        // a 是置换；返回其逆置换。
        T b{};
        for (int i = 0; i < 3; i++)
        {
            b[a[i]] = i;
        }
        return b;
    }
};

signed main()
{
    using M = MatrixGroup::T;
    M a{{{1, 1}, {0, 1}}};
    M b{{{1, 0}, {1, 1}}};
    M ab = MatrixGroup::op(a, b);
    assert(ab != MatrixGroup::op(b, a));

    PotentialDSU<MatrixGroup> mat(4);
    assert(mat.merge(0, 1, a));
    assert(mat.merge(1, 2, b));
    assert(mat.diff(0, 2) == ab);
    assert(mat.diff(2, 0) == MatrixGroup::inv(ab));
    assert(mat.merge(0, 2, ab));
    assert(!mat.merge(0, 2, MatrixGroup::op(b, a)));
    assert(mat.size(0) == 3 && mat.size(3) == 1);

    using P = PermGroup::T;
    P c{1, 2, 0};
    P d{1, 0, 2};
    P cd = PermGroup::op(c, d);
    assert(cd != PermGroup::op(d, c));

    PotentialDSU<PermGroup> perm(5);
    assert(perm.merge(0, 1, c));
    assert(perm.merge(1, 2, d));
    assert(perm.merge(3, 0, c));
    assert(perm.diff(3, 2) == PermGroup::op(c, cd));
    assert(perm.merge(3, 2, PermGroup::op(c, cd)));
    assert(!perm.merge(3, 2, PermGroup::op(cd, c)));
    assert(perm.merge(4, 4, PermGroup::id()));
    assert(!perm.merge(4, 4, c));

    // all[i] 是 S_3 的第 i 个置换，用固定序列覆盖不同合并方向与路径压缩。
    array<P, 6> all{{{0, 1, 2}, {0, 2, 1}, {1, 0, 2},
                     {1, 2, 0}, {2, 0, 1}, {2, 1, 0}}};
    mt19937_64 rng(0);
    vector<P> value(17);
    for (auto &x : value)
    {
        x = all[rng() % all.size()];
    }
    PotentialDSU<PermGroup> random((int)value.size());
    for (int q = 0; q < 2'000; q++)
    {
        int x = rng() % value.size();
        int y = rng() % value.size();
        P rel = PermGroup::op(PermGroup::inv(value[x]), value[y]);
        assert(random.merge(x, y, rel));
        for (int z = 0; z < 3; z++)
        {
            int u = rng() % value.size();
            int v = rng() % value.size();
            if (random.same(u, v))
            {
                assert(random.diff(u, v) == PermGroup::op(PermGroup::inv(value[u]), value[v]));
            }
        }
    }
    cout << "OK\n";
}
