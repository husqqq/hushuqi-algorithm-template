#include <bits/stdc++.h>

using namespace std;

constexpr int P = 998244353;

struct Func {
    int a = 1;
    int b = 0;
};

Func operator+(const Func& left, const Func& right) {
    return {
        int(1LL * right.a * left.a % P),
        int((1LL * right.a * left.b + right.b) % P),
    };
}

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

struct Node {
    Func value, sum;
    unsigned priority = rng();
    int size = 1;
    Node* left = nullptr;
    Node* right = nullptr;

    Node(Func value) : value(value), sum(value) {}
};

int size(Node* node) {
    return node ? node->size : 0;
}

Func sum(Node* node) {
    return node ? node->sum : Func{};
}

void pull(Node* node) {
    if (node) {
        node->size = 1 + size(node->left) + size(node->right);
        node->sum = sum(node->left) + node->value + sum(node->right);
    }
}

void split(Node* node, int k, Node*& left, Node*& right) {
    if (!node) {
        left = right = nullptr;
        return;
    }
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
        left->right = merge(left->right, right);
        pull(left);
        return left;
    }
    right->left = merge(left, right->left);
    pull(right);
    return right;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;
    Node* root = nullptr;
    while (q--) {
        int op;
        cin >> op;
        if (op == 0 || op == 1) {
            int a, b;
            cin >> a >> b;
            Node* node = new Node({a, b});
            root = op == 0 ? merge(node, root) : merge(root, node);
        } else if (op == 2) {
            Node* removed;
            split(root, 1, removed, root);
            delete removed;
        } else if (op == 3) {
            Node* removed;
            split(root, size(root) - 1, root, removed);
            delete removed;
        } else {
            int x;
            cin >> x;
            auto f = sum(root);
            cout << (1LL * f.a * x + f.b) % P << '\n';
        }
    }
    return 0;
}
