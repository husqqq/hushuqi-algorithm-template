#include "../include/full/3_4_003.hpp"

struct Join
{
    string operator()(const string &a, const string &b) const
    {
        return a + b;
    }
};

signed main()
{
    DST<string, Join> dst({"a", "bc", "d", "ef", "g"});
    assert(dst.query(0, 5) == "abcdefg");
    assert(dst.query(1, 4) == "bcdef");
    assert(dst.query(2, 3) == "d");
    cout << "OK\n";
}
