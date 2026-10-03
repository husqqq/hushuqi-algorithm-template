#include "../include/full/3_3_002.hpp"

struct DefaultTag
{
    long long add = 0;
    void apply(const DefaultTag &t) { add += t.add; }
};

struct SearchTag
{
    long long add = 0;
    void apply(const SearchTag &t) { add += t.add; }
};

struct SearchInfo
{
    long long mx = numeric_limits<long long>::lowest();
    SearchInfo() = default;
    SearchInfo(long long x) : mx(x) {}
    static SearchInfo identity() { return SearchInfo(); }
    void apply(const SearchTag &t) { mx += t.add; }
    friend SearchInfo operator+(SearchInfo a, SearchInfo b)
    {
        return SearchInfo(max(a.mx, b.mx));
    }
};

struct DefaultInfo
{
    long long sum = 0;
    int len = 1;
    DefaultInfo() = default;
    static DefaultInfo identity()
    {
        DefaultInfo c;
        c.len = 0;
        return c;
    }
    void apply(const DefaultTag &t) { sum += t.add * len; }
    friend DefaultInfo operator+(DefaultInfo a, DefaultInfo b)
    {
        DefaultInfo c;
        c.sum = a.sum + b.sum;
        c.len = a.len + b.len;
        return c;
    }
};

signed main()
{
    LazySeg<Info, Tag> st(4);
    assert(st.query(0, 4).sum == 0 && st.query(0, 4).len == 4);
    st.apply(0, 4, Tag{3});
    assert(st.query(0, 4).sum == 12);
    const auto &view = st;
    assert(view.get(2).sum == 3);
    st.apply(1, 3, Tag{-2});
    assert(st.query(0, 4).sum == 8);
    assert(st.get(1).sum == 1 && st.get(2).sum == 1);
    assert(st.query(1, 3).sum == 2 && st.query(1, 3).len == 2);

    LazySeg<SearchInfo, SearchTag> search(vector<SearchInfo>{1, 2, 3, 4, 5});
    search.apply(0, 5, SearchTag{10});
    search.apply(1, 4, SearchTag{-7});
    assert(search.findFirst(0, 5, [](SearchInfo x) { return x.mx >= 5; }) == 0);
    assert(search.findFirst(1, 5, [](SearchInfo x) { return x.mx >= 6; }) == 2);
    assert(search.findLast(0, 5, [](SearchInfo x) { return x.mx >= 5; }) == 4);
    assert(search.findLast(0, 4, [](SearchInfo x) { return x.mx >= 5; }) == 3);

    LazySeg<Info, Tag> empty(0);
    empty.apply(0, 0, Tag{7});
    assert(empty.query(0, 0).len == 0);

    LazySeg<DefaultInfo, DefaultTag> defaults(3);
    defaults.apply(0, 3, DefaultTag{2});
    assert(defaults.query(0, 3).sum == 6);
    cout << "OK\n";
    return 0;
}
