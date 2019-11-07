#include "testfunctions.h"


double f1(double x)
{
    (void) x;
    const double L1 = __L1;

    return L1;
}

double f2(double x)
{
    (void) x;
    const double L2 = __L2;

    return L2;
}

double f3(double x)
{
    (void) x;
    const double L3 = __L3;
    
    return L3;
}

double f4(double x)
{
    (void) x;
    const double L4 = __L4;

    return L4;
}