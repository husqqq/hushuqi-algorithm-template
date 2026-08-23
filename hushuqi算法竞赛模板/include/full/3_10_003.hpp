#include <bits/stdc++.h>
using namespace std;
#define int long long

struct TreeBlock
{
    vector<int> bel, cap;

    TreeBlock(const vector<vector<int>> &g, int root, int B) : bel(g.size(), -1)
    {
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
