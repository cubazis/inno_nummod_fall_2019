#include <iostream>
#include <vector>
#include <algorithm>
#include <string.h>
#include <fstream>

using namespace std;

vector<pair<double, pair<int, int>>> odes[30];

double arg[21];
double res[21];

int N; // number of reactants

void f(){
    for (int i = 0; i < N; ++i){
        res[i] = 0;
        for (const auto pair : odes[i]){
            res[i] += pair.first * arg[pair.second.first] * arg[pair.second.second];
        }
    }
}


int main(){

    int T; // how long we will run our virtual reactor
    int M; // number of reactions

    double conc[21];
    double k1[21];
    double k2[21];
    double k3[21];
    double k4[21];

    scanf("%d", &T);
    scanf("%d", &N);

    // reading initial concentrations
    for (int i = 0; i < N; ++i){
        scanf("%lf", &conc[i]);
    }

    conc[N] = 1.0; // constant pseudo-reactant
    arg[N] = 1.0;

    scanf("%d", &M);

    for (int i = 0; i < M; ++i){
        double k;
        int w;
        int A = N; // first molecule
        int C = N; // *possible* second molecule
        char op;

        scanf("%d %d %c", &w, &A, &op);

        if (w == 2){
            C = A;
        }

        if (op == '+'){
            scanf("%d %d %c", &w, &C, &op);
        }

        scanf("%lf", &k);

        odes[A].emplace_back(-k, make_pair(A, C));

        if (C != N){
            odes[C].emplace_back(-k, make_pair(A, C));
        }

        // Reading reaction output
        while (op != '#'){
            int ni;
            int B;
            scanf("%d %d %c", &ni, &B, &op);

            odes[B].emplace_back(ni * k, make_pair(A, C));
        }
    }

    double h = 0.1;
    int total_steps = T/h;

    // Runge-Kutta method
    for (int step = 0; step < total_steps; ++step){
        memcpy(arg, conc, N * sizeof(double));
        f();
        memcpy(k1, res, N * sizeof(double));

        for (int i = 0; i < N; ++i){
            arg[i] = conc[i] + h / 2 * k1[i];
        }
        f();
        memcpy(k2, res, N * sizeof(double));


        for (int i = 0; i < N; ++i){
            arg[i] = conc[i] + h / 2 * k2[i];
        }
        f();
        memcpy(k3, res, N * sizeof(double));

        for (int i = 0; i < N; ++i){
            arg[i] = conc[i] + h * k3[i];
        }
        f();
        memcpy(k4, res, N * sizeof(double));

        for (int i = 0; i < N; ++i){
            conc[i] = conc[i] + h / 6 * (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]);
        }
    }

    for (int i = 0; i < N-1; ++i){
        printf("%.7lf ", conc[i]);
    }
    printf("%.7lf", conc[N-1]);

    return 0;
}