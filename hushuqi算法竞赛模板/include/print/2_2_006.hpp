#include <bits/stdc++.h>
using namespace std;
#define int long long

#include "support/topic_common.hpp"

template <class T> struct FracCascade
{
    struct Item
    {
        // x 是当前增强表中的键值。
        T x;
        // own 是本层原表的下界，down 是下一层增强表的下界。
        int own, down;
    };

    // a 保存各层原始升序表。
    vector<vector<T>> a;
    // b 保存各层采样合并后的增强表及桥指针。
    vector<vector<Item>> b;

    FracCascade(vector<vector<T>> a) : a(move(a)), b(this->a.size())
    {
        // a 的每一层都必须升序；据此从后向前建立增强表。
        for (auto &v : this->a)
        {
            assert(is_sorted(v.begin(), v.end()));
        }
        for (int i = (int)(this->a.size()) - 1; i >= 0; i--)
        {
            vector<T> v = this->a[i];
            if (i + 1 < (int)b.size())
            {
                for (int j = 1; j < (int)b[i + 1].size(); j += 2)
                {
                    v.push_back(b[i + 1][j].x);
                }
            }
            sort(v.begin(), v.end());
            v.erase(unique(v.begin(), v.end()), v.end());
            for (T x : v)
            {
                int p = lower_bound(this->a[i].begin(), this->a[i].end(), x) - this->a[i].begin();
                int q = i + 1 == (int)b.size() ? 0
                                               : lower_bound(b[i + 1].begin(),
                                                             b[i + 1].end(),
                                                             x,
                                                             [](auto z, T y)
                                                             {
                                                                 return z.x < y;
                                                             }) -
                                                     b[i + 1].begin();
                b[i].push_back({x, p, q});
            }
        }
    }

    vector<int> lowerBound(T x) const
    {
        // 参数 x 表示 要在每一层查询 lower_bound 的值；返回每一层有序表中第一个不小于 x 的下标。
        vector<int> ans(a.size());
        int p = b.empty() ? 0
                          : lower_bound(b[0].begin(),
                                        b[0].end(),
                                        x,
                                        [](auto z, T y)
                                        {
                                            return z.x < y;
                                        }) -
                                b[0].begin();
        for (int i = 0; i < (int)a.size(); i++)
        {
            if (p == (int)b[i].size())
            {
                ans[i] = a[i].size();
            }
            else
            {
                ans[i] = b[i][p].own;
            }
            if (i + 1 == (int)a.size())
            {
                break;
            }
            p = p == (int)b[i].size() ? b[i + 1].size() : b[i][p].down;
            if (p && b[i + 1][p - 1].x >= x)
            {
                p--;
            }
        }
        return ans;
    }
};
