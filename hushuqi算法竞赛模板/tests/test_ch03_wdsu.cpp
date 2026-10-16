#include "../include/full/3_1_003.hpp"

void check(bool ok)
{
    // ok 是待核对的结论；失败抛出异常，关闭 assert 时仍执行所有操作和检查。
    if (!ok) throw runtime_error("势能并查集回归失败");
}

signed main()
{
    // 两棵树合并后立即查询深层节点，不先调用 same/find 帮查询压缩路径。
    WDSU<int> depth(4);
    check(depth.merge(0, 1, 5));
    check(depth.merge(2, 3, 7));
    check(depth.merge(0, 2, 11));
    check(depth.diff(0, 3) == 18);
    check(depth.diff(3, 0) == -18);
    XorDSU bits(4);
    check(bits.merge(0, 1, 5));
    check(bits.merge(2, 3, 7));
    check(bits.merge(0, 2, 11));
    check(bits.diff(0, 3) == 12);

    WDSU<int> d(4);
    check(d.merge(0, 1, 5));
    check(d.merge(1, 2, -2));
    check(d.diff(0, 2) == 3);
    check(d.diff(2, 0) == -3);
    check(d.merge(0, 2, 3));
    check(!d.merge(0, 2, 4));
    check(d.merge(3, 2, 7));
    check(d.diff(3, 0) == 4);

    XorDSU x(5);
    check(x.merge(0, 1, 5));
    check(x.merge(1, 2, 3));
    check(x.diff(0, 2) == 6);
    check(x.merge(0, 2, 6));
    check(!x.merge(0, 2, 7));
    check(x.merge(3, 2, 4));
    check(x.diff(3, 0) == 2);
    check(x.merge(4, 3, 1));
    check((x.value(4) ^ x.value(3)) == 1);
    for (int seed = 0; seed < 64; seed++)
    {
        mt19937_64 rng(seed);
        int n = 1 + rng() % 40;
        vector<int> value(n), mask(n), label(n);
        iota(label.begin(), label.end(), 0);
        for (int i = 0; i < n; i++)
        {
            value[i] = (int)(rng() % 2'000'001) - 1'000'000;
            mask[i] = rng() % (1 << 20);
        }
        WDSU<int> sum(n);
        XorDSU xorDsu(n);
        for (int q = 0; q < 1500; q++)
        {
            int x = rng() % n, y = rng() % n;
            int delta = value[y] - value[x], bits = mask[x] ^ mask[y];
            bool joined = label[x] == label[y];
            bool bad = joined && rng() % 3 == 0;
            bool ok = sum.merge(x, y, delta + bad) == !bad;
            ok &= xorDsu.merge(x, y, bits ^ bad) == !bad;
            if (!joined)
            {
                int from = label[y], to = label[x];
                for (auto &z : label)
                {
                    if (z == from) z = to;
                }
            }
            int u = rng() % n, v = rng() % n;
            if (label[u] == label[v])
            {
                ok &= sum.diff(u, v) == value[v] - value[u];
                ok &= xorDsu.diff(u, v) == (mask[u] ^ mask[v]);
            }
            ok &= sum.same(u, v) == (label[u] == label[v]);
            ok &= xorDsu.same(u, v) == (label[u] == label[v]);
            if (!ok)
            {
                cout << "WA " << seed << '\n';
                return 1;
            }
        }
        cout << "AC " << seed << '\n';
    }
}
