#include "../include/full/3_5_012.hpp"

signed main()
{
    {
        PersistQueue<long long> q(32);
        int v0 = q.emptyVer();
        int v1 = q.push(v0, 4);
        int v2 = q.push(v1, 7);
        int v3 = q.pop(v2);
        int v4 = q.push(v1, 9);
        int v5 = q.push(v3, 11);
        int v6 = q.pop(v4);

        assert(q.empty(v0) && q.size(v0) == 0);
        assert(q.front(v1) == 4 && q.front(v2) == 4);
        assert(q.front(v3) == 7 && q.front(v5) == 7);
        assert(q.front(v4) == 4 && q.front(v6) == 9);
        assert(q.size(v2) == 2 && q.size(v3) == 1 && q.size(v5) == 2);
    }

    {
        PersistQueue<int> q(20000);
        int version = q.emptyVer();
        for (int i = 0; i < 16384; i++)
        {
            version = q.push(version, i);
        }
        int cut = version;
        for (int i = 0; i < 8192; i++)
        {
            assert(q.front(cut) == i);
            cut = q.pop(cut);
        }
        assert(q.front(version) == 0);
        assert(q.front(cut) == 8192);
        int branch = q.push(cut, 1000000);
        assert(q.front(branch) == 8192 && q.size(branch) == 8193);
    }

    mt19937_64 rng(20260819);
    unsigned long long checksum = 0;
    for (int it = 0; it < 80; it++)
    {
        PersistQueue<long long> q(6000);
        vector<vector<long long>> versions(1);
        for (int step = 0; step < 5000; step++)
        {
            int base = rng() % versions.size();
            if (versions[base].empty() || rng() % 3)
            {
                long long value = (long long)(rng() % 1000000001ULL);
                vector<long long> cur = versions[base];
                cur.push_back(value);
                int version = q.push(base, value);
                assert(version == (int)versions.size());
                versions.push_back(move(cur));
            }
            else
            {
                assert(q.front(base) == versions[base].front());
                vector<long long> cur = versions[base];
                cur.erase(cur.begin());
                int version = q.pop(base);
                assert(version == (int)versions.size());
                versions.push_back(move(cur));
            }

            int probe = rng() % versions.size();
            assert(q.size(probe) == (int)versions[probe].size());
            if (!versions[probe].empty())
            {
                long long value = q.front(probe);
                assert(value == versions[probe].front());
                checksum = checksum * 1000003ULL + (unsigned long long)value;
            }
        }
    }
    cout << checksum << '\n';
    return 0;
}
