#include <iostream>
#include <cstdio>
using namespace std;



int main() {
    long n;
    scanf("%ld", &n);
    double x_in[n];
    char buffer[256];
    char buf;
    int stop;
    for(long i=0; i < n; i++) {
        scanf("%c", &buf);
        while ((buf < '0' || buf > '9') && buf != '-') {
            scanf("%c", &buf);
        }
        stop = 0;
        while ((buf >= '0' && buf <= '9') || buf == '.' || buf == 'e' || buf == 'E' || buf == '-') {
            buffer[stop++] = buf;
            scanf("%c", &buf);
        }
        buffer[stop] = '\0';
        x_in[i] = atof(buffer);
    }

    long m;
    scanf("%ld", &m);
    double y_in[m][n];
    for(long i=0; i < m; i++) {
        for(long j=0; j < n; j++) {
            scanf("%c", &buf);
            while ( (buf < '0' || buf > '9') && buf != '-' ) {
                scanf("%c", &buf);
            }
            stop = 0;
            while ((buf >= '0' && buf <= '9') || buf == '.' || buf == 'e' || buf == 'E' || buf == '-') {
                buffer[stop++] = buf;
                scanf("%c", &buf);
            }
            buffer[stop] = '\0';
            y_in[i][j] = atof(buffer);
        }
    }


    long k;
    scanf("%ld", &k);
    double x_hat[n];

    for(long i=0; i < k; i++) {
        scanf("%c", &buf);
        while ((buf < '0' || buf > '9') && buf != '-') {
            scanf("%c", &buf);
        }
        stop = 0;
        while ((buf >= '0' && buf <= '9') || buf == '.' || buf == 'e' || buf == 'E'|| buf == '-') {
            buffer[stop++] = buf;
            scanf("%c", &buf);
        }
        buffer[stop] = '\0';
        x_hat[i] = atof(buffer);
    }


//    get connection between x_in and x_hat
    long ind[k];
    fill_n(ind, k, -1);
    for (long i = 0; i< n-1; i++) {
        for (long j = 0; j < k; j++) {
            if (ind[j] == -1){
                if((x_hat[j] - x_in[i] >= 0) && (x_hat[j] - x_in[i + 1] < 0))
                    ind[j] = i;
            }
        }
    }



//    prepare coef.
    double alpha[n-2];
    double betta[n-2];
    double gamma[n-2];
    double delta[m][n-2];
    for (long i = 1; i < n-1; i++)
    {
        alpha[i-1] = (x_in[i] - x_in[i-1]);
        betta[i-1] = 2 * (x_in[i+1]-x_in[i-1]) ;
        gamma[i-1] = (x_in[i+1] - x_in[i]);

        for (long j = 0; j < m; j++) {
            delta[j][i-1] =6 * ((y_in[j][i+1] - y_in[j][i]) / (x_in[i+1] - x_in[i]) - (y_in[j][i] - y_in[j][i-1]) / (x_in[i] - x_in[i-1]));
        }
    }


//    forward pass
    double p[n-2];
    double q[m][n-2];
    double same;
    for (long i = 0; i < n-2; i++) {
        if(i == 0){
            p[i] =- gamma[i]/betta[i];
            for (long j = 0; j < m; j++) {
                q[j][i] = delta[j][i]/betta[i];
            }
        } else {
            same = (betta[i] + alpha[i]*p[i-1]);
            p[i] =- gamma[i]/same;
            for (long j = 0; j < m; j++) {
                q[j][i] = (delta[j][i] - alpha[i] * q[j][i-1])/same;
            }
        }

    }


//    backward pass
    double m_s[m][n];
    for (long i = n-1; i >= 0; i--) {
        if(i == n-1){
            for (long j = 0; j < m; j++)
                m_s[j][i] = 0;
        } else{
            if(i == 0){
                for (long j = 0; j < m; j++)
                    m_s[j][i] = 0;
            } else {
                if(i == n-2){
                    for (long j = 0; j < m; j++)
                        m_s[j][i] = q[j][i-1];
                } else{
                    for (long j = 0; j < m; j++)
                        m_s[j][i] = p[i-1] * m_s[j][i+1] + q[j][i-1];
                }
            }
        }
    }


//    extimate function
    long i;
    double h_i;
    double d_m;
    double d_x;
    double a_1;
    double ans;

    for (long j=0; j < m; j++)
    {
        for (long ii=0; ii < k; ii++){
            if ((ii> 0) && (ind[ii] == ind[ii-1])){

            }else{
                i = ind[ii];
                h_i = x_in[i + 1] - x_in[i];
                d_m = (m_s[j][i + 1] - m_s[j][i]) / 6;
                a_1 = (y_in[j][i + 1] - y_in[j][i]) / (h_i) - 0.5 * (h_i) * m_s[j][i] - (d_m) * (h_i);
            }

            d_x = x_hat[ii] - x_in[i];
            ans = y_in[j][i] + a_1 * d_x + m_s[j][i] / 2 * (d_x) * (d_x) + (d_m) / (h_i) * (d_x) *(d_x) * (d_x);
            printf("%.9g ",ans);
        }
        printf("\n");
    }
    return 0;
}