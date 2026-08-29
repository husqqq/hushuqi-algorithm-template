#include "../include/full/3_2_009.hpp"
#include "../include/full/1_1_008.hpp"

signed main()
{
    RectAddSum<int> s;
    s.add(0, 0, 3, 2, 4);
    s.add(1, 1, 4, 3, -1);
    int q0 = s.query(0, 0, 4, 3);
    int q1 = s.query(1, 0, 3, 2);
    int q2 = s.query(2, 2, 2, 5);
    vector<int> ans = s.work();
    assert(q0 == 0 && q1 == 1 && q2 == 2);
    assert(ans == vector<int>({18, 14, 0}));

    RectAddSum<Z> modular;
    modular.add(-2, -1, 2, 3, Z(3));
    modular.add(0, 1, 4, 4, Z(-5));
    int m0 = modular.query(-2, -1, 4, 4);
    int m1 = modular.query(0, 1, 2, 3);
    int m2 = modular.query(2, 0, 2, 9);
    assert(m0 == 0 && m1 == 1 && m2 == 2);
    assert(modular.work() == vector<Z>({Z(-12), Z(-8), Z(0)}));
    cout << "OK\n";
}
