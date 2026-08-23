#include "../include/full/4_3_002.hpp"

signed main()
{
    string s = "banana";
    vector<int> a;
    for (unsigned char c : s)
    {
        a.push_back(c);
    }
    assert(saDouble(a) == vector<int>({5, 3, 1, 0, 4, 2}));
    cout << "OK\n";
}
