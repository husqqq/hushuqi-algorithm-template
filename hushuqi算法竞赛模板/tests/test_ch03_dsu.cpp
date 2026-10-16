#ifdef TEMPLATE_PRINT
#include "../include/print/3_1_001.hpp"
#else
#include "../include/full/3_1_001.hpp"
#endif

void check(bool ok)
{
    // ok 是预期条件；检查与被测调用均不受 NDEBUG 影响。
    if (!ok)
    {
        abort();
    }
}

void randomTest(int seed)
{
    // seed 固定生成操作；用直接维护的集合编号核对每个公开接口。
    mt19937_64 rng(seed);
    int n = 1 + rng() % 80;
    DSU d(n);
    vector<int> id(n);
    iota(id.begin(), id.end(), 0);
    for (int i = 0; i < 2000; i++)
    {
        int x = rng() % n, y = rng() % n;
        int op = rng() % 5;
        if (op <= 1)
        {
            bool want = id[x] != id[y];
            check(d.merge(x, y) == want);
            int from = id[y], to = id[x];
            for (int &v : id)
            {
                if (v == from) v = to;
            }
        }
        else if (op == 2)
        {
            check(d.same(x, y) == (id[x] == id[y]));
        }
        else if (op == 3)
        {
            check(d.size(x) == count(id.begin(), id.end(), id[x]));
            int root = d.find(x);
            check(id[root] == id[x] && d.find(root) == root);
        }
        else
        {
            n = 1 + rng() % 80;
            d.init(n);
            id.resize(n);
            iota(id.begin(), id.end(), 0);
        }
    }
    for (int x = 0; x < n; x++)
    {
        check(d.size(x) == count(id.begin(), id.end(), id[x]));
        for (int y = 0; y < n; y++)
        {
            check(d.same(x, y) == (id[x] == id[y]));
        }
    }
}

signed main()
{
    DSU d(5);
    check(d.merge(0, 1));
    check(d.merge(1, 2));
    check(!d.merge(0, 2));
    check(d.same(0, 2) && !d.same(0, 3));
    check(d.size(1) == 3 && d.size(4) == 1);
    d.init(2);
    check(!d.same(0, 1));
    d.init(0);
    d.init(1);
    check(!d.merge(0, 0) && d.find(0) == 0 && d.size(0) == 1);
    d.init(256);
    for (int len = 1; len < 256; len *= 2)
    {
        for (int l = 0; l < 256; l += 2 * len)
        {
            check(d.merge(l, l + len));
        }
    }
    for (int x = 0; x < 256; x++)
    {
        check(d.size(x) == 256 && d.same(0, x));
    }
    for (int seed = 0; seed < 64; seed++)
    {
        randomTest(seed);
        cout << "AC " << seed << '\n';
    }
}
