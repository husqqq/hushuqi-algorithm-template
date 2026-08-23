import sys
from collections import deque

def hex_string(x):
    return format(x, "X")


def main():
    data = sys.stdin.buffer.read().split()
    t = int(data[0])
    out = []
    for i in range(t):
        a = int(data[2 * i + 1], 16)
        b = int(data[2 * i + 2], 16)
        q, r = divmod(a, b)
        out.append(f"{hex_string(q)} {hex_string(r)}")
    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    main()
