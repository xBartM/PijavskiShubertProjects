#pragma once

#define __L1 0.16807
#define __L2 1.708108607669900925176251096981825714586382224716205031244954883830979398142502538246 
#define __L3 4.13062
#define __L4 17.3365


double f1(double x);
double f2(double x);
double f3(double x);
double f4(double x);

//  h_ is an approximation of function f_
double h1(double x, double a, double b); // [a, b] is a given interval
double h2(double x, double a, double b);
double h3(double x, double a, double b);
double h4(double x, double a, double b);

//  h_min returns x for which h_ is minimal
double h1min(double a, double b); // [a, b] is a given interval
double h2min(double a, double b);
double h3min(double a, double b);
double h4min(double a, double b);

