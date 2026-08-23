#include "../include/full/5_3_010.hpp"
#include "../include/full/5_3_011.hpp"
#include "../include/full/5_3_005.hpp"

vector<vector<int>> norm(vector<vector<int>> a)
{
    for (auto &v : a)
    {
        sort(v.begin(), v.end());
    }
    sort(a.begin(), a.end());
    return a;
}

signed main()
{
    Cactus a(0);
    assert(a.build() && a.cycles.empty());

    Cactus b(6);
    b.add(0, 1);
    b.add(1, 2);
    b.add(2, 0);
    b.add(0, 3);
    b.add(3, 4);
    b.add(4, 0);
    b.add(4, 5);
    assert(b.build() && b.cycles.size() == 2);
    assert(b.build() && b.cycles.size() == 2);

    Cactus c(4);
    c.add(0, 1);
    c.add(1, 2);
    c.add(2, 0);
    c.add(1, 3);
    c.add(3, 2);
    assert(!c.build());

    assert(complementCC({}).empty());
    vector<vector<int>> x{{0, 1, 2}}, y{{0}, {1}, {2}}, z{{0, 1, 2, 3}};
    assert(norm(complementCC({{}, {}, {}})) == x);
    assert(norm(complementCC({{1, 2}, {0, 2}, {0, 1}})) == y);
    assert(norm(complementCC({{1}, {0, 2}, {1, 3}, {2}})) == z);

    VertexBCC vb(6);
    vb.addEdge(0, 1);
    vb.addEdge(1, 2);
    vb.addEdge(2, 0);
    vb.addEdge(1, 3);
    vb.addEdge(3, 4);
    vb.addEdge(3, 4);
    vb.build();
    assert(norm(vb.comps()) == vector<vector<int>>({{0, 1, 2}, {1, 3}, {3, 4}, {5}}));
    assert(vb.cutVertices()[1] && vb.cutVertices()[3] && !vb.cutVertices()[0]);
}
