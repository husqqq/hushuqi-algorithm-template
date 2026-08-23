#include "../include/full/3_5_009.hpp"

signed main()
{
    ErasableHeap<int> hp;
    for (int x : {5, 1, 5, 3})
    {
        hp.push(x);
    }
    hp.erase(1);
    assert(hp.top() == 5 && hp.size() == 3);
    hp.erase(5);
    assert(hp.top() == 5);
    hp.pop();
    assert(hp.top() == 3 && hp.size() == 1);
    hp.pop();
    assert(hp.empty());
    cout << "OK\n";
}
