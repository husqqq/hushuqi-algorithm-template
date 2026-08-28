// QOJ candidate template extracted from the verified standalone solver.
// Keep this file outside the formal template until it is approved.

#include <bits/stdc++.h>
using namespace std;

struct Point
{
    int x, y, z, w;
};

struct Event
{
    int y, z, w;
    bool source;
};

class Fenwick
{
    vector<int> tree;

  public:
    explicit Fenwick(int n) : tree(n + 1) {}

    void add(int position, int value)
    {
        for (; position < (int)tree.size(); position += position & -position)
        {
            tree[position] += value;
        }
    }

    int prefix(int position) const
    {
        int result = 0;
        for (; position; position -= position & -position)
        {
            result += tree[position];
        }
        return result;
    }
};

long long countThreeDimensional(vector<Event> events, int coordinateLimit)
{
    sort(events.begin(), events.end(), [](const Event &a, const Event &b)
    {
        return a.y < b.y;
    });
    vector<Event> buffer(events.size());
    Fenwick fenwick(coordinateLimit);
    long long answer = 0;
    auto solve = [&](auto &&self, int left, int right) -> void
    {
        if (right - left <= 1)
        {
            return;
        }
        int middle = (left + right) / 2;
        self(self, left, middle);
        self(self, middle, right);
        int i = left;
        for (int j = middle; j < right; j++)
        {
            while (i < middle && events[i].z < events[j].z)
            {
                if (events[i].source)
                {
                    fenwick.add(events[i].w, 1);
                }
                i++;
            }
            if (!events[j].source)
            {
                answer += fenwick.prefix(events[j].w - 1);
            }
        }
        for (int k = left; k < i; k++)
        {
            if (events[k].source)
            {
                fenwick.add(events[k].w, -1);
            }
        }
        merge(events.begin() + left, events.begin() + middle,
              events.begin() + middle, events.begin() + right,
              buffer.begin() + left, [](const Event &a, const Event &b)
        {
            return a.z < b.z;
        });
        copy(buffer.begin() + left, buffer.begin() + right, events.begin() + left);
    };
    solve(solve, 0, events.size());
    return answer;
}

long long countFourDimensional(vector<Point> points)
{
    sort(points.begin(), points.end(), [](const Point &a, const Point &b)
    {
        return a.x < b.x;
    });
    int n = points.size();
    long long answer = 0;
    auto solve = [&](auto &&self, int left, int right) -> void
    {
        if (right - left <= 1)
        {
            return;
        }
        int middle = (left + right) / 2;
        self(self, left, middle);
        self(self, middle, right);
        vector<Event> events;
        events.reserve(right - left);
        for (int i = left; i < middle; i++)
        {
            events.push_back({points[i].y, points[i].z, points[i].w, true});
        }
        for (int i = middle; i < right; i++)
        {
            events.push_back({points[i].y, points[i].z, points[i].w, false});
        }
        answer += countThreeDimensional(move(events), n);
    };
    solve(solve, 0, n);
    return answer;
}

inline void solveFourDimensionalOrder(istream &cin, ostream &cout)
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<Point> points(n);
    for (auto &[x, y, z, w] : points)
    {
        cin >> x >> y >> z >> w;
    }
    cout << countFourDimensional(move(points)) << '\n';
}
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solveFourDimensionalOrder(cin, cout);
}
