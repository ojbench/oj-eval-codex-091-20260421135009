#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;
using namespace __gnu_pbds;

// Ordered set with order statistics
template<typename T>
using OrderedSet = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int MAXS = 1000005; // supports up to ~1e6 sets if needed; will resize dynamically when accessed
    vector<unique_ptr<OrderedSet<long long>>> sets(1); // index from 0, will expand as needed

    auto ensure = [&](size_t idx){
        if (idx >= sets.size()) sets.resize(idx+1);
        if (!sets[idx]) sets[idx] = make_unique<OrderedSet<long long>>();
    };

    // Iterator emulation: we store current valid flag, current set index, and current value.
    bool valid = false;
    long long it_val = 0;
    long long it_set = -1;

    int op;
    while ( (cin >> op) ) {
        long long a,b,c;
        switch (op) {
            case 0: { // emplace a set[a] with value b
                cin >> a >> b;
                ensure(a);
                auto &S = *sets[a];
                auto p = S.insert(b);
                if (p.second) { // inserted
                    valid = true;
                    it_set = a;
                    it_val = b;
                }
                break;
            }
            case 1: { // erase value b from set[a]
                cin >> a >> b;
                if (valid && it_set == a && it_val == b) valid = false;
                if (a < (long long)sets.size() && sets[a]) {
                    sets[a]->erase(b);
                }
                break;
            }
            case 2: { // s[++lst] = s[a]
                cin >> a;
                ensure(a);
                size_t new_idx = sets.size();
                sets.emplace_back(make_unique<OrderedSet<long long>>(*sets[a]));
                break;
            }
            case 3: { // find b in s[a]
                cin >> a >> b;
                bool found = false;
                if (a < (long long)sets.size() && sets[a]) {
                    auto &S = *sets[a];
                    auto it = S.find(b);
                    if (it != S.end()) {
                        found = true;
                    }
                }
                if (found) {
                    cout << "true\n";
                    valid = true;
                    it_set = a;
                    it_val = b;
                } else {
                    cout << "false\n";
                }
                break;
            }
            case 4: { // range count [b,c] in s[a]
                cin >> a >> b >> c;
                if (b > c) {
                    cout << 0 << "\n";
                    break;
                }
                size_t ans = 0;
                if (a < (long long)sets.size() && sets[a]) {
                    auto &S = *sets[a];
                    // count of elements <= c minus count of elements < b
                    ans = (size_t)S.order_of_key(c + 1) - (size_t)S.order_of_key(b);
                }
                cout << ans << "\n";
                break;
            }
            case 5: { // prev iterator
                if (valid && it_set >= 0 && it_set < (long long)sets.size() && sets[(size_t)it_set]) {
                    auto &S = *sets[(size_t)it_set];
                    auto it = S.lower_bound(it_val);
                    if (it == S.end() || *it != it_val) {
                        // If stored value not in set anymore, invalidate
                        valid = false;
                    } else {
                        if (it == S.begin()) {
                            valid = false;
                        } else {
                            --it;
                            it_val = *it;
                            cout << it_val << "\n";
                            break;
                        }
                    }
                }
                cout << -1 << "\n";
                break;
            }
            case 6: { // next iterator
                if (valid && it_set >= 0 && it_set < (long long)sets.size() && sets[(size_t)it_set]) {
                    auto &S = *sets[(size_t)it_set];
                    auto it = S.lower_bound(it_val);
                    if (it == S.end() || *it != it_val) {
                        valid = false;
                    } else {
                        ++it;
                        if (it == S.end()) {
                            valid = false;
                        } else {
                            it_val = *it;
                            cout << it_val << "\n";
                            break;
                        }
                    }
                }
                cout << -1 << "\n";
                break;
            }
            default:
                // ignore unknown op
                break;
        }
    }
    return 0;
}

