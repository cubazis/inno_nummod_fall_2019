#include <cstdio>
#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

#define h 0.01
#define double long double

int n, m;
double t, c1[20], c[20], f_prev_prev[20], f_3prev[20], f_4prev[20], c_prev[20];
vector <vector<pair<double, pair<int,int> > > > f(20);

void assign(double *a, double *b)
{
	for(int i = 0; i < n; ++i)
	{
		a[i] = b[i];
	}
}

int main()
{
    //freopen("input.txt", "r", stdin);
	//freopen("output.txt", "w", stdout);
	cin >> t >> n;
	for(int i = 0; i < n; ++i)
	{
		cin >> c1[i];
		f[i].resize(0);
	}
	cin >> m;
	for(int i = 0; i < m; ++i)
	{
		int tmp, A, B;
		double k, ni;
		char zn;
		cin >> tmp >> A;
		if(tmp == 2)
		{
			cin >> zn >> k;
			f[A].push_back(make_pair(-2*k, make_pair(A, A)));
			while(zn != '#')
			{
				cin >> ni >> B >> zn;
				f[B].push_back(make_pair(ni*k, make_pair(A, A)));
			}	
		}
		else
		{
			cin >> zn;
			if(zn == '+')
			{
				int C; 
				cin >> tmp >> C >> zn >> k;
				f[A].push_back(make_pair(-k, make_pair(A, C)));
				f[C].push_back(make_pair(-k, make_pair(A, C)));
				while(zn != '#')
				{
					cin >> ni >> B >> zn;
					f[B].push_back(make_pair(ni*k, make_pair(A, C)));
				}	
			}
			else
			{
				cin >> k;
				f[A].push_back(make_pair(-1*k, make_pair(A, -1)));
				while(zn != '#')
				{
					cin >> ni >> B >> zn;
					f[B].push_back(make_pair(ni*k, make_pair(A, -1)));
				}	
			}
		}
	}


	/*for(int i = 0; i < n; ++i)
	{
		cout << i << "= ";
		for(int j = 0 ; j < f[i].size(); ++j)
		{
			cout << f[i][j].first << ' ' << f[i][j].second.first << ' ' << f[i][j].second.second << " | ";
		}
		cout << "\n";
	} */

	for (int i = 0 ; i < n; ++i)
	{
		double sum = 0, tmp;
		for(int j = 0; j < f[i].size(); ++j)
		{
			f[i][j].second.second == -1 ? tmp = 1 : tmp = c1[f[i][j].second.second];
			sum += (f[i][j].first * c1[f[i][j].second.first] * tmp);
		}
		c[i] = c1[i] + h * sum;
		f_4prev[i] = sum;
	}
	assign(c_prev, c);
	for (int i = 0 ; i < n; ++i)
	{
		double sum = 0, tmp;
		for(int j = 0; j < f[i].size(); ++j)
		{
			f[i][j].second.second == -1 ? tmp = 1 : tmp = c_prev[f[i][j].second.second];
			sum += (f[i][j].first * c_prev[f[i][j].second.first] * tmp);
		}
		c[i] = c_prev[i] + h * (1.5 * sum - 0.5 * f_4prev[i]);
		f_3prev[i] = sum;
	}
	assign(c_prev, c);
	for (int i = 0 ; i < n; ++i)
	{
		double sum = 0, tmp;
		for(int j = 0; j < f[i].size(); ++j)
		{
			f[i][j].second.second == -1 ? tmp = 1 : tmp = c_prev[f[i][j].second.second];
			sum += (f[i][j].first * c_prev[f[i][j].second.first] * tmp);
		}
		c[i] = c_prev[i] + h * (23.0 / 12.0 * sum - 4.0/3.0 * f_3prev[i] + 5.0/12.0*f_4prev[i]);
		//f_3prev[i] = f_prev_prev[i];
		f_prev_prev[i] = sum;
	}

	for(int q = 3; q < (int)t/h; ++q)
	{
		assign(c_prev, c);
		for (int i = 0 ; i < n; ++i)
		{
			double sum = 0, tmp;
			for(int j = 0; j < f[i].size(); ++j)
			{
				f[i][j].second.second == -1 ? tmp = 1 : tmp = c_prev[f[i][j].second.second];
				sum += (f[i][j].first * c_prev[f[i][j].second.first] * tmp);
			}
			c[i] = c_prev[i] + h * (55.0 / 24.0 * sum - 59.0/24.0 * f_prev_prev[i] + 37.0/24.0*f_3prev[i] - 3.0/8.0 * f_4prev[i]);
			f_4prev[i] = f_3prev[i];
			f_3prev[i] = f_prev_prev[i];
			f_prev_prev[i] = sum;
		}	
	}

	for(int i = 0; i < n; ++i)
	{
		cout << setprecision(9) << c[i] << ' ';
	}

}