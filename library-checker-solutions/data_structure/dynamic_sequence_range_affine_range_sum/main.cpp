#include <bits/stdc++.h>

using namespace std;

constexpr long long P = 998244353;
mt19937 randomEngine(712367821);

struct Node {
    long long value, sum, multiply = 1, add = 0;
    unsigned priority = randomEngine();
    int size = 1;
    bool reversed = false;
    Node *left = nullptr, *right = nullptr;

    Node(long long value) : value(value), sum(value) {}
};

int size(Node* node) {
    return node ? node->size : 0;
}

long long sum(Node* node) {
    return node ? node->sum : 0;
}

void applyAffine(Node* node, long long multiply, long long add) {
    if (!node) {
        return;
    }
    node->value = (node->value * multiply + add) % P;
    node->sum = (node->sum * multiply + add * node->size) % P;
    node->multiply = node->multiply * multiply % P;
    node->add = (node->add * multiply + add) % P;
}

void applyReverse(Node* node) {
    if (node) {
        swap(node->left, node->right);
        node->reversed ^= 1;
    }
}

void push(Node* node) {
    if (!node) {
        return;
    }
    if (node->reversed) {
        applyReverse(node->left);
        applyReverse(node->right);
        node->reversed = false;
    }
    if (node->multiply != 1 || node->add != 0) {
        applyAffine(node->left, node->multiply, node->add);
        applyAffine(node->right, node->multiply, node->add);
        node->multiply = 1;
        node->add = 0;
    }
}

void pull(Node* node) {
    node->size = size(node->left) + 1 + size(node->right);
    node->sum = (sum(node->left) + node->value + sum(node->right)) % P;
}

void split(Node* node, int count, Node*& left, Node*& right) {
    if (!node) {
        left = right = nullptr;
        return;
    }
    push(node);
    if (size(node->left) >= count) {
        split(node->left, count, left, node->left);
        right = node;
        pull(right);
    } else {
        split(node->right, count - size(node->left) - 1, node->right, right);
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
        long long value;
        cin >> value;
        root = merge(root, new Node(value));
    }
    while (q--) {
        int type;
        cin >> type;
        if (type == 0) {
            int position;
            long long value;
            cin >> position >> value;
            Node *left, *right;
            split(root, position, left, right);
            root = merge(merge(left, new Node(value)), right);
        } else if (type == 1) {
            int position;
            cin >> position;
            Node *left, *middle, *right;
            split(root, position + 1, middle, right);
            split(middle, position, left, middle);
            delete middle;
            root = merge(left, right);
        } else {
            int l, r;
            cin >> l >> r;
            Node *left, *middle, *right;
            split(root, r, middle, right);
            split(middle, l, left, middle);
            if (type == 2) {
                applyReverse(middle);
            } else if (type == 3) {
                long long multiply, add;
                cin >> multiply >> add;
                applyAffine(middle, multiply, add);
            } else {
                cout << sum(middle) << '\n';
            }
            root = merge(merge(left, middle), right);
        }
    }
    return 0;
}
