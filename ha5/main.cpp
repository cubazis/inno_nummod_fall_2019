#pragma GCC optimize("Ofast")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")
#pragma GCC optimize("unroll-loops")

#include <iostream>
#include <string>
#include <vector>

using namespace std;

double t;
int n;

double c[20];

vector<pair<vector<int>, double>> diff_eqs[20];

void calc(double *res, double *curr_c) {
    for (int i = 0; i < n; i++) {
        res[i] = 0;
    }
    for (int i = 0; i < n; i++) {
        for (auto &curr : diff_eqs[i]) {
            double val = curr.second;
            for (int id : curr.first) {
                val *= curr_c[id];
            }
            res[i] += val;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    cin >> t >> n;

    for (int i = 0; i < n; i++) {
        cin >> c[i];
    }

    int m;
    cin >> m;

    for (int i = 0; i < m; i++) {
        vector<int> cnts;
        vector<int> ids;
        int cnt, id;
        cin >> cnt >> id;
        cnts.push_back(cnt), ids.push_back(id);
        string s;
        cin >> s;
        if (s != ">") {
            cin >> cnt >> id;
            cnts.push_back(cnt), ids.push_back(id);
            cin >> s;
        }
        double k;
        cin >> k;

        if (cnts.size() == 2) {
            diff_eqs[ids[0]].push_back({ids, -k});
            diff_eqs[ids[1]].push_back({ids, -k});
        } else {
            if (cnts[0] == 2) {
                ids.push_back(ids[0]);
                diff_eqs[ids[0]].push_back({ids, -2 * k});
            } else {
                diff_eqs[ids[0]].push_back({ids, -k});
            }
        }
        while (true) {
            cin >> s;
            cnt = stold(s);
            cin >> s;
            id = stoi(s);
            diff_eqs[id].push_back({ids, k * cnt});
            cin >> s;
            if (s == "#") break;
        }
    }

    int iter = 1500;
    double h = t / iter;
    double *k1 = new double[n];
    double *k2 = new double[n];
    double *k3 = new double[n];
    double *k4 = new double[n];
    double *tmp = new double[n];

    while (iter--) {
        calc(k1, c);
        for (int i = 0; i < n; i++) {
            tmp[i] = c[i] + k1[i] * h / 2;
        }
        calc(k2, tmp);
        for (int i = 0; i < n; i++) {
            tmp[i] = c[i] + k2[i] * h / 2;
        }
        calc(k3, tmp);
        for (int i = 0; i < n; i++) {
            tmp[i] = c[i] + k3[i] * h;
        }
        calc(k4, tmp);
        for (int i = 0; i < n; i++) {
            c[i] += h / 6 * (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]);
        }
    }
    for (int i = 0; i < n; i++) {
        printf("%.20lf ", c[i]);
    }
    return 0;
}
