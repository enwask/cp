// Settings
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#include <bits/stdc++.h>

using namespace std;

// Few word type fast zoom
#define vec vector
#define pb push_back
#define eb emplace_back
#define uset unordered_set
#define umap unordered_map

// Type aliases
typedef long int li;
typedef long long ll;
typedef long double ld;
typedef unsigned char byte;
typedef pair<int, int> pi;
typedef pair<double, double> pd;
typedef pair<li, li> pli;
typedef pair<ll, ll> pll;
typedef pair<ld, ld> pld;
typedef vec<int> vi;
typedef vec<li> vli;
typedef vec<ll> vll;
typedef vec<pi> vpi;
typedef vec<string> vs;

// Other macros
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()
#define rep(i, a, b) for (int i = a; i < b; i++)
#define rev(i, a, b) for (int i = a; i > b; i--)
#define travis(x, s) for (auto &x: s)
#define integral(return_type) template<typename T> typename enable_if<is_integral<T>::value, return_type>::type
#define numeric(return_type) template<typename T> typename enable_if<is_arithmetic<T>::value, return_type>::type

// Misc templates
// @formatter:off
template<typename A, typename B> istream &operator>>(istream &stream, pair<A, B> &pair) { stream >> pair.first >> pair.second; return stream; }
template<typename A, typename B> ostream &operator<<(ostream &stream, const pair<A, B> &pair) { return stream << "(" << pair.first << ", " << pair.second << ")"; }
template<typename T> istream &operator>>(istream &stream, vector<T> &vector) { for (auto &x: vector) cin >> x; return stream; }
template<typename T> istream &operator>>(istream &stream, T array[]) { for (auto &x: array) cin >> x; return stream; }
template<typename T> class ipq : public priority_queue<T, vector<T>, greater<T>> {};
// @formatter:on

const ll MOD = 10e9 + 7;

ll exp(ll a, ll b, ll oa) {
    a %= MOD;
    if (b == 0) return 1;
    if (b == 1) return a;
    return exp((a * oa) % MOD, b - 1, oa) % MOD;
}

ll exp(ll a, ll b) {
    return exp(a, b, a);
}

void solve() {
    string s;
    while (cin >> s && s != "0") {
        ll n1 = 0;
        ll n2 = 0;

        rep(i, 0, s.size()) {
            int digit = s[i] - '0';

            n1 += (digit * exp(10, (int) s.size() - i - 1) % MOD) % MOD;
            n1 %= MOD;

            n2 += exp(digit, i + 1) % MOD;
            n2 %= MOD;
        }

//        cout << n1 << " " << n2 << endl;
        if (n1 == n2) cout << s << " is a powerful number.\n";
        else cout << s << " is not a powerful number.\n";
    }
}

int main() {
    ios::sync_with_stdio(false); // Makes io faster, desyncs c-style io (no scanf/printf)
    //cin.tie(nullptr); // Unties cin from cout (don't do this if you alternate input/output)

//    int tests;
//    cin >> tests;
//    while (tests--) solve();
    solve();
}