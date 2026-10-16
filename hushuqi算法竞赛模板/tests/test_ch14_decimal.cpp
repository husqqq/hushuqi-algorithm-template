#include "../include/full/14_1_020.hpp"

signed main()
{
    Decimal::setPrecision(6);
    assert((Decimal("1.25") + Decimal("-0.5")).str() == "0.75");
    assert((Decimal("1.2") * Decimal("3.4")).str() == "4.08");
    assert((Decimal(1) / Decimal(8)).str(6) == "0.125000");
    assert((Decimal(2) / Decimal(3)).str(6) == "0.666667");
    assert((Decimal("-5.5") % Decimal(2)).str() == "-1.5");
    assert((Decimal("5.5") % Decimal(-2)).str() == "1.5");
    assert(Decimal("-1.9").trunc() == Decimal(-1));
    assert(Decimal::pow(Decimal("1.5"), 3).str() == "3.375");
    assert(Decimal("1.23456789").str() == "1.234568");

    Decimal::setPrecision(2);
    assert(Decimal("0.005").str() == "0.01");
    assert(Decimal("-0.005").str() == "-0.01");

    Decimal::setPrecision(30);
    assert(Decimal(2).sqrt().str(30) == "1.414213562373095048801688724210");
    assert(Decimal("0.0004").sqrt().str() == "0.02");
    Decimal a("5.55"), b("0.11");
    Decimal::setPrecision(0);
    assert((a % b).str() == "0.05");
    assert((-a % b).str() == "-0.05");
    assert((a % -b).str() == "0.05");
    assert((-a % -b).str() == "-0.05");
    assert((b % a).str() == "0.11");
    assert((a % a).isZero());
    mt19937_64 rng(14020);
    for (int i = 0; i < 600; i++)
    {
        long long x = (long long)(rng() % 20000001) - 10000000;
        long long y = (long long)(rng() % 20000000) - 10000000;
        if (!y) y = 1;
        Decimal::setPrecision(6);
        Decimal a = Decimal(x) / Decimal(1000000);
        Decimal b = Decimal(y) / Decimal(1000000);
        Decimal rem = Decimal(x % y) / Decimal(1000000);
        Decimal::setPrecision(i % 3);
        assert(a % b == rem);
        assert((a % b).abs() < b.abs());
    }
    cout << "OK\n";
}
