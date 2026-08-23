#include <bits/stdc++.h>

using namespace std;

vector<int> suffixArray(vector<int> s) {
    s.push_back(0);
    int n = s.size();
    int alphabet = max(n, *max_element(s.begin(), s.end()) + 1);
    vector<int> order(n), rank(n), count(alphabet);
    for (int x : s) {
        count[x]++;
    }
    partial_sum(count.begin(), count.end(), count.begin());
    for (int i = n - 1; i >= 0; i--) {
        order[--count[s[i]]] = i;
    }
    int classes = 1;
    for (int i = 1; i < n; i++) {
        if (s[order[i]] != s[order[i - 1]]) {
            classes++;
        }
        rank[order[i]] = classes - 1;
    }

    vector<int> nextOrder(n), nextRank(n);
    for (int length = 1; length < n; length *= 2) {
        for (int i = 0; i < n; i++) {
            nextOrder[i] = order[i] - length;
            if (nextOrder[i] < 0) {
                nextOrder[i] += n;
            }
        }
        fill(count.begin(), count.begin() + classes, 0);
        for (int x : nextOrder) {
            count[rank[x]]++;
        }
        partial_sum(count.begin(), count.begin() + classes, count.begin());
        for (int i = n - 1; i >= 0; i--) {
            int x = nextOrder[i];
            order[--count[rank[x]]] = x;
        }

        classes = 1;
        nextRank[order[0]] = 0;
        for (int i = 1; i < n; i++) {
            pair current{rank[order[i]], rank[(order[i] + length) % n]};
            pair previous{rank[order[i - 1]], rank[(order[i - 1] + length) % n]};
            if (current != previous) {
                classes++;
            }
            nextRank[order[i]] = classes - 1;
        }
        rank.swap(nextRank);
        if (classes == n) {
            break;
        }
    }
    order.erase(order.begin());
    return order;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s, t;
    cin >> s >> t;
    int n = s.size();
    vector<int> text;
    for (char c : s) {
        text.push_back(c - 'a' + 2);
    }
    text.push_back(1);
    for (char c : t) {
        text.push_back(c - 'a' + 2);
    }

    auto sa = suffixArray(text);
    int total = text.size();
    vector<int> rank(total);
    for (int i = 0; i < total; i++) {
        rank[sa[i]] = i;
    }
    vector<int> lcp(max(0, total - 1));
    for (int i = 0, length = 0; i < total; i++) {
        if (rank[i] == 0) {
            continue;
        }
        int j = sa[rank[i] - 1];
        while (i + length < total && j + length < total && text[i + length] == text[j + length]) {
            length++;
        }
        lcp[rank[i] - 1] = length;
        if (length) {
            length--;
        }
    }

    auto side = [&](int position) {
        if (position < n) {
            return 0;
        }
        if (position > n) {
            return 1;
        }
        return -1;
    };

    int best = 0, positionS = 0, positionT = 0;
    for (int i = 0; i + 1 < total; i++) {
        int a = sa[i], b = sa[i + 1];
        if (side(a) == -1 || side(b) == -1 || side(a) == side(b)) {
            continue;
        }
        if (lcp[i] > best) {
            best = lcp[i];
            if (side(a) == 0) {
                positionS = a;
                positionT = b - n - 1;
            } else {
                positionS = b;
                positionT = a - n - 1;
            }
        }
    }
    cout << positionS << ' ' << positionS + best << ' ' << positionT << ' ' << positionT + best << '\n';
    return 0;
}
