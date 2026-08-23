#include "../include/full/10_2_005.hpp"

using Clock = chrono::steady_clock;

unsigned long long benchmarkSink = 0;

Poly randomSeries(int n, mt19937_64 &rng)
{
    Poly a(n);
    for (int i = 1; i < n; i++)
    {
        a[i] = rng() % mod;
    }
    return a;
}

template <class F>
long long timedRun(F &&f, const Poly &want)
{
    auto start = Clock::now();
    Poly got = f();
    auto finish = Clock::now();
    if (got != want)
    {
        cerr << "benchmark result mismatch\n";
        abort();
    }
    unsigned long long checksum = got.size();
    for (int i = 0; i < (int)got.size(); i += max<int>(1, got.size() / 16))
    {
        checksum = checksum * 1000003 + got[i].val();
    }
    benchmarkSink = benchmarkSink * 1000000007ULL + checksum;
    return chrono::duration_cast<chrono::microseconds>(finish - start).count();
}

long long median(vector<long long> a)
{
    sort(a.begin(), a.end());
    return a[a.size() / 2];
}

void printTimes(const vector<long long> &a)
{
    for (int i = 0; i < (int)a.size(); i++)
    {
        if (i)
        {
            cout << ',';
        }
        cout << a[i];
    }
}

void benchmark(int n, mt19937_64 &rng)
{
    Poly a = randomSeries(n, rng);
    Poly want = fpsExpRelax(a, n);
    if (fpsExpNewton(a, n) != want)
    {
        cerr << "warm-up result mismatch at n=" << n << '\n';
        abort();
    }

    vector<long long> relaxedTimes, newtonTimes;
    for (int round = 0; round < 7; round++)
    {
        auto relaxed = [&]
        {
            return fpsExpRelax(a, n);
        };
        auto newton = [&]
        {
            return fpsExpNewton(a, n);
        };
        if (round & 1)
        {
            newtonTimes.push_back(timedRun(newton, want));
            relaxedTimes.push_back(timedRun(relaxed, want));
        }
        else
        {
            relaxedTimes.push_back(timedRun(relaxed, want));
            newtonTimes.push_back(timedRun(newton, want));
        }
    }

    long long relaxedMedian = median(relaxedTimes);
    long long newtonMedian = median(newtonTimes);
    cout << "n=" << n << " relaxed_us=";
    printTimes(relaxedTimes);
    cout << " newton_us=";
    printTimes(newtonTimes);
    cout << " relaxed_median_us=" << relaxedMedian
         << " newton_median_us=" << newtonMedian
         << " speedup=" << fixed << setprecision(2)
         << (double)newtonMedian / relaxedMedian << "x\n";
}

signed main()
{
    mt19937_64 rng(20260816);
    Poly warm = randomSeries(4096, rng);
    benchmarkSink = fpsExpRelax(warm, warm.size()).back().val();
    benchmarkSink = benchmarkSink * 1000000007ULL
                    + fpsExpNewton(warm, warm.size()).back().val();

    benchmark(100000, rng);
    benchmark(131072, rng);
    cout << "checksum=" << benchmarkSink << '\n';
    return 0;
}
