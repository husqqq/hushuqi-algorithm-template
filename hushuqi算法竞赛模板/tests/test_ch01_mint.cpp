#include "../include/full/1_1_008.hpp"

template <uint32_t P> void checkMint(uint32_t divAns)
{
    // divAns 是 (P-2)/5 的已知最小非负余数；核对模 P 的基础四则运算与逆元。
    using M = MInt<P>;
    assert(M(-2).val() == P - 2);
    assert((M(P - 2) + M(5)).val() == 3);
    assert((M(2) - M(5)).val() == P - 3);
    assert((M(P - 1) * M(P - 1)).val() == 1);
    assert((M(P - 2) / M(5)).val() == divAns);
    assert((M(5) * M(5).inv()).val() == 1);
    assert(invTable<P>(5)[5] == M(5).inv());
}

signed main()
{
    checkMint<998244353>(798595482);
    checkMint<1000000007>(200000001);
}
