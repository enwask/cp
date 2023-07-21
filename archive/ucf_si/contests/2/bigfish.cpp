// settings
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#include <bits/stdc++.h>

using namespace std;

// primitive types
typedef long long ll;
typedef long int li;

// aliases
#define vec vector
typedef pair<int, int> pi;
typedef pair<ll, ll> pll;
typedef vec<int> vi;
typedef vec<li> vli;
typedef vec<ll> vll;
typedef vec<pi> vpi;
typedef vec<string> vs;

// meta macros
#define GET_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, NAME, ...) NAME
#define THANKS void GIVE_ME_A_SEMICOLON() // requires semicolon when calling b macro

// more aliases
#define pb push_back
#define eb emplace_back
#define mp make_pair

// other macros
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()
#define rep(i, a, b) for (int i = a; i < b; i++)
#define rev(i, a, b) for (int i = a; i > b; i--)

// templates
// @formatter:off
template<typename A, typename B> ostream &operator<<(ostream &stream, const pair<A, B> &pair) { return stream << "(" << pair.first << ", " << pair.second << ")"; }
// @formatter:on

void solve() {
    int n;
    cin >> n;

    li w;
    multiset<li> s;
    rep(i, 0, n) {
        cin >> w;
        s.insert(w);
    }

    int first_survivor = 0;
    for (auto it = s.begin(); it != s.end(); it++) {
        auto itb = s.upper_bound(*it * 2);
        if (itb == s.end()) break;

        li tmp = *itb;
        s.erase(itb);
        s.insert(tmp + *it);

        first_survivor ++;
    }

    int i = 0;
    cout << n - first_survivor << endl;
    for (auto it = s.begin() /*+ first_survivor*/; it != s.end(); it++) {
        if (i++ < first_survivor) continue;
        cout << *it << endl;
    }
}

int main() {
    ios::sync_with_stdio(false); // makes io faster, desyncs c-style io (no scanf/printf)
    cin.tie(nullptr); // unties cin from cout (don't do this if you alternate input/output)

    int tests;
    cin >> tests;
    while (tests--) solve();
}