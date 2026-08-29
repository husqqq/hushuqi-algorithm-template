#include <bits/stdc++.h>
#include <immintrin.h>
using namespace std;

#if defined(__GNUC__) && defined(__x86_64__)
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx512f,bmi,bmi2,popcnt")
#endif

using u64 = unsigned long long;

// Each entry is T(e_i), so the transform maps a 64-bit vector by XORing
// the entries selected by its set bits.
using Linear64 = array<u64, 64>;

static u64 xorshift64(u64 x)
{
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return x;
}

static u64 applyLinear(const Linear64 &t, u64 x)
{
    u64 y = 0;
    while (x)
    {
        unsigned b = static_cast<unsigned>(__builtin_ctzll(x));
        y ^= t[b];
        x &= x - 1;
    }
    return y;
}

static Linear64 compose(const Linear64 &a, const Linear64 &b)
{
    // compose(a,b)(x) = a(b(x)).
    Linear64 c{};
    for (int i = 0; i < 64; ++i)
    {
        c[static_cast<size_t>(i)] = applyLinear(a, b[static_cast<size_t>(i)]);
    }
    return c;
}

static Linear64 power(Linear64 base, u64 exponent)
{
    Linear64 result{};
    for (int i = 0; i < 64; ++i)
    {
        result[static_cast<size_t>(i)] = 1ULL << i;
    }
    while (exponent)
    {
        if (exponent & 1)
        {
            result = compose(base, result);
        }
        base = compose(base, base);
        exponent >>= 1;
    }
    return result;
}

static Linear64 make_transform()
{
    Linear64 t{};
    for (int i = 0; i < 64; ++i)
    {
        t[static_cast<size_t>(i)] = xorshift64(1ULL << i);
    }
    return t;
}

static inline int select1(u64 x, unsigned k)
{
    // k is zero-based. The problem's missing-answer value is 64.
    return static_cast<int>(_tzcnt_u64(_pdep_u64(1ULL << k, x)));
}

using Vec8 = u64 __attribute__((vector_size(64)));

static inline Vec8 next8(Vec8 x)
{
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return x;
}

static u64 solve(u64 n, u64 seed)
{
    u64 answer = 0;

    // Finish the prefix so that the remaining work consists of 128-query blocks.
    u64 tail = n & 127;
    for (u64 i = 0; i < tail; ++i)
    {
        u64 x = seed = xorshift64(seed);
        u64 k = (seed = xorshift64(seed)) & 63;
        answer += static_cast<u64>(select1(x, static_cast<unsigned>(k)));
    }
    n -= tail;
    if (n == 0)
    {
        return answer;
    }

    // One query consumes two xorshift steps. Split the stream into 16 chunks;
    // neighboring chunk starts are n/8 states apart, and each chunk has n/16 queries.
    Linear64 jump = power(make_transform(), n / 8);
    array<u64, 16> starts{};
    starts[0] = seed;
    for (int i = 1; i < 16; ++i)
    {
        starts[static_cast<size_t>(i)] =
            applyLinear(jump, starts[static_cast<size_t>(i - 1)]);
    }

    Vec8 s0{}, s1{};
    memcpy(&s0, starts.data(), 8 * sizeof(u64));
    memcpy(&s1, starts.data() + 8, 8 * sizeof(u64));
    u64 answer0 = 0, answer1 = 0;
    for (u64 block = 0; block < n / 128; ++block)
    {
        #pragma GCC unroll 8
        for (int round = 0; round < 8; ++round)
        {
            Vec8 x0 = next8(s0);
            s0 = next8(x0);
            #pragma GCC unroll 8
            for (int lane = 0; lane < 8; ++lane)
            {
                answer0 += static_cast<u64>(
                    select1(x0[lane], static_cast<unsigned>(s0[lane] & 63)));
            }

            Vec8 x1 = next8(s1);
            s1 = next8(x1);
            #pragma GCC unroll 8
            for (int lane = 0; lane < 8; ++lane)
            {
                answer1 += static_cast<u64>(
                    select1(x1[lane], static_cast<unsigned>(s1[lane] & 63)));
            }
        }
    }
    return answer + answer0 + answer1;
}

int main()
{
#ifdef LOCAL
    auto brute = [](u64 n, u64 seed)
    {
        u64 answer = 0;
        while (n--)
        {
            u64 x = seed = xorshift64(seed);
            unsigned k = static_cast<unsigned>((seed = xorshift64(seed)) & 63);
            answer += static_cast<u64>(select1(x, k));
        }
        return answer;
    };
    mt19937_64 rng(712367821);
    for (u64 n = 0; n <= 512; ++n)
    {
        for (int test = 0; test < 20; ++test)
        {
            u64 seed = rng();
            assert(solve(n, seed) == brute(n, seed));
        }
    }
    assert(solve(2048, 60576) == 100000);
    cerr << "self-test passed\n";
    return 0;
#endif

    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    u64 n, seed;
    cin >> n >> seed;
    cout << solve(n, seed) << '\n';
}
