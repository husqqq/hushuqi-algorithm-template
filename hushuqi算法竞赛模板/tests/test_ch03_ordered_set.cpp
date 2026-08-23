#include "../include/supplement_pbds.hpp"

signed main()
{
    using OSet = pbds_ordered::OrdSet<int>;
    OSet tr;
    set<int> brute;
    mt19937_64 rng(0);
    for (int q = 0; q < 20'000; q++)
    {
        int op = rng() % 6;
        int x = rng() % 1001;
        if (!op)
        {
            tr.insert(x);
            brute.insert(x);
        }
        else if (op == 1)
        {
            tr.erase(x);
            brute.erase(x);
        }
        else if (op == 2)
        {
            int k = 1 + rng() % 1100;
            int got = k > (int)tr.size() ? -1 : *tr.find_by_order(k - 1);
            int want = k > (int)brute.size() ? -1 : *next(brute.begin(), k - 1);
            assert(got == want);
        }
        else if (op == 3)
        {
            assert((int)tr.order_of_key(x + 1) == (int)distance(brute.begin(), brute.upper_bound(x)));
        }
        else if (op == 4)
        {
            auto a = tr.upper_bound(x);
            int got = a == tr.begin() ? -1 : *prev(a);
            auto b = brute.upper_bound(x);
            int want = b == brute.begin() ? -1 : *prev(b);
            assert(got == want);
        }
        else
        {
            auto a = tr.lower_bound(x);
            int got = a == tr.end() ? -1 : *a;
            auto b = brute.lower_bound(x);
            int want = b == brute.end() ? -1 : *b;
            assert(got == want);
        }
    }
    cout << "OK\n";
}
