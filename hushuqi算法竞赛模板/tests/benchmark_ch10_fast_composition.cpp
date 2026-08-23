#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#include <psapi.h>
#else
#include <sys/resource.h>
#endif

#include "../include/full/10_2_012.hpp"
#include "../include/full/10_3_005.hpp"

double peakMemoryMb()
{
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS info{};
    GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));
    return info.PeakWorkingSetSize / 1048576.0;
#else
    rusage usage{};
    getrusage(RUSAGE_SELF, &usage);
#ifdef __APPLE__
    return usage.ru_maxrss / 1048576.0;
#else
    return usage.ru_maxrss / 1024.0;
#endif
#endif
}

template<class F> double measure(F &&run)
{
    auto start = chrono::steady_clock::now();
    run();
    return chrono::duration<double, milli>(chrono::steady_clock::now() - start).count();
}

signed main()
{
    constexpr int n = 131072;
    mt19937_64 rng(20260816);
    Poly f(n), g(n);
    vector<Z> w(n);
    for (int i = 0; i < n; i++)
    {
        f[i] = rng() % mod;
        g[i] = rng() % mod;
        w[i] = rng() % mod;
    }
    g[0] = 0;

    Poly composed;
    double composeZeroMs = measure([&] { composed = composePoly(f, g, n).cut(n); });
    bool composeOk = composed.cut(32) == composePoly(f.cut(32), g.cut(32), 32).cut(32);
    g[0] = 123456789;
    Poly shiftedComposition;
    double composeShiftedMs = measure([&] {
        shiftedComposition = composePoly(f, g, n).cut(n);
    });
    Z value = 0, derivative = 0;
    for (int i = n - 1; i >= 0; i--)
    {
        derivative = derivative * g[0] + value;
        value = value * g[0] + f[i];
    }
    composeOk = composeOk && shiftedComposition[0] == value &&
                shiftedComposition[1] == derivative * g[1];
    vector<Z> projected;
    double projectionMs = measure([&] { projected = powerProj(f, w, n); });
    Z firstPower = 0;
    for (int i = 0; i < n; i++)
    {
        firstPower += w[i] * f[i];
    }
    bool projectionOk = projected[0] == w[0] && projected[1] == firstPower;

    f[0] = 0;
    f[1] = f[1] == Z(0) ? Z(1) : f[1];
    Poly inverse;
    double inverseMs = measure([&] { inverse = compInverse(f, n); });
    Poly checked;
    double verifyMs = measure([&] { checked = composePoly(f, inverse, n).cut(n); });
    bool inverseOk = checked[1] == Z(1);
    for (int i = 0; i < n; i++)
    {
        if (i != 1 && checked[i] != Z(0))
        {
            inverseOk = false;
        }
    }
    if (!composeOk || !projectionOk || !inverseOk)
    {
        cerr << "verification failed\n";
        return 1;
    }

    Z checksum = composed[n - 1] + shiftedComposition[n - 1] +
                 projected[n - 1] + inverse[n - 1];
    cout << fixed << setprecision(2)
         << "n=" << n
         << " compose_zero_ms=" << composeZeroMs
         << " compose_shifted_ms=" << composeShiftedMs
         << " projection_ms=" << projectionMs
         << " inverse_ms=" << inverseMs
         << " verify_ms=" << verifyMs
         << " peak_mb=" << peakMemoryMb()
         << " checksum=" << checksum.val() << '\n';
    return 0;
}
