#include <bits/stdc++.h>
#include <variant>
#pragma GCC optimize("Ofast")
using namespace std;
using ld = double;

struct Dependency {
    Dependency(variant<int, pair<int, int>> type, ld value) : type(move(type)), value(value) {
    }

    variant<int, pair<int, int>> type;
    ld value;
};
vector<ld> res;

inline vector<ld> calc_f(
        int n,
        const vector<vector<Dependency>> &all_deps,
        const vector<ld> &current_concentrations) {
    res.assign(n, 0);
    for (int i = 0; i < n; ++i) {
        const auto &deps = all_deps[i];
        for (const auto &dep : deps) {
            if (holds_alternative<int>(dep.type)) {
                int ind = get<int>(dep.type);
                res[i] += current_concentrations[ind] * dep.value;
            } else {
                auto[ind1, ind2] = get<pair<int, int>>(dep.type);
                res[i] += current_concentrations[ind1] * current_concentrations[ind2] * dep.value;
            }
        }
    }

    return res;
}

int main() {
#ifdef zxc
    freopen("../kolya_input.txt", "r", stdin);
#else
    //    freopen("damage.in", "r", stdin);
    //    freopen("damage.out", "w", stdout);
#endif
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.precision(6);
    cout << fixed;
    ld t;
    cin >> t;
    int n;
    cin >> n;
    res.resize(n);
    vector<ld> concentration(n);
    for (int i = 0; i < n; ++i) {
        cin >> concentration[i];
    }
    vector<vector<Dependency>> deps(n);
    int m;
    cin >> m;
    for (int i = 0; i < m; ++i) {
        vector<int> cnts;
        vector<int> ids;
        cin >> cnts.emplace_back();
        cin >> ids.emplace_back();
        string s;
        cin >> s;
        if (s != ">") {
            cin >> cnts.emplace_back();
            cin >> ids.emplace_back();
            cin >> s;
        }
        ld k;
        cin >> k;

        variant<int, pair<int, int>> type;
        if (cnts.size() == 2) {
            type = make_pair(ids[0], ids[1]);
            deps[ids[0]].emplace_back(type, -k);
            deps[ids[1]].emplace_back(type, -k);
        } else {
            if (cnts[0] == 2) {
                type = make_pair(ids[0], ids[0]);
                deps[ids[0]].emplace_back(type, -2 * k);
            } else {
                type = ids[0];
                deps[ids[0]].emplace_back(type, -k);
            }
        }
        for (;;) {
            cin >> s;
            ld cur_n = stold(s);
            cin >> s;
            int cur_id = stoi(s);
            deps[cur_id].emplace_back(type, k * cur_n);
            cin >> s;
            if (s == "#") {
                break;
            }
        }
    }

    int iter = 2000;
    ld step = t / iter;
    vector<vector<ld>> k(5);
    vector<ld> mu_hat(n);
    for (int i = 1; i <= iter; ++i) {

        k[1] = calc_f(n, deps, concentration);
        for (int j = 0; j < n; ++j) {
            mu_hat[j] = concentration[j] + step / 2 * k[1][j];
            if (mu_hat[j] < 0) {
                assert(false);
            }
        }
        k[2] = calc_f(n, deps, mu_hat);
        for (int j = 0; j < n; ++j) {
            mu_hat[j] = concentration[j] + step / 2 * k[2][j];
            if (mu_hat[j] < 0) {
                assert(false);
            }
        }
        k[3] = calc_f(n, deps, mu_hat);
        for (int j = 0; j < n; ++j) {
            mu_hat[j] = concentration[j] + step * k[3][j];
            if (mu_hat[j] < 0) {
                assert(false);
            }
        }
        k[4] = calc_f(n, deps, mu_hat);
        for (int j = 0; j < n; ++j) {
            concentration[j] += step / 6 * (k[1][j] + 2 * (k[2][j] + k[3][j]) + k[4][j]);
            if (concentration[j] < 0) {
                assert(false);
            }
        }
    }
    for (ld el:concentration) {
        cout << el << " ";
    }
}
