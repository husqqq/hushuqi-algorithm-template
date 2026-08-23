#include "../include/full/5_10_001.hpp"
#include "../include/full/5_10_002.hpp"
#include "../include/full/5_10_004.hpp"
#include "../include/full/5_10_005.hpp"
#include "../include/full/5_10_006.hpp"
#include "../include/full/5_10_011.hpp"
#include "../include/full/5_10_012.hpp"
#include "../include/full/5_10_015.hpp"

signed main()
{
    assert(chordal({{1}, {0, 2}, {1}}).ok);
    auto c4 = chordal({{1, 3}, {0, 2}, {1, 3}, {0, 2}});
    assert(!c4.ok);
    auto cert = chordalCert(4, {{0, 1}, {1, 2}, {2, 3}, {3, 0}});
    assert(!cert.ok && cert.cycle.size() == 4);
    assert(chordalCert(4, {{0, 1}, {1, 2}, {2, 3}}).ok);
    assert(weightClique(3, {1, 2, 3}, {{0, 1}, {1, 2}, {0, 2}}, 998244353) == 23);
    auto side = planar_graph::chordPlanar(4, {{0, 2}, {1, 3}});
    assert(side.has_value() && (*side)[0] != (*side)[1]);
    assert(!planar_graph::chordPlanar(6, {{0, 3}, {1, 4}, {2, 5}}).has_value());
    assert(planar_graph::faceCount(4, 4, 1) == 2);
    assert(planar_graph::faceCount(0, 0, 0) == 1);

    assert(tourScore({0, 1, 2}));
    assert(tourScore({1, 1, 1}));
    assert(!tourScore({0, 0, 3}));
    assert(graphical({2, 2, 2}));
    assert(graphical({3, 3, 2, 2, 2, 2}));
    assert(!graphical({3, 3, 1, 1}));

    auto first = [](const vector<bool> &in)
    {
        return in[0] + in[1] <= 1 && in[2] + in[3] + in[4] <= 1;
    };
    auto second = [](const vector<bool> &in)
    {
        return in[0] + in[2] <= 1 && in[1] + in[3] + in[4] <= 1;
    };
    auto common = matroidMeet(5, first, second);
    assert(common.size() == 2);

    vector<vector<int>> tri = {{1, 2}, {0, 2}, {0, 1}};
    auto dir = bipolarOrd(tri, 0, 1);
    assert(dir.size() == 3);
    vector<int> in(3), out(3);
    for (auto [u, v] : dir)
    {
        out[u]++;
        in[v]++;
    }
    assert(in[0] == 0 && out[1] == 0);
    assert(in[2] > 0 && out[2] > 0);

    vector col(5, vector<int>(5));
    for (int i = 0; i < 5; i++)
    {
        col[i][(i + 1) % 5] = 1;
        col[(i + 1) % 5][i] = 1;
    }
    assert(!ramsey::monoTriangle(col).has_value());
    assert(ramsey::monoTriangle(vector(6, vector<int>(6))).has_value());

    puts("OK");
    return 0;
}
