#include "../include/full/10_2_020.hpp"

signed main()
{
    constexpr int n = 50000;
    mt19937_64 rng(0);
    Poly f(n), g(n + 1);
    for (auto &x : f)
    {
        x = rng() % mod;
    }
    for (auto &x : g)
    {
        x = rng() % mod;
    }
    g.back() = 1;
    auto st = chrono::steady_clock::now();
    Poly d = polyGcd(f, g);
    auto gm = chrono::steady_clock::now();
    auto inv = polyInvMod(f, g);
    auto mid = chrono::steady_clock::now();
    if (d != Poly{1} || !inv || polyDivmod(f * *inv, g).second != Poly{1})
    {
        cerr << "verification failed\n";
        return 1;
    }
    auto ed = chrono::steady_clock::now();
    cout << "gcd_ms="
         << chrono::duration<double, milli>(gm - st).count()
         << " inverse_ms="
         << chrono::duration<double, milli>(mid - gm).count()
         << " verify_ms="
         << chrono::duration<double, milli>(ed - mid).count() << '\n';
    return 0;
}
