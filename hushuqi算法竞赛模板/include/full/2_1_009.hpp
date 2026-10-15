#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

struct DLX
{
    // m 是必须恰好覆盖的列数。
    int m;
    // l、r、u、d 是十字链表节点向左、右、上、下的指针。
    vector<int> l, r, u, d;
    // row、col 分别是数据节点对应的行编号和列编号。
    vector<int> row, col;
    // sz[c] 是当前列 c 中仍未删除的数据节点数。
    vector<int> sz;
    // cur 是当前递归路径选中的行编号，ans 是找到的完整答案。
    vector<int> cur, ans;

    explicit DLX(int cols, int reserveNodes = 0)
        : m(cols), l(m + 1), r(m + 1), u(m + 1), d(m + 1), row(m + 1), col(m + 1), sz(m + 1)
    {
        // cols 是列数，reserveNodes 是预估的数据节点数；建立带根节点的列链表。
        assert(m >= 0 && reserveNodes >= 0); // 调试检查，可删。
        for (int i = 0; i <= m; i++)
        {
            l[i] = i - 1;
            r[i] = i + 1;
            u[i] = d[i] = col[i] = i;
        }
        l[0] = m;
        r[m] = 0;
        if (reserveNodes > 0)
        {
            auto cap = m + 1 + reserveNodes;
            l.reserve(cap);
            r.reserve(cap);
            u.reserve(cap);
            d.reserve(cap);
            row.reserve(cap);
            col.reserve(cap);
        }
    }

    void addRow(int id, const vector<int> &columns)
    {
        // id 是返回答案时使用的行编号，columns 是该行覆盖的列编号；把一行加入十字链表。
        int head = -1;
        for (int k = 0; k < (int)columns.size(); k++)
        {
            int c = columns[k];
            // 调试检查，可删。
            assert(1 <= c && c <= m);
            // 调试检查，可删。
            assert(find(columns.begin(), columns.begin() + k, c) == columns.begin() + k);
            int x = (int)l.size();
            l.push_back(x);
            r.push_back(x);
            u.push_back(u[c]);
            d.push_back(c);
            row.push_back(id);
            col.push_back(c);
            d[u[c]] = x;
            u[c] = x;
            sz[c]++;
            if (head == -1)
            {
                head = x;
            }
            else
            {
                l[x] = l[head];
                r[x] = head;
                r[l[head]] = x;
                l[head] = x;
            }
        }
    }

    optional<vector<int>> solve()
    {
        // 无参数；返回一组覆盖全部列的行编号，无解时返回空。
        cur.clear();
        ans.clear();
        if (!dfs())
        {
            return nullopt;
        }
        return ans;
    }

  private:
    void cover(int c)
    {
        // c 是当前仍在列链表中的列编号；从结构中删除该列及与其冲突的行。
        r[l[c]] = r[c];
        l[r[c]] = l[c];
        for (int i = d[c]; i != c; i = d[i])
        {
            for (int j = r[i]; j != i; j = r[j])
            {
                d[u[j]] = d[j];
                u[d[j]] = u[j];
                sz[col[j]]--;
            }
        }
    }

    void uncover(int c)
    {
        // c 是最近一次删除的列编号；按相反顺序恢复列和冲突行。
        for (int i = u[c]; i != c; i = u[i])
        {
            for (int j = l[i]; j != i; j = l[j])
            {
                sz[col[j]]++;
                d[u[j]] = j;
                u[d[j]] = j;
            }
        }
        r[l[c]] = c;
        l[r[c]] = c;
    }

    bool dfs()
    {
        // 无参数；返回当前子问题是否有解，并在成功时把行编号写入 ans。
        if (r[0] == 0)
        {
            ans = cur;
            return true;
        }
        int c = r[0];
        for (int j = r[c]; j != 0; j = r[j])
        {
            if (sz[j] < sz[c])
            {
                c = j;
            }
        }
        if (sz[c] == 0)
        {
            return false;
        }
        cover(c);
        for (int i = d[c]; i != c; i = d[i])
        {
            cur.push_back(row[i]);
            for (int j = r[i]; j != i; j = r[j])
            {
                cover(col[j]);
            }
            bool found = dfs();
            for (int j = l[i]; j != i; j = l[j])
            {
                uncover(col[j]);
            }
            cur.pop_back();
            if (found)
            {
                uncover(c);
                return true;
            }
        }
        uncover(c);
        return false;
    }
};
