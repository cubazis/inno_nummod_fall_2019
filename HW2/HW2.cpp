#include <cstdio>
#include <vector>
#include <cmath>
using namespace std;    
double* x; 
int N, M, K;
class Spline
{
    public:
        double *k, *a, *b, *y, *c, *P, *d, *Q; 
        void init()
        {
            k = new double[N - 2];
            a = new double[N - 2];
            b = new double[N - 2];
            c = new double[N - 2];
            d = new double[N - 2];
            P = new double[N - 2];
            Q = new double[N - 2];
            k = new double[N];
            y = new double[N];
 
            a[0] = 0.0; c[N - 3] = 0;
            b[0] = 2.0 * (x[0] - x[2]);
            c[0] = x[1] - x[2];
  
            a[N - 3] = x[N - 3] - x[N - 2];
            b[N - 3] = 2 * (x[N - 3] - x[N - 1]);
              
           
            for (int i = 1; i < N - 2; ++i)
            {
                a[i] = x[i] - x[i+1];
                b[i] = 2*(x[i] - x[i + 2]);
                c[i] = x[i+1] - x[i + 2];
            }
           
        }
            
        void set_points(vector<double> y_temp)
        {
            for(int i=0;i<y_temp.size();i++){
                y[i] = y_temp[i];
            }
             
            d[N - 3] = 6*((y[N - 3] - y[N - 2])/(x[N - 3] - x[N - 2]) - (y[N - 2] - y[N - 1])/(x[N - 2] - x[N - 1]));
            d[0] = 6.0 * (( y[0] - y[1] ) / (x[0] - x[1]) - (y[1] - y[2]) / (x[1] - x[2]));
             
            for (int i = 1; i < N - 2; ++i)
                d[i] = 6*((y[i] - y[i+1])/(x[i] - x[i+1]) - (y[i+1] - y[i + 2])/(x[i+1] - x[i + 2]));
         
            calculate_matrix(d, N-2);
        }
        void calculate_matrix(double* d,  int n)
        {
          
           // Forward pass
           int i = 0;
           double denominator = b[i];
           P[i] = -c[i] / denominator;
           Q[i] =  d[i] / denominator;
             
           for ( i = 1; i < n; ++i )
           {
              denominator = b[i] + a[i] * P[i-1];
              P[i] =  -c[i]                   / denominator;
              Q[i] = ( d[i] - a[i] * Q[i-1] ) / denominator;
           }
 
           d[n-1] = Q[n-1];
           for (i = n - 2; i >= 0; --i) d[i] = P[i] * d[i+1] + Q[i];
           k[0] = 0;
           k[N-1] = 0;
           for(int i=1;i<N-1;i++)  k[i] = *d++;
           *d-=N-2;
        }
            
        double get_value(int i, double index)
        {
            double e = index - x[i];
            double v = x[i] - x[i+1], a = (index - x[i+1]) * (index - x[i+1]) * (index - x[i+1]);
            double b = pow(e, 3);
            return k[i]/6.0 * ( a / v - (index - x[i+1]) * v ) 
                        -   ( k[i+1] / 6.0 * ( ( b/v ) - ( e * v ) ) )
                        +   ( ( y[i] * (index - x[i+1]) - (y[i+1] * e )) / v);
        }   
                
     double operator() (double index) 
        {
            int l = 0, r = N - 1;
            while(l<r){
                int mid = (l+r)/2;
                if(x[mid]>=index){
                    r = mid;
                }
                else {
                    l=mid+1;
                }
            }
            return get_value(r-1, index); 
          
        }
};
    
int main()
{
    //freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
    Spline s;
    double t;
    vector<double>  x2;
    vector< vector<double> > y;
      
    scanf("%d", &N);
    x = new double[N];
    for (int i = 0; i < N; ++i)
    {
        scanf("%lf", &x[i]);
    }
        
    s.init();
    scanf("%d", &M);
    for(int iter = 0; iter < M; ++iter)
    {
        vector <double> temp;
        for(int i = 0; i < N; ++i)
        {
            scanf("%lf", &t);
            temp.push_back(t);
        }
        y.push_back(temp);
    }
    scanf("%d", &K);
    for(int i = 0; i < K; ++i)
    {
        scanf("%lf", &t);
        x2.push_back(t);
    }
    for(int i = 0; i < y.size(); ++i)
    {
        s.set_points(y[i]);
 
        for(int j = 0; j < K; ++j) 
        {
            printf("%.9lf ", s(x2[j]));
        } 
        printf("\n");
    }
    
    return 0;   
}
