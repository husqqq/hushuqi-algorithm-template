
#include <bits/stdc++.h>
using namespace std;
#define int long long

struct NimProduct64
{
    inline static array<uint16_t, 131075> ex{};
    inline static array<uint16_t, 65536> lg{};
    inline static bool ready = false;

    static void init()
    {
        if (ready)
        {
            return;
        }
        ready = true;
        const uint16_t basis[16] = {
            10279, 15417, 35722, 52687, 44124, 62628, 15661, 5686,
            3862, 1323, 334, 647, 61560, 20636, 4267, 8445
        };
        array<uint16_t, 65536> nxt{};
        for (int i = 0; i < 16; i++)
        {
            for (int s = 0; s < (1LL << i); s++)
            {
                nxt[s | 1LL << i] = nxt[s] ^ basis[i];
            }
        }
        ex[0] = 1;
        for (int i = 0; i < 65534; i++)
        {
            ex[i + 1] = nxt[ex[i]];
        }
        for (int i = 0; i < 65535; i++)
        {
            lg[ex[i]] = i;
            ex[65535 + i] = ex[i];
        }
        for (int i = 0; i < 5; i++)
        {
            ex[131070 + i] = ex[i];
        }
    }

    static uint16_t p16(uint16_t a, uint16_t b, int shift = 0)
    {
        if (!a || !b)
        {
            return 0;
        }
        return ex[(uint32_t)lg[a] + lg[b] + shift];
    }

    static uint32_t p32high(uint32_t a, uint32_t b)
    {
        uint16_t al = a, ah = a >> 16, bl = b, bh = b >> 16;
        uint16_t x = p16(al, bl, 3);
        uint16_t y = p16(ah, bh, 6);
        uint16_t z = p16(al ^ ah, bl ^ bh, 3);
        return (uint32_t)(y ^ z) << 16 | p16(z ^ x, 1, 3);
    }

    static uint32_t p32(uint32_t a, uint32_t b)
    {
        uint16_t al = a, ah = a >> 16, bl = b, bh = b >> 16;
        uint16_t c = p16(al, bl);
        return (uint32_t)(p16(al ^ ah, bl ^ bh) ^ c) << 16 |
               (p16(ah, bh, 3) ^ c);
    }

    static unsigned long long product(unsigned long long a, unsigned long long b)
    {
        init();
        uint32_t al = a, ah = a >> 32, bl = b, bh = b >> 32;
        uint32_t c = p32(al, bl);
        return (unsigned long long)(p32(al ^ ah, bl ^ bh) ^ c) << 32 |
               (p32high(ah, bh) ^ c);
    }
};

unsigned long long nimProduct(unsigned long long a, unsigned long long b)
{
    return NimProduct64::product(a, b);
}

signed main()
{
    uint32_t t, sa, sb, sc;
    cin >> t >> sa >> sb >> sc;
    auto rng = [&]() -> uint32_t
    {
        sa ^= sa << 16;
        sa ^= sa >> 5;
        sa ^= sa << 1;
        uint32_t z = sa;
        sa = sb;
        sb = sc;
        sc ^= z ^ sa;
        return sc;
    };
    NimProduct64::init();
    uint32_t last = 0;
    while (t--)
    {
        uint32_t x = rng() + last;
        uint32_t y = rng();
        last = NimProduct64::p32(x, y);
    }
    cout << last << '\n';
}
