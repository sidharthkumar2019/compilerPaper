#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 1000
#define EPSILON 1.0e-16

double **a, *b, *x;

void init_matrix()
{
    int i, j;
    srand(time(NULL));
    a = (double **)malloc(N * sizeof(double *));
    for (i = 0; i < N; i++) {
        a[i] = (double *)malloc(N * sizeof(double));
        for (j = 0; j < N; j++) {
            a[i][j] = (double)rand() / RAND_MAX;
        }
    }
    b = (double *)malloc(N * sizeof(double));
    for (i = 0; i < N; i++) {
        b[i] = (double)rand() / RAND_MAX;
    }
    x = (double *)malloc(N * sizeof(double));
}

void solve_system()
{
    int i, j, k, maxind;
    double maxval, temp;
    for (k = 0; k < N-1; k++) {
        maxval = fabs(a[k][k]);
        maxind = k;
        for (i = k+1; i < N; i++) {
            if (fabs(a[i][k]) > maxval) {
                maxval = fabs(a[i][k]);
                maxind = i;
            }
        }
        if (maxval < EPSILON) {
            printf("Error: singular matrix\n");
            exit(1);
        }
        if (maxind != k) {
            for (j = k; j < N; j++) {
                temp = a[k][j];
                a[k][j] = a[maxind][j];
                a[maxind][j] = temp;
            }
            temp = b[k];
            b[k] = b[maxind];
            b[maxind] = temp;
        }
        for (i = k+1; i < N; i++) {
            temp = a[i][k] / a[k][k];
            for (j = k+1; j < N; j++) {
                a[i][j] -= temp * a[k][j];
            }
            b[i] -= temp * b[k];
        }
    }
    if (fabs(a[N-1][N-1]) < EPSILON) {
        printf("Error: singular matrix\n");
        exit(1);
    }
    x[N-1] = b[N-1] / a[N-1][N-1];
    for (i = N-2; i >= 0; i--) {
        temp = b[i];
        for (j = i+1; j < N; j++) {
            temp -= a[i][j] * x[j];
        }
        x[i] = temp / a[i][i];
    }
}

double measure_time()
{
    clock_t start, end;
    double time;
    start = clock();
    solve_system();
    end = clock();
    time = (double)(end - start) / CLOCKS_PER_SEC;
    return time;
}

int main()
{
    int i, ntrials = 10;
    double ttotal = 0.0;
    
    init_matrix();
    for (i = 0; i < ntrials; i++) {
        ttotal += measure_time();
        printf("%.3f\n", ttotal);
    }
    ttotal /= ntrials;
    // printf("Average run time: %.3f seconds\n", ttotal);
    
    return 0;
}
