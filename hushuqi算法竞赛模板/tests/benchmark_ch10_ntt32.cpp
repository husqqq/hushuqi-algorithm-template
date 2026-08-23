#include "../include/full/10_1_003.hpp"

using Clock = chrono::steady_clock;

unsigned long long sink = 0;

long long med(vector<long long> a)
{
    // a 是奇数次计时结果；返回中位数微秒数。
    sort(a.begin(), a.end());
    return a[a.size() / 2];
}

template <class F> long long run(F &&f)
{
    // f 是待计时操作；执行一次并返回微秒数。
    auto st = Clock::now();
    f();
    auto ed = Clock::now();
    return chrono::duration_cast<chrono::microseconds>(ed - st).count();
}

void bench(int n, int rep, mt19937_64 &rng)
{
    // n 是变换长度，rep 是每轮正逆变换对数，rng 生成固定输入；比较两种实现的中位耗时。
    vector<Z> a(n);
    vector<uint32_t> b(n);
    for (int i = 0; i < n; i++)
    {
        b[i] = rng() % mod;
        a[i].x = b[i];
    }
    auto oa = a;
    auto ob = b;
    NTT32 nt(n);

    dft(a);
    idft(a);
    nt.dft(b.data(), n);
    nt.idft(b.data(), n);
    if (a != oa || b != ob)
    {
        cerr << "warm-up result mismatch\n";
        abort();
    }

    vector<long long> tf, ts;
    for (int tc = 0; tc < 7; tc++)
    {
        auto fast = [&]
        {
            for (int i = 0; i < rep; i++)
            {
                dft(a);
                idft(a);
            }
        };
        auto small = [&]
        {
            for (int i = 0; i < rep; i++)
            {
                nt.dft(b.data(), n);
                nt.idft(b.data(), n);
            }
        };
        if (tc & 1)
        {
            ts.push_back(run(small));
            tf.push_back(run(fast));
        }
        else
        {
            tf.push_back(run(fast));
            ts.push_back(run(small));
        }
    }
    if (a != oa || b != ob)
    {
        cerr << "benchmark result mismatch\n";
        abort();
    }
    sink = sink * 1000003 + a[n / 2].val() + b[n / 2];
    cout << "n=" << n << " rep=" << rep
         << " fast_us=" << med(tf)
         << " ntt32_us=" << med(ts)
         << " ratio=" << fixed << setprecision(2)
         << (double)med(tf) / med(ts) << "x\n";
}

signed main()
{
    mt19937_64 rng(20260816);
    for (int n : {64, 256, 1024, 4096, 16384, 65536, 131072})
    {
        bench(n, max<int>(8, (1 << 21) / n), rng);
    }
    cout << "checksum=" << sink << '\n';
    return 0;
}
