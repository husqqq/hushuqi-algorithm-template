#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int inf = 1E9;
constexpr long long INF = 4E18;
constexpr long double eps = 1E-12L;

template <class T> bool chmin(T &a, const T &b)
{
    // 若 b 更小则更新 a，返回是否更新。
    return b < a ? a = b, true : false;
}
template <class T> bool chmax(T &a, const T &b)
{
    // 若 b 更大则更新 a，返回是否更新。
    return a < b ? a = b, true : false;
}

template <class T, class C = T> class LiChao
{
    // 当前实现维护最小值。改成最大值有两种等价方式：反向所有优劣比较并把空值改为负无穷，
    // 或在 k、b 与函数值都能安全取反（不能是最小负数）时插入 {-k,-b}，再把非空结果取反。
  public:
    static_assert(is_floating_point_v<T> || (is_integral_v<T> && is_signed_v<T>));

    struct Line
    {
        // k、b 是一次函数 y=kx+b 的斜率与截距。
        T k{}, b{};

        C get(T x) const
        {
            // x 是查询横坐标；用比较类型 C 计算函数值。
            return (C)k * x + b;
        }
    };

  private:
    // xs 是全部允许查询的横坐标，tr[p] 是线段树节点 p 保存的候选直线。
    vector<T> xs;
    vector<optional<Line>> tr;

    void add(int p, int l, int r, Line f)
    {
        // p 是节点编号，[l,r) 是横坐标下标段，f 是待加入直线。
        if (!tr[p])
        {
            tr[p] = f;
            return;
        }
        int m = midpoint(l, r);
        bool lef = f.get(xs[l]) < tr[p]->get(xs[l]);
        bool mid = f.get(xs[m]) < tr[p]->get(xs[m]);
        if (mid)
        {
            swap(f, *tr[p]);
        }
        if (r - l == 1)
        {
            return;
        }
        if (lef != mid)
        {
            add(p << 1, l, m, f);
        }
        else
        {
            add(p << 1 | 1, m, r, f);
        }
    }

  public:
    LiChao(vector<T> xs) : xs(move(xs))
    {
        // xs 是所有可能查询的横坐标；排序去重并建立最小值李超树。
        // 改最大值时可在系数可安全取反时优先对直线取反再对答案取反，可保留李超树的比较不变量。
        sort(this->xs.begin(), this->xs.end());
        this->xs.erase(unique(this->xs.begin(), this->xs.end()), this->xs.end());
        assert(!this->xs.empty()); // 调试检查，可删
        tr.resize(4 * this->xs.size());
    }

    void add(Line f)
    {
        // f 是待加入直线；使其参与全部预存横坐标的最小值查询。
        // 最大值版可在 -f.k、-f.b 不溢出时调用 add({-f.k, -f.b})，并在 query 的返回值上取负。
        add(1, 0, xs.size(), f);
    }

    optional<C> query(T x) const
    {
        // x 必须在构造坐标中；返回最小函数值，尚未加入直线时返回空。
        // 最大值版若按直线取反，调用者取负即可；空结果仍保持 nullopt。
        int q = lower_bound(xs.begin(), xs.end(), x) - xs.begin();
        assert(q < (int)xs.size() && xs[q] == x); // 调试检查，可删
        optional<C> ans;
        int p = 1, l = 0, r = xs.size();
        while (true)
        {
            if (tr[p])
            {
                C y = tr[p]->get(x);
                if (!ans || y < *ans)
                {
                    ans = y;
                }
            }
            if (r - l == 1)
            {
                break;
            }
            int m = midpoint(l, r);
            if (q < m)
            {
                p <<= 1;
                r = m;
            }
            else
            {
                p = p << 1 | 1;
                l = m;
            }
        }
        return ans;
    }
};
