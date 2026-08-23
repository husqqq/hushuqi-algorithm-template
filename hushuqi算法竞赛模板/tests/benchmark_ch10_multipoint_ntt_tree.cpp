#include "../include/full/10_2_007.hpp"

vector<Z> evalNttTree(const Poly &f, const vector<Z> &x)
{
    // f 是待求值多项式，x 是求值点；转发到当前频域乘积树实现。
    return multiEval(f, x);
}

vector<Z> evalSlow(const Poly &f, const vector<Z> &x)
{
    // f 是待求值多项式，x 是求值点；用 Horner 返回与 x 同序的点值。
    vector<Z> ans(x.size());
    for (int i = 0; i < (int)x.size(); i++)
    {
        for (auto it = f.rbegin(); it != f.rend(); ++it)
        {
            ans[i] = ans[i] * x[i] + *it;
        }
    }
    return ans;
}

double median(vector<double> a)
{
    // a 是非空耗时序列；返回中位数。
    sort(a.begin(), a.end());
    return a[a.size() / 2];
}

signed main()
{
    mt19937_64 rng(20260824);
    for (int nf : {0, 1, 2, 3, 7, 31, 65, 129, 257, 521, 5000})
    {
        for (int nx : {0, 1, 2, 5, 16, 67, 130, 259})
        {
            Poly f(nf);
            vector<Z> x(nx);
            for (auto &v : f)
            {
                v = rng() % mod;
            }
            for (auto &v : x)
            {
                v = rng() % mod;
            }
            auto want = evalSlow(f, x);
            assert(PolyTree(x).eval(f) == want);
            assert(evalNttTree(f, x) == want);
        }
    }
    for (int nf : {1, 5, 130, 513})
    {
        for (int nx : {2, 3, 17, 129, 300})
        {
            Poly f(nf);
            vector<Z> x(nx, Z(7));
            for (auto &v : f)
            {
                v = rng() % mod;
            }
            auto want = evalSlow(f, x);
            assert(PolyTree(x).eval(f) == want);
            assert(evalNttTree(f, x) == want);
        }
    }

    for (int n : {4096, 16384, 65536, 131072})
    {
        Poly f(n);
        vector<Z> x(n);
        for (auto &v : f)
        {
            v = rng() % mod;
        }
        for (auto &v : x)
        {
            v = rng() % mod;
        }
        vector<double> oldMs, newMs;
        vector<Z> oldAns, newAns;
        for (int rep = 0; rep < 5; rep++)
        {
            auto st = chrono::steady_clock::now();
            if (rep & 1)
            {
                newAns = evalNttTree(f, x);
            }
            else
            {
                oldAns = PolyTree(x).eval(f);
            }
            auto mid = chrono::steady_clock::now();
            if (rep & 1)
            {
                oldAns = PolyTree(x).eval(f);
            }
            else
            {
                newAns = evalNttTree(f, x);
            }
            auto ed = chrono::steady_clock::now();
            double a = chrono::duration<double, milli>(mid - st).count();
            double b = chrono::duration<double, milli>(ed - mid).count();
            if (rep & 1)
            {
                newMs.push_back(a);
                oldMs.push_back(b);
            }
            else
            {
                oldMs.push_back(a);
                newMs.push_back(b);
            }
            assert(oldAns == newAns);
        }
        cout << n << ' ' << fixed << setprecision(3)
             << median(oldMs) << ' ' << median(newMs) << '\n';
    }
    return 0;
}
