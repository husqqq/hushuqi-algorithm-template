#include <bits/stdc++.h>

using namespace std;

using u64 = unsigned long long;

struct Hash {
    static u64 splitmix64(u64 x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }

    size_t operator()(u64 x) const {
        static const u64 seed = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + seed);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;
    unordered_map<u64, u64, Hash> value;
    value.reserve(2 * q);
    while (q--) {
        int op;
        u64 key;
        cin >> op >> key;
        if (op == 0) {
            u64 x;
            cin >> x;
            value[key] = x;
        } else {
            auto it = value.find(key);
            cout << (it == value.end() ? 0 : it->second) << '\n';
        }
    }
    return 0;
}
