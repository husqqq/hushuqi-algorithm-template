#include "../include/full/4_2_002.hpp"

signed main()
{
    AC<26> ac;
    int x = ac.add("aba"), y = ac.add("ba");
    ac.build();
    assert(ac.nodes().size() == 6 && x != y);
    cout << "OK\n";
}
