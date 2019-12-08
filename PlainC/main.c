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

#include "../testfunctions/testfunctions.h"


const double epsilon = 0.0000001; // we'll see
double global_fmin = INFINITY;
 
double alg1 (double, double);
double alg2 (double, double);
double alg3 (double, double);
double alg4 (double, double);
 
int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;
    struct timeval t1, t2;
    double avg_time, min_time, max_time;
    double elapsed_time;
    double ret;


    double a3 = __A3;
    double b3 = __B3;

    elapsed_time = 0;
    avg_time = 0;
    min_time = INFINITY;
    max_time = 0;
    for (int i = 0; i < 1000; ++i) {
        global_fmin = INFINITY;
        gettimeofday(&t1, NULL);
        ret = alg3(a3,b3);
        gettimeofday(&t2, NULL);
        elapsed_time = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
        elapsed_time += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
        avg_time += elapsed_time;
        if (min_time > elapsed_time)
            min_time = elapsed_time;
        if (max_time < elapsed_time)
            max_time = elapsed_time;
    }
    avg_time /= 1000;
    printf("f3 average time=%.3fms\nf3 min time=%.3fms\nf3 max time=%.3fms\nx=%f\nf(x)=%f\n\n", avg_time, min_time, max_time, ret, f3(ret));

    double a4 = __A4;
    double b4 = __B4;
    
    elapsed_time = 0;
    avg_time = 0;
    min_time = INFINITY;
    max_time = 0;
    for (int i = 0; i < 1000; ++i) {
        global_fmin = INFINITY; 
        gettimeofday(&t1, NULL);
        ret = alg4(a4,b4);
        gettimeofday(&t2, NULL);
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




double alg1 (double a, double b)
{
    double xmin = h1min (a, b);  //  x value for which h(x) is min 
    double hmin = h1(xmin, a, b);  //  min(h(x))
    double to_return1, to_return2; // temp
    double f_val;   // temp2 xd
    // printf("a=%f, b=%f, xmin=%f, hmin=%f, global_fmin=%f, fabs=%f\n", a, b, xmin, hmin, global_fmin, fabs(a-b));

    // this is important - global_fmin should be a vector, and.. W8 xd - just update global_fmin according to f_val :V - so much faster O:)
    if (hmin > global_fmin)
        return NAN;
    else if (fabs(a - b) < epsilon)
    {
        f_val = f1(xmin);
        if (global_fmin > f_val)
            global_fmin = f_val; // change global fmin -- it should have mutex on it later
        return xmin;
    }
    else
    {
        f_val = f1(xmin);
        if (global_fmin > f_val)
            global_fmin = f_val; // change global fmin -- it should have mutex on it later
        
        to_return1 = alg1 (a, xmin);
        to_return2 = alg1 (xmin, b);

        if (isnan(to_return1))
            return to_return2;
        else if (isnan(to_return2))
            return to_return1;
        else if (f1(to_return1) < f1(to_return2))
            return to_return1;
        else
            return to_return2;
        
    }
    
}

double alg2 (double a, double b)
{
    double xmin = h2min (a, b);  //  x value for which h(x) is min 
    double hmin = h2(xmin, a, b);  //  min(h(x))
    double to_return1, to_return2; // temp
    double f_val;   // temp2 xd
    // printf("a=%f, b=%f, xmin=%f, hmin=%f, global_fmin=%f, fabs=%f\n", a, b, xmin, hmin, global_fmin, fabs(a-b));

    // this is important - global_fmin should be a vector, and.. W8 xd - just update global_fmin according to f_val :V - so much faster O:)
    if (hmin > global_fmin)
        return NAN;
    else if (fabs(a - b) < epsilon)
    {
        f_val = f2(xmin);
        if (global_fmin > f_val)
            global_fmin = f_val; // change global fmin -- it should have mutex on it later
        return xmin;
    }
    else
    {
        f_val = f2(xmin);
        if (global_fmin > f_val)
            global_fmin = f_val; // change global fmin -- it should have mutex on it later
        
        to_return1 = alg2 (a, xmin);
        to_return2 = alg2 (xmin, b);

        if (isnan(to_return1))
            return to_return2;
        else if (isnan(to_return2))
            return to_return1;
        else if (f2(to_return1) < f2(to_return2))
            return to_return1;
        else
            return to_return2;
        
    }
    
}

double alg3 (double a, double b)
{
    double xmin = h3min (a, b);  //  x value for which h(x) is min 
    double hmin = h3(xmin, a, b);  //  min(h(x))
    double to_return1, to_return2; // temp
    double f_val;   // temp2 xd
    // printf("a=%f, b=%f, xmin=%f, hmin=%f, global_fmin=%f, fabs=%f\n", a, b, xmin, hmin, global_fmin, fabs(a-b));

    // this is important - global_fmin should be a vector, and.. W8 xd - just update global_fmin according to f_val :V - so much faster O:)
    if (hmin > global_fmin)
        return NAN;
    else if (fabs(a - b) < epsilon)
    {
        f_val = f3(xmin);
        if (global_fmin > f_val)
            global_fmin = f_val; // change global fmin -- it should have mutex on it later
        return xmin;
    }
    else
    {
        f_val = f3(xmin);
        if (global_fmin > f_val)
            global_fmin = f_val; // change global fmin -- it should have mutex on it later
        
        to_return1 = alg3 (a, xmin);
        to_return2 = alg3 (xmin, b);

        if (isnan(to_return1))
            return to_return2;
        else if (isnan(to_return2))
            return to_return1;
        else if (f3(to_return1) < f3(to_return2))
            return to_return1;
        else
            return to_return2;
        
    }
    
}

double alg4 (double a, double b)
{
    double xmin = h4min (a, b);  //  x value for which h(x) is min 
    double hmin = h4(xmin, a, b);  //  min(h(x))
    double to_return1, to_return2; // temp
    double f_val;   // temp2 xd
    // printf("a=%f, b=%f, xmin=%f, hmin=%f, global_fmin=%f, fabs=%f\n", a, b, xmin, hmin, global_fmin, fabs(a-b));

    // this is important - global_fmin should be a vector, and.. W8 xd - just update global_fmin according to f_val :V - so much faster O:)
    if (hmin > global_fmin)
        return NAN;
    else if (fabs(a - b) < epsilon)
    {
        f_val = f4(xmin);
        if (global_fmin > f_val)
            global_fmin = f_val; // change global fmin -- it should have mutex on it later
        return xmin;
    }
    else
    {
        f_val = f4(xmin);
        if (global_fmin > f_val)
            global_fmin = f_val; // change global fmin -- it should have mutex on it later
        
        to_return1 = alg4 (a, xmin);
        to_return2 = alg4 (xmin, b);

        if (isnan(to_return1))
            return to_return2;
        else if (isnan(to_return2))
            return to_return1;
        else if (f4(to_return1) < f4(to_return2))
            return to_return1;
        else
            return to_return2;
        
    }
    
}