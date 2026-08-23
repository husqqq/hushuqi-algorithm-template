#include "../include/full/4_1_011.hpp"

vector<bool> bruteWildcard(const string &s, const string &t, char any = '*')
{
    if (t.empty()) return vector<bool>(s.size() + 1, true);
    if (t.size() > s.size()) return {};
    vector<bool> ans(s.size() - t.size() + 1, true);
    for (int i = 0; i + (int)t.size() <= (int)s.size(); i++)
    {
        for (int j = 0; j < (int)t.size(); j++)
        {
            if (s[i + j] != any && t[j] != any && s[i + j] != t[j])
            {
                ans[i] = false;
            }
        }
    }
    return ans;
}

signed main()
{
    assert(wildcard("abc", "a*c")[0]);
    assert(wildcard("a*c", "abc")[0]);
    assert(wildcard("a*c", "a*c")[0]);
    assert(!wildcard("abc", "a*d")[0]);
    assert(wildcard("abc", "") == vector<bool>(4, true));
    assert(wildcard("a", "aa").empty());
    mt19937 rng(712367);
    const string alphabet = "abc*";
    for (int tc = 0; tc < 3000; tc++)
    {
        int n = rng() % 40;
        int m = rng() % 45;
        string s(n, 'a'), t(m, 'a');
        for (char &c : s) c = alphabet[rng() % alphabet.size()];
        for (char &c : t) c = alphabet[rng() % alphabet.size()];
        assert(wildcard(s, t) == bruteWildcard(s, t));
    }
    int n = 524288;
    string s(n, 'z'), t(n, 'a');
    assert(!wildcard(s, t)[0]);
    cout << "OK\n";
}
