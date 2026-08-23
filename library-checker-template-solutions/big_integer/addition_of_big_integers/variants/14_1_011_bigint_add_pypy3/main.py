import sys
from collections import deque

def main():
    if hasattr(sys, "set_int_max_str_digits"):
        sys.set_int_max_str_digits(0)
    data = sys.stdin.buffer.read().split()
    t = int(data[0])
    out = []
    for i in range(t):
        out.append(str(int(data[2 * i + 1]) + int(data[2 * i + 2])))
    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    main()
