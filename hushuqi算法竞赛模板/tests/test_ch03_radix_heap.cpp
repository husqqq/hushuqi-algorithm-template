#include "../include/full/3_5_013.hpp"

signed main()
{
    RadixHeap<int> h;
    priority_queue<pair<unsigned long long, int>,
                   vector<pair<unsigned long long, int>>, greater<>> q;
    mt19937_64 rng(20260903);
    unsigned long long last = 0;
    for (int t = 0; t < 200000; t++)
    {
        if (q.empty() || rng() % 3)
        {
            auto k = last + rng() % 1000000;
            int v = rng();
            h.push(k, v);
            q.push({k, v});
        }
        else
        {
            auto x = h.pop();
            auto y = q.top();
            q.pop();
            assert(x.first == y.first);
            last = x.first;
        }
        assert(h.size() == (int)q.size());
    }
    while (!q.empty())
    {
        assert(h.pop().first == q.top().first);
        q.pop();
    }
    assert(h.empty());
    cout << "OK\n";
}
