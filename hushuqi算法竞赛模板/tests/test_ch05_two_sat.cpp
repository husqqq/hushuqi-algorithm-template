#include "../include/full/5_4_001.hpp"
#include "../include/full/5_4_002.hpp"
#include "../include/full/5_4_003.hpp"

template <class T>
void commonCases()
{
    T empty(0);
    auto a = empty.solve();
    assert(a && a->empty());

    T q(2);
    q.addOr(0, true, 1, true);
    q.addOr(0, false, 1, true);
    q.addOr(0, true, 1, false);
    auto b = q.solve();
    assert(b && (*b)[0] && (*b)[1]);

    T r(1);
    r.addOr(0, true, 0, true);
    r.addOr(0, false, 0, false);
    assert(!r.solve());

    T s(2);
    s.addOr(0, true, 0, true);
    s.imply(0, true, 1, false);
    auto c = s.solve();
    assert(c && (*c)[0] && !(*c)[1]);
}

signed main()
{
    commonCases<TarjanTwoSAT>();
    commonCases<Kosaraju2SAT>();

    using lex_two_sat::lexTwoSat;
    auto a = lexTwoSat(0, {});
    assert(a && a->empty());
    auto b = lexTwoSat(3, {{0, 1, 1, 1}});
    assert(b && *b == vector<int>({0, 1, 0}));
    assert(!lexTwoSat(1, {{0, 1, 0, 1}, {0, 0, 0, 0}}));
}
