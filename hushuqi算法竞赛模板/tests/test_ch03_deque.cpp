#include <bits/stdc++.h>

using namespace std;

signed main()
{
    deque<long long> q;
    q.push_front(2);
    q.push_back(5);
    q.push_front(1);
    assert(q.front() == 1 && q.back() == 5 && q[1] == 2);
    q.pop_front();
    q.pop_back();
    assert(q.size() == 1 && q[0] == 2);
    cout << "OK\n";
}
