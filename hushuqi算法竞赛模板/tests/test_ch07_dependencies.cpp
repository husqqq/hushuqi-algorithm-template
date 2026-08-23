#include <bits/stdc++.h>
using namespace std;

namespace arithmetic_topic {
#include "../include/full/7_1_001.hpp"
#include "../include/full/7_2_009.hpp"
}
#undef int

signed main()
{
    using namespace arithmetic_topic;
    assert(mulMod(-3, 7, 11) == 1 && powerMod(2, 10, 1000) == 24);
    assert(binaryGcd(-48, 18) == 6 && safeLcm(12, 18) == 36);
    auto eg = exgcd(30, 18);
    assert(eg[0] == 6 && (__int128)30 * eg[1] + (__int128)18 * eg[2] == 6);
    assert(invMod(3, 11) == 4 && !invMod(6, 9));
    assert((congruence(4, 2, 6) == optional<Cong>({2, 3})));
    assert((crt(Cong{2, 3}, Cong{3, 5}) == optional<Cong>({8, 15})));
    assert((batchInv({1, 2, 4}, 7) == vector<long long>{1, 4, 2}));

    PPFactorial fact(2, 3);
    assert(fact.modulus() == 8 && fact.exponent(6) == 4);
    assert(fact.fact(3) == 6 && fact.fact(4) == 0);
    assert(fact.binomial(6, 2) == 7 && fact.binomial(3, 5) == 0);
    cout << "OK\n";
}
