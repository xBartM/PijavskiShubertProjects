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
 
int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;
    
    // printf("%f\n", __L1);
    printf("Hello, World!\n");
    return 0;
}
