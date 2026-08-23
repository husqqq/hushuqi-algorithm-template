#include "../include/full/14_1_019.hpp"

string hex128(__int128 x)
{
    if (!x)
    {
        return "0";
    }
    bool neg = x < 0;
    __uint128_t y = neg ? 0 - (__uint128_t)x : (__uint128_t)x;
    const char *d = "0123456789ABCDEF";
    string s;
    while (y)
    {
        s += d[y & 15];
        y >>= 4;
    }
    if (neg)
    {
        s += '-';
    }
    reverse(s.begin(), s.end());
    return s;
}

string randomHex(mt19937_64 &rng, int n)
{
    const char *d = "0123456789ABCDEF";
    string s(1, d[rng() % 15 + 1]);
    while ((int)s.size() < n)
    {
        s += d[rng() & 15];
    }
    return s;
}

signed main()
{
    mt19937_64 rng(0);
    unsigned long long agg = 0;
    for (int tc = 0; tc < 100000; tc++)
    {
        long long x = (long long)(rng() & ((1ULL << 61) - 1));
        long long y = (long long)(rng() & ((1ULL << 61) - 1)) + 1;
        if (rng() & 1)
        {
            x = -x;
        }
        if (rng() & 1)
        {
            y = -y;
        }
        HexBigInt a(x), b(y);
        assert((a + b).toHex() == hex128((__int128)x + y));
        assert((a - b).toHex() == hex128((__int128)x - y));
        assert((a * b).toHex() == hex128((__int128)x * y));
        assert((a / b).toHex() == hex128((__int128)x / y));
        assert((a % b).toHex() == hex128((__int128)x % y));
        agg = agg * 1000000007ULL + (a % b).a.size();
    }
    for (int tc = 0; tc < 120; tc++)
    {
        HexBigInt a(randomHex(rng, rng() % 1800 + 1200));
        HexBigInt b(randomHex(rng, rng() % 700 + 520));
        HexBigInt c(randomHex(rng, rng() % 900 + 1100));
        assert((a + b) - b == a);
        assert((a - b) + b == a);
        assert(a * b == b * a);
        assert(a * (b + c) == a * b + a * c);
        auto [q, r] = divmod(a, b);
        assert(q * b + r == a && r >= HexBigInt(0) && r < b);
        auto [q2, r2] = divmod(a * b, a);
        assert(q2 == b && r2.isZero());
        agg = agg * 1000000007ULL + q.a.size() * 131 + r.a.size();
    }
    for (string s : {"0", "1", "-1", "000001234567890123456789", "-999999999999999999999999"})
    {
        string normalized = s;
        bool neg = !normalized.empty() && normalized[0] == '-';
        int p = neg || normalized[0] == '+';
        while (p + 1 < (int)normalized.size() && normalized[p] == '0') normalized.erase(p, 1);
        if (normalized == "-0" || normalized == "+0") normalized = "0";
        assert(HexBigInt::fromDecimal(s).toDecimal() == normalized);
    }
    for (int tc = 0; tc < 200; tc++)
    {
        auto makeDecimal = [&]()
        {
            int n = rng() % 180 + 1;
            string s(n, '0');
            s[0] = char('1' + rng() % 9);
            for (int i = 1; i < n; i++) s[i] = char('0' + rng() % 10);
            if (rng() & 1) s.insert(s.begin(), '-');
            return s;
        };
        string sa = makeDecimal(), sb = makeDecimal();
        HexBigInt a = HexBigInt::fromDecimal(sa), b = HexBigInt::fromDecimal(sb);
        assert(a.toDecimal() == sa);
        HexBigInt product = a * b;
        assert(product / b == a && product % b == HexBigInt(0));
        auto [q, r] = divmod(a, b);
        assert(q * b + r == a);
        assert(r.isZero() || (r.neg == a.neg && r < (b.neg ? -b : b)));
    }
    {
        BigInt a("1234567890123456789012345678901234567890");
        BigInt b("98765432109876543210987654321");
        assert((a * b).toDecimal() == "121932631137021795226185032733744855963362292333223746380111126352690");
        auto [q, r] = divmod(a, b);
        assert(q * b + r == a && r >= BigInt(0) && r < b);
        assert((BigInt("999999999999999999999999") / BigInt("3")).toDecimal() == "333333333333333333333333");
    }
    string s(1000000, 'F');
    HexBigInt x(s);
    HexBigInt y = x + HexBigInt(1);
    assert(y.toHex() == "1" + string(1000000, '0'));
    cout << agg << ' ' << y.a.size() << '\n';
    return 0;
}
