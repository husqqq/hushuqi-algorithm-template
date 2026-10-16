#include "../include/full/4_2_002.hpp"

signed main()
{
    // 无参数；核对 Trie 状态数、fail 链和经典多模式匹配结果，成功返回 0。
    AC<26> ac;
    int x = ac.add("aba"), y = ac.add("ba");
    ac.build();
    assert(ac.nodes().size() == 6 && x != y);
    AC<26> aho;
    vector<string> pat{"he", "she", "his", "hers"};
    for (const string &s : pat)
    {
        aho.add(s);
    }
    aho.build();
    const auto &nd = aho.nodes();
    assert(nd.size() == 10);
    vector<int> hit(nd.size());
    int u = 0;
    for (char c : string("ahishers"))
    {
        u = aho.step(u, c - 'a');
        hit[u]++;
    }
    vector<int> cnt(nd.size());
    for (int s = 1; s < (int)nd.size(); s++)
    {
        for (int p = s; p > 0; p = nd[p].fail)
        {
            cnt[p] += hit[s];
        }
    }
    vector<int> ans;
    for (int p : aho.ends())
    {
        ans.push_back(cnt[p]);
    }
    assert(ans == vector<int>({1, 1, 1, 1}));
    cout << "OK\n";
}
