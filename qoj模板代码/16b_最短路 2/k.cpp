#include <iostream>
#include <cstring>
#include <cstdio>
#include <vector>
#include <queue>
#define MAXN 5010
#define MAXM 300010
#define INF 0x3f3f3f3f3f3f3f3f
#define nil (&node[0])
#define int long long
using namespace std;

struct Edge {
    int u, v, w;
};

struct Node{
    int fa, val, depth;
    vector<int> ch;
} node[MAXM];

int n, m, S, T, B, ans, tclock = 0;
vector<int> G[MAXN];
Edge edge[MAXM];
int d[MAXN];

bool bad[MAXN];

inline int min(int a, int b) {
    return a > b ? b : a;
}

inline void addEdge(int u, int v, int w) {
    edge[B].u = u;
    edge[B].v = v;
    edge[B].w = w;
    G[u].push_back(B ++);
}

template <class T> inline void readInt(T &l) {
    char a;
    T res = 0;
    int minus = false;
    for(;;) {
        a = getchar();
        if(a == ' ' || a == '\n' || a == EOF || a == '\0') {
            l = res;
            break;
        }
        if(a == '-') {
            minus = true;
            continue;
        }
        if(a >= '0' && a <= '9') res *= 10, res += a - '0';
    }
    if(minus) l = -l;
}

inline void in() {
    int i, a, b, c;
    readInt(n);
    readInt(m);
    readInt(S);
    B = 0;
    for(i = 1;i <= n;i ++) {
        G[i].clear();
        node[i].ch.clear();
        node[i].depth = 0;
        node[i].fa = 0;
        node[i].val = 0;
        bad[i] = false;
    }
    for(i = 0;i < m;i ++) {
        readInt(a);
        readInt(b);
        readInt(c);
        addEdge(a, b, c);
    }
}
//////////////////////////////////////////////////////////
inline bool Change(int x, int u, int val) {
    if(bad[x]) return 0;
    if(x == u) return 1;
    node[x].val = val;
    for(int i = 0;i < (int)node[x].ch.size();i ++) {
        int y = node[x].ch[i];
        if(bad[y]) continue;
        node[y].val = val;
        if(Change(y, u, val)) return 1;
    }
    return 0;
}

inline void Maintain(int x) {
    if(bad[x]) return;
    for(int i = 0;i < (int)node[x].ch.size();i ++) {
        int y = node[x].ch[i];
        if(bad[y]) continue;
        node[y].depth = node[x].depth + 1;
        Maintain(y);
    }
}

inline int Query(int x) {
    return node[x].val;
}

inline void Link(int x, int y) {
    node[x].fa = y;
    node[x].depth = node[y].depth + 1;
    Maintain(x);
    node[y].ch.push_back(x);
}

inline void Cut(int x) {
    if(node[x].fa == 0) return;
    Node &s = node[node[x].fa];
    node[x].depth = 1;
    Maintain(x);
    for(int i = 0;i < (int)s.ch.size();i ++) {
        if(s.ch[i] == x) {
            for(int j = i;j < (int)s.ch.size() - 1;j ++) {
                s.ch[j] = s.ch[j + 1];
            }
            s.ch.pop_back();
            break;
        }
    }
    node[x].fa = 0;
}

inline void mark_bad(int u, queue<int> &nQ) {
    if(u <= 0) return;
    if(bad[u]) return;
    bad[u] = true;
    node[u].val = 0;
    nQ.push(u);
}

inline void Mark(int x, int anc, queue<int> &nQ) {
    int cur = x;
    while(cur != 0 && cur != anc) {
        mark_bad(cur, nQ);
        cur = node[cur].fa;
    }
    mark_bad(anc, nQ);
}

inline void Prop(queue<int> &nQ) {
    while(!nQ.empty()) {
        int u = nQ.front(); nQ.pop();
        for(int id : G[u]) {
            int v = edge[id].v;
            if(!bad[v]) mark_bad(v, nQ);
        }
    }
}

inline bool SPFA() {
    int x;
    queue<int> Q;
    queue<int> nQ;

    memset(d, 0x3f, sizeof(d));
    d[S] = 0;

    node[S].depth = 1;
    Q.push(S);
    Change(S, -1, 1);

    while(!Q.empty()) {
        x = Q.front();
        Q.pop();
        if(bad[x]||!Query(x)) continue;
        for(int i = 0;i < (int)G[x].size();i ++) {
            Edge &e = edge[G[x][i]];
            if(bad[e.v]) continue;
            if(d[x] + e.w < d[e.v]) {
                d[e.v] = d[x] + e.w;
                Q.push(e.v);

                if(Change(e.v, x, 0)) {
                    Mark(x, e.v, nQ);
                    Prop(nQ);
                    continue;
                }

                node[e.v].val = 1;
                Cut(e.v);
                Link(e.v, x);
            }
        }
    }
    return 1;
}

signed main() {
    in();
    SPFA();

    for (int i = 1; i <= n; i++) {
        if(bad[i]) {
            printf("-inf ");
        } else if(d[i] > (INF >> 2)) {
            printf("N/A ");
        } else {
            printf("%lld ", d[i]);
        }
    }
    return 0;
}
