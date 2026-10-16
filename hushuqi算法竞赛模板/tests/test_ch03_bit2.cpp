#include "../include/full/3_2_011.hpp"

void check(bool ok)
{
    // ok 是独立预期与模板结果是否相同；失败不受 NDEBUG 影响。
    if (!ok) throw runtime_error("二维树状数组结果不符");
}

signed main()
{
    BIT2 b({{1, 2}, {1, 5}, {4, 2}});
    b.add(1, 2, 3);
    b.add(1, 5, 7);
    b.add(4, 2, -2);

    check(b.prefixSum(1, 9) == 0);
    check(b.prefixSum(2, 3) == 3);
    check(b.prefixSum(2, 6) == 10);
    check(b.prefixSum(5, 3) == 1);

    BIT2 rect({{0, 0}, {0, 3}, {2, 0}, {2, 3}, {1, 1}, {1, 4}, {4, 1}, {4, 4}});
    rect.addRectangle(0, 0, 2, 3, 5);
    rect.addRectangle(1, 1, 4, 4, -2);
    check(rect.pointGet(0, 0) == 5);
    check(rect.pointGet(1, 1) == 3);
    check(rect.pointGet(2, 1) == -2);
    check(rect.pointGet(1, 3) == -2);
    check(rect.pointGet(4, 4) == 0);

    int lo = numeric_limits<int>::min(), hi = numeric_limits<int>::max();
    vector<int> coords = {lo, lo + 1, -5, 0, 3, hi - 1, hi};
    BIT2 empty;
    check(empty.prefixSum(hi, hi, true) == 0);
    check(empty.pointGet(hi, hi) == 0);
    BIT2 edge({{lo, lo}, {lo, hi}, {hi, lo}, {hi, hi}});
    edge.addRectangle(lo, lo, hi, hi, 7);
    check(edge.pointGet(lo, lo) == 7);
    check(edge.pointGet(hi - 1, hi - 1) == 7);
    check(edge.pointGet(hi, lo) == 0);
    check(edge.pointGet(lo, hi) == 0);
    check(edge.pointGet(hi, hi) == 0);
    edge.prepare({{hi, hi}, {hi, hi}});
    edge.add(hi, hi, 3000000000LL);
    check(edge.prefixSum(hi, hi) == 0);
    check(edge.prefixSum(hi, hi, true) == 3000000000LL);
    edge.prepare({});
    check(edge.pointGet(hi, hi) == 0);

    for (int seed = 0; seed < 8; seed++)
    {
        mt19937_64 rng(20261016 + seed);
        vector<pair<int, int>> pts;
        for (int x : coords)
        {
            for (int y : coords)
            {
                pts.push_back({x, y});
            }
        }
        pts.push_back({hi, hi}); // 重复登记不增加权值。
        BIT2 cur(pts);
        vector<long long> val(pts.size());
        for (int t = 0; t < 150; t++)
        {
            int at = rng() % pts.size();
            long long w = (long long)(rng() % 2001) - 1000;
            cur.add(pts[at].first, pts[at].second, w);
            val[at] += w;
            int x = coords[rng() % coords.size()], y = coords[rng() % coords.size()];
            long long open = 0, closed = 0;
            for (int j = 0; j < (int)pts.size(); j++)
            {
                auto [px, py] = pts[j];
                if (px < x && py < y) open += val[j];
                if (px <= x && py <= y) closed += val[j];
            }
            const BIT2 &ref = cur;
            check(ref.prefixSum(x, y) == open);
            check(ref.prefixSum(x, y, true) == closed);
        }

        BIT2 diff(pts);
        vector<array<int, 5>> updates; // 每项为左、下、右、上边界和增量。
        for (int t = 0; t < 80; t++)
        {
            int l = rng() % 6, r = l + 1 + rng() % (6 - l);
            int d = rng() % 6, u = d + 1 + rng() % (6 - d);
            int w = (int)(rng() % 101) - 50;
            updates.push_back({coords[l], coords[d], coords[r], coords[u], w});
            diff.addRectangle(coords[l], coords[d], coords[r], coords[u], w);
            for (auto [x, y] : pts)
            {
                long long want = 0;
                for (auto [left, down, right, up, weight] : updates)
                {
                    if (left <= x && x < right && down <= y && y < up) want += weight;
                }
                check(diff.pointGet(x, y) == want);
            }
        }
    }

    cout << "OK\n";
}
