

#include <bits/stdc++.h>

using namespace std;

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;
    deque<long long> a;
    while (q--)
    {
        int type;
        cin >> type;
        if (type == 0)
        {
            long long x;
            cin >> x;
            a.push_front(x);
        }
        else if (type == 1)
        {
            long long x;
            cin >> x;
            a.push_back(x);
        }
        else if (type == 2)
        {
            a.pop_front();
        }
        else if (type == 3)
        {
            a.pop_back();
        }
        else
        {
            int i;
            cin >> i;
            cout << a[i] << '\n';
        }
    }
    return 0;
}
