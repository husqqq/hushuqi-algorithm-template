#include <bits/stdc++.h>
using namespace std;
namespace lca_st_topic {
#include "../include/full/6_1_004.hpp"
}
#undef int
namespace lca_tarjan_topic {
#include "../include/full/6_1_005.hpp"
}
#undef int
namespace path_topic {
#include "../include/full/6_1_009.hpp"
}
#undef int
namespace edge_decomp_topic {
#include "../include/full/6_2_003.hpp"
}
#undef int
namespace ancestor_topic {
#include "../include/full/6_1_006.hpp"
}
#undef int
namespace reroot_topic {
#include "../include/full/6_3_002.hpp"
}
#undef int
namespace virtual_topic {
#include "../include/full/6_3_003.hpp"
}
#undef int
namespace lct_forward_topic {
#include "../include/full/6_4_003.hpp"
#include "../include/full/6_4_004.hpp"
#include "../include/full/6_4_005.hpp"
}
#undef int
namespace knapsack_forward_topic {
#include "../include/full/6_5_002.hpp"
}
#undef int

signed main()
{
    vector<vector<long long>> g{{1, 2}, {0, 3, 4}, {0, 5}, {1}, {1}, {2}};
    lca_st_topic::LCARMQ st(g);
    assert(st.lca(3, 4) == 1 && st.lca(3, 5) == 0);
    auto off = lca_tarjan_topic::offlineLCA(g, 0, {{3, 4}, {3, 5}, {2, 5}});
    assert((off == vector<long long>{1, 0, 2}));
    path_topic::LCA plca(g);
    assert((path_topic::pathIsect(3, 5, 4, 5, plca) == optional<pair<long long, long long>>({1, 5})));
    auto code = path_topic::pruferCode(g);
    auto rebuilt = path_topic::pruferTree(code);
    for (auto &[u, v] : rebuilt) if (u > v) swap(u, v);
    ranges::sort(rebuilt);
    assert((rebuilt == vector<pair<long long, long long>>({{0, 1}, {0, 2}, {1, 3}, {1, 4}, {2, 5}})));

    vector<vector<array<long long, 2>>> weighted(4);
    auto add = [&](int u, int v) { weighted[u].push_back({v, 1}); weighted[v].push_back({u, 1}); };
    add(0, 1); add(0, 2); add(2, 3);
    edge_decomp_topic::EdgeDecomp ed(weighted);
    assert(ed.origNodes() == 4 && ed.cuts().size() == 3);
    ancestor_topic::KthAncestor ka(g);
    assert(ka.query(4, 1) == 1 && ka.query(4, 2) == 0 && ka.query(4, 3) == -1);
    reroot_topic::RerootHLD hld(g);
    assert(hld.lca(3, 4) == 1 && hld.distance(3, 5) == 4);
    auto sub = hld.subtree(1, 5);
    int count = 0;
    for (auto [l, r] : sub) count += r - l;
    assert(count == 3);
    virtual_topic::LCA vlca(g);
    auto vt = virtual_topic::virtualTree({3, 4, 5}, vlca);
    assert(vt.root == 0 && vt.nodes.size() == 5 && vt.edges.size() == 4);
    cout << "OK\n";
}
