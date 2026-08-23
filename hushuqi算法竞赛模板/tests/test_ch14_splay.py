class Splay:
    def __init__(self):
        self.ch = [[0, 0]]
        self.fa = [0]
        self.val = [0]
        self.cnt = [0]
        self.siz = [0]
        self.rt = 0
        self.free = []

    def _new(self, x, f):
        if self.free:
            u = self.free.pop()
            self.ch[u] = [0, 0]
            self.fa[u] = f
            self.val[u] = x
            self.cnt[u] = 1
            self.siz[u] = 1
            return u
        self.ch.append([0, 0])
        self.fa.append(f)
        self.val.append(x)
        self.cnt.append(1)
        self.siz.append(1)
        return len(self.val) - 1

    def _recycle(self, u):
        self.ch[u] = [0, 0]
        self.fa[u] = 0
        self.cnt[u] = 0
        self.siz[u] = 0
        self.free.append(u)

    def _pull(self, u):
        self.siz[u] = self.siz[self.ch[u][0]] + self.siz[self.ch[u][1]] + self.cnt[u]

    def _rotate(self, x):
        y = self.fa[x]
        z = self.fa[y]
        d = int(self.ch[y][1] == x)
        b = self.ch[x][d ^ 1]
        if z:
            self.ch[z][int(self.ch[z][1] == y)] = x
        else:
            self.rt = x
        self.fa[x] = z
        self.ch[x][d ^ 1] = y
        self.fa[y] = x
        self.ch[y][d] = b
        if b:
            self.fa[b] = y
        self._pull(y)
        self._pull(x)

    def _splay(self, x, goal=0):
        while self.fa[x] != goal:
            y = self.fa[x]
            z = self.fa[y]
            if z != goal:
                if (self.ch[y][1] == x) == (self.ch[z][1] == y):
                    self._rotate(y)
                else:
                    self._rotate(x)
            self._rotate(x)

    def _find(self, x):
        u = self.rt
        last = 0
        while u and self.val[u] != x:
            last = u
            u = self.ch[u][x > self.val[u]]
        if u:
            self._splay(u)
        elif last:
            self._splay(last)
        return u

    def insert(self, x):
        if not self.rt:
            self.rt = self._new(x, 0)
            return
        u = self.rt
        while True:
            if self.val[u] == x:
                self.cnt[u] += 1
                self._pull(u)
                self._splay(u)
                return
            d = int(x > self.val[u])
            if not self.ch[u][d]:
                v = self._new(x, u)
                self.ch[u][d] = v
                self._pull(u)
                self._splay(v)
                return
            u = self.ch[u][d]

    def erase(self, x):
        u = self._find(x)
        if not u:
            return False
        if self.cnt[u] > 1:
            self.cnt[u] -= 1
            self._pull(u)
            return True
        l, r = self.ch[u]
        if not l:
            self.rt = r
            if r:
                self.fa[r] = 0
            self._recycle(u)
            return True
        self.rt = l
        self.fa[l] = 0
        v = l
        while self.ch[v][1]:
            v = self.ch[v][1]
        self._splay(v)
        self.ch[v][1] = r
        if r:
            self.fa[r] = v
        self._pull(v)
        self._recycle(u)
        return True

    def kth(self, k):
        if k < 0 or k >= self.siz[self.rt]:
            return None
        u = self.rt
        while True:
            left = self.siz[self.ch[u][0]]
            if k < left:
                u = self.ch[u][0]
            elif k < left + self.cnt[u]:
                self._splay(u)
                return self.val[u]
            else:
                k -= left + self.cnt[u]
                u = self.ch[u][1]

    def rank(self, x):
        u = self.rt
        last = 0
        ans = 0
        while u:
            last = u
            if x <= self.val[u]:
                u = self.ch[u][0]
            else:
                ans += self.siz[self.ch[u][0]] + self.cnt[u]
                u = self.ch[u][1]
        if last:
            self._splay(last)
        return ans


t = Splay()
for x in [3, 1, 3, 2]:
    t.insert(x)
assert [t.kth(i) for i in range(4)] == [1, 2, 3, 3]
assert t.rank(3) == 2
assert t.erase(3) and [t.kth(i) for i in range(3)] == [1, 2, 3]
assert t.erase(3) and not t.erase(3)
for x in range(1000):
    t.insert(x + 10)
    assert t.erase(x + 10)
assert len(t.val) == 4
print("OK")
