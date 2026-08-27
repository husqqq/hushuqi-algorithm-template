#include <bits/stdc++.h>
using namespace std;

struct SAM
{
    struct Node { int next[26]{}, link = -1, len = 0; long long cnt = 0; };
    vector<Node> st{{}};
    int last = 0;
    void add(char ch)
    {
        int c = ch - 'a', cur = st.size();
        st.push_back({});
        st[cur].len = st[last].len + 1;
        st[cur].cnt = 1;
        int p = last;
        while (p != -1 && !st[p].next[c]) st[p].next[c] = cur, p = st[p].link;
        if (p == -1) st[cur].link = 0;
        else
        {
            int q = st[p].next[c];
            if (st[p].len + 1 == st[q].len) st[cur].link = q;
            else
            {
                int clone = st.size();
                st.push_back(st[q]);
                st[clone].len = st[p].len + 1;
                st[clone].cnt = 0;
                while (p != -1 && st[p].next[c] == q) st[p].next[c] = clone, p = st[p].link;
                st[q].link = st[cur].link = clone;
            }
        }
        last = cur;
    }
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    cin >> s;
    SAM sam;
    for (char c : s) sam.add(c);
    vector<int> order(sam.st.size());
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b){ return sam.st[a].len > sam.st[b].len; });
    long long ans = 0;
    for (int u : order)
    {
        if (sam.st[u].cnt > 1) ans = max(ans, sam.st[u].cnt * sam.st[u].len);
        if (sam.st[u].link >= 0) sam.st[sam.st[u].link].cnt += sam.st[u].cnt;
    }
    cout << ans << '\n';
}
