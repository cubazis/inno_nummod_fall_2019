#include <iostream>
#include <vector>
using namespace std;


vector<pair<double, pair<int, int>>> reactions[30];
double arg[21];
int N;



void summ(double *t_res, double *t_c){
    for (int i = 0; i < N; i++){
        t_res[i] = 0;
        for (const auto p : reactions[i])
            t_res[i] += p.first * t_c[p.second.first] * t_c[p.second.second];
    }
}



int main(){

    int T;
    int M;
    scanf("%d", &T);
    scanf("%d", &N);


    double c[N+1];
    double k1[N+1];
    double k2[N+1];
    double k3[N+1];
    double k4[N+1];


    // reading  c
    for (int i = 0; i < N; i++)
        scanf("%lf", &c[i]);

//    preudo react
    c[N] = 1.0;
    arg[N] = 1.0;


    scanf("%d", &M);

    for (int i = 0; i < M; i++){
        double k;
        int num;
        int mol1 = N; // first molecule
        int mol2 = N; // *possible* second molecule
        char op;

        scanf("%d %d %c", &num, &mol1, &op);

        mol2 = num == 2 ? mol1 : N;

        if (op == '+'){
//        otherwords num ==1
            scanf("%d %d %c", &num, &mol2, &op);
        }


        scanf("%lf", &k);


        reactions[mol1].emplace_back(-k, make_pair(mol1, mol2));
        if (mol2 != N)
            reactions[mol2].emplace_back(-k, make_pair(mol1, mol2));


        // Reading reaction output
        while (op != '#'){
            int b_num;
            int b_ind;
            scanf("%d %d %c", &b_num, &b_ind, &op);

            reactions[b_ind].emplace_back(b_num * k, make_pair(mol1, mol2));
        }
    }


    int steps = 1150; // gets imperically
    double h = (double) T/steps;

    for (int _ = 0; _ < steps; _++){
        summ(k1, c);

        for (int i = 0; i < N; i++)
            arg[i] = c[i] + h / 2 * k1[i];
        summ(k2,arg);


        for (int i = 0; i < N; i++)
            arg[i] = c[i] + h / 2 * k2[i];
        summ(k3,arg);

        for (int i = 0; i < N; i++)
            arg[i] = c[i] + h * k3[i];
        summ(k4,arg);

        for (int i = 0; i < N; i++)
            c[i] = c[i] + h * (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]) / 6;

    }



    for (int i = 0; i < N; i++)
        printf("%.7lf ", c[i]);
    return 0;
}