#include "../include/full/3_5_008.hpp"

signed main()
{
    mt19937_64 rng(0);
    LeftistHeap<int> h;
    vector<int> root, a;
    for (int i = 0; i < 5000; i++)
    {
        int x = (int)(rng() % 1000000);
        a.push_back(x);
        root.push_back(h.makeHeap(x));
    }
    while (root.size() > 1)
    {
        vector<int> next;
        for (int i = 0; i < (int)root.size(); i += 2)
        {
            next.push_back(i + 1 < (int)root.size() ? h.merge(root[i], root[i + 1]) : root[i]);
        }
        root = move(next);
    }
    sort(a.begin(), a.end());
    int p = root[0];
    for (int x : a)
    {
        assert(h.top(p) == x);
        p = h.pop(p);
    }
    assert(!p);
    cout << "OK\n";
    return 0;
}
