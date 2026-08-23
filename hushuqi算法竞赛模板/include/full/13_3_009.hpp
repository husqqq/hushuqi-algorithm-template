#pragma once

#include "13_1_001.hpp"

template <class T> struct Complex
{
    T a; // 实部。
    T b; // 虚部的系数。

    Complex(const T &a_ = T(), const T &b_ = T()) : a(a_), b(b_)
    {
        // a_ 是实部，b_ 是虚部的系数；构造 a_+b_*i。
    }

    Complex operator+(const Complex &rhs) const
    {
        // rhs 是加数；返回当前复数与 rhs 的和。
        return {a + rhs.a, b + rhs.b};
    }

    Complex operator-(const Complex &rhs) const
    {
        // rhs 是减数；返回当前复数减 rhs。
        return {a - rhs.a, b - rhs.b};
    }

    Complex operator*(const Complex &rhs) const
    {
        // rhs 是乘数；返回当前复数与 rhs 的积。
        return {a * rhs.a - b * rhs.b, a * rhs.b + b * rhs.a};
    }

    Complex operator/(const Complex &rhs) const
    {
        // rhs 是除数；返回当前复数除以 rhs，要求 rhs 的范数在 T 中可逆。
        return *this * rhs.inv();
    }

    Complex &operator+=(const Complex &rhs)
    {
        // rhs 是加数；把当前复数增加 rhs 并返回自身引用。
        return *this = *this + rhs;
    }

    Complex &operator-=(const Complex &rhs)
    {
        // rhs 是减数；把当前复数减去 rhs 并返回自身引用。
        return *this = *this - rhs;
    }

    Complex &operator*=(const Complex &rhs)
    {
        // rhs 是乘数；把当前复数乘以 rhs 并返回自身引用。
        return *this = *this * rhs;
    }

    Complex &operator/=(const Complex &rhs)
    {
        // rhs 是除数；把当前复数除以 rhs 并返回自身引用，要求 rhs 的范数在 T 中可逆。
        return *this = *this / rhs;
    }

    bool operator==(const Complex &rhs) const
    {
        // rhs 是待比较复数；返回两个系数是否分别相等。
        return a == rhs.a && b == rhs.b;
    }

    bool operator!=(const Complex &rhs) const
    {
        // rhs 是待比较复数；返回两个复数是否不等。
        return !(*this == rhs);
    }

    bool isZero() const
    {
        // 返回当前复数是否为加法零元。
        return a == T() && b == T();
    }

    Complex conj() const
    {
        // 返回当前复数的共轭。
        return {a, -b};
    }

    T norm() const
    {
        // 返回当前复数与其共轭之积 a^2+b^2。
        return a * a + b * b;
    }

    Complex inv() const
    {
        // 返回当前复数的乘法逆元，要求范数在 T 中非零且可逆。
        T n = norm();
        assert(n != T());
        return {a / n, -b / n};
    }
};

template <class T> Complex<T> conj(const Complex<T> &z)
{
    // z 是待共轭复数；返回 z 的共轭。
    return z.conj();
}

template <class T> struct Complex3
{
    T a; // 常数项的系数。
    T b; // 三次单位根 omega 的系数。

    Complex3(const T &a_ = T(), const T &b_ = T()) : a(a_), b(b_)
    {
        // a_ 是常数项系数，b_ 是 omega 的系数；构造 a_+b_*omega。
    }

    Complex3 operator+(const Complex3 &rhs) const
    {
        // rhs 是加数；返回当前元素与 rhs 的和。
        return {a + rhs.a, b + rhs.b};
    }

    Complex3 operator-(const Complex3 &rhs) const
    {
        // rhs 是减数；返回当前元素减 rhs。
        return {a - rhs.a, b - rhs.b};
    }

    Complex3 operator*(const Complex3 &rhs) const
    {
        // rhs 是乘数；返回按 omega^2=-1-omega 化简后的积。
        T bd = b * rhs.b;
        return {a * rhs.a - bd, a * rhs.b + b * rhs.a - bd};
    }

    Complex3 operator/(const Complex3 &rhs) const
    {
        // rhs 是除数；返回当前元素除以 rhs，要求 rhs 的范数在 T 中可逆。
        return *this * rhs.inv();
    }

    Complex3 &operator+=(const Complex3 &rhs)
    {
        // rhs 是加数；把当前元素增加 rhs 并返回自身引用。
        return *this = *this + rhs;
    }

    Complex3 &operator-=(const Complex3 &rhs)
    {
        // rhs 是减数；把当前元素减去 rhs 并返回自身引用。
        return *this = *this - rhs;
    }

    Complex3 &operator*=(const Complex3 &rhs)
    {
        // rhs 是乘数；把当前元素乘以 rhs 并返回自身引用。
        return *this = *this * rhs;
    }

    Complex3 &operator/=(const Complex3 &rhs)
    {
        // rhs 是除数；把当前元素除以 rhs 并返回自身引用，要求 rhs 的范数在 T 中可逆。
        return *this = *this / rhs;
    }

    bool operator==(const Complex3 &rhs) const
    {
        // rhs 是待比较元素；返回两个系数是否分别相等。
        return a == rhs.a && b == rhs.b;
    }

    bool operator!=(const Complex3 &rhs) const
    {
        // rhs 是待比较元素；返回两个元素是否不等。
        return !(*this == rhs);
    }

    bool isZero() const
    {
        // 返回当前元素是否为加法零元。
        return a == T() && b == T();
    }

    Complex3 conj() const
    {
        // 返回把 omega 替换为 omega^2 后的共轭元素。
        return {a - b, -b};
    }

    T norm() const
    {
        // 返回当前元素与其共轭之积 a^2-a*b+b^2。
        return a * a - a * b + b * b;
    }

    Complex3 inv() const
    {
        // 返回当前元素的乘法逆元，要求范数在 T 中非零且可逆。
        T n = norm();
        assert(n != T());
        return {(a - b) / n, -b / n};
    }
};

template <class T> Complex3<T> conj(const Complex3<T> &z)
{
    // z 是待共轭元素；返回 z 的共轭。
    return z.conj();
}
