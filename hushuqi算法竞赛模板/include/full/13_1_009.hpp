#pragma once

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
        const array<uint16_t, 16> basis = {
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
        uint16_t al = a & 65535, ah = a >> 16;
        uint16_t bl = b & 65535, bh = b >> 16;
        uint16_t x = p16(al, bl, 3);
        uint16_t y = p16(ah, bh, 6);
        uint16_t z = p16(al ^ ah, bl ^ bh, 3);
        return (uint32_t)(y ^ z) << 16 | p16(z ^ x, 1, 3);
    }

    static uint32_t p32(uint32_t a, uint32_t b)
    {
        uint16_t al = a & 65535, ah = a >> 16;
        uint16_t bl = b & 65535, bh = b >> 16;
        uint16_t c = p16(al, bl);
        return (uint32_t)(p16(al ^ ah, bl ^ bh) ^ c) << 16 |
               (p16(ah, bh, 3) ^ c);
    }

    static uint32_t product32(uint32_t a, uint32_t b)
    {
        // a、b 是 32 位 nimber；自动初始化后返回它们的积。
        init();
        return p32(a, b);
    }

    static unsigned long long product(unsigned long long a, unsigned long long b)
    {
        init();
        uint32_t al = a & 0xffffffffULL, ah = a >> 32;
        uint32_t bl = b & 0xffffffffULL, bh = b >> 32;
        uint32_t c = p32(al, bl);
        return (unsigned long long)(p32(al ^ ah, bl ^ bh) ^ c) << 32 |
               (p32high(ah, bh) ^ c);
    }
};

unsigned long long nimProduct(unsigned long long a, unsigned long long b)
{
    return NimProduct64::product(a, b);
}

uint32_t nimProduct32(uint32_t a, uint32_t b)
{
    // a、b 是 32 位 nimber；返回乘积，不要求调用方预先初始化。
    return NimProduct64::product32(a, b);
}
