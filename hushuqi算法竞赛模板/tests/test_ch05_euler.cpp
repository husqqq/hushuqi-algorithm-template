#include "../include/full/5_9_001.hpp"

void checkDirected(const vector<pair<int, int>> &edge, const EulerTrail &trail)
{
    // edge 是有向边，trail 是候选迹；检查每条边恰用一次且方向吻合。
    assert(trail.v.size() == edge.size() + 1 && trail.id.size() == edge.size());
    vector<bool> used(edge.size());
    for (int i = 0; i < (int)edge.size(); i++)
    {
        int id = trail.id[i];
        assert(0 <= id && id < (int)edge.size() && !used[id]);
        used[id] = true;
        assert(edge[id] == make_pair(trail.v[i], trail.v[i + 1]));
    }
}

void checkUndirected(const vector<pair<int, int>> &edge, const EulerTrail &trail)
{
    // edge 是无向边，trail 是候选迹；检查每条边恰用一次且端点吻合。
    assert(trail.v.size() == edge.size() + 1 && trail.id.size() == edge.size());
    vector<bool> used(edge.size());
    for (int i = 0; i < (int)edge.size(); i++)
    {
        int id = trail.id[i];
        assert(0 <= id && id < (int)edge.size() && !used[id]);
        used[id] = true;
        auto [u, v] = edge[id];
        assert((u == trail.v[i] && v == trail.v[i + 1]) ||
               (v == trail.v[i] && u == trail.v[i + 1]));
    }
}

signed main()
{
    auto empty = dirEuler(0, {});
    assert(empty.has_value() && empty->v.empty() && empty->id.empty());
    auto point = undirEuler(3, {});
    assert(point.has_value() && point->v == vector<int>({0}));

    vector<pair<int, int>> open = {{0, 1}, {1, 2}, {2, 1}};
    auto a = dirEuler(3, open);
    assert(a.has_value() && a->v.front() == 0 && a->v.back() == 1);
    checkDirected(open, *a);

    vector<pair<int, int>> edge = {{0, 0}, {0, 1}, {0, 1}, {1, 2}};
    auto b = undirEuler(3, edge);
    assert(b.has_value() && b->v.front() == 1 && b->v.back() == 2);
    checkUndirected(edge, *b);

    assert(!dirEuler(4, {{0, 1}, {1, 0}, {2, 3}, {3, 2}}).has_value());
    assert(!undirEuler(6, {{0, 1}, {1, 2}, {2, 0}, {3, 4}, {4, 5}, {5, 3}}).has_value());

    auto lex = dirEuler(3, {{0, 2}, {0, 1}, {1, 0}, {2, 0}}, true);
    assert(lex.has_value() && lex->v == vector<int>({0, 1, 0, 2, 0}));

    vector<pair<int, int>> longTrail;
    for (int i = 0; i < 200000; i++) longTrail.push_back({i, i + 1});
    auto deep = dirEuler(200001, longTrail);
    assert(deep && deep->v.size() == 200001 && deep->id.size() == 200000);
    puts("OK");
    return 0;
}
