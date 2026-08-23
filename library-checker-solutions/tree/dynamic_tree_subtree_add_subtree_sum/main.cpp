#include <bits/stdc++.h>

// Library Checker official reference implementation, adapted to this template's style.
// Problem: dynamic_tree_subtree_add_subtree_sum
using namespace std;
int ri() {
    int n;
    scanf("%d", &n);
    return n;
}
struct Node;
extern Node* NONE;
struct Node {
    Node* p = NONE;
    Node* ch[2] = {NONE, NONE};
    int64_t val = 0;
    int size = 0;
    int size_light = 0; // LIGHT
    int64_t sum = 0;
    int64_t sum_light = 0; // LIGHT
    int64_t added = 0;
    int64_t cancel = 0;
    bool rev = false;
    void flush() {
        if (p != NONE) {
            add(p->added - cancel);
            cancel = p->added;
        }
        if (rev) {
            ch[0]->rev ^= 1;
            ch[1]->rev ^= 1;
            swap(ch[0], ch[1]);
            rev = false;
        }
    }
    void fetch() {
        if (ch[0] != NONE) ch[0]->flush();
        if (ch[1] != NONE) ch[1]->flush();
        size = 1 + ch[0]->size + ch[1]->size + size_light;
        sum = val + ch[0]->sum + ch[1]->sum + sum_light;
    }
    void add(int64_t add_val) {
        val += add_val;
        sum += (int64_t)size * add_val;
        sum_light += (int64_t)size_light * add_val;
        added += add_val;
    }
    void rotate(int dir) {
        Node* new_root = ch[!dir];
        if (new_root->ch[dir] != NONE) new_root->ch[dir]->flush();
        ch[!dir] = new_root->ch[dir];
        ch[!dir]->p = this;
        ch[!dir]->cancel = added;
        new_root->ch[dir] = this;
        new_root->p = p;
        new_root->cancel = p->added;
        if (p->ch[0] == this) p->ch[0] = new_root;
        if (p->ch[1] == this) p->ch[1] = new_root;
        p = new_root;
        cancel = new_root->added;
        return fetch(), new_root->fetch();
    }
    bool is_root() { return p == NONE || (p->ch[0] != this && p->ch[1] != this); }
    void splay() {
        while (!is_root()) {
            if (p->is_root()) {
                p->flush(), flush();
                p->rotate(p->ch[0] == this);
            } else {
                Node* pp = p->p;
                pp->flush(), p->flush(), flush();
                bool flag0 = pp->ch[0] == p;
                bool flag1 = p->ch[0] == this;
                if (flag0 == flag1) pp->rotate(flag0);
                p->rotate(flag1);
                if (flag0 != flag1) pp->rotate(flag0);
            }
        }
        flush();
    }
    Node* expose() {
        Node* prev = NONE;
        for (Node* cur = this; cur != NONE; cur = cur->p) {
            cur->splay();
            if (cur->ch[1] != NONE) { // add
                cur->ch[1]->flush();
                cur->size_light += cur->ch[1]->size;
                cur->sum_light += cur->ch[1]->sum;
            }
            cur->ch[1] = prev;
            if (cur->ch[1] != NONE) { // remove
                cur->ch[1]->flush();
                cur->size_light -= cur->ch[1]->size;
                cur->sum_light -= cur->ch[1]->sum;
            }
            cur->fetch();
            prev = cur;
        }
        splay();
        return prev;
    }
    void link(Node* parent) {
        expose();
        parent->expose();
        parent->ch[1] = this;
        p = parent;
        cancel = parent->added;
        parent->fetch();
    }
    void cut() {
        expose();
        ch[0]->flush();
        ch[0]->p = NONE;
        ch[0] = NONE;
        fetch();
    }
    void evert() {
        expose();
        rev ^= 1;
        flush();
    }
};
Node* NONE = new Node;
int main() {
    int n = ri(), q = ri();
    vector<Node> nodes(n);
    for (auto& i : nodes) i.val = ri(), i.fetch();
    for (int i = 1; i < n; i++) {
        int x = ri();
        int y = ri();
        nodes[x].evert();
        nodes[x].link(&nodes[y]);
    }
    for (int i = 0; i < q; i++) {
        int t = ri();
        if (t == 0) {
            int x = ri();
            int y = ri();
            nodes[x].evert();
            nodes[y].cut();
            x = ri();
            y = ri();
            nodes[x].evert();
            nodes[x].link(&nodes[y]);
        } else {
            int x = ri();
            int p = ri();
            nodes[p].evert();
            nodes[x].cut();
            if (t == 1) nodes[x].add(ri());
            else printf("%lld\n", (long long)nodes[x].sum);
            nodes[x].link(&nodes[p]);
        }
    }
    return 0;
}
