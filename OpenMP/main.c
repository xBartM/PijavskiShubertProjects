// Pijavski-Shubert algorithm
// L = max{abs[f(x)']} on the interval [a, b]


// Test functions

// f1(x) = { sin(x) / x, x != 0  
//         { 1         , x == 0 
// interval [a, b] = [-13, 13]
// L1 = 0.16807;

// f2(x) = 1/40 * x^2 +1 - cos(x)
// interval [a, b] = [-20, 20]
// L2 = 1.708108607669900925176251096981825714586382224716205031244954883830979398142502538246; 

// f3(x) = 5 * [sin(0.2 * x) + cos(0.7 * x)]
// interval [a, b] = [-10, 15]
// L3 = 4.13062;

// f4(x) = sin(cos(x)) * x
// interval [a, b] = [-12, 20]
// L4 = 17.3365;

#include <stdio.h>
#include <sys/time.h>
#include <math.h>
// #include <omp.h>

#include "../testfunctions/testfunctions.h"


const double epsilon = 0.0000001; // we'll see
double global_fmin = INFINITY;
double global_xmin = 0;
 
void alg3 (double, double, int);
void alg4 (double, double, int);
 
int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;
    struct timeval t1, t2;
    double avg_time, min_time, max_time;
    double elapsed_time;
    double ret;

    // printf("%d\n", omp_get_thread_limit());
    double a3 = __A3;
    double b3 = __B3;

    elapsed_time = 0;
    avg_time = 0;
    min_time = INFINITY;
    max_time = 0;
    for (int i = 0; i < 1000; ++i) {
        global_fmin = INFINITY;
        gettimeofday(&t1, NULL);
        // ret = alg3(a3,b3,0);
        alg3(a3,b3,0);
        gettimeofday(&t2, NULL);
        ret = global_xmin;
        elapsed_time = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
        elapsed_time += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
        avg_time += elapsed_time;
        if (min_time > elapsed_time)
            min_time = elapsed_time;
        if (max_time < elapsed_time)
            max_time = elapsed_time;
    }
    avg_time /= 1000;
    printf("f3 average time=%.3fms\nf3 min time=%.3fms\nf3 max time=%.3fms\nx=%f\nf(x)=%f\nglobal_fmin%f\n\n", avg_time, min_time, max_time, ret, f3(ret), global_fmin);

    double a4 = __A4;
    double b4 = __B4;
    
    elapsed_time = 0;
    avg_time = 0;
    min_time = INFINITY;
    max_time = 0;
    for (int i = 0; i < 1000; ++i) {
        global_fmin = INFINITY; 
        gettimeofday(&t1, NULL);
        alg4(a4,b4,0);
        gettimeofday(&t2, NULL);
        ret = global_xmin;
        elapsed_time = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
        elapsed_time += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
        avg_time += elapsed_time;
        if (min_time > elapsed_time)
            min_time = elapsed_time;
        if (max_time < elapsed_time)
            max_time = elapsed_time;
        
    }
    avg_time /= 1000;
    printf("f4 average time=%.3fms\nf4 min time=%.3fms\nf4 max time=%.3fms\nx=%f\nf(x)=%f\n\n", avg_time, min_time, max_time, ret, f4(ret));
    return 0;
}





void alg3 (double a, double b, int depth)
{
    double xmin = h3min (a, b);  //  x value for which h(x) is min 
    double hmin = h3(xmin, a, b);  //  min(h(x))
    // double to_return1, to_return2; // temp
    double f_val;   // temp2 xd
    // printf("a=%f, b=%f, xmin=%f, hmin=%f, global_fmin=%f, fabs=%f\n", a, b, xmin, hmin, global_fmin, fabs(a-b));
    // printf("depth=%d\n",depth);

    // this is important - global_fmin should be a vector, and.. W8 xd - just update global_fmin according to f_val :V - so much faster O:)
    if (hmin > global_fmin)
        return;
    else if (fabs(a - b) < epsilon)
    {
        f_val = f3(xmin);
        #pragma omp critical (update_global_fmin)
        {
            if (global_fmin > f_val)
            {
                global_fmin = f_val; // change global fmin -- it should have mutex on it later
                global_xmin = xmin;
            }
        }
        return;
    }
    else
    {
        f_val = f3(xmin);
        #pragma omp critical (update_global_fmin)
        {
            if (global_fmin > f_val)
            {
                global_fmin = f_val; // change global fmin -- it should have mutex on it later
                global_xmin = xmin;
            }
        }

        if (depth == 15)    // gets best performance
        {
            #pragma omp parallel  // num_threads(16) 
            {
                #pragma omp sections nowait
                {
                    #pragma omp section   
                    {
                        alg3 (a, xmin, depth + 1);
                    }
                    #pragma omp section 
                    {
                        alg3 (xmin, b, depth + 1);
                    }
                }
                // #pragma omp barrier
            }
        }
        else 
        {
            alg3 (a, xmin, depth + 1);
            alg3 (xmin, b, depth + 1);
        }
        
        
    }
    
}

void alg4 (double a, double b, int depth)
{
    double xmin = h4min (a, b);  //  x value for which h(x) is min 
    double hmin = h4(xmin, a, b);  //  min(h(x))
    // double to_return1, to_return2; // temp
    double f_val;   // temp2 xd
    // printf("a=%f, b=%f, xmin=%f, hmin=%f, global_fmin=%f, fabs=%f\n", a, b, xmin, hmin, global_fmin, fabs(a-b));

    // this is important - global_fmin should be a vector, and.. W8 xd - just update global_fmin according to f_val :V - so much faster O:)
    if (hmin > global_fmin)
        return;
    else if (fabs(a - b) < epsilon)
    {
        f_val = f4(xmin);
        #pragma omp critical (update_global_fmin)
        {
            if (global_fmin > f_val)
            {
                global_fmin = f_val; // change global fmin -- it should have mutex on it later
                global_xmin = xmin;
            }
        }
        return;
    }
    else
    {
        f_val = f4(xmin);
        #pragma omp critical (update_global_fmin)
        {
            if (global_fmin > f_val)
            {
                global_fmin = f_val; // change global fmin -- it should have mutex on it later
                global_xmin = xmin;
            }
        }

        if (depth == 15)    // gets best performance
        {
            #pragma omp parallel  // num_threads(16) 
            {
                #pragma omp sections nowait
                {
                    #pragma omp section   
                    {
                        alg4 (a, xmin, depth + 1);
                    }
                    #pragma omp section 
                    {
                        alg4 (xmin, b, depth + 1);
                    }
                }
                // #pragma omp barrier
            }
        }
        else 
        {
            alg4 (a, xmin, depth + 1);
            alg4 (xmin, b, depth + 1);
        }
        
    }
    
}