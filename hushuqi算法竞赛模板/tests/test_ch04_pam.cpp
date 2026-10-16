#include "../include/full/4_4_001.hpp"

signed main()
{
    // 无参数；核对五种回文和出现次数上传，成功返回 0。
    PAM<26, 'a'> p;
    vector<int> id;
    for (char c : string("ababa"))
    {
        id.push_back(p.add(c));
    }
    p.count();
    p.count();
    assert((int)p.t.size() - 2 == 5);
    assert(id == vector<int>({2, 3, 4, 5, 6}));
    assert(p.t[id[0]].cnt == 3);
    assert(p.t[id[1]].cnt == 2);
    assert(p.t[id[2]].cnt == 2);
    assert(p.t[id[3]].cnt == 1);
    assert(p.t[id[4]].cnt == 1);
    cout << "OK\n";
}
