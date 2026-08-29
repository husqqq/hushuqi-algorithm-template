#include "../include/base.hpp"

signed main()
{
    long long mn;
    long long mx;
    unsigned long long umx;
    long long bad;
    string tail;

    FILE *src = tmpfile();
    assert(src);
    fputs("-9223372036854775808 9223372036854775807 18446744073709551615 - tail\n", src);
    rewind(src);
    FastInput in(src);
    assert(in.read(mn));
    assert(in.read(mx));
    assert(in.read(umx));
    assert(!in.read(bad));
    assert(in.read(tail));
    assert(mn == numeric_limits<long long>::min());
    assert(mx == numeric_limits<long long>::max());
    assert(umx == numeric_limits<unsigned long long>::max());
    assert(tail == "tail");
    fclose(src);

    array<string, 3> text{
        "0",
        "170141183460469231731687303715884105727",
        "-170141183460469231731687303715884105728"
    };
    for (const auto &s : text)
    {
        auto x = parseInt128(s);
        assert(x && toString(*x) == s);
    }
    assert(!parseInt128("+"));
    assert(!parseInt128("1x"));
    assert(!parseInt128("170141183460469231731687303715884105728"));
    assert(!parseInt128("-170141183460469231731687303715884105729"));

    i128 a((int64_t)-123456789);
    i128 b((int64_t)987654321);
    assert((a + b).toString() == "864197532");
    assert((a - b).toString() == "-1111111110");
    assert((a * b).toString() == "-121932631112635269");
    assert((b / a).toString() == "-8");
    assert((b % a).toString() == "9");

    DynBitset x(70);
    DynBitset y(70);
    x.set(0);
    x.set(63);
    x.set(69);
    y.set(63);
    y.set(64);
    DynBitset u = x;
    DynBitset v = x;
    u |= y;
    v &= y;
    assert(u.count() == 4 && u.test(64));
    assert(v.count() == 1 && v.test(63));

    cout << "OK\n";
}
