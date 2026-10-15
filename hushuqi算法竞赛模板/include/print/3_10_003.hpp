#include <bits/stdc++.h>
using namespace std;
#define int long long

struct TreeBlock
{
    vector<int> bel, cap; // bel[u] 是点 u 所属块编号，cap[k] 是第 k 块的块根（省会）。

    TreeBlock(const vector<vector<int>> &g, int root, int B) : bel(g.size(), -1)
    {
        // g 是树的邻接表，root 是根，B 是块大小下界；王室联邦分块，每块至少 B 个点（剩余点并入最后一块），块根可能不在块内。
        assert(!g.empty() && 0 <= root && root < (int)g.size() && B > 0); // 调试检查，可删。
        vector<int> st;
        auto dfs = [&](auto &&self, int u, int f) -> void
        {
            int base = st.size();
            for (int v : g[u])
            {
                if (v == f)
                {
                    continue;
                }
                self(self, v, u);
                if ((int)st.size() - base >= B)
                {
                    cap.push_back(u);
                    while ((int)st.size() > base)
                    {
                        bel[st.back()] = (int)cap.size() - 1;
                        st.pop_back();
                    }
                }
            }
            st.push_back(u);
        };
        dfs(dfs, root, -1);
        if (cap.empty())
        {
            cap.push_back(root);
        }
        while (!st.empty())
        {
            bel[st.back()] = (int)cap.size() - 1;
            st.pop_back();
        }
    }
};
