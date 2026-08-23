// Adapted from yosupo06/library-checker-problems (Apache-2.0).
// Problem: deque; upstream commit: 04c8de378bab67be926325de2871f0babb8e6451.
#include <deque>
#include <cassert>
#include <cstdio>

using namespace std;

void solve() {
  int Q;
  scanf("%d", &Q);

  deque<int> que;
  for (int q = 0; q < Q; ++q) {
    int t, x;
    scanf("%d", &t);
    if (t == 0) {
      scanf("%d", &x);
      que.push_front(x);
    }
    if (t == 1) {
      scanf("%d", &x);
      que.push_back(x);
    }
    if (t == 2) {
      assert(!que.empty());
      que.pop_front();
    }
    if (t == 3) {
      assert(!que.empty());
      que.pop_back();
    }
    if (t == 4) {
      scanf("%d", &x);
      assert(0 <= x && x < int(que.size()));
      printf("%d\n", que[x]);
    }
  }
}

signed main() { solve(); }
