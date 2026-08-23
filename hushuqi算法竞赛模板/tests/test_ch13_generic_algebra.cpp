#include "../include/full/13_3_009.hpp"

signed main()
{
    Complex<int> x(3, 4), y(1, -2);
    assert(x + y == Complex<int>(4, 2));
    assert(x - y == Complex<int>(2, 6));
    assert(x * y == Complex<int>(11, -2));
    assert(x.norm() == 25);
    assert(conj(x) == Complex<int>(3, -4));
    assert((x * conj(x)) == Complex<int>(25, 0));
    assert(!x.isZero() && Complex<int>().isZero());
    auto xi = x;
    xi += y;
    xi -= y;
    assert(xi == x);

    Complex<long double> xf(3, 4), yf(1, -2);
    auto q = xf;
    q /= yf;
    assert(fabsl(q.a + 1) < 1E-18L);
    assert(fabsl(q.b - 2) < 1E-18L);
    q *= yf;
    assert(fabsl(q.a - xf.a) < 1E-18L);
    assert(fabsl(q.b - xf.b) < 1E-18L);
    auto yi = yf.inv();
    assert(fabsl((yf * yi).a - 1) < 1E-18L);
    assert(fabsl((yf * yi).b) < 1E-18L);

    Complex3<int> w(0, 1);
    assert(w * w == Complex3<int>(-1, -1));
    assert(w * w * w == Complex3<int>(1, 0));
    Complex3<int> z(2, 3), t(4, -1);
    assert(z + t == Complex3<int>(6, 2));
    assert(z - t == Complex3<int>(-2, 4));
    assert(z * t == Complex3<int>(11, 13));
    assert(z.norm() == 7);
    assert(conj(z) == Complex3<int>(-1, -3));
    assert(z * conj(z) == Complex3<int>(7, 0));
    assert(!z.isZero() && Complex3<int>().isZero());
    auto zi = z;
    zi += t;
    zi -= t;
    assert(zi == z);

    Complex3<long double> zf(2, 3), tf(4, -1);
    auto r = zf;
    r /= tf;
    r *= tf;
    assert(fabsl(r.a - zf.a) < 1E-18L);
    assert(fabsl(r.b - zf.b) < 1E-18L);
    auto ti = tf.inv();
    assert(fabsl((tf * ti).a - 1) < 1E-18L);
    assert(fabsl((tf * ti).b) < 1E-18L);
    assert(zf != tf);
    return 0;
}
