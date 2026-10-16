#pragma once

#include "14_1_001.hpp"

struct Date
{
    // y 保存公历年份；完整 64 位日序对应的年份可能超过 32 位。
    long long y;
    // m 保存公历月份。
    int m;
    // d 保存公历月内日期。
    int d;

    static bool leap(long long y)
    {
        // y 是公历年份；返回该年是否为闰年。
        return y % 4 == 0 && (y % 100 != 0 || y % 400 == 0);
    }

    static int monthDays(long long y, int m)
    {
        // y、m 是公历年份和月份；返回该月的天数。
        static constexpr array<int, 13> a = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        assert(1 <= m && m <= 12); // 调试检查，可删。
        return a[m] + (m == 2 && leap(y));
    }

    bool valid() const
    {
        // 无参数；返回当前年月日是否为合法公历日期。
        return 1 <= m && m <= 12 && 1 <= d && d <= monthDays(y, m);
    }

    // 与 1970-01-01 相差的天数，可为负。
    long long days() const
    {
        // 无参数；返回当前日期与 1970-01-01 相差的天数。
        assert(valid()); // 调试检查，可删。
        __int128 yy = (__int128)y - (m <= 2);
        __int128 era = (yy >= 0 ? yy : yy - 399) / 400;
        __int128 yo = yy - era * 400;
        __int128 mp = m + (m > 2 ? -3 : 9);
        __int128 dy = (153 * mp + 2) / 5 + d - 1;
        __int128 de = yo * 365 + yo / 4 - yo / 100 + dy;
        __int128 z = era * 146097 + de - 719468;
        assert(LLONG_MIN <= z && z <= LLONG_MAX); // 调试检查，可删。
        return (long long)z;
    }

    // days() 的逆变换。
    static Date fromDays(long long z)
    {
        // z 是相对 1970-01-01 的天数；返回对应公历日期。
        __int128 zz = (__int128)z + 719468;
        __int128 era = (zz >= 0 ? zz : zz - 146096) / 146097;
        __int128 de = zz - era * 146097;
        __int128 yo = (de - de / 1460 + de / 36524 - de / 146096) / 365;
        __int128 yy = yo + era * 400;
        __int128 dy = de - (365 * yo + yo / 4 - yo / 100);
        __int128 mp = (5 * dy + 2) / 153;
        __int128 dd = dy - (153 * mp + 2) / 5 + 1;
        __int128 mm = mp + (mp < 10 ? 3 : -9);
        yy += mm <= 2;
        return {(long long)yy, (int)mm, (int)dd};
    }

    // 周日为 0，周一为 1，...，周六为 6。
    int weekday() const
    {
        // 无参数；返回当前日期的星期编号，周日为 0。
        return (int)((days() % 7 + 11) % 7);
    }
};
