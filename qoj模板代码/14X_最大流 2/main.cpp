#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

class FastInput
{
    static constexpr int BUFFER_SIZE = 1 << 20;
    char buffer[BUFFER_SIZE];
    int position = 0;
    int length = 0;

    char getChar()
    {
        if (position == length)
        {
            length = (int)fread(buffer, 1, BUFFER_SIZE, stdin);
            position = 0;
            if (length == 0)
            {
                return 0;
            }
        }
        return buffer[position++];
    }

public:
    template <class T>
    bool read(T &value)
    {
        char c = getChar();
        while (c && c <= ' ')
        {
            c = getChar();
        }
        if (!c)
        {
            return false;
        }

        value = 0;
        do
        {
            value = value * 10 + (c - '0');
            c = getChar();
        } while (c > ' ');
        return true;
    }
};

struct HLPP
{
    struct Edge
    {
        int to;
        int rev;
        i64 cap;
    };

    int n;
    int source = -1;
    int sink = -1;
    int highestActive = -1;
    int relabelWork = 0;

    vector<vector<Edge>> graph;
    vector<i64> excess;
    vector<int> height;
    vector<int> current;

    // activeHead[h] 是高度 h 的活跃点链表表头。
    vector<int> activeHead;
    vector<int> activeNext;
    vector<char> active;

    // gapHead[h] 是高度 h 的全部点链表表头。
    vector<int> gapHead;
    vector<int> gapPrevious;
    vector<int> gapNext;

    explicit HLPP(int vertexCount)
        : n(vertexCount), graph(n), excess(n), height(n), current(n),
          activeHead(n, -1), activeNext(n, -1), active(n), gapHead(n, -1),
          gapPrevious(n, -1), gapNext(n, -1)
    {
    }

    void addEdge(int from, int to, i64 cap)
    {
        int fromId = (int)graph[from].size();
        int toId = (int)graph[to].size() + (from == to);
        graph[from].push_back({to, toId, cap});
        graph[to].push_back({from, fromId, 0});
    }

    void insertGapVertex(int u)
    {
        int h = height[u];
        gapPrevious[u] = -1;
        gapNext[u] = gapHead[h];
        if (gapHead[h] != -1)
        {
            gapPrevious[gapHead[h]] = u;
        }
        gapHead[h] = u;
    }

    void eraseGapVertex(int u)
    {
        int h = height[u];
        if (gapPrevious[u] == -1)
        {
            gapHead[h] = gapNext[u];
        }
        else
        {
            gapNext[gapPrevious[u]] = gapNext[u];
        }
        if (gapNext[u] != -1)
        {
            gapPrevious[gapNext[u]] = gapPrevious[u];
        }
        gapPrevious[u] = gapNext[u] = -1;
    }

    void activate(int u)
    {
        if (u == source || u == sink || excess[u] == 0 || height[u] >= n || active[u])
        {
            return;
        }
        active[u] = true;
        activeNext[u] = activeHead[height[u]];
        activeHead[height[u]] = u;
        highestActive = max(highestActive, height[u]);
    }

    void globalRelabel()
    {
        fill(height.begin(), height.end(), n);
        fill(current.begin(), current.end(), 0);
        fill(activeHead.begin(), activeHead.end(), -1);
        fill(activeNext.begin(), activeNext.end(), -1);
        fill(active.begin(), active.end(), false);
        fill(gapHead.begin(), gapHead.end(), -1);
        fill(gapPrevious.begin(), gapPrevious.end(), -1);
        fill(gapNext.begin(), gapNext.end(), -1);
        highestActive = -1;

        queue<int> q;
        height[sink] = 0;
        q.push(sink);
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            for (const auto &edge : graph[u])
            {
                int v = edge.to;
                if (v != source && height[v] == n && graph[v][edge.rev].cap > 0)
                {
                    height[v] = height[u] + 1;
                    q.push(v);
                }
            }
        }

        for (int u = 0; u < n; u++)
        {
            if (height[u] < n)
            {
                insertGapVertex(u);
            }
        }
        for (int u = 0; u < n; u++)
        {
            activate(u);
        }
        relabelWork = 0;
    }

    void applyGap(int emptyHeight)
    {
        for (int h = emptyHeight + 1; h < n; h++)
        {
            int u = gapHead[h];
            gapHead[h] = -1;
            while (u != -1)
            {
                int next = gapNext[u];
                height[u] = n;
                gapPrevious[u] = gapNext[u] = -1;
                active[u] = false;
                activeNext[u] = -1;
                u = next;
            }
            activeHead[h] = -1;
        }
        highestActive = min(highestActive, emptyHeight - 1);
    }

    void relabel(int u)
    {
        int oldHeight = height[u];
        int newHeight = n;
        for (const auto &edge : graph[u])
        {
            if (edge.cap > 0)
            {
                newHeight = min(newHeight, height[edge.to] + 1);
            }
        }

        eraseGapVertex(u);
        bool createsGap = gapHead[oldHeight] == -1;
        if (createsGap)
        {
            applyGap(oldHeight);
        }

        height[u] = createsGap ? n : newHeight;
        current[u] = 0;
        if (height[u] < n)
        {
            insertGapVertex(u);
        }
        relabelWork++;
    }

    void push(int u, Edge &edge)
    {
        i64 amount = min(excess[u], edge.cap);
        edge.cap -= amount;
        graph[edge.to][edge.rev].cap += amount;
        excess[u] -= amount;
        excess[edge.to] += amount;
        activate(edge.to);
    }

    void discharge(int u)
    {
        while (excess[u] > 0 && height[u] < n)
        {
            if (current[u] == (int)graph[u].size())
            {
                relabel(u);
                continue;
            }

            Edge &edge = graph[u][current[u]];
            if (edge.cap == 0 || height[u] != height[edge.to] + 1)
            {
                current[u]++;
            }
            else
            {
                push(u, edge);
            }
        }
    }

    i64 maxFlow(int s, int t)
    {
        if (s == t)
        {
            return 0;
        }

        source = s;
        sink = t;
        fill(excess.begin(), excess.end(), 0);
        globalRelabel();

        // 建立初始预流：源点的全部出边一次性推满。
        for (auto &edge : graph[source])
        {
            if (edge.to == source || edge.cap == 0)
            {
                continue;
            }
            i64 amount = edge.cap;
            edge.cap = 0;
            graph[edge.to][edge.rev].cap += amount;
            excess[source] -= amount;
            excess[edge.to] += amount;
            activate(edge.to);
        }

        while (highestActive >= 0)
        {
            if (activeHead[highestActive] == -1)
            {
                highestActive--;
                continue;
            }

            int u = activeHead[highestActive];
            activeHead[highestActive] = activeNext[u];
            activeNext[u] = -1;
            active[u] = false;
            discharge(u);
            activate(u);

            if (relabelWork >= 4 * max(1, n))
            {
                globalRelabel();
            }
        }
        return excess[sink];
    }
};

int main()
{
    FastInput input;
    int n, m, source, sink;
    input.read(n);
    input.read(m);
    input.read(source);
    input.read(sink);
    source--;
    sink--;

    HLPP flow(n);
    for (int i = 0; i < m; i++)
    {
        int from, to;
        i64 cap;
        input.read(from);
        input.read(to);
        input.read(cap);
        flow.addEdge(from - 1, to - 1, cap);
    }

    printf("%lld\n", flow.maxFlow(source, sink));
    return 0;
}
