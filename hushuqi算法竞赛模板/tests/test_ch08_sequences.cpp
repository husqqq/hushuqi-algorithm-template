#include "../include/full/7_1_007.hpp"
#include "../include/full/8_2_001.hpp"
#include "../include/full/8_2_002.hpp"
#include "../include/full/8_2_003.hpp"
#include "../include/full/8_2_004.hpp"
#include "../include/full/8_2_005.hpp"
#include "../include/full/8_2_006.hpp"
#include "../include/full/8_2_007.hpp"
#include "../include/full/8_2_008.hpp"
#include "../include/full/8_2_009.hpp"

signed main()
{
    int md = numeric_limits<int>::max();
    auto got = fibPair(100, md);
    int a = 0;
    int b = 1;
    for (int i = 0; i < 100; i++)
    {
        int c = ((__int128)a + b) % md;
        a = b;
        b = c;
    }
    assert((got == array<int, 2>{a, b}));
    int luc = ((__int128)2 * b - a) % md;
    if (luc < 0)
    {
        luc += md;
    }
    assert(lucasNumber(100, md) == luc);

    assert((catalan<int>(5) == vector<int>{1, 1, 2, 5, 14, 42}));
    assert((motzkin<int>(6) == vector<int>{1, 1, 2, 4, 9, 21, 51}));
    assert((bell<int>(5) == vector<int>{1, 1, 2, 5, 15, 52}));
    assert((bellFast(5) == vector<Z>{1, 1, 2, 5, 15, 52}));
    auto s1 = stirling1<int>(6);
    auto s2 = stirling2<int>(6);
    assert(s1[6][2] == 274);
    assert(s2[6][3] == 90);
    assert(stirling1F(6)[2] == Z(274));
    assert(stirling2F(6)[3] == Z(90));
    auto e = eulerian<int>(6);
    assert((e[6] == vector<int>{1, 57, 302, 302, 57, 1, 0}));
    auto d = derangements<int>(6);
    assert((d == vector<int>{1, 0, 1, 2, 9, 44, 265}));
    assert((derangeMod(6, 1000) == vector<int>{1, 0, 1, 2, 9, 44, 265}));
    auto part = partitions<int>(10);
    assert(part[10] == 42);
    assert(fastPart(10)[10] == Z(42));

    auto bern = bernoulli<Z>(3);
    assert(bern[0] == Z(1));
    assert(bern[1] == Z(1) / Z(2));
    auto bernFast = fastBern(3);
    assert(bernFast[0] == Z(1));
    assert(bernFast[1] == -Z(1) / Z(2));
    assert(bernFast[2] == Z(1) / Z(6));
    assert(bernFast[3] == Z(0));
    int n = 12345;
    Z sum = (Z(n) * n + n) / Z(2);
    Z formula = (bern[0] * Z(n).pow(2) + Z(2) * bern[1] * Z(n)) / Z(2);
    assert(sum == formula);

    cout << "OK\n";
    return 0;
}
