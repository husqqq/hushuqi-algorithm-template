#pragma once

#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int inf = 1E9;
constexpr long long INF = 4E18;
constexpr long double eps = 1E-12L;

template<class T>
bool chmin(T &a, const T &b)
{
    if (b < a)
    {
        a = b;
        return true;
    }
    return false;
}

template<class T>
bool chmax(T &a, const T &b)
{
    if (a < b)
    {
        a = b;
        return true;
    }
    return false;
}

bool bashWin(long long n, long long m)
{
    assert(n >= 0 && m >= 1);
    if (m == numeric_limits<long long>::max())
    {
        return n != 0;
    }
    return n % (m + 1) != 0;
}
