#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

struct Node {
    i64 value, sum;
    unsigned priority = rng();
    int size = 1;
    bool reversed = false;
    Node* left = nullptr;
    Node* right = nullptr;

    Node(i64 value) : value(value), sum(value) {}
};

int size(Node* node) {
    return node ? node->size : 0;
}

i64 sum(Node* node) {
    return node ? node->sum : 0;
}

void applyReverse(Node* node) {
    if (node) {
        swap(node->left, node->right);
        node->reversed ^= 1;
    }
}

void push(Node* node) {
    if (node && node->reversed) {
        applyReverse(node->left);
        applyReverse(node->right);
        node->reversed = false;
    }
}

void pull(Node* node) {
    node->size = 1 + size(node->left) + size(node->right);
    node->sum = sum(node->left) + node->value + sum(node->right);
}

void split(Node* node, int k, Node*& left, Node*& right) {
    if (!node) {
        left = right = nullptr;
        return;
    }
    push(node);
    if (size(node->left) >= k) {
        split(node->left, k, left, node->left);
        right = node;
        pull(right);
    } else {
        split(node->right, k - size(node->left) - 1, node->right, right);
        left = node;
        pull(left);
    }
}

Node* merge(Node* left, Node* right) {
    if (!left || !right) {
        return left ? left : right;
    }
    if (left->priority < right->priority) {
        push(left);
        left->right = merge(left->right, right);
        pull(left);
        return left;
    }
    push(right);
    right->left = merge(left, right->left);
    pull(right);
    return right;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    Node* root = nullptr;
    for (int i = 0; i < n; i++) {
        i64 x;
        cin >> x;
        root = merge(root, new Node(x));
    }
    while (q--) {
        int op, l, r;
        cin >> op >> l >> r;
        Node *left, *middle, *right;
        split(root, r, middle, right);
        split(middle, l, left, middle);
        if (op == 0) {
            applyReverse(middle);
        } else {
            cout << sum(middle) << '\n';
        }
        root = merge(merge(left, middle), right);
    }
    return 0;
}
