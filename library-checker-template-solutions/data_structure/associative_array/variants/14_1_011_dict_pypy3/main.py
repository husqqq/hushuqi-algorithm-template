import sys
from collections import deque

def main():
    data = list(map(int, sys.stdin.buffer.read().split()))
    q = data[0]
    pos = 1
    value = {}
    out = []
    for _ in range(q):
        t, key = data[pos], data[pos + 1]
        pos += 2
        if t == 0:
            value[key] = data[pos]
            pos += 1
        else:
            out.append(str(value.get(key, 0)))
    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    main()
