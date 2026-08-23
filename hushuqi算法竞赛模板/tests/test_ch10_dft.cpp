#include "../include/full/10_1_001.hpp"

signed main()
{
    vector<Complex> a{{1, 0}, {2, 0}, {3, 0}, {4, 0}};
    auto b = dftNaive(a);
    auto c = dftNaive(b, true);
    for (int i = 0; i < 4; ++i) assert(abs(c[i] - a[i]) < 1E-12L);
    assert(abs(b[0] - Complex(10, 0)) < 1E-12L);
    cout << "OK\n";
}
