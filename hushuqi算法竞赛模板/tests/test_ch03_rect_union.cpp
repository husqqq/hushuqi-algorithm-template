#include "../include/full/3_3_012.hpp"

signed main()
{
    RectUnion<int> q;
    vector<Rectangle<int>> a{{0, 0, 2, 2}, {1, 1, 3, 2}, {5, 0, 5, 3}};
    assert(q.area(a) == 5);
    assert(q.perimeter(a) == 10);
    assert(q.solve(a) == 5);
    assert(q.area({}) == 0);
    cout << "5 10 5 0\n";
}
