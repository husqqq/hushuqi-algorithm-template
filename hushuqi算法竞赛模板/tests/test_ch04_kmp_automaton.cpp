#include "../include/full/4_1_002.hpp"

signed main()
{
    KMPAutomaton<26, 'a'> a("aba");
    int state = 0;
    vector<int> hit;
    string s = "ababa";
    for (int i = 0; i < (int)s.size(); i++)
    {
        state = a.next(state, s[i]);
        if (a.matched(state))
        {
            hit.push_back(i - (int)a.pattern.size() + 1);
        }
    }
    assert((hit == vector<int>{0, 2}));
    KMPAutomaton<2, '0'> empty("");
    assert(empty.matched(0));
    assert(empty.next(0, '1') == 0);
    cout << "OK\n";
}
