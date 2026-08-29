#include "../include/full/1_1_008.hpp"
#include "../include/full/3_3_020.hpp"

Z rangeSum(const vector<Z> &a, int l, int r)
{
    // a 是一个朴素版本，l、r 是半开区间；返回 a[l,r) 的模意义元素和。
    Z ans = 0;
    for (int i = l; i < r; i++)
    {
        ans += a[i];
    }
    return ans;
}

void applyAffine(vector<Z> &a, int l, int r, Z mul, Z add)
{
    // a 是要原地修改的朴素版本，[l,r) 是修改区间；对其中每项执行 x->mul*x+add。
    for (int i = l; i < r; i++)
    {
        a[i] = a[i] * mul + add;
    }
}

signed main()
{
    {
        vector<Z> a{1, 2, 3, 4, 5, 6, 7, 8};
        PerAffSeg<Z> seg(a);
        int r0 = seg.root();
        int r1 = seg.affine(r0, 0, 8, Z(3), Z(1));
        int r2 = seg.affine(r0, 2, 6, Z(5), Z(7));
        int r3 = seg.splice(r1, r2, 1, 7);

        assert(seg.sum(r0, 0, 8) == Z(36));
        assert(seg.sum(r1, 0, 8) == Z(116));
        assert(seg.sum(r2, 0, 8) == Z(136));
        assert(seg.sum(r3, 0, 8) == Z(156));
        assert(seg.sum(r3, 0, 2) == Z(6));
        assert(seg.sum(r3, 2, 6) == Z(118));
        assert(seg.sum(r3, 6, 8) == Z(32));
        assert(seg.affine(r3, 3, 3, Z(9), Z(2)) == r3);
        assert(seg.splice(r3, r2, 4, 4) == r3);
        assert(seg.splice(r3, r3, 1, 7) == r3);
    }

    mt19937_64 rng(20260819);
    unsigned long long checksum = 0;
    for (int it = 0; it < 180; it++)
    {
        int n = 1 + rng() % 20;
        vector<Z> initial(n);
        for (auto &x : initial)
        {
            x = Z((long long)(rng() % mod));
        }
        PerAffSeg<Z> seg(initial);
        vector<int> roots{seg.root()};
        vector<vector<Z>> versions{initial};
        for (int step = 0; step < 180; step++)
        {
            int target = rng() % roots.size();
            int l = rng() % (n + 1);
            int r = rng() % (n + 1);
            if (l > r)
            {
                swap(l, r);
            }
            int kind = rng() % 3;
            if (kind == 0)
            {
                Z mul((long long)(rng() % mod));
                Z add((long long)(rng() % mod));
                vector<Z> cur = versions[target];
                applyAffine(cur, l, r, mul, add);
                roots.push_back(seg.affine(roots[target], l, r, mul, add));
                versions.push_back(move(cur));
            }
            else if (kind == 1)
            {
                int source = rng() % roots.size();
                vector<Z> cur = versions[target];
                for (int i = l; i < r; i++)
                {
                    cur[i] = versions[source][i];
                }
                roots.push_back(seg.splice(roots[target], roots[source], l, r));
                versions.push_back(move(cur));
            }
            else
            {
                assert(seg.sum(roots[target], l, r) == rangeSum(versions[target], l, r));
            }

            int probe = rng() % roots.size();
            int pl = rng() % (n + 1);
            int pr = rng() % (n + 1);
            if (pl > pr)
            {
                swap(pl, pr);
            }
            Z got = seg.sum(roots[probe], pl, pr);
            assert(got == rangeSum(versions[probe], pl, pr));
            checksum = checksum * 1000003ULL + (unsigned long long)got.val();
        }
    }
    cout << checksum << '\n';
    return 0;
}
