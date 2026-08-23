#include "../include/full/6_2_004.hpp"

signed main()
{
    vector<vector<int>> g{{1}, {0, 2}, {1}};
    DsuOnTree q(g);
    vector<int> cnt(3), ans(3);
    auto add = [&](int u)
    {
        cnt[u]++;
    };
    auto remove = [&](int u)
    {
        cnt[u]--;
    };
    auto answer = [&](int u)
    {
        ans[u] = accumulate(cnt.begin(), cnt.end(), 0LL);
    };
    q.run(add, remove, answer, 0);
    assert((ans == vector<int>{3, 2, 1}));
    fill(cnt.begin(), cnt.end(), 0);
    q.run(add, remove, answer, 2);
    assert((ans == vector<int>{1, 2, 3}));
    return 0;
}
