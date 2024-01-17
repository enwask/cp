#include <bits/stdc++.h>

using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define vec vector
typedef long long ll;
typedef long double ld;

vec<int> s;

struct node {
    int lo, hi;
    node *left, *right;
    ll sum;

    node(int lo, int hi) : lo(lo), hi(hi) {
        if (lo == hi) sum = s[lo];
        else {
            left = new node(lo, (lo + hi) / 2);
            right = new node((lo + hi) / 2 + 1, hi);
            merge();
        }
    }

    void merge() {
        sum = left->sum + right->sum;
    }

    void set(int ind, int val) {
        if (ind < lo || ind > hi) return;
        if (lo == hi) {
            sum = s[ind] = val;
            return;
        }

        left->set(ind, val);
        right->set(ind, val);
        merge();
    }

    ll query(int l, int r) {
        if (l > hi || r < lo) return 0;
        if (l <= lo && r >= hi) return sum;

        return left->query(l, r) + right->query(l, r);
    }
};

void solve() {
    int n, m;
    cin >> n >> m;
    s = vec<int>(n);
    rep(i, 0, n) cin >> s[i];

    node *st = new node(0, n - 1);
    rep(i, 0, m) {
        int q, a, b;
        cin >> q >> a >> b;

        if (q == 1) st->set(a, b);
        else cout << st->query(a, b - 1) << "\n";
    }
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin.exceptions(cin.failbit);

    int t = 1;
//    cin >> t; // uncomment for multiple cases
    while (t--) solve();

    return 0;
}
