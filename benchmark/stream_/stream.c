#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 100000000

double *a, *b, *c;

void init_arrays()
{
    int i;
    
    a = (double *)malloc(N * sizeof(double));
    b = (double *)malloc(N * sizeof(double));
    c = (double *)malloc(N * sizeof(double));
    srand(0);
    for (i = 0; i < N; i++) {
        a[i] = (double)rand() / RAND_MAX;
        b[i] = (double)rand() / RAND_MAX;
        c[i] = 0.0;
    }
}

void copy()
{
    int i;
    
    for (i = 0; i < N; i++) {
        c[i] = a[i];
    }
}

void scale(double scalar)
{
    int i;
    
    for (i = 0; i < N; i++) {
        b[i] = scalar * a[i];
    }
}

void add()
{
    int i;
    
    for (i = 0; i < N; i++) {
        c[i] = a[i] + b[i];
    }
}

void triad(double scalar)
{
    int i;
    
    for (i = 0; i < N; i++) {
        a[i] = b[i] + scalar * c[i];
    }
}

double get_time()
{
    struct timespec ts;
    
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

int main()
{
    int i, ntrials = 10;
    double tcopy, tscale, tadd, ttriad, copy_bw, scale_bw, add_bw, triad_bw;
    
    init_arrays();
    for (i = 0; i < ntrials; i++) {
        tcopy = get_time();
        copy();
        tcopy = get_time() - tcopy;
        tscale = get_time();
        scale(2.0);
        tscale = get_time() - tscale;
        tadd = get_time();
        add();
        tadd = get_time() - tadd;
        ttriad = get_time();
        triad(2.0);
        ttriad = get_time() - ttriad;
        copy_bw = (double)N * sizeof(double) / tcopy / 1e9;
        scale_bw = (double)N * sizeof(double) / tscale / 1e9;
        add_bw = (double)N * sizeof(double) / tadd / 1e9;
        triad_bw = (double)N * sizeof(double) / ttriad / 1e9;
        // printf("Trial %d: Copy = %.2f GB/s, Scale = %.2f GB/s, Add = %.2f GB/s, Triad = %.2f GB/s\n",
        //     i, copy_bw, scale_bw, add_bw, triad_bw);

        printf("%.2f\n",
            (copy_bw +scale_bw+ add_bw+ triad_bw) / 4.0);
    }
    free(a);
    free(b);
    free(c);
    return 0;
}

