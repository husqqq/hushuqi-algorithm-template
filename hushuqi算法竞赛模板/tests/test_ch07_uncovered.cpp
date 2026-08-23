#include <bits/stdc++.h>
using namespace std;
namespace rational_topic {
#include "../include/full/7_1_013.hpp"
}
#undef int
namespace segment_sieve_topic {
#include "../include/full/7_2_002.hpp"
}
#undef int
namespace multiplicative_topic {
#include "../include/full/7_3_001.hpp"
}
#undef int
namespace carmichael_topic {
#include "../include/full/7_4_007.hpp"
}
#undef int

signed main()
{
    auto r = rational_topic::ratRebuild(51, 101, 5, 5);
    assert(r && r->first == 1 && r->second == 2);
    assert((segment_sieve_topic::segPrimes(90, 115) == vector<long long>{97, 101, 103, 107, 109, 113}));
    assert((multiplicative_topic::phiTable(10) == vector<long long>{0, 1, 1, 2, 2, 4, 2, 6, 4, 6, 4}));
    assert((multiplicative_topic::mobiusTable(10) == vector<long long>{0, 1, -1, -1, 0, -1, 1, -1, 0, 0, 1}));
    assert(carmichael_topic::carmichael(8) == 2 && carmichael_topic::carmichael(15) == 4);
    cout << "OK\n";
}
