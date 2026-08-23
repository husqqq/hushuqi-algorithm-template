#include <bits/stdc++.h>
using namespace std;
namespace deldag_topic {
#include "../include/full/5_1_006.hpp"
}
#undef int
namespace kosa_topic {
#include "../include/full/5_3_002.hpp"
}
#undef int
namespace bitkosa_topic {
#include "../include/full/5_3_003.hpp"
}
#undef int
namespace blockcut_topic {
#include "../include/full/5_3_007.hpp"
}
#undef int
namespace prim_topic {
#include "../include/full/5_5_013.hpp"
}
#undef int
namespace edmonds_forward_topic {
#include "../include/full/5_7_004.hpp"
}
#undef int
namespace euler_forward_topic {
#include "../include/full/5_9_002.hpp"
}
#undef int

signed main()
{
    deldag_topic::DelDAG dag(4);
    dag.addEdge(0, 1); dag.addEdge(1, 2); dag.addEdge(0, 2); dag.addEdge(2, 3);
    assert((dag.solve() == vector<long long>{2, 2, 1, 2}));
    vector<vector<long long>> g{{1}, {2}, {0, 3}, {4}, {3}};
    auto a = kosa_topic::kosarajuSCC(g), b = bitkosa_topic::bitsetSCC(g);
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            assert((a[i] == a[j]) == (b[i] == b[j]));
    assert(a[0] == a[2] && a[3] == a[4] && a[0] != a[3]);

    auto tree = blockcut_topic::blockCutTree(4, {{0, 1, 2}, {2, 3}});
    assert(tree.size() == 6 && tree[2].size() == 2);
    vector<vector<pair<long long, long long>>> sparse(4);
    auto add = [&](int u, int v, int w) { sparse[u].push_back({v, w}); sparse[v].push_back({u, w}); };
    add(0, 1, 1); add(1, 2, 2); add(0, 2, 5); add(2, 3, 3);
    auto ps = prim_topic::primSparse(sparse);
    const long long I = numeric_limits<long long>::max() / 4;
    vector<vector<long long>> dense{{0, 1, 5, I}, {1, 0, 2, I}, {5, 2, 0, 3}, {I, I, 3, 0}};
    auto pd = prim_topic::primDense(dense);
    assert(ps && pd && ps->sum == 6 && pd->sum == 6);
    cout << "OK\n";
}
