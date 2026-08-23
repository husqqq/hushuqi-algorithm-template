
#include <bits/stdc++.h>
using namespace std;
#define int long long

struct GaussianInt
{
    int x, y; // x、y 分别是实部与虚部。

    GaussianInt(int x = 0, int y = 0) : x(x), y(y)
    {
        // x、y 是高斯整数 x+yi 的两个系数。
    }

    bool zero() const
    {
        // 无参数；返回当前高斯整数是否为 0。
        return x == 0 && y == 0;
    }

    GaussianInt conjugate() const
    {
        // 无参数；返回共轭 x-yi。
        return {x, -y};
    }

    GaussianInt &operator+=(GaussianInt b)
    {
        // b 是加数；原地加 b 并返回自身。
        x += b.x;
        y += b.y;
        return *this;
    }

    GaussianInt &operator-=(GaussianInt b)
    {
        // b 是减数；原地减 b 并返回自身。
        x -= b.x;
        y -= b.y;
        return *this;
    }

    GaussianInt &operator*=(GaussianInt b)
    {
        // b 是乘数；原地乘 b 并返回自身，题目范围保证结果可由 64 位表示。
        __int128 nx = (__int128)x * b.x - (__int128)y * b.y;
        __int128 ny = (__int128)x * b.y + (__int128)y * b.x;
        x = (int)nx;
        y = (int)ny;
        return *this;
    }

    friend GaussianInt operator+(GaussianInt a, GaussianInt b)
    {
        // a、b 是高斯整数；返回 a+b。
        return a += b;
    }

    friend GaussianInt operator-(GaussianInt a, GaussianInt b)
    {
        // a、b 是高斯整数；返回 a-b。
        return a -= b;
    }

    friend GaussianInt operator*(GaussianInt a, GaussianInt b)
    {
        // a、b 是高斯整数；返回 a*b。
        return a *= b;
    }

    static int nearestDiv(__int128 a, __int128 b)
    {
        // a 是被除数，b 是正除数；返回距 a/b 最近的整数，半整数向正无穷取整。
        assert(b > 0);
        return (int)(a >= 0 ? (a + b / 2) / b : -((-a + b / 2) / b));
    }

    GaussianInt div(GaussianInt b) const
    {
        // b 非零；返回使余数范数严格小于 b 范数的一组商。
        assert(!b.zero());
        __int128 d = (__int128)b.x * b.x + (__int128)b.y * b.y;
        return {nearestDiv((__int128)x * b.x + (__int128)y * b.y, d),
                nearestDiv((__int128)y * b.x - (__int128)x * b.y, d)};
    }

    GaussianInt mod(GaussianInt b) const
    {
        // b 非零；返回当前值除以 b 的欧几里得余数。
        return *this - div(b) * b;
    }

    static GaussianInt gcd(GaussianInt a, GaussianInt b)
    {
        // a、b 是高斯整数；返回任意一个高斯整数最大公约数。
        while (!b.zero())
        {
            GaussianInt r = a.mod(b);
            a = b;
            b = r;
        }
        return a;
    }
};

signed main()
{
    int q; cin >> q;
    while (q--)
    {
        int a, b, c, d;
        cin >> a >> b >> c >> d;
        auto g = GaussianInt::gcd({a, b}, {c, d});
        cout << g.x << ' ' << g.y << '\n';
    }
}
