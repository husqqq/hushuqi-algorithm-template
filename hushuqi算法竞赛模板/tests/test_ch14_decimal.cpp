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
    cout << "OK\n";
}
