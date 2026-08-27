#include <bits/stdc++.h>
using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    cin >> s;
    vector<int> pi(s.size());
    for (int i = 1; i < (int)s.size(); i++)
    {
        int j = pi[i - 1];
        while (j && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }
    for (int i = 0; i < (int)pi.size(); i++) cout << pi[i] << " \n"[i + 1 == (int)pi.size()];
}
