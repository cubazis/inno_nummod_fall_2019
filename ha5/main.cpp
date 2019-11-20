#include <string>
#include <vector>
#include <iostream>

using namespace std;
const int MAX_N = 21;

double c[MAX_N];
int n_iter = 2000;
auto *temp = new double[MAX_N];
auto *k1 = new double[MAX_N];
auto *k2 = new double[MAX_N];
auto *k3 = new double[MAX_N];
auto *k4 = new double[MAX_N];
double t;
int n;
int m;


vector<pair<vector<int>, double>> diffs[21];
void parse(){
    cin >> t >> n;

    for (int i = 0; i < n; i++) {
        cin >> c[i];
    }

    cin >> m;

    for (int i = 0; i < m; i++) {
        vector<int> ids;
        vector<int> counts;

        int cnt, id;
        cin >> cnt >> id;
        counts.push_back(cnt), ids.push_back(id);
        string s;
        cin >> s;
        if (s != ">") {
            cin >> cnt >> id;
            counts.push_back(cnt), ids.push_back(id);
            cin >> s;
        }
        double k_i;
        cin >> k_i;

        if (counts.size() != 2) {
            if (counts[0] == 2) {
                ids.push_back(ids[0]);
                diffs[ids[0]].push_back({ids, -2 * k_i});
            } else {
                diffs[ids[0]].push_back({ids, -k_i});
            }

        } else {
            diffs[ids[0]].push_back({ids, -k_i});
            diffs[ids[1]].push_back({ids, -k_i});
        }
        while (s!="#") {
            cin >> s;
            cnt = stold(s);
            cin >> s;
            id = stoi(s);
            diffs[id].emplace_back(ids, k_i * cnt);
            cin >> s;
        }
    }
}


void calc(double *result, double *curr_c) {
    for (int i = 0; i < n; i++) {
        result[i] = 0;
    }
    for (int i = 0; i < n; i++) {
        for (int j=0; j<diffs[i].size();j++) {

            double value = diffs[i][j].second;
            for (int idc : diffs[i][j].first) {
                value *= curr_c[idc];
            }
            result[i] += value;
        }
    }
}


void solve(){


    double h = t / n_iter;
    for (int f =0; f<n_iter;f++) {
        calc(k1, c);
        for (int i = 0; i < n; i++) {
            temp[i] = c[i] + k1[i] * h / 2;
        }
        calc(k2, temp);
        for (int i = 0; i < n; i++) {
            temp[i] = c[i] + k2[i] * h / 2;
        }
        calc(k3, temp);
        for (int i = 0; i < n; i++) {
            temp[i] = c[i] + k3[i] * h;
        }
        calc(k4, temp);
        for (int i = 0; i < n; i++) {
            c[i] +=  (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i])*h / 6;
        }
    }
}

int main() {

    parse();
    solve();

    for (int i = 0; i < n; i++) {
        printf("%.8f ", c[i]);
    }
    return 0;
}
