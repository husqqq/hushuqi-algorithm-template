#include "support/base.hpp"

class Freq
{
    // a 是当前数组，id 把值映射到内部编号。
    vector<int> a;
    unordered_map<int, int, CustomHash> id;
    // pos[k] 是值 k 预登记过的位置，bit[k] 是对应树状数组。
    vector<vector<int>> pos, bit;

    void add(int v, int p, int d)
    {
        // v 是值，p 是预登记位置，d 是出现次数增量。
        auto it = id.find(v);
        assert(it != id.end()); // 调试检查，可删
        int k = it->second;
        auto jt = lower_bound(pos[k].begin(), pos[k].end(), p);
        assert(jt != pos[k].end() && *jt == p); // 调试检查，可删
        int x = jt - pos[k].begin() + 1;
        for (int i = x; i < (int)bit[k].size(); i += i & -i)
        {
            bit[k][i] += d;
        }
    }

    int pre(int k, int p) const
    {
        // k 是内部值编号，p 是位置上界；返回该值在 [0,p) 的出现次数。
        int x = lower_bound(pos[k].begin(), pos[k].end(), p) - pos[k].begin();
        int ans = 0;
        for (int i = x; i > 0; i -= i & -i)
        {
            ans += bit[k][i];
        }
        return ans;
    }

  public:
    Freq(vector<int> init, const vector<pair<int, int>> &qs) : a(move(init))
    {
        // init 是初始数组，qs 是以后可能写入的全部 {位置,值}。
        vector<pair<int, int>> all = qs;
        all.reserve(all.size() + a.size());
        for (int p = 0; p < (int)a.size(); p++)
        {
            all.push_back({p, a[p]});
        }
        id.max_load_factor(.7);
        id.reserve(2 * all.size() + 1);
        for (auto [p, v] : all)
        {
            assert(0 <= p && p < (int)a.size()); // 调试检查，可删
            auto [it, ok] = id.emplace(v, id.size());
            if (ok)
            {
                pos.emplace_back();
            }
            pos[it->second].push_back(p);
        }
        bit.resize(pos.size());
        for (int k = 0; k < (int)pos.size(); k++)
        {
            auto &p = pos[k];
            sort(p.begin(), p.end());
            p.erase(unique(p.begin(), p.end()), p.end());
            bit[k].assign(p.size() + 1, 0);
        }
        for (int p = 0; p < (int)a.size(); p++)
        {
            add(a[p], p, 1);
        }
    }

    void setValue(int p, int v)
    {
        // p 是位置，v 是新值；要求 {p,v} 已在构造时预登记。
        assert(0 <= p && p < (int)a.size()); // 调试检查，可删
        auto it = id.find(v);
        assert(it != id.end() && binary_search(pos[it->second].begin(), pos[it->second].end(), p)); // 调试检查，可删
        add(a[p], p, -1);
        a[p] = v;
        add(v, p, 1);
    }

    int frequency(int l, int r, int v) const
    {
        // l、r 是半开区间端点，v 是查询值；返回 v 在 [l,r) 的出现次数。
        assert(0 <= l && l <= r && r <= (int)a.size()); // 调试检查，可删
        auto it = id.find(v);
        if (it == id.end())
        {
            return 0;
        }
        int k = it->second;
        return pre(k, r) - pre(k, l);
    }
};
