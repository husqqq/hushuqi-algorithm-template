import sys

x = [float(v) for v in open(sys.argv[1]).read().split()]
y = [float(v) for v in open(sys.argv[2]).read().split()]
ok = len(x) == len(y) and all(
    abs(a - b) <= 1e-6 * max(1.0, b)
    for a, b in zip(x, y)
)
raise SystemExit(not ok)
