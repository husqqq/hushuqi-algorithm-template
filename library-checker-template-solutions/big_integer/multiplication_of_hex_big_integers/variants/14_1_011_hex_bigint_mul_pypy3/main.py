import sys
from collections import deque

def hex_string(x):
    return ("-" if x < 0 else "") + format(abs(x), "X")


def main():
    data = sys.stdin.buffer.read().split()
    t = int(data[0])
    out = []
    for i in range(t):
        out.append(hex_string(int(data[2 * i + 1], 16) * int(data[2 * i + 2], 16)))
    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    main()
