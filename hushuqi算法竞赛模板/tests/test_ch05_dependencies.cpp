#include <bits/stdc++.h>
using namespace std;

namespace bellman_topic {
#include "../include/full/5_2_002.hpp"
}
#undef int
namespace block_cut_topic {
#include "../include/full/5_3_006.hpp"
}
#undef int
namespace forest_topic {
#include "../include/full/5_5_003.hpp"
}
#undef int
namespace flow_topic {
#include "../include/full/5_8_001.hpp"
}
#undef int

signed main()
{
    auto shortest = bellman_topic::bellmanFord(4, {{0, 1, 2}, {1, 2, -1}, {0, 2, 5}}, 0);
    assert(shortest.cycle.empty() && shortest.d[2] == 1 && !shortest.d[3]);
    auto negative = bellman_topic::bellmanFord(3, {{0, 1, 1}, {1, 2, -3}, {2, 1, 1}}, 0);
    assert(!negative.cycle.empty());

    auto tree = block_cut_topic::blockCutTree(4, {{0, 1, 2}, {2, 3}});
    assert(tree.size() == 6 && tree[2].size() == 2 && tree[4].size() == 3);

    vector<forest_topic::MEdge> edges{{0, 1, 1}, {1, 2, 2}, {2, 3, 3}, {0, 3, 10}};
    assert(forest_topic::minKForest(4, 2, edges) == 3);
    assert(!forest_topic::minKForest(5, 1, edges));

    flow_topic::Flow<long long> flow(4);
    flow.add(0, 1, 3);
    flow.add(0, 2, 2);
    flow.add(1, 2, 1);
    flow.add(1, 3, 2);
    flow.add(2, 3, 3);
    assert(flow.flow(0, 3) == 5);
    auto cut = flow.cut(0);
    assert(cut[0] && !cut[3]);
    cout << "OK\n";
}
