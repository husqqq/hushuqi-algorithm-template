import cmath
import random
from collections import deque


class Dinic:
    def __init__(self, n):
        self.n = n
        self.g = [[] for _ in range(n)]

    def add(self, u, v, c):
        assert 0 <= u < self.n and 0 <= v < self.n
        assert c >= 0
        a = [v, len(self.g[v]), c]
        b = [u, len(self.g[u]), 0]
        self.g[u].append(a)
        self.g[v].append(b)

    def flow(self, s, t):
        assert 0 <= s < self.n and 0 <= t < self.n and s != t
        ans = 0
        while True:
            dep = [-1] * self.n
            dep[s] = 0
            q = deque([s])
            while q:
                u = q.popleft()
                for v, _, c in self.g[u]:
                    if c and dep[v] < 0:
                        dep[v] = dep[u] + 1
                        q.append(v)
            if dep[t] < 0:
                return ans
            cur = [0] * self.n

            def send():
                path_v = [s]
                path_e = []
                while path_v:
                    u = path_v[-1]
                    if u == t:
                        f = min(self.g[x][i][2] for x, i in path_e)
                        for x, i in path_e:
                            e = self.g[x][i]
                            e[2] -= f
                            self.g[e[0]][e[1]][2] += f
                        return f
                    while cur[u] < len(self.g[u]):
                        e = self.g[u][cur[u]]
                        if e[2] and dep[e[0]] == dep[u] + 1:
                            break
                        cur[u] += 1
                    if cur[u] == len(self.g[u]):
                        path_v.pop()
                        if path_e:
                            x, _ = path_e.pop()
                            cur[x] += 1
                        continue
                    path_e.append((u, cur[u]))
                    path_v.append(self.g[u][cur[u]][0])
                return 0

            while True:
                f = send()
                if not f:
                    break
                ans += f


class MinCostFlow:
    def __init__(self, n):
        self.n = n
        self.g = [[] for _ in range(n)]

    def add(self, u, v, c, w):
        assert 0 <= u < self.n and 0 <= v < self.n
        assert c >= 0
        a = [v, len(self.g[v]), c, w]
        b = [u, len(self.g[u]), 0, -w]
        self.g[u].append(a)
        self.g[v].append(b)

    def flow(self, s, t, lim=None):
        n = self.n
        assert 0 <= s < n and 0 <= t < n and s != t
        assert lim is None or lim >= 0
        total = 0
        cost = 0
        while lim is None or total < lim:
            dis = [None] * n
            pre = [None] * n
            inq = [False] * n
            depth = [0] * n
            dis[s] = 0
            q = deque([s])
            inq[s] = True
            while q:
                u = q.popleft()
                inq[u] = False
                for i, e in enumerate(self.g[u]):
                    v, _, c, w = e
                    nd = dis[u] + w
                    if c and (dis[v] is None or nd < dis[v]):
                        dis[v] = nd
                        pre[v] = (u, i)
                        depth[v] = depth[u] + 1
                        if depth[v] >= n:
                            raise ValueError("source-reachable negative cycle")
                        if not inq[v]:
                            inq[v] = True
                            q.append(v)
            if pre[t] is None:
                break
            f = None if lim is None else lim - total
            v = t
            while v != s:
                u, i = pre[v]
                c = self.g[u][i][2]
                f = c if f is None else min(f, c)
                v = u
            v = t
            while v != s:
                u, i = pre[v]
                e = self.g[u][i]
                e[2] -= f
                self.g[v][e[1]][2] += f
                cost += f * e[3]
                v = u
            total += f
        return total, cost


def fft(a, inv=False):
    n = len(a)
    if n == 0 or n & (n - 1):
        raise ValueError("FFT length must be a nonzero power of two")
    j = 0
    for i in range(1, n):
        bit = n >> 1
        while j & bit:
            j ^= bit
            bit >>= 1
        j ^= bit
        if i < j:
            a[i], a[j] = a[j], a[i]
    length = 2
    while length <= n:
        wn = cmath.exp((2j if inv else -2j) * cmath.pi / length)
        for l in range(0, n, length):
            w = 1
            half = length >> 1
            for i in range(l, l + half):
                x = a[i]
                y = a[i + half] * w
                a[i] = x + y
                a[i + half] = x - y
                w *= wn
        length <<= 1
    if inv:
        for i in range(n):
            a[i] /= n


def conv_fft(a, b):
    if not a or not b:
        return []
    n = 1
    while n < len(a) + len(b) - 1:
        n <<= 1
    x = list(map(complex, a)) + [0j] * (n - len(a))
    y = list(map(complex, b)) + [0j] * (n - len(b))
    fft(x)
    fft(y)
    for i in range(n):
        x[i] *= y[i]
    fft(x, True)
    return [x[i].real for i in range(len(a) + len(b) - 1)]


def ntt(a, inv=False, mod=998244353, root=3):
    n = len(a)
    supported = {(998244353, 3), (1004535809, 3), (469762049, 3)}
    if (mod, root) not in supported:
        raise ValueError("unsupported NTT modulus or primitive root")
    if n == 0 or n & (n - 1) or (mod - 1) % n:
        raise ValueError("invalid NTT length")
    j = 0
    for i in range(1, n):
        bit = n >> 1
        while j & bit:
            j ^= bit
            bit >>= 1
        j ^= bit
        if i < j:
            a[i], a[j] = a[j], a[i]
    length = 2
    while length <= n:
        wn = pow(root, (mod - 1) // length, mod)
        if inv:
            wn = pow(wn, mod - 2, mod)
        for l in range(0, n, length):
            w = 1
            half = length >> 1
            for i in range(l, l + half):
                x = a[i]
                y = a[i + half] * w % mod
                a[i] = (x + y) % mod
                a[i + half] = (x - y) % mod
                w = w * wn % mod
        length <<= 1
    if inv:
        ni = pow(n, mod - 2, mod)
        for i in range(n):
            a[i] = a[i] * ni % mod


def conv_ntt(a, b, mod=998244353, root=3):
    if not a or not b:
        return []
    n = 1
    while n < len(a) + len(b) - 1:
        n <<= 1
    x = [v % mod for v in a] + [0] * (n - len(a))
    y = [v % mod for v in b] + [0] * (n - len(b))
    ntt(x, mod=mod, root=root)
    ntt(y, mod=mod, root=root)
    for i in range(n):
        x[i] = x[i] * y[i] % mod
    ntt(x, True, mod, root)
    return x[:len(a) + len(b) - 1]


rng = random.Random(0)
for n in range(2, 8):
    for _ in range(30):
        edges = []
        flow = Dinic(n)
        for u in range(n):
            for v in range(n):
                if u != v and rng.randrange(4) == 0:
                    c = rng.randrange(6)
                    edges.append((u, v, c))
                    flow.add(u, v, c)
        got = flow.flow(0, n - 1)
        want = None
        for mask in range(1 << n):
            if not (mask & 1) or mask >> (n - 1) & 1:
                continue
            cut = sum(c for u, v, c in edges
                      if mask >> u & 1 and not (mask >> v & 1))
            want = cut if want is None else min(want, cut)
        assert got == want

chain = Dinic(1500)
for i in range(1499):
    chain.add(i, i + 1, 10**120)
assert chain.flow(0, 1499) == 10**120

mcf = MinCostFlow(3)
mcf.add(0, 1, 2, 10**101)
mcf.add(1, 2, 2, -3)
assert mcf.flow(0, 2) == (2, 2 * (10**101 - 3))

got = conv_fft([1, 2, 3], [4, -1])
assert all(abs(x - y) < 1e-8 for x, y in zip(got, [4, 7, 10, -3]))
for _ in range(100):
    a = [rng.randrange(-20, 21) for _ in range(rng.randrange(1, 20))]
    b = [rng.randrange(-20, 21) for _ in range(rng.randrange(1, 20))]
    got = conv_ntt(a, b)
    want = [0] * (len(a) + len(b) - 1)
    for i, x in enumerate(a):
        for j, y in enumerate(b):
            want[i + j] = (want[i + j] + x * y) % 998244353
    assert got == want

print("OK")
