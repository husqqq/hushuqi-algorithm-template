#include <bits/stdc++.h>
using namespace std;
namespace hash_topic {
#include "../include/full/4_1_001.hpp"
}
#undef int
namespace rotation_topic {
#include "../include/full/4_1_005.hpp"
}
#undef int
namespace sunday_topic {
#include "../include/full/4_1_009.hpp"
}
#undef int
namespace subseq_topic {
#include "../include/full/4_2_004.hpp"
}
#undef int
namespace exsam_topic {
#include "../include/full/4_3_005.hpp"
}
#undef int
namespace suffix_tree_topic {
#include "../include/full/4_3_008.hpp"
}
#undef int
namespace pam_series_topic {
#include "../include/full/4_4_003.hpp"
}
#undef int
namespace sqam_topic {
#include "../include/full/4_4_004.hpp"
}
#undef int
namespace common_topic {
#include "../include/full/4_5_004.hpp"
}
#undef int
namespace lcs_topic {
#include "../include/full/4_5_001.hpp"
}
#undef int

signed main()
{
    hash_topic::StrHash h("abracadabra");
    assert(h.get(0, 3) == h.get(7, 10));
    assert(h.getrev(0, 4) == hash_topic::StrHash("arba").get(0, 4));
    assert(h.concat(h.get(0, 4), 4, 7) == hash_topic::StrHash("abracad").get(0, 7));
    assert(h.concatrev(h.get(0, 4), 4, 7) == hash_topic::StrHash("abradac").get(0, 7));
    assert((hash_topic::StrHash::match("abracadabra", "abra") == vector<long long>{0, 7}));
    hash_topic::DynStrHash dynamic;
    for (char c : string("abracadabra"))
    {
        dynamic.push_back(c);
    }
    assert(dynamic.size() == 11 && dynamic.get(0, 3) == h.get(0, 3));
    assert(dynamic.concat(dynamic.get(0, 4), 4, 7) == h.get(0, 7));
    mt19937_64 rng(712367);
    constexpr unsigned long long mod = hash_topic::HashValue::mod;
    for (int tc = 0; tc < 10000; tc++)
    {
        unsigned long long x = rng() % mod, y = rng() % mod;
        hash_topic::HashValue a(x), b(y);
        assert((a + b).x == (x + y) % mod);
        assert((a - b).x == (x + mod - y) % mod);
        assert((a * b).x == (unsigned long long)((__uint128_t)x * y % mod));
        unsigned long long z = rng();
        assert(hash_topic::HashValue(z).x == z % mod);
    }
    for (int tc = 0; tc < 2000; tc++)
    {
        int n = rng() % 80, m = rng() % 30;
        string s(n, 'a'), t(m, 'a');
        for (char &c : s) c = 'a' + rng() % 4;
        for (char &c : t) c = 'a' + rng() % 4;
        vector<long long> expected;
        for (int i = 0; i + m <= n; i++)
        {
            if (s.substr(i, m) == t)
            {
                expected.push_back(i);
            }
        }
        assert(hash_topic::StrHash::match(s, t) == expected);
        hash_topic::StrHash hs(s);
        for (int q = 0; q < 20; q++)
        {
            int l = n ? rng() % (n + 1) : 0;
            int r = l + (n == l ? 0 : rng() % (n - l + 1));
            string part = s.substr(l, r - l);
            string reversed = part;
            reverse(reversed.begin(), reversed.end());
            assert(hs.get(l, r) == hash_topic::StrHash(part).get(0, part.size()));
            assert(hs.getrev(l, r) == hash_topic::StrHash(reversed).get(0, reversed.size()));
        }
    }
    assert(rotation_topic::minRepr(vector<int>{2, 1, 2, 1}) == 1);
    assert((rotation_topic::minRotation(vector<int>{2, 1, 2, 1}) == vector<int>{1, 2, 1, 2}));
    assert((sunday_topic::sunday("abracadabra", "cad") == vector<long long>{4}));

    subseq_topic::SubseqAM<3> am({0, 1, 0, 2, 1});
    assert(am.contains({0, 2, 1}) && !am.contains({2, 0, 0}));
    exsam_topic::ExSAM<3, 'a'> ex;
    ex.insert("ababa");
    ex.insert("aba");
    ex.build();
    assert(ex.count("aba") == 3 && ex.count("bab") == 1);
    assert(ex.distinct() == 9);

    suffix_tree_topic::SuffixTree st("banana$");
    assert(st.getText() == "banana$" && st.getNodes().size() >= 8);

    pam_series_topic::PamSeries<3, 'a'> pam;
    for (char c : string("abacaba")) pam.add(c);
    int series = 0;
    pam.eachSeries(pam.last, [&](int) { ++series; });
    assert(series >= 1);

    sqam_topic::SqAM<3> sq;
    for (int x : {0, 1, 0, 2, 1}) sq.add(x);
    assert(sq.size() == 5 && sq.contains({0, 2, 1}) && !sq.contains({2, 0, 0}));

    assert(common_topic::multiSubstr({"banana", "ananas", "nana"}) == 4);
    assert(common_topic::multiSubseq({"abcde", "ace", "bce"}) == 2);
    auto lcs = lcs_topic::lcs(vector<int>{1, 2, 3, 4}, vector<int>{2, 4, 3, 4});
    assert(lcs.size() == 3 && lcs_topic::lcsBitset("abracadabra", "avadakedavra") == 7);
    auto scs = lcs_topic::scs("abac", "cab");
    assert(scs.size() == 5);
    cout << "OK\n";
}
