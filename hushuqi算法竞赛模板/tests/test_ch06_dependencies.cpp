#include <bits/stdc++.h>
using namespace std;
#include "../include/topic_common.hpp"

namespace path_topic {
#include "../include/full/6_1_007.hpp"
}
#undef int
namespace ancestor_topic {
#include "../include/full/6_2_005.hpp"
}
#undef int

signed main()
{
    vector<vector<long long>> g{{1, 2}, {0, 3, 4}, {0}, {1}, {1}};
    path_topic::LCA lca(g);
    assert(lca.lca(3, 4) == 1 && lca.pathKth(3, 2, 2) == 0);
    assert(path_topic::pathIsect(3, 2, 4, 2, lca).has_value());
    auto code = path_topic::pruferCode(g);
    assert(path_topic::pruferTree(code).size() == 4);

    ancestor_topic::KthAncestor anc(g);
    assert(anc.query(4, 0) == 4);
    assert(anc.query(4, 2) == 0);
    assert(anc.query(4, 3) == -1);
    cout << "OK\n";
}
