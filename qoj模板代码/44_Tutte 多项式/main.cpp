// QOJ candidate template extracted from the verified standalone solver.
// Keep this file outside the formal template until it is approved.

#include <bits/stdc++.h>
using namespace std;

constexpr int MOD = 998244353;

int addMod(int a, int b)
{
    int result = a + b;
    return result >= MOD ? result - MOD : result;
}

int subtractMod(int a, int b)
{
    int result = a - b;
    return result < 0 ? result + MOD : result;
}

int powerMod(long long base, int exponent)
{
    long long result = 1;
    while (exponent)
    {
        if (exponent & 1) result = result * base % MOD;
        base = base * base % MOD;
        exponent >>= 1;
    }
    return result;
}

void rankedTransform(vector<int> &a, int variables, bool inverse)
{
    int count = 1 << variables;
    int width = variables + 1;
    for (int bit = 0; bit < variables; bit++)
    {
        for (int mask = 0; mask < count; mask++)
        {
            if (!(mask >> bit & 1)) continue;
            int *to = a.data() + (size_t)mask * width;
            int *from = a.data() + (size_t)(mask ^ (1 << bit)) * width;
            if (inverse)
            {
                for (int degree = 0; degree <= variables; degree++)
                {
                    to[degree] = subtractMod(to[degree], from[degree]);
                }
            }
            else
            {
                for (int degree = 0; degree <= variables; degree++)
                {
                    to[degree] = addMod(to[degree], from[degree]);
                }
            }
        }
    }
}

vector<int> setExponential(const vector<int> &input, int variables,
                           const array<int, 22> &inverse)
{
    int count = 1 << variables;
    int width = variables + 1;
    vector<int> ranked((size_t)count * width);
    for (int mask = 0; mask < count; mask++)
    {
        ranked[(size_t)mask * width + popcount((unsigned)mask)] = input[mask];
    }
    rankedTransform(ranked, variables, false);

    array<int, 22> logarithm{};
    for (int mask = 0; mask < count; mask++)
    {
        int *value = ranked.data() + (size_t)mask * width;
        copy(value, value + width, logarithm.begin());
        value[0] = 1;
        for (int degree = 1; degree <= variables; degree++)
        {
            long long sum = 0;
            for (int part = 1; part <= degree; part++)
            {
                sum += (long long)part * logarithm[part] % MOD * value[degree - part] % MOD;
                if (sum >= (1LL << 62)) sum %= MOD;
            }
            value[degree] = sum % MOD * inverse[degree] % MOD;
        }
    }

    rankedTransform(ranked, variables, true);
    vector<int> result(count);
    for (int mask = 0; mask < count; mask++)
    {
        result[mask] = ranked[(size_t)mask * width + popcount((unsigned)mask)];
    }
    return result;
}

inline void solveTuttePolynomial(istream &cin, ostream &cout)
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<unsigned> adjacency(n);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int edge;
            cin >> edge;
            adjacency[i] |= (unsigned)edge << j;
        }
    }
    int x, y;
    cin >> x >> y;
    int xMinusOne = subtractMod(x, 1);

    array<int, 22> inverse{};
    for (int i = 1; i <= n; i++) inverse[i] = powerMod(i, MOD - 2);
    array<int, 22> geometric{};
    for (int i = 1; i <= n; i++)
    {
        geometric[i] = addMod((long long)geometric[i - 1] * y % MOD, 1);
    }

    int count = 1 << n;
    vector<int> connected(count), temporary(count);
    for (int vertex = 0; vertex < n; vertex++)
    {
        int previousCount = 1 << vertex;
        for (int mask = 0; mask < previousCount; mask++)
        {
            int incident = popcount(adjacency[vertex] & (unsigned)mask);
            temporary[mask] = (long long)connected[mask] * geometric[incident] % MOD;
        }
        vector<int> added(temporary.begin(), temporary.begin() + previousCount);
        added = setExponential(added, vertex, inverse);
        for (int mask = 0; mask < previousCount; mask++)
        {
            connected[previousCount | mask] = added[mask];
        }
    }

    vector<char> visited(n), containsComponentRoot(count);
    for (int start = 0; start < n; start++)
    {
        if (visited[start]) continue;
        queue<int> queue;
        queue.push(start);
        visited[start] = true;
        while (!queue.empty())
        {
            int u = queue.front();
            queue.pop();
            for (int v = 0; v < n; v++)
            {
                if ((adjacency[u] >> v & 1) && !visited[v])
                {
                    visited[v] = true;
                    queue.push(v);
                }
            }
        }
        for (int mask = 0; mask < count; mask++)
        {
            if (mask >> start & 1) containsComponentRoot[mask] = true;
        }
    }
    for (int mask = 1; mask < count; mask++)
    {
        if (!containsComponentRoot[mask])
        {
            connected[mask] = (long long)connected[mask] * xMinusOne % MOD;
        }
    }
    cout << setExponential(connected, n, inverse).back() << '\n';
}
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solveTuttePolynomial(cin, cout);
}
