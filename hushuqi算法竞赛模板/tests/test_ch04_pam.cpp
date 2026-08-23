#include "../include/full/4_4_001.hpp"

signed main()
{
    PAM<26, 'a'> p; p.build("ababa"); p.count();
    assert((int)p.t.size() - 2 == 5);
    cout << "OK\n";
}
