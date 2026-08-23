class Splay:
    def __init__(self):
        # 无参数；建立空伸展树，各列表第 0 项表示空节点。
        self.ch = [[0, 0]]
        self.fa = [0]
        self.val = [0]
        self.cnt = [0]
        self.siz = [0]
        self.rt = 0
        self.free = []

    def _new(self, x, f):
        # x 是键值，f 是父节点；创建节点并返回编号。
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
        # u 是已从树中断开的节点；清空并回收其编号，无返回值。
        self.ch[u] = [0, 0]
        self.fa[u] = 0
        self.cnt[u] = 0
        self.siz[u] = 0
        self.free.append(u)

    def _pull(self, u):
        # u 是节点编号；根据儿子和重复计数重算子树大小，无返回值。
        self.siz[u] = self.siz[self.ch[u][0]] + self.siz[self.ch[u][1]] + self.cnt[u]

    def _rotate(self, x):
        # x 是待上旋节点；完成一次旋转并维护父子关系，无返回值。
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
        # x 是目标节点，goal 是目标父亲；把 x 旋到 goal 下，无返回值。
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
        # x 是键值；若存在则伸展到根并返回节点，否则伸展最后访问点并返回 0。
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
        # x 是键值；插入一个 x，无返回值。
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
        # x 是键值；删除一个 x，成功返回 True，不存在返回 False。
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
        # k 是从 0 开始的排名；返回对应键值，越界返回 None。
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
        # x 是查询值；返回严格小于 x 的元素个数，并伸展最后访问节点。
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
import sys


def main():
    data = list(map(int, sys.stdin.buffer.read().split()))
    it = iter(data)
    n, q = next(it), next(it)
    tree = Splay()
    for _ in range(n):
        tree.insert(next(it))
    out = []
    for _ in range(q):
        t, x = next(it), next(it)
        if t == 0:
            if tree._find(x) == 0:
                tree.insert(x)
        elif t == 1:
            tree.erase(x)
        elif t == 2:
            value = tree.kth(x - 1)
            out.append(str(-1 if value is None else value))
        elif t == 3:
            out.append(str(tree.rank(x + 1)))
        elif t == 4:
            value = tree.kth(tree.rank(x + 1) - 1)
            out.append(str(-1 if value is None else value))
        else:
            value = tree.kth(tree.rank(x))
            out.append(str(-1 if value is None else value))
    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    main()
