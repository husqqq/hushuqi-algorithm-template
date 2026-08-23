#pragma once
#include <bits/stdc++.h>
using namespace std;
#define int long long

struct Euclid
{
    __int128 x = 0;  // 已拼接路径的横步总数
    __int128 y = 0;  // 已拼接路径的纵步总数
    __int128 xy = 0; // 每个横步之前的纵步数之和

    Euclid operator+(const Euclid &other) const
    {
        // 把 other 路径接在当前路径后，返回合并结果。
        return {x + other.x, y + other.y,
                xy + other.xy + y * other.x};
    }
};

inline Euclid monoidPower(Euclid value, long long exponent)
{
    // 返回 value 在幺半群 + 下的 exponent 次幂。
    assert(exponent >= 0); // 调试检查，可删
    Euclid result;
    while (exponent)
    {
        if (exponent & 1)
        {
            result = result + value;
        }
        value = value + value;
        exponent >>= 1;
    }
    return result;
}

inline Euclid floorPath(long long p, long long q, long long r,
                        long long length, Euclid up, Euclid right)
{
    // p、q、r 定义取整直线，length 是横坐标上界；up、right 是纵步与横步元素，返回整段聚合。
    assert(p >= 0 && q > 0 && 0 <= r && r < q && length >= 0); // 调试检查，可删
    if (!length)
    {
        return {};
    }
    if (p >= q)
    {
        return floorPath(p % q, q, r, length, up,
                         monoidPower(up, p / q) + right);
    }
    long long height = (long long)(((__int128)length * p + r) / q);
    if (!height)
    {
        return monoidPower(right, length);
    }
    long long first = (long long)(((__int128)q - r - 1) / p);
    long long suffix = (long long)((__int128)length -
                                   ((__int128)q * height - r - 1) / p);
    return monoidPower(right, first) + up +
           floorPath(q, p, (q - r - 1) % p, height - 1,
                     right, up) +
           monoidPower(right, suffix);
}
