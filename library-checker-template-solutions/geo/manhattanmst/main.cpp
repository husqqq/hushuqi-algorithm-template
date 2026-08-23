

#include <bits/stdc++.h>
using namespace std;
#define int long long

struct DSU
{
    // f 是并查集父亲，s 仅在根处保存连通块大小。
    vector<int> f, s;

    DSU(int n = 0)
    {
        // n 是元素个数；建立 n 个单点集合。
        init(n);
    }

    void init(int n)
    {
        // n 是元素个数；清空旧状态并建立 n 个单点集合，无返回值。
        f.resize(n);
        iota(f.begin(), f.end(), 0);
        s.assign(n, 1);
    }

    int find(int x)
    {
        // x 是元素编号；返回所在集合的根并压缩路径。
        return x == f[x] ? x : f[x] = find(f[x]);
    }

    bool merge(int x, int y)
    {
        // x、y 是元素编号；合并所在集合，原本不连通时返回 true。
        x = find(x);
        y = find(y);
        if (x == y)
        {
            return false;
        }
        if (s[x] < s[y])
        {
            swap(x, y);
        }
        f[y] = x;
        s[x] += s[y];
        return true;
    }

    bool same(int x, int y)
    {
        // x、y 是元素编号；返回二者是否属于同一集合。
        return find(x) == find(y);
    }

    int size(int x)
    {
        // x 是元素编号；返回所在集合的元素个数。
        return s[find(x)];
    }
};

struct MEdge
{
    // x、y 是无向边端点，w 是边权。
    int x, y, w;
};

optional<pair<int, vector<MEdge>>> kruskal(int n, vector<MEdge> e)
{
    // n 是点数，e 是无向边集；返回 MST 总权与所选边，不连通时返回空。
    sort(e.begin(), e.end(), [](const MEdge &a, const MEdge &b)
         { return a.w < b.w; });
    DSU d(n);
    int ans = 0;
    vector<MEdge> use;
    for (auto v : e)
    {
        if (!d.merge(v.x, v.y))
        {
            continue;
        }
        ans += v.w;
        use.push_back(v);
    }
    if ((int)use.size() + 1 != n && n)
    {
        return nullopt;
    }
    return pair{ans, use};
}

// CP[0..1] 依次是原点的 x、y 坐标。
using CP = array<int, 2>;

struct GeometricMST
{
    __int128 weight = 0; // 最小生成树总权。
    vector<pair<int, int>> edge; // 所选边的原点编号。
};

GeometricMST chebyshevMstEdges(const vector<CP> &input)
{
    // input[i][0..1] 是原点坐标；返回切比雪夫 MST 总权与所选边。
    struct Point
    {
        // x、y 是变换后的坐标，id 是原点编号。
        __int128 x, y;
        int id;
    };
    struct Edge
    {
        // w 是候选边权，u、v 是原点编号。
        __int128 w;
        int u, v;
    };
    int n = input.size();
    vector<Point> p;
    for (int i = 0; i < n; ++i)
    {
        p.push_back({(__int128)input[i][0] + input[i][1],
                     (__int128)input[i][0] - input[i][1], i});
    }
    vector<Edge> e;
    for (int dir = 0; dir < 4; ++dir)
    {
        ranges::sort(p,
                     [](const Point &a, const Point &b)
                     {
                         return a.x + a.y != b.x + b.y ? a.x + a.y < b.x + b.y : a.x < b.x;
                     });
        map<__int128, int> sweep;
        for (int i = 0; i < n; ++i)
        {
            for (auto it = sweep.lower_bound(-p[i].y); it != sweep.end();)
            {
                int j = it->second;
                if (p[i].x - p[j].x < p[i].y - p[j].y)
                {
                    break;
                }
                __int128 dx = p[i].x - p[j].x;
                __int128 dy = p[i].y - p[j].y;
                if (dx < 0)
                {
                    dx = -dx;
                }
                if (dy < 0)
                {
                    dy = -dy;
                }
                e.push_back({(dx + dy) / 2, p[i].id, p[j].id});
                it = sweep.erase(it);
            }
            sweep[-p[i].y] = i;
        }
        for (auto &v : p)
        {
            if (dir & 1)
            {
                v.x = -v.x;
            }
            else
            {
                swap(v.x, v.y);
            }
        }
    }
    ranges::sort(e,
                 {},
                 [](const Edge &a)
                 {
                     return a.w;
                 });
    DSU d(n);
    GeometricMST ans;
    for (auto [w, u, v] : e)
    {
        if (!d.merge(u, v))
        {
            continue;
        }
        ans.weight += w;
        ans.edge.push_back({u, v});
    }
    return ans;
}

__int128 chebyshevMst(const vector<CP> &input)
{
    // input[i][0..1] 是原点坐标；返回切比雪夫 MST 总权。
    return chebyshevMstEdges(input).weight;
}

signed main(){ios::sync_with_stdio(false);cin.tie(nullptr);int n;cin>>n;vector<CP>p(n),q(n);for(int i=0;i<n;i++){cin>>p[i][0]>>p[i][1];q[i]={p[i][0]+p[i][1],p[i][0]-p[i][1]};}auto r=chebyshevMstEdges(q);cout<<(long long)r.weight<<'\n';for(auto[u,v]:r.edge)cout<<u<<' '<<v<<'\n';}
