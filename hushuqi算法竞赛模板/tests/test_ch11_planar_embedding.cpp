#include "../include/full/11_2_012.hpp"

using Embedding = PlanarEmbed<long long, long long>;

void checkEmbedding(const Embedding &g, int bridgeCount)
{
    // g 是已经 build 的连通直线平面嵌入，bridgeCount 是预期桥数；检查面游走、面积与对偶图不变量，无返回值。
    int n = g.point.size();
    int m = g.edge.size();
    int faces = g.faceArea2.size();
    assert((int)g.halfEdge.size() == 2 * m);
    assert((int)g.nextHalfEdge.size() == 2 * m);
    assert((int)g.faceOfHalf.size() == 2 * m);
    assert((int)g.dual.size() == faces);
    assert(0 <= g.outer && g.outer < faces);

    vector<char> seen(2 * m);
    int cycles = 0;
    for (int start = 0; start < 2 * m; start++)
    {
        if (seen[start])
        {
            continue;
        }
        int face = g.faceOfHalf[start];
        assert(0 <= face && face < faces);
        cycles++;
        int h = start;
        do
        {
            assert(0 <= h && h < 2 * m);
            assert(!seen[h]);
            assert(g.faceOfHalf[h] == face);
            seen[h] = true;
            h = g.nextHalfEdge[h];
        } while (h != start);
    }
    assert(count(seen.begin(), seen.end(), true) == 2 * m);
    assert(cycles == faces || m == 0);

    __int128 areaSum = 0;
    for (__int128 area : g.faceArea2)
    {
        areaSum += area;
    }
    assert(areaSum == 0);
    assert(g.outer == min_element(g.faceArea2.begin(), g.faceArea2.end()) - g.faceArea2.begin());
    if (faces > 1)
    {
        assert(g.faceArea2[g.outer] < 0);
        for (int face = 0; face < faces; face++)
        {
            if (face != g.outer)
            {
                assert(g.faceArea2[face] > 0);
            }
        }
    }

    int gotBridges = 0;
    int dualArcs = 0;
    multiset<tuple<int, int, long long>> expectedDual;
    multiset<tuple<int, int, long long>> actualDual;
    for (const auto &adj : g.dual)
    {
        dualArcs += adj.size();
    }
    for (int a = 0; a < faces; a++)
    {
        for (auto [b, weight] : g.dual[a])
        {
            actualDual.insert({a, b, weight});
        }
    }
    for (int i = 0; i < m; i++)
    {
        int a = g.faceOfHalf[2 * i];
        int b = g.faceOfHalf[2 * i + 1];
        if (a == b)
        {
            gotBridges++;
            continue;
        }
        expectedDual.insert({a, b, g.edge[i].weight});
        expectedDual.insert({b, a, g.edge[i].weight});
    }
    assert(gotBridges == bridgeCount);
    assert(dualArcs == 2 * (m - bridgeCount));
    assert(actualDual == expectedDual);
    if (n >= 1)
    {
        assert(faces == m - n + 2);
    }
}

signed main()
{
    Embedding empty;
    empty.build();
    assert(empty.outer == 0 && empty.faceArea2 == vector<__int128>{0});
    assert(empty.halfEdge.empty() && empty.dual.size() == 1 && empty.dual[0].empty());

    Embedding single(vector<Point<long long>>{{7, -3}});
    single.build();
    checkEmbedding(single, 0);

    Embedding segment(vector<Point<long long>>{{0, 0}, {3, 2}});
    segment.addEdge(0, 1, 7);
    segment.build();
    checkEmbedding(segment, 1);
    assert(segment.outer == 0 && segment.faceArea2 == vector<__int128>{0});
    assert(segment.faceOfHalf[0] == segment.faceOfHalf[1] && segment.dual[0].empty());

    Embedding triangle(vector<Point<long long>>{{0, 0}, {4, 0}, {0, 3}});
    triangle.addEdge(0, 1, 2);
    triangle.addEdge(1, 2, 3);
    triangle.addEdge(2, 0, 5);
    triangle.build();
    checkEmbedding(triangle, 0);
    assert(triangle.faceArea2.size() == 2);
    assert(triangle.faceArea2[triangle.outer] == -12);
    for (int i = 0; i < 3; i++)
    {
        assert(triangle.faceOfHalf[2 * i] != triangle.faceOfHalf[2 * i + 1]);
    }

    Embedding splitSquare(vector<Point<long long>>{{0, 0}, {4, 0}, {4, 4}, {0, 4}, {6, 0}});
    splitSquare.addEdge(0, 1, 10);
    splitSquare.addEdge(1, 2, 20);
    splitSquare.addEdge(2, 3, 30);
    splitSquare.addEdge(3, 0, 40);
    splitSquare.addEdge(0, 2, 50);
    splitSquare.addEdge(1, 4, 60);
    splitSquare.build();
    checkEmbedding(splitSquare, 1);
    assert(splitSquare.faceArea2.size() == 3);
    assert(splitSquare.faceArea2[splitSquare.outer] == -32);
    assert(splitSquare.faceOfHalf[10] == splitSquare.faceOfHalf[11]);
    for (const auto &adj : splitSquare.dual)
    {
        for (auto [face, weight] : adj)
        {
            assert(weight != 60);
        }
    }

    const long long big = 5'000'000'000'000'000'000LL;
    Embedding wide(vector<Point<long long>>{{-big, 0}, {big, 0}, {-big, 1}});
    wide.addEdge(0, 1);
    wide.addEdge(1, 2);
    wide.addEdge(2, 0);
    wide.build();
    checkEmbedding(wide, 0);
    assert(wide.faceArea2[wide.outer] == -(__int128)2 * big);

    mt19937_64 rng(0);
    for (int rep = 0; rep < 2000; rep++)
    {
        int cycleSize = 3 + rng() % 8;
        int leaves = rng() % 4;
        vector<Point<long long>> point;
        for (int i = 0; i < cycleSize + leaves; i++)
        {
            point.push_back({i, i * i});
        }
        Embedding g(point);
        long long weight = 1;
        for (int i = 0; i < cycleSize; i++)
        {
            g.addEdge(i, (i + 1) % cycleSize, weight++);
        }
        for (int i = 2; i + 1 < cycleSize; i++)
        {
            if (rng() & 1)
            {
                g.addEdge(0, i, weight++);
            }
        }
        for (int i = 0; i < leaves; i++)
        {
            g.addEdge(cycleSize - 1 + i, cycleSize + i, weight++);
        }
        g.build();
        checkEmbedding(g, leaves);
        for (int i = 0; i < leaves; i++)
        {
            int edgeId = g.edge.size() - leaves + i;
            assert(g.faceOfHalf[2 * edgeId] == g.faceOfHalf[2 * edgeId + 1]);
        }
    }

    cout << "OK\n";
}
