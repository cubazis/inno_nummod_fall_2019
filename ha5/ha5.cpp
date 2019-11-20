#include <iostream>
#include <cstdio>
#include <vector>
#include <iomanip>

using namespace std;

#define double long double
#define pb push_back
#define fi first
#define se second
#define mp make_pair

const int N = 20;

int n, m;
double t, c1[N], c[N], f_prev_prev[N], c_prev[N];
vector <vector<pair<double, pair<int,int> > > > f(N);

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
	double h = 0.01;
	// 0.07183674344492988 0.10402346010604198 0.05806655446230451 0.03156383807129276 0.010369607466402765
	cin >> t >> n;
	for (int i = 0; i < n; ++i) {
		cin >> *(c1 + i);
		f[i].resize(0);
	}
	cin >> m;
	for (int i = 0; i < m; ++i) {
		int op, A, B;
		double k, ni;
		char zn;
		cin >> op >> A;
		if (op == 2) {
			cin >> zn >> k;
			f[A].pb(mp(op * -k, mp(A, A)));
			while (zn != '#') {
				cin >> ni >> B >> zn;
				f[B].pb(mp(ni * k, mp(A, A)));
			}
			continue;
		}
        cin >> zn;
        if (zn == '+') {
            int C;
            cin >> op >> C >> zn >> k;
            f[A].pb(mp(-k, mp(A, C)));
            f[C].pb(mp(-k, mp(A, C)));
            while (zn != '#') {
                cin >> ni >> B >> zn;
                f[B].pb(make_pair(ni*k, make_pair(A, C)));
            }
        }
        else {
            cin >> k;
            f[A].pb(make_pair(-k, make_pair(A, -1)));
            while (zn != '#') {
                cin >> ni >> B >> zn;
                f[B].pb(make_pair(ni*k, make_pair(A, -1)));
            }
        }

	}

	for (int i = 0 ; i < n; ++i) {
		double sum = 0.0, second_react;
		for (int j = 0; j < f[i].size(); ++j)
			second_react = (f[i][j].se.se == -1 ? 1 : c1[f[i][j].se.se]),
			sum += (f[i][j].fi * c1[f[i][j].se.fi] * second_react);
		*(c + i) = *(c1 + i) + h * sum,
		*(f_prev_prev + i) = sum;
	}
    if (t > 40000) {
        h = 0.3;
    }
    else if (t > 12000) {
        h = 0.1;
    }
	for (int q = 1; q < (int)t / h; ++q) {
		for (int i = 0; i < n; ++i)
            *(c_prev + i) = *(c + i);
		for (int i = 0 ; i < n; ++i) {
			double sum = 0.0, second_react;
			for (int j = 0; j < f[i].size(); ++j)
				 second_react = (f[i][j].se.se == -1 ? 1 : c_prev[f[i][j].se.se]),
				sum += (f[i][j].fi * c_prev[f[i][j].se.fi] * second_react);
			*(c + i) = *(c_prev + i) + h * (1.5 * sum - 0.5 * *(f_prev_prev + i)),
			*(f_prev_prev + i) = sum;
		}
	}

	for (int i = 0; i < n; ++i)
		cout << setprecision(7) << *(c + i) << ' ';

}
