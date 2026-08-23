#include "../include/full/4_1_007.hpp"

vector<Run> brute(const string &s)
{
    int n = s.size();
    vector<Run> ans;
    for (int l = 0; l < n; l++)
    {
        for (int r = l + 1; r <= n; r++)
        {
            int p = 0;
            for (int q = 1; 2 * q <= r - l && !p; q++)
            {
                bool ok = true;
                for (int i = l + q; i < r; i++)
                {
                    ok &= s[i] == s[i - q];
                }
                if (ok)
                {
                    p = q;
                }
            }
            if (!p)
            {
                continue;
            }
            bool left = l && s[l - 1] == s[l - 1 + p];
            bool right = r < n && s[r] == s[r - p];
            if (!left && !right)
            {
                ans.push_back({p, l, r});
            }
        }
    }
    sort(ans.begin(), ans.end());
    return ans;
}

signed main()
{
    for (int n = 0; n <= 13; n++)
    {
        for (int mask = 0; mask < (1LL << n); mask++)
        {
            string s(n, 'a');
            for (int i = 0; i < n; i++)
            {
                s[i] += mask >> i & 1;
            }
            if (runs(s) != brute(s))
            {
                cout << "WA " << s << '\n';
                return 0;
            }
        }
    }
    int pw = 1;
    for (int n = 0; n <= 9; n++, pw *= 3)
    {
        for (int mask = 0; mask < pw; mask++)
        {
            int x = mask;
            string s(n, 'a');
            for (int i = 0; i < n; i++, x /= 3)
            {
                s[i] += x % 3;
            }
            if (runs(s) != brute(s))
            {
                cout << "WA " << s << '\n';
                return 0;
            }
        }
    }
    mt19937 rng(0);
    for (int t = 0; t < 20000; t++)
    {
        int n = rng() % 40;
        string s(n, 'a');
        for (char &c : s)
        {
            c += rng() % 4;
        }
        if (runs(s) != brute(s))
        {
            cout << "WA " << s << '\n';
            return 0;
        }
    }
    cout << "OK\n";
}
