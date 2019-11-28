#include "testfunctions.h"
#include <math.h>


double f1(double x)
{
    if (x < __FLT_EPSILON__ && x > -__FLT_EPSILON__) return 1.0; // yeah, no
    return sin(x)/x;
}

double f2(double x)
{
    return x*x/40.0+(1.0-cos(x));
}

double f3(double x)
{
   return 5.0 * (sin(0.2 * x) + cos(0.7 * x));
}

double f4(double x)
{
    return sin(cos(x)) * x;
}


double h1(double x, double a, double b)
{
    double lhand = f1(a) - __L1 * (x - a);
    double rhand = f1(b) - __L1 * (b - x);

    return fmax(lhand, rhand);
}

double h2(double x, double a, double b)
{
    double lhand = f2(a) - __L2 * (x - a);
    double rhand = f2(b) - __L2 * (b - x);

    return fmax(lhand, rhand);
}

double h3(double x, double a, double b)
{
    double lhand = f3(a) - __L3 * (x - a);
    double rhand = f3(b) - __L3 * (b - x);

    return fmax(lhand, rhand);
}

double h4(double x, double a, double b)
{
    double lhand = f4(a) - __L4 * (x - a);
    double rhand = f4(b) - __L4 * (b - x);

    return fmax(lhand, rhand);

}


double h1min(double a, double b)
{
    return (f1(a) - f1(b) + __L1 * (b - a))/(2.0 * __L1);
}

double h2min(double a, double b)
{
    return (f2(a) - f2(b) + __L2 * (b - a))/(2.0 * __L2);
}

double h3min(double a, double b)
{
    return (f3(a) - f3(b) + __L3 * (b - a))/(2.0 * __L3);
}

double h4min(double a, double b)
{
    return (f4(a) - f4(b) + __L4 * (b - a))/(2.0 * __L4);
}