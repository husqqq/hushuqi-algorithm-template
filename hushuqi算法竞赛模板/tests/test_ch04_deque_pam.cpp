#include <bits/stdc++.h>
#define private public
#include "../include/full/4_4_002.hpp"
#undef private
using namespace std;
vector<array<int, 2>> ops;
int checks = 0, seed = 0; // 当前核对状态数、可复现种子。
void need(bool ok)
{
    // ok 是检查结果；失败时保存操作序列并非零退出，无返回值。
    if (ok)
    {
        return;
    }
    ofstream out(filesystem::temp_directory_path() / "template-deque-pam-failure.txt");
    for (auto [op, c] : ops)
    {
        out << op << ' ' << c << '\n';
    }
    cerr << "failure at check " << checks << "\n";
    cout << "WA " << seed << "\n";
    exit(1);
}
template <int A, char S> void check(const DequePAM<A, S> &p, const vector<int> &s)
{
    // p 是被测对象，s 是独立字符编号串；枚举回文核对查询、表面、计数、链接和回收池，无返回值。
    ++checks;
    int n = s.size();
    vector<vector<bool>> pal(n, vector<bool>(n));
    vector<int> pre(n), suf(n);
    map<vector<int>, int> ids;
    function<void(int, vector<int>)> visit = [&](int v, vector<int> val)
    {
        if (v >= 2)
        {
            need(ids.emplace(val, v).second);
        }
        for (int c = 0; c < A; c++)
        {
            if (int w = p.t[v].ch[c])
            {
                vector<int> next = {c};
                if (v != 0)
                {
                    next.insert(next.end(), val.begin(), val.end());
                    next.push_back(c);
                }
                visit(w, next);
            }
        }
    };
    visit(0, {});
    visit(1, {});
    set<vector<int>> all;
    vector<int> cnt(p.t.size()), sons(p.t.size()), dp(n, 1), ds(n, 1);
    for (int len = 1; len <= n; len++)
    {
        for (int l = 0; l + len <= n; l++)
        {
            int r = l + len - 1;
            if (s[l] == s[r] && (len <= 2 || pal[l + 1][r - 1]))
            {
                pal[l][r] = true;
                pre[l] = max(pre[l], len);
                suf[r] = max(suf[r], len);
                all.insert(vector<int>(s.begin() + l, s.begin() + r + 1));
            }
        }
    }
    need(p.size() == n && p.distinct() == (int)all.size() && ids.size() == all.size());
    need(p.longPrefix() == (n ? pre[0] : 0) && p.longSuffix() == (n ? suf[n - 1] : 0));
    for (auto &v : all)
    {
        need(ids.count(v));
    }
    for (auto &[val, v] : ids)
    {
        need(p.t[v].len == (int)val.size());
        int w = 1;
        for (int len = 1; len < (int)val.size(); len++)
        {
            if (equal(val.begin(), val.begin() + len, val.end() - len))
            {
                w = ids.at(vector<int>(val.end() - len, val.end()));
            }
        }
        need(p.t[v].link == w);
        sons[w]++;
    }
    for (int r = 0; r < n; r++)
    {
        cnt[ids.at(vector<int>(s.begin() + r - suf[r] + 1, s.begin() + r + 1))]++;
        for (int l = 0; l <= r; l++)
        {
            if (pal[l][r] && r - l + 1 == pre[l] && r - l + 1 == suf[r])
            {
                int v = ids.at(vector<int>(s.begin() + l, s.begin() + r + 1));
                dp[l] = v;
                ds[r] = v;
            }
        }
    }
    set<int> free(p.spare.begin(), p.spare.end());
    need(free.size() == p.spare.size());
    for (auto &[val, v] : ids)
    {
        need(!free.count(v) && p.t[v].cnt == cnt[v] && p.t[v].sons == sons[v]);
    }
    need(ids.size() + free.size() + 2 == p.t.size());
    for (int i = 0; i < n; i++)
    {
        need(p.d[i][0] == s[i] && p.d[i][1] == dp[i] && p.d[i][2] == ds[i]);
    }
}
template <int A, char S> void apply(DequePAM<A, S> &p, vector<int> &s, int op, int c)
{
    // p、s 是等价状态；op 是四种合法操作，c 是插入字符编号；执行后核对，无返回值。
    ops.push_back({op, c});
    if (op == 0)
    {
        p.pushFront((char)((unsigned char)S + c));
        s.insert(s.begin(), c);
    }
    if (op == 1)
    {
        p.pushBack((char)((unsigned char)S + c));
        s.push_back(c);
    }
    if (op == 2)
    {
        p.popFront();
        s.erase(s.begin());
    }
    if (op == 3)
    {
        p.popBack();
        s.pop_back();
    }
    check(p, s);
}
void exhaust(DequePAM<2, 'a'> p, vector<int> s, int depth)
{
    // p、s 是当前状态，depth 是剩余操作数；递归枚举并核对复制、移动，无返回值。
    check(p, s);
    if (!depth)
    {
        return;
    }
    int base = ops.size();
    for (int op = 0; op < 4; op++)
    {
        for (int c = 0; c < (op < 2 ? 2 : 1); c++)
        {
            if (op >= 2 && s.empty())
            {
                continue;
            }
            auto q = p;
            auto z = s;
            apply(q, z, op, c);
            exhaust(move(q), move(z), depth - 1);
            ops.resize(base);
        }
    }
}
template <int A, char S> void randomTest(int sd)
{
    // sd 是独立种子；连续增删、复制、移动、删空并核对，无返回值。
    mt19937_64 rng(sd);
    DequePAM<A, S> p;
    vector<int> s;
    ops.clear();
    for (int i = 0; i < 600; i++)
    {
        int op = rng() % 4;
        if (s.empty())
        {
            op %= 2;
        }
        if (s.size() >= 40)
        {
            op = 2 + op % 2;
        }
        apply(p, s, op, rng() % A);
        if (i % 31 == 0)
        {
            auto q = p;
            check(q, s);
            auto z = move(q);
            check(z, s);
        }
    }
    while (!s.empty())
    {
        apply(p, s, rng() % 2 + 2, 0);
    }
}

void longTest(bool alt)
{
    // alt 选择交替串或同字符串；核对长失败链、连续删空及回收复用，无返回值。
    DequePAM<3, 'a'> p;
    int n = 200000;
    for (int i = 0; i < n; i++)
    {
        p.pushBack(alt && i % 2 ? 'b' : 'a');
    }
    int longest = alt ? n - 1 : n;
    need(p.distinct() == n && p.longPrefix() == longest && p.longSuffix() == longest);
    for (int i = 0; i < 2000; i++)
    {
        if (i % 2)
        {
            p.pushFront('c');
        }
        else
        {
            p.pushBack('c');
        }
        need(p.distinct() == n + 1 && p.size() == n + 1);
        need(p.longPrefix() == (i % 2 ? 1 : longest) && p.longSuffix() == (i % 2 ? longest : 1));
        if (i % 2)
        {
            p.popFront();
        }
        else
        {
            p.popBack();
        }
        need(p.t.size() <= n + 3);
    }
    while (n)
    {
        if (n % 2)
        {
            p.popFront();
        }
        else
        {
            p.popBack();
        }
        n--;
        int len = alt && n % 2 == 0 && n ? n - 1 : n;
        need(p.distinct() == n && p.longPrefix() == len && p.longSuffix() == len);
    }
    check(p, {});
}

signed main()
{
    // 无参数；穷举、长链和 600 个独立种子逐状态核对，成功返回 0。
    exhaust({}, {}, 7);
    ops.clear();
    longTest(false);
    longTest(true);
    cout << "AC 0\n";
    for (seed = 1; seed <= 600; seed++)
    {
        randomTest<2, 'a'>(seed);
        randomTest<3, 'x'>(seed);
        randomTest<26, 'a'>(seed);
        randomTest<256, 0>(seed);
        randomTest<16, (char)240>(seed);
        cout << "AC " << seed << '\n';
    }
    cerr << "checks " << checks << '\n';
}
