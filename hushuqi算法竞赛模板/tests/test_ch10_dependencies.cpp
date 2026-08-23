#include <bits/stdc++.h>
using namespace std;

namespace fft_topic {
#include "../include/full/10_1_002.hpp"
}
#undef int
namespace poly_topic {
#include "../include/full/10_2_001.hpp"
#include "../include/full/10_2_004.hpp"
#include "../include/full/10_4_004.hpp"
}
#undef int
namespace recurrence_topic {
#include "../include/full/10_4_003.hpp"
}
#undef int

signed main()
{
    auto c = fft_topic::convolution({1, 2, 3}, {4, 5});
    vector<long double> want{4, 13, 22, 15};
    for (int i = 0; i < 4; ++i) assert(abs(c[i] - want[i]) < 1E-10L);
    vector<fft_topic::C> a{{1, 0}, {2, 0}, {3, 0}, {4, 0}};
    auto original = a;
    fft_topic::fft(a);
    fft_topic::fft(a, true);
    for (int i = 0; i < 4; ++i) assert(abs(a[i] - original[i]) < 1E-10L);

    using poly_topic::Poly;
    using poly_topic::Z;
    Poly p{Z(1), Z(2), Z(3)}, q{Z(4), Z(5)};
    assert((p * q == Poly{Z(4), Z(13), Z(22), Z(15)}));
    assert((poly_topic::midProd(p, q) == Poly{Z(14), Z(23)}));
    assert((poly_topic::deriv(p) == Poly{Z(2), Z(6)}));
    assert((poly_topic::integr(poly_topic::deriv(p)) == Poly{Z(0), Z(2), Z(3)}));
    assert((poly_topic::batchInv({Z(2), Z(0), Z(4)}) == vector<Z>{Z(499122177), Z(0), Z(748683265)}));
    auto [num, den] = poly_topic::recFraction({Z(0), Z(1)}, {Z(1), Z(1)});
    assert((num == Poly{Z(0), Z(1)}) && (den == Poly{Z(1), Z(-1), Z(-1)}));

    vector<long long> init{0, 1}, rec{1, 1};
    assert((recurrence_topic::recMul(vector<long long>{1, 0}, vector<long long>{0, 1}, rec) ==
            vector<long long>{0, 1}));
    assert(recurrence_topic::linearRec(init, rec, 20) == 6765);
    cout << "OK\n";
}
