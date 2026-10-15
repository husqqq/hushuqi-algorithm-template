#pragma once

#include "5_7_001.hpp"

optional<int> vertexGame(int n, const vector<array<int, 2>> &ed, int start)
{
    // n 是点数，ed[i] 两维为简单无向边端点，start 是当前点；每步走到邻点并删掉原点。
    // 返回一条必胜首步的目标点；先手必败时返回空。无法行动者负。
    assert(n > 0 && 0 <= start && start < n);
    Blossom all(n), cut(n);
    for (auto [u, v] : ed)
    {
        assert(0 <= u && u < n && 0 <= v && v < n && u != v);
        all.add(u, v);
        if (u != start && v != start) cut.add(u, v);
    }
    int best = all.work();
    if (all.match[start] == -1 || cut.work() == best) return nullopt;
    return all.match[start];
}
