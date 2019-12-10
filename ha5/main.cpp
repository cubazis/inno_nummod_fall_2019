#define xx first
#define yy second
#define mp make_pair

#include <vector>
#include <algorithm>
#include <string.h>

using namespace std;


int T, n, m;


const double h = 0.1;

double y[30];
double k1[30];
double k2[30];
double k3[30];
double k4[30];

vector<pair<double, pair<int, int>>> derivatives[30];

double arg[30];
double res[30];

void perform_f()
{
    for (int i = 0; i < n; ++i)
    {
        res[i] = 0;
        for (const auto pair : derivatives[i])
        {
            res[i] += pair.xx * arg[pair.yy.xx] * arg[pair.yy.yy];
        }
    }
}


int main()
{
    scanf("%d", &T);
    scanf("%d", &n);

    for (int i = 0; i < n; ++i)
    {
        scanf("%lf", &y[i]);
    }

    y[n] = 1.0; // constant pseudo-reactant
    arg[n] = 1.0;

    scanf("%d", &m);

    for (int i = 0; i < m; ++i)
    {
        double k;
        int w;
        int A = n;
        int C = n;
        char ch;

        scanf("%d %d %c", &w, &A, &ch);

        if (w == 2)
        {
            C = A;
        }

        if (ch == '+')
        {
            scanf("%d %d %c", &w, &C, &ch);
        }

        scanf("%lf", &k);

        derivatives[A].emplace_back(-k, mp(A, C));

        if (C != n)
        {
            derivatives[C].emplace_back(-k, mp(A, C));
        }

        while (true)
        {
            int ni;
            int B;
            scanf("%d %d %c", &ni, &B, &ch);

            derivatives[B].emplace_back(ni * k, mp(A, C));

            if (ch == '#')
                break;
        }
    }

    const int total_steps = T / h;

    for (int step = 0; step < total_steps; ++step)
    {
        memcpy(arg, y, n * sizeof(double));
        perform_f();
        memcpy(k1, res, n * sizeof(double));

        for (int i = 0; i < n; ++i)
        {
            arg[i] = y[i] + h / 2 * k1[i];
        }
        perform_f();
        memcpy(k2, res, n * sizeof(double));


        for (int i = 0; i < n; ++i)
        {
            arg[i] = y[i] + h / 2 * k2[i];
        }
        perform_f();
        memcpy(k3, res, n * sizeof(double));

        for (int i = 0; i < n; ++i)
        {
            arg[i] = y[i] + h * k3[i];
        }
        perform_f();
        memcpy(k4, res, n * sizeof(double));

        for (int i = 0; i < n; ++i)
        {
            y[i] = y[i] + h / 6 * (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]);
        }
    }

    for (int i = 0; i < n; ++i)
    {
        printf("%.12lf ", y[i]);
    }

    return 0;
}