#!/usr/bin/env python3
"""Generate self-contained Library Checker sources from chapter 03 headers.

The adapter bodies below only parse a problem's input, select a documented
template interface, and print that interface's result.  Algorithm code comes
from the synchronized chapter 03 full headers, which are recursively inlined
into a temporary source before the final main.cpp is refreshed.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import os
import platform
import re
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from datetime import datetime, timezone
from pathlib import Path


@dataclass(frozen=True)
class Spec:
    name: str
    title: str
    headers: tuple[str, ...]
    topic: str | None
    symbols: tuple[str, ...]
    tests: tuple[str, ...]
    adapter: str
    extra_markdown: tuple[tuple[str, str, tuple[str, ...]], ...] = ()


OFFICIAL_REPOSITORY_URL = "https://github.com/yosupo06/library-checker-problems"
OFFICIAL_COMMIT = "04c8de378bab67be926325de2871f0babb8e6451"
OFFICIAL_LICENSE = "Apache-2.0"
OFFICIAL_LICENSE_BLOB = "91554e3bd99da6761c82645d6c740ced590d17bb"

MODDOC = (
    (
        "07-数论.md",
        "7.1.007",
        ("template <long long P> struct MInt", "using Z = MInt<mod>;"),
    ),
)


SPECS = (
    Spec(
        "area_of_union_of_rectangles",
        "Area of Union of Rectangles",
        ("3_3_012.hpp",),
        "3.3.012",
        ("template <class T> struct Rectangle", "template <class T, class Acc = T> class RectUnion"),
        ("tests/test_ch03_rect_union.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<Rectangle<long long>> a(n);
    for (auto &[x1, y1, x2, y2] : a)
    {
        cin >> x1 >> y1 >> x2 >> y2;
    }
    RectUnion<long long> rect;
    cout << rect.area(a) << '\n';
    return 0;
}
''',
    ),
    Spec(
        "associative_array",
        "Associative Array",
        ("3_6_006.hpp",),
        "3.6.006",
        ("template <class K, class V> using HashMap",),
        ("tests/test_ch03_hash_map.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;
    HashMap<int, int> value;
    value.max_load_factor(.7);
    value.reserve(2 * q);
    while (q--)
    {
        int type, key;
        cin >> type >> key;
        if (type == 0)
        {
            int x;
            cin >> x;
            value[key] = x;
        }
        else
        {
            auto it = value.find(key);
            cout << (it == value.end() ? 0 : it->second) << '\n';
        }
    }
    return 0;
}
''',
    ),
    Spec(
        "deque",
        "Deque",
        (),
        None,
        (),
        ("tests/test_ch03_deque.cpp",),
        r'''
#include <bits/stdc++.h>

using namespace std;

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;
    deque<long long> a;
    while (q--)
    {
        int type;
        cin >> type;
        if (type == 0)
        {
            long long x;
            cin >> x;
            a.push_front(x);
        }
        else if (type == 1)
        {
            long long x;
            cin >> x;
            a.push_back(x);
        }
        else if (type == 2)
        {
            a.pop_front();
        }
        else if (type == 3)
        {
            a.pop_back();
        }
        else
        {
            int i;
            cin >> i;
            cout << a[i] << '\n';
        }
    }
    return 0;
}
''',
        (("01-基础与工程.md", "1.4.002", ("deque<int> q;",)),),
    ),
    Spec(
        "rectangle_sum",
        "Rectangle Sum",
        ("3_2_010.hpp",),
        "3.2.010",
        ("template <class T> struct StaticRect",),
        ("tests/test_ch03_static_rect_sum.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    StaticRect<long long> st;
    for (int i = 0; i < n; ++i)
    {
        long long x, y, w;
        cin >> x >> y >> w;
        st.add(x, y, w);
    }
    for (int i = 0; i < q; ++i)
    {
        long long l, d, r, u;
        cin >> l >> d >> r >> u;
        st.query(l, d, r, u);
    }
    for (auto x : st.work())
    {
        cout << x << '\n';
    }
    return 0;
}
''',
    ),
    Spec(
        "unionfind",
        "Unionfind",
        ("3_1_001.hpp",),
        "3.1.001",
        ("struct DSU",),
        ("tests/test_ch03_dsu.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    DSU d(n);
    while (q--)
    {
        int type, u, v;
        cin >> type >> u >> v;
        if (type == 0)
        {
            d.merge(u, v);
        }
        else
        {
            cout << d.same(u, v) << '\n';
        }
    }
    return 0;
}
''',
    ),
    Spec(
        "persistent_unionfind",
        "Persistent Unionfind",
        ("3_1_005.hpp",),
        "3.1.005",
        ("struct PerDSU",),
        ("tests/test_ch03_perdsu.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    PerDSU d(n);
    vector<int> root(q + 1);
    root[0] = d.build();
    for (int i = 0; i < q; ++i)
    {
        int type, version, u, v;
        cin >> type >> version >> u >> v;
        int cur = root[version + 1];
        if (type == 0)
        {
            cur = d.merge(cur, u, v);
        }
        else
        {
            cout << d.same(cur, u, v) << '\n';
        }
        root[i + 1] = cur;
    }
    return 0;
}
''',
    ),
    Spec(
        "point_add_range_sum",
        "Point Add Range Sum",
        ("3_2_001.hpp",),
        "3.2.001",
        ("struct BIT",),
        ("tests/test_ch03_bit.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    BIT<long long> bit(n);
    for (int i = 0; i < n; ++i)
    {
        long long x;
        cin >> x;
        bit.add(i, x);
    }
    while (q--)
    {
        int type, x, y;
        cin >> type >> x >> y;
        if (type == 0)
        {
            bit.add(x, y);
        }
        else
        {
            cout << bit.sum(x, y) << '\n';
        }
    }
    return 0;
}
''',
    ),
    Spec(
        "range_add_range_min",
        "Range Add Range Min",
        ("3_3_004.hpp",),
        "3.3.004",
        ("class AddMinSeg",),
        ("tests/test_ch03_add_min_seg.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<long long> a(n);
    for (auto &x : a)
    {
        cin >> x;
    }
    AddMinSeg seg(a);
    while (q--)
    {
        int type, l, r;
        cin >> type >> l >> r;
        if (type == 0)
        {
            long long x;
            cin >> x;
            seg.add(l, r, x);
        }
        else
        {
            cout << seg.rangeMin(l, r) << '\n';
        }
    }
    return 0;
}
''',
    ),
    Spec(
        "range_chmin_chmax_add_range_sum",
        "Range Chmin Chmax Add Range Sum",
        ("3_3_011.hpp",),
        "3.3.011",
        ("struct SegBeats",),
        ("tests/test_ch03_beats.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<long long> a(n);
    for (auto &x : a)
    {
        cin >> x;
    }
    SegBeats seg(a);
    while (q--)
    {
        int type, l, r;
        cin >> type >> l >> r;
        if (type == 3)
        {
            cout << seg.sum(l, r) << '\n';
            continue;
        }
        long long x;
        cin >> x;
        if (type == 0)
        {
            seg.chmin(l, r, x);
        }
        else if (type == 1)
        {
            seg.chmax(l, r, x);
        }
        else
        {
            seg.add(l, r, x);
        }
    }
    return 0;
}
''',
    ),
    Spec(
        "range_linear_add_range_min",
        "Range Linear Add Range Min",
        ("3_3_019.hpp",),
        "3.3.019",
        ("class LineMinSeg",),
        ("tests/test_ch03_revised.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<long long> a(n);
    for (auto &x : a)
    {
        cin >> x;
    }
    LineMinSeg seg(a);
    while (q--)
    {
        int type;
        cin >> type;
        if (type == 0)
        {
            int l, r;
            long long b, c;
            cin >> l >> r >> b >> c;
            seg.add(l, r, b, c);
        }
        else
        {
            int l, r;
            cin >> l >> r;
            cout << seg.min(l, r) << '\n';
        }
    }
    return 0;
}
''',
    ),
    Spec(
        "staticrmq",
        "Static RMQ",
        ("3_4_001.hpp",),
        "3.4.001",
        ("struct ST",),
        ("tests/test_ch03_st.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<long long> a(n);
    for (auto &x : a)
    {
        cin >> x;
    }
    auto op = [](long long x, long long y)
    {
        return min(x, y);
    };
    ST<long long, decltype(op)> st(a, op);
    while (q--)
    {
        int l, r;
        cin >> l >> r;
        cout << st.query(l, r) << '\n';
    }
    return 0;
}
''',
    ),
    Spec(
        "range_kth_smallest",
        "Range Kth Smallest",
        ("3_4_005.hpp",),
        "3.4.005",
        ("struct KthWavelet",),
        ("tests/test_ch03_wavelet.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (auto &x : a)
    {
        cin >> x;
    }
    KthWavelet<30> wt(a);
    while (q--)
    {
        int l, r, k;
        cin >> l >> r >> k;
        cout << wt.kth(l, r, k) << '\n';
    }
    return 0;
}
''',
    ),
    Spec(
        "static_range_frequency",
        "Static Range Frequency",
        ("3_4_005.hpp",),
        "3.4.005",
        ("struct Wavelet",),
        ("tests/test_ch03_wavelet.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (auto &x : a)
    {
        cin >> x;
    }
    Wavelet<30> wt(a);
    while (q--)
    {
        int l, r, x;
        cin >> l >> r >> x;
        cout << wt.frequency(l, r, x) << '\n';
    }
    return 0;
}
''',
    ),
    Spec(
        "static_range_sum_with_upper_bound",
        "Static Range Sum with Upper Bound",
        ("3_4_005.hpp",),
        "3.4.005",
        ("struct Wavelet",),
        ("tests/test_ch03_wavelet.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (auto &x : a)
    {
        cin >> x;
    }
    Wavelet<30> wt(a);
    while (q--)
    {
        int l, r, x;
        cin >> l >> r >> x;
        auto [count, sum] = wt.countSumLE(l, r, x);
        cout << count << ' ' << sum << '\n';
    }
    return 0;
}
''',
    ),
    Spec(
        "set_xor_min",
        "Set Xor-Min",
        ("3_6_003.hpp",),
        "3.6.003",
        ("struct EraseBinTrie",),
        ("tests/test_ch03_erasable_trie.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;
    EraseBinTrie<30> tr(q);
    while (q--)
    {
        int type;
        unsigned long long x;
        cin >> type >> x;
        if (type == 0)
        {
            if (!tr.count(x))
            {
                tr.insert(x);
            }
        }
        else if (type == 1)
        {
            tr.erase(x);
        }
        else
        {
            cout << tr.minXor(x) << '\n';
        }
    }
    return 0;
}
''',
    ),
    Spec(
        "line_add_get_min",
        "Line Add Get Min",
        ("3_7_001.hpp",),
        "3.7.001",
        ("class LiChao",),
        ("tests/test_ch03_li_chao.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<pair<long long, long long>> initial(n);
    for (auto &[a, b] : initial)
    {
        cin >> a >> b;
    }
    struct Query
    {
        int type;
        long long a, b;
    };
    vector<Query> query(q);
    vector<long long> xs;
    for (auto &[type, a, b] : query)
    {
        cin >> type >> a;
        b = 0;
        if (type == 0)
        {
            cin >> b;
        }
        else
        {
            xs.push_back(a);
        }
    }
    if (xs.empty())
    {
        return 0;
    }
    LiChao<long long, __int128> tree(xs);
    for (auto [a, b] : initial)
    {
        tree.add({a, b});
    }
    for (auto [type, a, b] : query)
    {
        if (type == 0)
        {
            tree.add({a, b});
        }
        else
        {
            cout << (long long)*tree.query(a) << '\n';
        }
    }
    return 0;
}
''',
    ),
    Spec(
        "dynamic_point_set_rectangle_affine_rectangle_sum",
        "Dynamic Point Rectangle Affine Rectangle Sum",
        ("7_1_007.hpp", "3_7_009.hpp"),
        "3.7.009",
        ("struct KDRectAffine",),
        ("tests/test_ch03_kd_rect_affine.cpp", "tests/test_ch07_revised.cpp"),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<long long> xs(n), ys(n);
    vector<optional<Z>> weight(n);
    for (int i = 0; i < n; i++)
    {
        long long w;
        cin >> xs[i] >> ys[i] >> w;
        weight[i] = Z(w);
    }
    struct Query
    {
        int type;
        array<long long, 6> v{};
    };
    vector<Query> query(q);
    for (auto &e : query)
    {
        cin >> e.type;
        if (e.type == 0)
        {
            cin >> e.v[0] >> e.v[1] >> e.v[2];
            xs.push_back(e.v[0]);
            ys.push_back(e.v[1]);
            weight.push_back(nullopt);
        }
        else if (e.type == 1)
        {
            cin >> e.v[0] >> e.v[1];
        }
        else if (e.type == 2)
        {
            cin >> e.v[0] >> e.v[1] >> e.v[2] >> e.v[3];
        }
        else
        {
            cin >> e.v[0] >> e.v[1] >> e.v[2] >> e.v[3] >> e.v[4] >> e.v[5];
        }
    }
    KDRectAffine<long long, Z> tree(xs, ys, weight);
    int point_count = n;
    for (const auto &e : query)
    {
        if (e.type == 0)
        {
            tree.set(point_count++, Z(e.v[2]));
        }
        else if (e.type == 1)
        {
            tree.set(e.v[0], Z(e.v[1]));
        }
        else if (e.type == 2)
        {
            cout << tree.query(e.v[0], e.v[2], e.v[1], e.v[3]).val() << '\n';
        }
        else
        {
            tree.apply(e.v[0], e.v[2], e.v[1], e.v[3], Z(e.v[4]), Z(e.v[5]));
        }
    }
    return 0;
}
''',
        MODDOC,
    ),
    Spec(
        "dynamic_sequence_range_affine_range_sum",
        "Dynamic Sequence Range Affine Range Sum",
        ("7_1_007.hpp", "3_5_011.hpp"),
        "3.5.011",
        ("template <class T> struct ImpTreap",),
        ("tests/test_ch03_implicit_treap.cpp", "tests/test_ch07_revised.cpp"),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<Z> a(n);
    for (int i = 0; i < n; i++)
    {
        long long x;
        cin >> x;
        a[i] = Z(x);
    }
    ImpTreap<Z> seq(a, n + q);
    while (q--)
    {
        int type;
        cin >> type;
        if (type == 0)
        {
            int i;
            long long x;
            cin >> i >> x;
            seq.insert(i, Z(x));
        }
        else if (type == 1)
        {
            int i;
            cin >> i;
            seq.erase(i);
        }
        else if (type == 2)
        {
            int l, r;
            cin >> l >> r;
            seq.reverse(l, r);
        }
        else if (type == 3)
        {
            int l, r;
            long long b, c;
            cin >> l >> r >> b >> c;
            seq.affine(l, r, Z(b), Z(c));
        }
        else
        {
            int l, r;
            cin >> l >> r;
            cout << seq.sum(l, r).val() << '\n';
        }
    }
    return 0;
}
''',
        MODDOC,
    ),
    Spec(
        "persistent_queue",
        "Persistent Queue",
        ("3_5_012.hpp",),
        "3.5.012",
        ("template <class T, int LOG = 20> class PersistQueue",),
        ("tests/test_ch03_persistent_queue.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int q;
    cin >> q;
    PersistQueue<long long> queue(q);
    vector<int> version(q + 1);
    version[0] = queue.emptyVer();
    for (int i = 0; i < q; i++)
    {
        int type, t;
        cin >> type >> t;
        int base = version[t + 1];
        if (type == 0)
        {
            long long x;
            cin >> x;
            version[i + 1] = queue.push(base, x);
        }
        else
        {
            cout << queue.front(base) << '\n';
            version[i + 1] = queue.pop(base);
        }
    }
    return 0;
}
''',
    ),
    Spec(
        "persistent_range_affine_range_sum",
        "Persistent Range Affine Range Sum",
        ("7_1_007.hpp", "3_3_020.hpp"),
        "3.3.020",
        ("template <class T> class PerAffSeg",),
        ("tests/test_ch03_persistent_affine.cpp", "tests/test_ch07_revised.cpp"),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<Z> a(n);
    for (int i = 0; i < n; i++)
    {
        long long x;
        cin >> x;
        a[i] = Z(x);
    }
    PerAffSeg<Z> seg(a);
    vector<int> root(q + 1);
    root[0] = seg.root();
    for (int i = 0; i < q; i++)
    {
        int type, k;
        cin >> type >> k;
        int base = root[k + 1];
        if (type == 0)
        {
            int l, r;
            long long b, c;
            cin >> l >> r >> b >> c;
            root[i + 1] = seg.affine(base, l, r, Z(b), Z(c));
        }
        else if (type == 1)
        {
            int s, l, r;
            cin >> s >> l >> r;
            root[i + 1] = seg.splice(base, root[s + 1], l, r);
        }
        else
        {
            int l, r;
            cin >> l >> r;
            cout << seg.sum(base, l, r).val() << '\n';
            root[i + 1] = base;
        }
    }
    return 0;
}
''',
        MODDOC,
    ),
    Spec(
        "range_reverse_range_sum",
        "Range Reverse Range Sum",
        ("3_5_011.hpp",),
        "3.5.011",
        ("template <class T> struct ImpTreap",),
        ("tests/test_ch03_implicit_treap.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<long long> a(n);
    for (auto &x : a)
    {
        cin >> x;
    }
    ImpTreap<long long> seq(a, n);
    while (q--)
    {
        int type, l, r;
        cin >> type >> l >> r;
        if (type == 0)
        {
            seq.reverse(l, r);
        }
        else
        {
            cout << seq.sum(l, r) << '\n';
        }
    }
    return 0;
}
''',
    ),
    Spec(
        "range_set_range_composite",
        "Range Set Range Composite",
        ("7_1_007.hpp", "3_3_022.hpp"),
        "3.3.022",
        ("template <class T, class F> class RangeSetSeg",),
        ("tests/test_ch03_sparse_affine.cpp", "tests/test_ch07_revised.cpp"),
        r'''
using Fn = array<Z, 2>;

struct Compose
{
    Fn operator()(const Fn &left, const Fn &right) const
    {
        return {left[0] * right[0], left[1] * right[0] + right[1]};
    }
};

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<Fn> a(n);
    for (auto &[mul, add] : a)
    {
        long long x, y;
        cin >> x >> y;
        mul = Z(x);
        add = Z(y);
    }
    RangeSetSeg<Fn, Compose> seg(a, Fn{Z(1), Z(0)}, Compose{});
    seg.reserveTags(q);
    while (q--)
    {
        int type, l, r;
        cin >> type >> l >> r;
        if (type == 0)
        {
            long long c, d;
            cin >> c >> d;
            seg.assign(l, r, Fn{Z(c), Z(d)});
        }
        else
        {
            long long x;
            cin >> x;
            Fn f = seg.prod(l, r);
            cout << (f[0] * Z(x) + f[1]).val() << '\n';
        }
    }
    return 0;
}
''',
        MODDOC,
    ),
    Spec(
        "segment_add_get_min",
        "Segment Add Get Min",
        ("3_7_010.hpp",),
        "3.7.010",
        ("template <class T, class C = T> class SegLiChao",),
        ("tests/test_ch03_segment_li_chao.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    struct Segment
    {
        long long l, r, a, b;
    };
    struct Query
    {
        int type;
        long long l = 0, r = 0, a = 0, b = 0;
    };
    int n, q;
    cin >> n >> q;
    vector<Segment> initial(n);
    for (auto &[l, r, a, b] : initial)
    {
        cin >> l >> r >> a >> b;
    }
    vector<Query> query(q);
    vector<long long> xs;
    for (auto &e : query)
    {
        cin >> e.type;
        if (e.type == 0)
        {
            cin >> e.l >> e.r >> e.a >> e.b;
        }
        else
        {
            cin >> e.l;
            xs.push_back(e.l);
        }
    }
    if (xs.empty())
    {
        return 0;
    }
    SegLiChao<long long, __int128> tree(xs);
    for (const auto &e : initial)
    {
        tree.add({e.a, e.b}, e.l, e.r);
    }
    for (const auto &e : query)
    {
        if (e.type == 0)
        {
            tree.add({e.a, e.b}, e.l, e.r);
        }
        else
        {
            auto ans = tree.query(e.l);
            if (ans)
            {
                cout << (long long)*ans << '\n';
            }
            else
            {
                cout << "INFINITY\n";
            }
        }
    }
    return 0;
}
''',
    ),
    Spec(
        "range_affine_point_get",
        "Range Affine Point Get",
        ("7_1_007.hpp", "3_3_003.hpp"),
        "3.3.003",
        ("template <class T> class AffPointSeg", "template <class T> struct AffineTag"),
        ("tests/test_ch03_sparse_affine.cpp", "tests/test_ch07_revised.cpp"),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<Z> a;
    a.reserve(n);
    for (int i = 0; i < n; i++)
    {
        long long x;
        cin >> x;
        a.emplace_back(Z(x));
    }
    AffPointSeg<Z> seg(move(a));
    while (q--)
    {
        int type;
        cin >> type;
        if (type == 0)
        {
            int l, r;
            long long b, c;
            cin >> l >> r >> b >> c;
            seg.affine(l, r, Z(b), Z(c));
        }
        else
        {
            int i;
            cin >> i;
            cout << seg.get(i).val() << '\n';
        }
    }
    return 0;
}
''',
        MODDOC,
    ),
    Spec(
        "range_affine_range_sum",
        "Range Affine Range Sum",
        ("7_1_007.hpp", "3_3_003.hpp"),
        "3.3.003",
        ("template <class Info, class Tag> struct LazySeg", "template <class T> struct AffineTag", "template <class T> struct AffineInfo"),
        ("tests/test_ch03_sparse_affine.cpp", "tests/test_ch07_revised.cpp"),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<AffineInfo<Z>> a;
    a.reserve(n);
    for (int i = 0; i < n; i++)
    {
        long long x;
        cin >> x;
        a.emplace_back(Z(x));
    }
    LazySeg<AffineInfo<Z>, AffineTag<Z>> seg(a);
    while (q--)
    {
        int type, l, r;
        cin >> type >> l >> r;
        if (type == 0)
        {
            long long b, c;
            cin >> b >> c;
            seg.apply(l, r, {Z(b), Z(c)});
        }
        else
        {
            cout << seg.query(l, r).sum.val() << '\n';
        }
    }
    return 0;
}
''',
        MODDOC,
    ),
    Spec(
        "range_affine_range_sum_large_array",
        "Range Affine Range Sum (Large Array)",
        ("7_1_007.hpp", "3_3_021.hpp"),
        "3.3.021",
        ("template <class T> class SparseAffSeg",),
        ("tests/test_ch03_sparse_affine.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long n;
    int q;
    cin >> n >> q;
    SparseAffSeg<Z> seg(0, n);
    while (q--)
    {
        int type;
        long long l, r;
        cin >> type >> l >> r;
        if (type == 0)
        {
            long long b, c;
            cin >> b >> c;
            seg.affine(l, r, Z(b), Z(c));
        }
        else
        {
            cout << seg.sum(l, r).val() << '\n';
        }
    }
    return 0;
}
''',
        MODDOC,
    ),
    Spec(
        "deque_operate_all_composite",
        "Deque Operate All Composite",
        ("3_7_007.hpp",),
        "3.7.007",
        ("template <class T, class F> struct AggDeque",),
        ("tests/test_ch03_agg_deque.cpp",),
        r'''
constexpr int mod = 998244353;

struct Aff
{
    int a = 1, b = 0;
};

struct Compose
{
    Aff operator()(const Aff &f, const Aff &g) const
    {
        return {g.a * f.a % mod, (g.a * f.b + g.b) % mod};
    }
};

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;
    AggDeque<Aff, Compose> dq(Compose{}, Aff{});
    while (q--)
    {
        int type;
        cin >> type;
        if (type == 0)
        {
            Aff f;
            cin >> f.a >> f.b;
            dq.pushFront(f);
        }
        else if (type == 1)
        {
            Aff f;
            cin >> f.a >> f.b;
            dq.pushBack(f);
        }
        else if (type == 2)
        {
            dq.popFront();
        }
        else if (type == 3)
        {
            dq.popBack();
        }
        else
        {
            int x;
            cin >> x;
            auto f = dq.query();
            cout << (f.a * x + f.b) % mod << '\n';
        }
    }
    return 0;
}
''',
    ),
    Spec(
        "queue_operate_all_composite",
        "Queue Operate All Composite",
        ("3_7_007.hpp",),
        "3.7.007",
        ("template <class T, class F> struct AggDeque",),
        ("tests/test_ch03_agg_deque.cpp",),
        r'''
constexpr int mod = 998244353;

struct Aff
{
    int a = 1, b = 0;
};

struct Compose
{
    Aff operator()(const Aff &f, const Aff &g) const
    {
        return {g.a * f.a % mod, (g.a * f.b + g.b) % mod};
    }
};

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;
    AggDeque<Aff, Compose> que(Compose{}, Aff{});
    while (q--)
    {
        int type;
        cin >> type;
        if (type == 0)
        {
            Aff f;
            cin >> f.a >> f.b;
            que.pushBack(f);
        }
        else if (type == 1)
        {
            que.popFront();
        }
        else
        {
            int x;
            cin >> x;
            auto f = que.query();
            cout << (f.a * x + f.b) % mod << '\n';
        }
    }
    return 0;
}
''',
    ),
    Spec(
        "double_ended_priority_queue",
        "Double-Ended Priority Queue",
        ("3_5_007.hpp",),
        "3.5.007",
        ("using OrderedMSet = multiset<int>;",),
        ("tests/test_ch03_ordered_multiset.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    OrderedMSet st;
    while (n--)
    {
        int x;
        cin >> x;
        st.insert(x);
    }
    while (q--)
    {
        int type;
        cin >> type;
        if (type == 0)
        {
            int x;
            cin >> x;
            st.insert(x);
        }
        else if (type == 1)
        {
            auto it = st.begin();
            cout << *it << '\n';
            st.erase(it);
        }
        else
        {
            auto it = prev(st.end());
            cout << *it << '\n';
            st.erase(it);
        }
    }
    return 0;
}
''',
    ),
    Spec(
        "majority_voting",
        "Majority Voting",
        ("3_3_017.hpp",),
        "3.3.017",
        ("class MajRange",),
        ("tests/test_ch03_revised.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (auto &x : a)
    {
        cin >> x;
    }
    vector<array<int, 3>> qs(q);
    vector<pair<int, int>> plan;
    for (auto &z : qs)
    {
        cin >> z[0] >> z[1] >> z[2];
        if (z[0] == 0)
        {
            plan.push_back({z[1], z[2]});
        }
    }
    MajRange st(a, plan);
    for (auto [type, x, y] : qs)
    {
        if (type == 0)
        {
            st.set(x, y);
        }
        else
        {
            auto ans = st.query(x, y);
            cout << (ans ? *ans : -1) << '\n';
        }
    }
    return 0;
}
''',
    ),
    Spec(
        "ordered_set",
        "Ordered Set",
        ("supplement_pbds.hpp",),
        "3.9.001",
        ("template <class T> using OrdSet",),
        ("tests/test_ch03_ordered_set.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    pbds_ordered::OrdSet<int> st;
    while (n--)
    {
        int x;
        cin >> x;
        st.insert(x);
    }
    while (q--)
    {
        int type, x;
        cin >> type >> x;
        if (type == 0)
        {
            st.insert(x);
        }
        else if (type == 1)
        {
            st.erase(x);
        }
        else if (type == 2)
        {
            cout << (x > (int)st.size() ? -1 : *st.find_by_order(x - 1)) << '\n';
        }
        else if (type == 3)
        {
            cout << st.order_of_key(x + 1) << '\n';
        }
        else if (type == 4)
        {
            auto it = st.upper_bound(x);
            cout << (it == st.begin() ? -1 : *prev(it)) << '\n';
        }
        else
        {
            auto it = st.lower_bound(x);
            cout << (it == st.end() ? -1 : *it) << '\n';
        }
    }
    return 0;
}
''',
    ),
    Spec(
        "predecessor_problem",
        "Predecessor Problem",
        ("3_6_014.hpp",),
        "3.6.014",
        ("class FastSet",),
        ("tests/test_ch03_fast_set.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    string s;
    cin >> n >> q >> s;
    FastSet st(n);
    for (int i = 0; i < n; ++i)
    {
        if (s[i] == '1')
        {
            st.set(i);
        }
    }
    while (q--)
    {
        int type, x;
        cin >> type >> x;
        if (type == 0)
        {
            st.set(x);
        }
        else if (type == 1)
        {
            st.reset(x);
        }
        else if (type == 2)
        {
            cout << st.test(x) << '\n';
        }
        else if (type == 3)
        {
            cout << st.next(x) << '\n';
        }
        else
        {
            cout << st.prev(x) << '\n';
        }
    }
    return 0;
}
''',
    ),
    Spec(
        "point_set_range_composite",
        "Point Set Range Composite",
        ("7_1_007.hpp", "3_3_001.hpp"),
        "3.3.001",
        ("template <class Info> struct Seg",),
        ("tests/test_ch03_seg.cpp", "tests/test_ch07_revised.cpp"),
        r'''
struct Aff
{
    Z a = 1, b = 0;

    friend Aff operator+(Aff f, Aff g)
    {
        return {g.a * f.a, g.a * f.b + g.b};
    }
};

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<Aff> a(n);
    for (auto &f : a)
    {
        int x, y;
        cin >> x >> y;
        f = {Z(x), Z(y)};
    }
    Seg<Aff> st(a);
    while (q--)
    {
        int type, x, y, z;
        cin >> type >> x >> y >> z;
        if (type == 0)
        {
            st.set(x, {Z(y), Z(z)});
        }
        else
        {
            auto f = st.query(x, y);
            cout << (f.a * Z(z) + f.b).val() << '\n';
        }
    }
    return 0;
}
''',
        MODDOC,
    ),
    Spec(
        "point_set_range_composite_large_array",
        "Point Set Range Composite (Large Array)",
        ("7_1_007.hpp", "3_3_007.hpp"),
        "3.3.007",
        ("template <class T, class F> class SparsePtSeg",),
        ("tests/test_ch03_sparse_affine.cpp", "tests/test_ch07_revised.cpp"),
        r'''
struct Aff
{
    Z a = 1, b = 0;
};

struct Compose
{
    Aff operator()(const Aff &f, const Aff &g) const
    {
        return {g.a * f.a, g.a * f.b + g.b};
    }
};

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    SparsePtSeg<Aff, Compose> st(0, n, Aff{}, Compose{});
    st.reserveNodes(q);
    while (q--)
    {
        int type;
        cin >> type;
        if (type == 0)
        {
            int p, a, b;
            cin >> p >> a >> b;
            st.set(p, {Z(a), Z(b)});
        }
        else
        {
            int l, r, x;
            cin >> l >> r >> x;
            auto f = st.prod(l, r);
            cout << (f.a * Z(x) + f.b).val() << '\n';
        }
    }
    return 0;
}
''',
        MODDOC,
    ),
    Spec(
        "point_set_range_frequency",
        "Point Set Range Frequency",
        ("3_6_009.hpp",),
        "3.6.009",
        ("class Freq",),
        ("tests/test_ch03_freq.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (auto &x : a)
    {
        cin >> x;
    }
    vector<array<int, 4>> qs(q);
    vector<pair<int, int>> plan;
    for (auto &z : qs)
    {
        cin >> z[0];
        if (z[0] == 0)
        {
            cin >> z[1] >> z[2];
            plan.push_back({z[1], z[2]});
        }
        else
        {
            cin >> z[1] >> z[2] >> z[3];
        }
    }
    Freq st(a, plan);
    for (auto z : qs)
    {
        if (z[0] == 0)
        {
            st.setValue(z[1], z[2]);
        }
        else
        {
            cout << st.frequency(z[1], z[2], z[3]) << '\n';
        }
    }
    return 0;
}
''',
    ),
    Spec(
        "point_set_range_sort_range_composite",
        "Point Set Range Sort Range Composite",
        ("7_1_007.hpp", "3_3_018.hpp"),
        "3.3.018",
        ("template <class M> class SortSeg",),
        ("tests/test_ch03_revised.cpp", "tests/test_ch07_revised.cpp"),
        r'''
struct Aff
{
    Z a = 1, b = 0;
};

struct AffMonoid
{
    using T = Aff;

    static T id()
    {
        return {};
    }

    static T op(const T &f, const T &g)
    {
        return {g.a * f.a, g.a * f.b + g.b};
    }
};

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> key(n), allKey;
    vector<Aff> a(n);
    allKey.reserve(n + q);
    for (int i = 0; i < n; ++i)
    {
        int p, x, y;
        cin >> p >> x >> y;
        key[i] = p;
        a[i] = {Z(x), Z(y)};
        allKey.push_back(p);
    }
    vector<array<int, 5>> qs(q);
    for (auto &z : qs)
    {
        cin >> z[0];
        if (z[0] == 0)
        {
            cin >> z[1] >> z[2] >> z[3] >> z[4];
            allKey.push_back(z[2]);
        }
        else if (z[0] == 1)
        {
            cin >> z[1] >> z[2] >> z[3];
        }
        else
        {
            cin >> z[1] >> z[2];
        }
    }
    sort(allKey.begin(), allKey.end());
    allKey.erase(unique(allKey.begin(), allKey.end()), allKey.end());
    for (auto &p : key)
    {
        p = lower_bound(allKey.begin(), allKey.end(), p) - allKey.begin();
    }
    SortSeg<AffMonoid> st(allKey.size(), key, a);
    for (auto z : qs)
    {
        if (z[0] == 0)
        {
            int p = lower_bound(allKey.begin(), allKey.end(), z[2]) - allKey.begin();
            st.assign(z[1], p, {Z(z[3]), Z(z[4])});
        }
        else if (z[0] == 1)
        {
            auto f = st.prod(z[1], z[2]);
            cout << (f.a * Z(z[3]) + f.b).val() << '\n';
        }
        else if (z[0] == 2)
        {
            st.sortAsc(z[1], z[2]);
        }
        else
        {
            st.sortDesc(z[1], z[2]);
        }
    }
    return 0;
}
''',
        MODDOC,
    ),
    Spec(
        "point_add_rectangle_sum",
        "Point Add Rectangle Sum",
        ("3_2_008.hpp",),
        "3.2.008",
        ("class BIT2",),
        ("tests/test_ch03_bit2.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<array<int, 3>> init(n);
    vector<pair<int, int>> plan;
    for (auto &[x, y, w] : init)
    {
        cin >> x >> y >> w;
        plan.push_back({x, y});
    }
    vector<array<int, 5>> qs(q);
    for (auto &z : qs)
    {
        cin >> z[0];
        if (z[0] == 0)
        {
            cin >> z[1] >> z[2] >> z[3];
            plan.push_back({z[1], z[2]});
        }
        else
        {
            cin >> z[1] >> z[2] >> z[3] >> z[4];
        }
    }
    BIT2 bit(plan);
    for (auto [x, y, w] : init)
    {
        bit.add(x, y, w);
    }
    for (auto z : qs)
    {
        if (z[0] == 0)
        {
            bit.add(z[1], z[2], z[3]);
        }
        else
        {
            cout << bit.prefixSum(z[3], z[4]) - bit.prefixSum(z[1], z[4])
                 - bit.prefixSum(z[3], z[2]) + bit.prefixSum(z[1], z[2]) << '\n';
        }
    }
    return 0;
}
''',
    ),
    Spec(
        "rectangle_add_point_get",
        "Rectangle Add Point Get",
        ("3_2_011.hpp",),
        "3.2.011",
        ("class BIT2",),
        ("tests/test_ch03_bit2.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<array<int, 5>> init(n);
    vector<pair<int, int>> plan;
    auto addCorners = [&](const array<int, 5> &z)
    {
        plan.push_back({z[0], z[1]});
        plan.push_back({z[0], z[3]});
        plan.push_back({z[2], z[1]});
        plan.push_back({z[2], z[3]});
    };
    for (auto &z : init)
    {
        cin >> z[0] >> z[1] >> z[2] >> z[3] >> z[4];
        addCorners(z);
    }
    vector<array<int, 6>> qs(q);
    for (auto &z : qs)
    {
        cin >> z[0];
        if (z[0] == 0)
        {
            cin >> z[1] >> z[2] >> z[3] >> z[4] >> z[5];
            addCorners({z[1], z[2], z[3], z[4], z[5]});
        }
        else
        {
            cin >> z[1] >> z[2];
        }
    }
    BIT2 bit(plan);
    for (auto z : init)
    {
        bit.addRectangle(z[0], z[1], z[2], z[3], z[4]);
    }
    for (auto z : qs)
    {
        if (z[0] == 0)
        {
            bit.addRectangle(z[1], z[2], z[3], z[4], z[5]);
        }
        else
        {
            cout << bit.pointGet(z[1], z[2]) << '\n';
        }
    }
    return 0;
}
''',
    ),
    Spec(
        "static_rectangle_add_rectangle_sum",
        "Static Rectangle Add Rectangle Sum",
        ("3_2_009.hpp", "7_1_007.hpp"),
        "3.2.009",
        ("template <class T> struct RectAddSum",),
        ("tests/test_ch03_rect_add_sum.cpp", "tests/test_ch07_revised.cpp"),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    RectAddSum<Z> st;
    for (int i = 0; i < n; ++i)
    {
        int l, d, r, u, w;
        cin >> l >> d >> r >> u >> w;
        st.add(l, d, r, u, Z(w));
    }
    for (int i = 0; i < q; ++i)
    {
        int l, d, r, u;
        cin >> l >> d >> r >> u;
        st.query(l, d, r, u);
    }
    for (auto x : st.work())
    {
        cout << x.val() << '\n';
    }
    return 0;
}
''',
        MODDOC,
    ),
    Spec(
        "range_parallel_unionfind",
        "Range Parallel Unionfind",
        ("3_1_006.hpp", "7_1_007.hpp"),
        "3.1.006",
        ("struct RangeDSU",),
        ("tests/test_ch03_revised.cpp", "tests/test_ch07_revised.cpp"),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<Z> sum(n);
    for (auto &x : sum)
    {
        int v;
        cin >> v;
        x = Z(v);
    }
    RangeDSU st(n);
    Z ans;
    while (q--)
    {
        int k, a, b;
        cin >> k >> a >> b;
        st.merge(a, b, k, [&](int root, int absorbed)
        {
            ans += sum[root] * sum[absorbed];
            sum[root] += sum[absorbed];
        });
        cout << ans.val() << '\n';
    }
    return 0;
}
''',
        MODDOC,
    ),
    Spec(
        "static_range_count_distinct",
        "Static Range Count Distinct",
        ("3_4_010.hpp",),
        "3.4.010",
        ("distinct",),
        ("tests/test_ch03_static_range_queries.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (auto &x : a)
    {
        cin >> x;
    }
    vector<pair<int, int>> qs(q);
    for (auto &[l, r] : qs)
    {
        cin >> l >> r;
    }
    for (auto x : distinct(a, qs))
    {
        cout << x << '\n';
    }
    return 0;
}
''',
    ),
    Spec(
        "static_range_inversions_query",
        "Static Range Inversions Query",
        ("3_4_011.hpp",),
        "3.4.011",
        ("rangeInv",),
        ("tests/test_ch03_static_range_queries.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (auto &x : a)
    {
        cin >> x;
    }
    vector<pair<int, int>> qs(q);
    for (auto &[l, r] : qs)
    {
        cin >> l >> r;
    }
    for (auto x : rangeInv(a, qs))
    {
        cout << x << '\n';
    }
    return 0;
}
''',
    ),
    Spec(
        "static_range_lis_query",
        "Static Range LIS Query",
        ("3_4_009.hpp",),
        "3.4.009",
        ("template <int LOG = 17> class StaticRngLIS",),
        ("tests/test_ch03_static_lis.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> p(n);
    for (auto &x : p)
    {
        cin >> x;
    }
    StaticRngLIS<> st(p);
    while (q--)
    {
        int l, r;
        cin >> l >> r;
        cout << st.query(l, r) << '\n';
    }
    return 0;
}
''',
    ),
    Spec(
        "static_range_mode_query",
        "Static Range Mode Query",
        ("3_4_008.hpp",),
        "3.4.008",
        ("class RangeMode",),
        ("tests/test_ch03_revised.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (auto &x : a)
    {
        cin >> x;
    }
    RangeMode st(a);
    while (q--)
    {
        int l, r;
        cin >> l >> r;
        auto [x, cnt] = *st.query(l, r);
        cout << x << ' ' << cnt << '\n';
    }
    return 0;
}
''',
    ),
    Spec(
        "static_range_sum",
        "Static Range Sum",
        ("3_4_012.hpp",),
        "3.4.012",
        ("prefix",),
        ("tests/test_ch03_static_range_queries.cpp",),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<long long> a(n);
    for (auto &x : a)
    {
        cin >> x;
    }
    auto pre = prefix(a);
    while (q--)
    {
        int l, r;
        cin >> l >> r;
        cout << pre[r] - pre[l] << '\n';
    }
    return 0;
}
''',
    ),
    Spec(
        "unionfind_with_potential",
        "Unionfind with Potential",
        ("3_1_003.hpp", "7_1_007.hpp"),
        "3.1.003",
        ("template <class T> struct WDSU",),
        ("tests/test_ch03_wdsu.cpp", "tests/test_ch07_revised.cpp"),
        r'''
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    WDSU<Z> st(n);
    while (q--)
    {
        int type, u, v;
        cin >> type >> u >> v;
        if (type == 0)
        {
            int x;
            cin >> x;
            cout << st.merge(v, u, Z(x)) << '\n';
        }
        else if (!st.same(u, v))
        {
            cout << -1 << '\n';
        }
        else
        {
            cout << st.diff(v, u).val() << '\n';
        }
    }
    return 0;
}
''',
        MODDOC,
    ),
    Spec(
        "unionfind_with_potential_non_commutative_group",
        "Unionfind with Potential (Non-Commutative Group)",
        ("3_1_008.hpp", "7_1_007.hpp"),
        "3.1.008",
        ("template <class Group> struct PotentialDSU",),
        ("tests/test_ch03_group_dsu.cpp", "tests/test_ch07_revised.cpp"),
        r'''
struct MatrixGroup
{
    using T = array<array<Z, 2>, 2>;

    static T id()
    {
        return {{{1, 0}, {0, 1}}};
    }

    static T op(const T &a, const T &b)
    {
        T c{};
        for (int i = 0; i < 2; ++i)
        {
            for (int k = 0; k < 2; ++k)
            {
                for (int j = 0; j < 2; ++j)
                {
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }
        return c;
    }

    static T inv(const T &a)
    {
        return {{{a[1][1], -a[0][1]}, {-a[1][0], a[0][0]}}};
    }
};

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    PotentialDSU<MatrixGroup> st(n);
    while (q--)
    {
        int type, u, v;
        cin >> type >> u >> v;
        if (type == 0)
        {
            MatrixGroup::T x;
            for (int i = 0; i < 2; ++i)
            {
                for (int j = 0; j < 2; ++j)
                {
                    int z;
                    cin >> z;
                    x[i][j] = Z(z);
                }
            }
            cout << st.merge(v, u, x) << '\n';
        }
        else if (!st.same(u, v))
        {
            cout << -1 << '\n';
        }
        else
        {
            auto x = st.diff(v, u);
            cout << x[0][0].val() << ' ' << x[0][1].val() << ' '
                 << x[1][0].val() << ' ' << x[1][1].val() << '\n';
        }
    }
    return 0;
}
''',
        MODDOC,
    ),
)


QUOTED_INCLUDE = re.compile(r'^\s*#\s*include\s+"([^"]+)"\s*$')


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def read_text(path: Path) -> str:
    data = path.read_bytes()
    if data.startswith(b"\xef\xbb\xbf"):
        raise ValueError(f"UTF-8 BOM is not allowed: {path}")
    return data.decode("utf-8").replace("\r\n", "\n").replace("\r", "\n")


def write_text(path: Path, text: str) -> None:
    with path.open("w", encoding="utf-8", newline="\n") as file:
        file.write(text)


def workspace_root() -> Path:
    return Path(__file__).resolve().parents[2]


def template_root(workspace: Path) -> Path:
    roots = [path for path in workspace.iterdir() if path.is_dir() and path.name.startswith("hushuqi")]
    if len(roots) != 1:
        raise ValueError("cannot identify the template directory")
    return roots[0]


def chapter_path(root: Path) -> Path:
    paths = sorted(root.glob("03-*.md"))
    if len(paths) != 1:
        raise ValueError("cannot identify chapter 03 Markdown")
    return paths[0]


def official_problems_path(root: Path) -> Path:
    path = root / "library-checker-template-solutions" / ".local" / "library-checker-problems"
    if not path.is_dir():
        raise ValueError(f"missing frozen official problem repository: {path}")
    return path


def git_bytes(repository: Path, arguments: list[str], label: str) -> bytes:
    try:
        result = subprocess.run(
            ["git", "-C", str(repository), *arguments],
            capture_output=True,
            check=False,
            timeout=30,
        )
    except (OSError, subprocess.TimeoutExpired) as error:
        raise ValueError(f"cannot read frozen official repository for {label}") from error
    if result.returncode:
        detail = result.stderr.decode("utf-8", errors="replace").strip()
        raise ValueError(f"cannot read frozen official repository for {label}: {detail}")
    return result.stdout


def official_names(root: Path) -> list[str]:
    output = git_bytes(
        official_problems_path(root),
        ["ls-tree", "-d", "--name-only", f"{OFFICIAL_COMMIT}:data_structure"],
        "data_structure problem list",
    )
    names = sorted(name for name in output.decode("utf-8").splitlines() if name)
    if len(names) != len(set(names)):
        raise ValueError("frozen data_structure problem list has duplicate names")
    return names


def official_reference(root: Path, spec: Spec) -> dict:
    repository = official_problems_path(root)
    path = f"data_structure/{spec.name}/sol/correct.cpp"
    license_blob = git_bytes(
        repository,
        ["rev-parse", f"{OFFICIAL_COMMIT}:LICENSE"],
        "official license blob",
    ).decode("ascii").strip()
    if license_blob != OFFICIAL_LICENSE_BLOB:
        raise ValueError("frozen official license blob does not match the pinned license")
    blob = git_bytes(
        repository,
        ["rev-parse", f"{OFFICIAL_COMMIT}:{path}"],
        f"{spec.name} official correct.cpp blob",
    ).decode("ascii").strip()
    source = git_bytes(
        repository,
        ["show", f"{OFFICIAL_COMMIT}:{path}"],
        f"{spec.name} official correct.cpp",
    )
    return {
        "repository": OFFICIAL_REPOSITORY_URL,
        "commit": OFFICIAL_COMMIT,
        "license": OFFICIAL_LICENSE,
        "license_blob": OFFICIAL_LICENSE_BLOB,
        "path": path,
        "git_blob": blob,
        "content_sha256": sha256(source),
    }


def validate_spec_coverage(root: Path) -> None:
    names = official_names(root)
    spec_names = [spec.name for spec in SPECS]
    duplicated = sorted({name for name in spec_names if spec_names.count(name) != 1})
    if duplicated:
        raise ValueError("duplicate chapter 03 spec names: " + ", ".join(duplicated))
    missing = sorted(set(names) - set(spec_names))
    extra = sorted(set(spec_names) - set(names))
    if missing or extra:
        detail = []
        if missing:
            detail.append("missing=" + ", ".join(missing))
        if extra:
            detail.append("extra=" + ", ".join(extra))
        raise ValueError("chapter 03 specs do not equal frozen official set: " + "; ".join(detail))


def render_header(path: Path, header_root: Path, seen: set[Path], used: list[Path]) -> str:
    path = path.resolve()
    if path in seen:
        return ""
    try:
        path.relative_to(header_root.resolve())
    except ValueError as error:
        raise ValueError(f"header escapes template include directory: {path}") from error
    seen.add(path)
    used.append(path)
    result = []
    for line in read_text(path).splitlines(keepends=True):
        if line.strip() == "#pragma once":
            continue
        match = QUOTED_INCLUDE.match(line.rstrip("\n"))
        if match is None:
            result.append(line)
            continue
        child = (path.parent / match.group(1)).resolve()
        result.append(render_header(child, header_root, seen, used))
    return "".join(result)


def assert_template_sync(spec: Spec, templates: Path, chapter: str, headers: list[Path]) -> None:
    if spec.topic is not None and f"### {spec.topic} " not in chapter:
        raise ValueError(f"{spec.name}: missing Markdown topic {spec.topic}")
    for symbol in spec.symbols:
        if symbol not in chapter:
            raise ValueError(f"{spec.name}: Markdown lacks {symbol}")
        if not any(symbol in read_text(path) for path in headers):
            raise ValueError(f"{spec.name}: full headers lack {symbol}")
    for markdown_name, topic, symbols in spec.extra_markdown:
        markdown = templates / markdown_name
        text = read_text(markdown)
        if f"### {topic} " not in text:
            raise ValueError(f"{spec.name}: missing Markdown topic {topic}")
        for symbol in symbols:
            if symbol not in text:
                raise ValueError(f"{spec.name}: Markdown {markdown_name} lacks {symbol}")


def resolve_header(templates: Path, name: str) -> Path:
    include = templates / "include"
    full = include / "full" / name
    if full.is_file():
        return full
    direct = include / name
    if direct.is_file():
        return direct
    raise ValueError(f"missing template header: {name}")


def assemble(spec: Spec, root: Path) -> tuple[bytes, list[Path]]:
    templates = template_root(root)
    chapter = read_text(chapter_path(templates))
    include = templates / "include"
    seen: set[Path] = set()
    used: list[Path] = []
    bodies = []
    for name in spec.headers:
        bodies.append(render_header(resolve_header(templates, name), include, seen, used))
    assert_template_sync(spec, templates, chapter, used)
    source = "".join(bodies).rstrip() + "\n\n" + spec.adapter.strip() + "\n"
    if "\r" in source:
        raise ValueError(f"{spec.name}: generated CRLF")
    if QUOTED_INCLUDE.search(source):
        raise ValueError(f"{spec.name}: generated local include")
    data = source.encode("utf-8")
    if len(data) > (1 << 20):
        raise ValueError(f"{spec.name}: generated source exceeds 1 MiB")
    return data, used


def selected(raw: str | None) -> list[Spec]:
    by_name = {spec.name: spec for spec in SPECS}
    if raw is None:
        return list(SPECS)
    names = [part.strip() for part in raw.split(",") if part.strip()]
    if not names:
        raise ValueError("--only is empty")
    unknown = sorted(set(names) - set(by_name))
    if unknown:
        raise ValueError("unknown problem: " + ", ".join(unknown))
    return [by_name[name] for name in names]


def source_path(spec: Spec) -> str:
    return f"data_structure/{spec.name}/main.cpp"


def meta_path(spec: Spec) -> str:
    return f"data_structure/{spec.name}/meta.json"


def test_paths(root: Path, spec: Spec) -> tuple[str, ...]:
    prefix = template_root(root).relative_to(root).as_posix()
    return tuple(f"{prefix}/{path}" for path in spec.tests)


def reference_path(root: Path, spec: Spec) -> Path:
    return root / "library-checker-solutions" / "data_structure" / spec.name / "main.cpp"


def canonical_sha256(value: object) -> str:
    return sha256(json.dumps(value, ensure_ascii=False, sort_keys=True, separators=(",", ":")).encode("utf-8"))


def metadata(root: Path, spec: Spec, report: dict, generator_argv: list[str]) -> dict:
    path = root / "library-checker-template-solutions" / meta_path(spec)
    old = {}
    if path.is_file():
        old = json.loads(read_text(path))
    attempts = old.get("submission_attempts", []) if isinstance(old, dict) else []
    reference = reference_path(root, spec)
    if not reference.is_file():
        raise ValueError(f"{spec.name}: reference main.cpp is missing")
    official = official_reference(root, spec)
    source_hash = report["source_sha256"]
    generator_path = "library-checker-template-solutions/tools/generate_ch03_lc.py"
    generator_hash = sha256((root / generator_path).read_bytes())
    generation = {
        "schema_version": 2,
        "problem_id": f"data_structure/{spec.name}",
        "source_path": f"library-checker-template-solutions/{source_path(spec)}",
        "source_sha256": source_hash,
        "generator_path": generator_path,
        "generator_sha256": generator_hash,
        "working_directory": ".",
        "runner": f"Python {platform.python_version()}",
        "argv": generator_argv,
        "inputs": report["template_inputs"],
        "generated_sha256": source_hash,
        "byte_equal": True,
    }
    evidence = {
        "method": "deterministic_generation",
        "generator": {
            "path": generator_path,
            "sha256": generator_hash,
            "working_directory": ".",
            "runner": f"Python {platform.python_version()}",
            "argv": generator_argv,
            "generation_evidence": generation,
            "generation_evidence_sha256": canonical_sha256(generation),
        },
        "adapter_sha256": sha256(spec.adapter.strip().encode("utf-8")),
        "template_inputs": report["template_inputs"],
        "temporary_regenerations": 2,
        "byte_identical": True,
        "source_sha256": source_hash,
    }
    current_checks = ["source_cxx20_compile", *test_paths(root, spec)]
    old_provenance = old.get("provenance") if isinstance(old, dict) else None
    old_validation = old.get("validation") if isinstance(old, dict) else None
    evidence["evidence_sha256"] = canonical_sha256(evidence)
    # 在线判定绑定提交源码字节，而不是生成器或 Markdown 的快照。
    # 当前重生成仍会刷新 provenance；只要候选字节未变，保留同一字节已有的服务端结果。
    if (
        isinstance(old_validation, dict)
        and isinstance(old_provenance, dict)
        and old_provenance.get("source_sha256") == source_hash
        and old_validation.get("current_source_sha256") == source_hash
    ):
        validation = dict(old_validation)
        validation["checks"] = current_checks
        validation.pop("certification_context_sha256", None)
    else:
        validation = {
            "status": "local_only",
            "checked_at": None,
            "checks": current_checks,
            "current_source_sha256": source_hash,
            "current_source_online_verdict": None,
            "current_source_submission_id": None,
            "current_source_time": None,
            "current_source_memory": None,
            "current_source_submission_time": None,
            "current_source_is_latest": None,
            "server_source_sha256": None,
            "note": "Generated from the current chapter 03 template; online verification has not run for this source.",
        }
    return {
        "schema_version": 3,
        "name": spec.name,
        "title": spec.title,
        "category": "data_structure",
        "provenance": {
            "kind": "template_solution",
            "source": source_path(spec),
            "source_sha256": source_hash,
            "evidence": evidence,
        },
        "reference": {
            "kind": "read_only_baseline",
            "source": f"library-checker-solutions/data_structure/{spec.name}/main.cpp",
            "source_sha256": sha256(reference.read_bytes()),
        },
        "official_reference": official,
        "validation": validation,
        "submission_attempts": attempts if isinstance(attempts, list) else [],
        "notes": [
            "The only submission candidate is this self-contained main.cpp.",
            "The reference source is read-only comparison material and is not a submission input.",
        ],
    }


def write_json(path: Path, value: dict) -> None:
    write_text(path, json.dumps(value, ensure_ascii=False, indent=2) + "\n")


def update_index(
    root: Path,
    specs: list[Spec],
    reports: dict[str, dict],
    generator_argv: list[str],
) -> None:
    directory = root / "library-checker-template-solutions"
    path = directory / "manifest.json"
    manifest = json.loads(read_text(path))
    entries = {entry["name"]: entry for entry in manifest["problems"]}
    for spec in specs:
        entries[spec.name] = {
            "name": spec.name,
            "title": spec.title,
            "category": "data_structure",
            "source": source_path(spec),
            "source_sha256": reports[spec.name]["source_sha256"],
            "meta": meta_path(spec),
            "state": "template_solution",
        }
        meta = metadata(root, spec, reports[spec.name], generator_argv)
        meta_file = directory / meta_path(spec)
        meta_file.parent.mkdir(parents=True, exist_ok=True)
        write_json(meta_file, meta)
    problems = [entries[name] for name in sorted(entries)]
    counts = {"template_solution": len(problems)}
    if (
        manifest.get("problems") != problems
        or manifest.get("problem_count") != len(problems)
        or manifest.get("counts") != counts
    ):
        manifest["problems"] = problems
        manifest["problem_count"] = len(problems)
        manifest["counts"] = counts
        manifest["updated_at"] = datetime.now(timezone.utc).isoformat().replace("+00:00", "Z")
        write_json(path, manifest)


def describe(root: Path, spec: Spec, data: bytes, headers: list[Path]) -> dict:
    templates = template_root(root)
    chapter = chapter_path(templates)
    rel = lambda path: path.resolve().relative_to(root.resolve()).as_posix()
    inputs = [
        *([chapter] if spec.topic is not None else []),
        *headers,
        *(templates / markdown for markdown, _, _ in spec.extra_markdown),
        *(root / test for test in test_paths(root, spec)),
    ]
    if not all(path.is_file() for path in inputs):
        missing = [str(path) for path in inputs if not path.is_file()]
        raise ValueError(f"{spec.name}: missing template input: {', '.join(missing)}")
    return {
        "problem": spec.name,
        "source_path": source_path(spec),
        "source_sha256": sha256(data),
        "template_inputs": [
            {"path": rel(path), "sha256": sha256(path.read_bytes())}
            for path in sorted(set(inputs))
        ],
        "reference": {
            "path": f"library-checker-solutions/data_structure/{spec.name}/main.cpp",
            "sha256": sha256(reference_path(root, spec).read_bytes()),
        },
    }


def generate_twice(root: Path, spec: Spec) -> tuple[bytes, list[Path]]:
    first, headers = assemble(spec, root)
    second, second_headers = assemble(spec, root)
    if first != second or headers != second_headers:
        raise ValueError(f"{spec.name}: deterministic regeneration differs")
    for index, data in enumerate((first, second), start=1):
        with tempfile.TemporaryDirectory(prefix="ch03-lc-generate-") as directory:
            temporary = Path(directory) / "main.cpp"
            temporary.write_bytes(data)
            if temporary.read_bytes() != data:
                raise ValueError(f"{spec.name}: temporary generation {index} changed bytes")
    return first, headers


def main() -> None:
    parser = argparse.ArgumentParser(description="Generate chapter 03 Library Checker sources")
    parser.add_argument("--only", help="comma-separated problem names")
    parser.add_argument("--check", action="store_true", help="only compare generated bytes with main.cpp")
    parser.add_argument("--write-index", action="store_true", help="refresh meta.json and manifest.json")
    parser.add_argument("--describe", action="store_true", help="print source input identities as JSON")
    args = parser.parse_args()

    root = workspace_root()
    validate_spec_coverage(root)
    specs = selected(args.only)
    reports_by_name = {}
    reports = []
    for spec in specs:
        data, headers = generate_twice(root, spec)
        target = root / "library-checker-template-solutions" / source_path(spec)
        if args.check:
            if not target.is_file() or target.read_bytes() != data:
                raise ValueError(f"{spec.name}: main.cpp is not byte-identical to generated output")
            print(f"MATCHED {spec.name} {sha256(data)}")
        else:
            target.parent.mkdir(parents=True, exist_ok=True)
            if not target.is_file() or target.read_bytes() != data:
                target.write_bytes(data)
            print(f"GENERATED {spec.name} {sha256(data)}")
        report = describe(root, spec, data, headers)
        reports_by_name[spec.name] = report
        reports.append(report)
    if args.write_index:
        if args.check:
            raise ValueError("--check cannot update the index")
        generator_argv = []
        if args.only is not None:
            generator_argv.extend(["--only", args.only])
        generator_argv.append("--write-index")
        update_index(root, specs, reports_by_name, generator_argv)
    if args.describe:
        print(json.dumps(reports, ensure_ascii=False, indent=2))


if __name__ == "__main__":
    main()
