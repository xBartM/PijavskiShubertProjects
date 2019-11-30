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
#include <math.h>

#include "../testfunctions/testfunctions.h"

// const double L1 = __L1;
// const double L2 = __L2; 
// const double L3 = __L3;
// const double L4 = __L4;


const double epsilon = 0.0001; // we'll see
double global_fmin = INFINITY;
 
double alg (double, double);
 
int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    // double a = __A1;
    // double b = __B1;

    // double a = __A2;
    // double b = __B2;

    double a = __A3;
    double b = __B3;

    // double a = __A4;
    // double b = __B4;
    
    double ret = alg(a,b);
    printf("x=%f\nf(x)=%f\n", ret, f3(ret));

   
    printf("Hello, World!\n");
    return 0;
}


double alg (double a, double b)
{
    double xmin = h3min (a, b);  //  x value for which h(x) is min 
    double hmin = h3(xmin, a, b);  //  min(h(x))
    double to_return1, to_return2; // temp
    double f_val;   // temp2 xd
    printf("a=%f, b=%f, xmin=%f, hmin=%f, global_fmin=%f, fabs=%f\n", a, b, xmin, hmin, global_fmin, fabs(a-b));

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
        
        to_return1 = alg (a, xmin);
        to_return2 = alg (xmin, b);

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