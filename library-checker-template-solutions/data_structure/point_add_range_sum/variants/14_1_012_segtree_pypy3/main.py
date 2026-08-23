import cmath
from collections import deque


class DSU:
    def __init__(self, n):
        # n 是点数；建立 n 个单点集合，无返回值。
        self.fa = list(range(n))
        self.siz = [1] * n

    def find(self, x):
        # x 是点编号；返回 x 所在集合的根并做路径压缩。
        while x != self.fa[x]:
            self.fa[x] = self.fa[self.fa[x]]
            x = self.fa[x]
        return x

    def merge(self, x, y):
        # x、y 是点编号；合并成功返回 True，原本连通返回 False。
        x, y = self.find(x), self.find(y)
        if x == y:
            return False
        if self.siz[x] < self.siz[y]:
            x, y = y, x
        self.fa[y] = x
        self.siz[x] += self.siz[y]
        return True


class SegTree:
    def __init__(self, a):
        # a 是初始数列；建立点修改、区间和线段树，无返回值。
        self.n = 1
        while self.n < len(a):
            self.n <<= 1
        self.tr = [0] * (2 * self.n)
        self.tr[self.n:self.n + len(a)] = a
        for p in range(self.n - 1, 0, -1):
            self.tr[p] = self.tr[p << 1] + self.tr[p << 1 | 1]

    def set(self, p, x):
        # p 是 0 下标位置，x 是新值；完成单点赋值，无返回值。
        p += self.n
        self.tr[p] = x
        while p > 1:
            p >>= 1
            self.tr[p] = self.tr[p << 1] + self.tr[p << 1 | 1]

    def prod(self, l, r):
        # l、r 定义半开区间；返回该区间元素和。
        l += self.n
        r += self.n
        x = y = 0
        while l < r:
            if l & 1:
                x += self.tr[l]
                l += 1
            if r & 1:
                r -= 1
                y += self.tr[r]
            l >>= 1
            r >>= 1
        return x + y


class Dinic:
    def __init__(self, n):
        # n 是点数；g 保存边的终点、反向边编号和剩余容量。
        self.n = n
        self.g = [[] for _ in range(n)]

    def add(self, u, v, c):
        # u、v 是端点，c 是非负容量；加入一条有向边，无返回值。
        assert 0 <= u < self.n and 0 <= v < self.n
        assert c >= 0
        a = [v, len(self.g[v]), c]
        b = [u, len(self.g[u]), 0]
        self.g[u].append(a)
        self.g[v].append(b)

    def flow(self, s, t):
        # s、t 是源汇；返回最大流并修改残量网络。
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
                # 无参数；在当前层次图中用显式栈找到一条增广路并返回流量。
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
        # n 是点数；g 保存终点、反向边编号、容量和单位费用。
        self.n = n
        self.g = [[] for _ in range(n)]

    def add(self, u, v, c, w):
        # u、v 是端点，c 是容量，w 是单位费用；加入有向边，无返回值。
        assert 0 <= u < self.n and 0 <= v < self.n
        assert c >= 0
        a = [v, len(self.g[v]), c, w]
        b = [u, len(self.g[u]), 0, -w]
        self.g[u].append(a)
        self.g[v].append(b)

    def flow(self, s, t, lim=None):
        # s、t 是源汇，lim 是可选流量上限；返回实际流量与最小费用。
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
    # a 是长度为二次幂的复数表，inv 表示逆变换；原地修改 a，无返回值。
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
    # a、b 是实数系数表；返回普通卷积的浮点近似值。
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
    # a 是模 mod 系数表，inv 表示逆变换，root 是原根；原地修改 a。
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
    # a、b 是模 mod 系数表；返回精确模卷积，模数与原根必须在受支持表中。
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
import sys


def main():
    data = list(map(int, sys.stdin.buffer.read().split()))
    it = iter(data)
    n, q = next(it), next(it)
    a = [next(it) for _ in range(n)]
    seg = SegTree(a)
    out = []
    for _ in range(q):
        t, x, y = next(it), next(it), next(it)
        if t == 0:
            a[x] += y
            seg.set(x, a[x])
        else:
            out.append(str(seg.prod(x, y)))
    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    main()
