#include "../include/full/3_7_008.hpp"

signed main()
{
    PermTree tr({0, 1, 2, 3});
    assert(tr.t.size() == 5);
    assert(tr.root >= 0);
    const auto &root = tr.t[tr.root];
    assert(root.l == 0 && root.r == 4 && root.par == -1);
    assert(root.ch.size() == 4);
    for (int u : root.ch)
    {
        assert(tr.t[u].r - tr.t[u].l == 1 && tr.t[u].par == tr.root);
    }

    PermTree empty(vector<int>{});
    assert(empty.root == -1 && empty.t.empty());

    long long lo = numeric_limits<long long>::lowest();
    PermTree edge(vector<long long>{lo + 2, lo, lo + 1});
    assert(edge.root >= 0);
    assert(edge.t[edge.root].l == 0 && edge.t[edge.root].r == 3);
    cout << "OK\n";
}
