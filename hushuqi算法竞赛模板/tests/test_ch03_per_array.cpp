#include "../include/full/3_6_007.hpp"

signed main()
{
    PerArray<string> a({"a", "b", "c"});
    int v1 = a.set(0, 1, "B");
    int v2 = a.set(v1, 2, "C");
    assert(a.get(0, 1) == "b");
    assert(a.get(v1, 1) == "B" && a.get(v1, 2) == "c");
    assert(a.get(v2, 1) == "B" && a.get(v2, 2) == "C");

    PerArray<int> empty(vector<int>{});
    cout << "OK\n";
}
