#include "../include/full/11_3_013.hpp"

signed main()
{
    mt19937_64 rng(0);
    unsigned long long agg = 0;
    for (int tc = 0; tc < 300; tc++)
    {
        int n = rng() % 35 + 1;
        set<pair<int, int>> s;
        while ((int)s.size() < n)
        {
            s.insert({(int)(rng() % 31) - 15, (int)(rng() % 31) - 15});
        }
        vector<Point<long long>> p;
        for (auto [x, y] : s)
        {
            p.push_back({x, y});
        }
        auto got = convexLayers(p);
        vector<int> a(n), b(n), alive(n, 1);
        for (int k = 0; k < (int)got.size(); k++)
        {
            for (int x : got[k])
            {
                assert(a[x] == 0);
                a[x] = k + 1;
            }
        }
        int left = n, layer = 0;
        while (left)
        {
            layer++;
            vector<int> id;
            for (int i = 0; i < n; i++)
            {
                if (alive[i])
                {
                    id.push_back(i);
                }
            }
            vector<int> h;
            sort(id.begin(), id.end(), [&](int x, int y) { return p[x] < p[y]; });
            if (id.size() <= 2)
            {
                h = id;
            }
            else
            {
                for (int z = 0; z < 2; z++)
                {
                    int base = h.size();
                    for (int x : id)
                    {
                        while ((int)h.size() >= base + 2 &&
                               cross(p[h.back()] - p[h[h.size() - 2]], p[x] - p[h.back()]) < 0)
                        {
                            h.pop_back();
                        }
                        h.push_back(x);
                    }
                    h.pop_back();
                    reverse(id.begin(), id.end());
                }
            }
            sort(h.begin(), h.end());
            h.erase(unique(h.begin(), h.end()), h.end());
            for (int x : h)
            {
                alive[x] = 0;
                b[x] = layer;
                left--;
            }
        }
        assert(a == b);
        for (int x : a)
        {
            agg = agg * 1000000007ULL + x;
        }
    }
    for (int n = 1; n <= 20; n++)
    {
        vector<Point<long long>> p;
        for (int i = 0; i < n; i++)
        {
            p.push_back({i, 2 * i + 1});
        }
        auto a = convexLayers(p);
        assert(a.size() == 1 && (int)a[0].size() == n);
        sort(a[0].begin(), a[0].end());
        for (int i = 0; i < n; i++)
        {
            assert(a[0][i] == i);
        }
    }
    cout << agg << '\n';
    return 0;
}
