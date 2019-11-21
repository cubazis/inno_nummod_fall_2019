#include <iostream>
#include <string>
#include <vector>

using namespace std;

double t;
int n;
double c[20];

vector<pair<vector<int>, double>> equations[20];

void compute(double *res, double *current_concentrations) {
    for (int i = 0; i < n; i++)
        res[i] = 0;
    for (int i = 0; i < n; i++) {
        for (auto &eq : equations[i]) {
            double val = eq.second;
            for (int id : eq.first)
                val *= current_concentrations[id];
            res[i] += val;
        }
    }
}

int main() {
    cin >> t >> n;
    for (int i = 0; i < n; i++)
        cin >> c[i];

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
            equations[ids[0]].push_back({ids, -k});
            equations[ids[1]].push_back({ids, -k});
        } else {
            if (cnts[0] == 2) {
                ids.push_back(ids[0]);
                equations[ids[0]].push_back({ids, -2 * k});
            } else {
                equations[ids[0]].push_back({ids, -k});
            }
        }
        
        do {
            cin >> s;
            cnt = stoi(s);
            cin >> s;
            id = stoi(s);
            equations[id].push_back({ids, k * cnt});
            cin >> s;
        } while (s != "#");
    }

    int iter = 1500;
    double h = t / iter;
    double *k1 = new double[n];
    double *k2 = new double[n];
    double *k3 = new double[n];
    double *k4 = new double[n];
    double *rungekutta = new double[n];

    while (iter--) {
        compute(k1, c);
        for (int i = 0; i < n; i++)
            rungekutta[i] = c[i] + k1[i] * h / 2;
        compute(k2, rungekutta);
        for (int i = 0; i < n; i++)
            rungekutta[i] = c[i] + k2[i] * h / 2;
        compute(k3, rungekutta);
        for (int i = 0; i < n; i++)
            rungekutta[i] = c[i] + k3[i] * h;
        compute(k4, rungekutta);
        for (int i = 0; i < n; i++)
            c[i] += h / 6 * (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]);
    }
    
    for (int i = 0; i < n; i++)
        printf("%.12lf ", c[i]);
}
