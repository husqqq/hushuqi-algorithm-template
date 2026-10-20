#ifndef TEST_HEADER
#define TEST_HEADER "../include/full/5_1_001.hpp"
#endif
#include TEST_HEADER

signed main(signed argc, char **argv)
{
    if (argc > 1)
    {
        ForwardStar g(2);
        g.add(0, 1);
        g.addResEdge(0, 1, 7, 3);
        return 0;
    }
    auto check = [](bool ok)
    {
        if (!ok) exit(1);
    };
    ForwardStar g(3);
    check(g.add(0, 1, 4) == 0);
    check(g.add(1, 2, 5) == 1);
    int a = g.addResEdge(0, 2, 7, 3);
    int b = g.addResEdge(1, 1, 9, 2);
    int c = g.addResEdge(0, 2, 0);
    check(a == 2 && b == 4 && c == 6);
    const auto &e = g.edges();
    check(e[a] == (array<int, 4>{2, 7, 0, 0}));
    check(e[a ^ 1] == (array<int, 4>{0, 3, -1, 2}));
    check(e[b] == (array<int, 4>{1, 9, 1, 1}));
    check(e[b ^ 1] == (array<int, 4>{1, 2, b, 1}));
    check(e[c][1] == 0 && e[c ^ 1][1] == 0);
    vector<char> seen(e.size());
    for (int u = 0; u < 3; u++)
    {
        for (int id = g.heads()[u]; id != -1; id = e[id][2])
        {
            check(!seen[id] && e[id][3] == u);
            seen[id] = true;
        }
    }
    check(count(seen.begin(), seen.end(), true) == (int)e.size());
    cout << "OK\n";
}
