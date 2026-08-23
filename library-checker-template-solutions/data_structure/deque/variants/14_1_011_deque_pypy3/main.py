import sys
from collections import deque

def main():
    data = sys.stdin.buffer.read().split()
    q = int(data[0])
    pos = 1
    a = deque()
    out = []
    for _ in range(q):
        t = int(data[pos])
        pos += 1
        if t == 0:
            a.appendleft(int(data[pos]))
            pos += 1
        elif t == 1:
            a.append(int(data[pos]))
            pos += 1
        elif t == 2:
            a.popleft()
        elif t == 3:
            a.pop()
        else:
            out.append(str(a[int(data[pos])]))
            pos += 1
    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    main()
