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
#include <stdlib.h>
#include <unistd.h>

#include "../testfunctions/testfunctions.h"


typedef struct 
{
    double a;
    double b;

} Interval;


Interval * Ival_begin;      // pointer pointing at the beginning of the array of Intervals
Interval * Ival_read_ptr;   // pointer at which to read next values
Interval * Ival_write_ptr;  // pointer at which to insert next values
Interval * Ival_end;        // pointer pointing at the end of the array of Intervals

const double epsilon = 0.0000001; // we'll see
double global_fmin = INFINITY;
double global_xmin = 0;

void populateInterval(Interval *, double, double, int);

int main(int argc, char *argv[])
{
    if (getenv("OMP_CANCELLATION") == NULL)
    {
        fprintf(stderr, "No OMP_CANCELLATION env\n");
        fprintf(stderr, "Run: export OMP_CANCELLATION=\"true\"\n");
        return -1;
    }

    if (argc != 3)
    {
        fprintf(stderr, "Need more args:\n");
        fprintf(stderr, "1. number of tests\n");
        fprintf(stderr, "2. max number of itarations\n");
        fprintf(stderr, "x. TBA\n");

        return -1;
    }

    int n_tests = atoi(argv[1]);
    int max_iters = atoi(argv[2]);

    struct timeval t1, t2;
    double avg_time, min_time, max_time;
    double elapsed_time;

    Ival_begin = Ival_read_ptr = Ival_write_ptr = (Interval *) malloc(sizeof (Interval) * max_iters);
    Ival_end = (&Ival_read_ptr[max_iters-1]) + 1;


    double a3 = __A3;
    double b3 = __B3;
    // double diff = fabs(a3 - b3);

    double xmin;
    double hmin;
    double f_val;

    long time_wasted = 0l;

    // divide the interval
    // populateInterval(Ival_write_ptr, a3, b3, 40);
    Ival_write_ptr->a = a3;
    Ival_write_ptr->b = b3;
    ++Ival_write_ptr;


    elapsed_time = 0;
    avg_time = 0;
    min_time = INFINITY;
    max_time = 0;
    for (int i = 0; i < n_tests; ++i) {
        global_fmin = INFINITY;
        Ival_read_ptr = Ival_write_ptr = Ival_begin;
        ++Ival_write_ptr;
        gettimeofday(&t1, NULL);

        #pragma omp parallel num_threads(4)
        {
            #pragma omp for schedule(static, 2300), private(a3, b3, xmin, hmin, f_val)
            for (Ival_read_ptr = Ival_begin; Ival_read_ptr < Ival_end; ++Ival_read_ptr)
            {
                #pragma omp cancellation point for

                while (Ival_read_ptr >= Ival_write_ptr)
                {
                    // #pragma omp atomic
                    // time_wasted++;
                    // hold up until there is something to do
                    #pragma omp cancellation point for
                    // usleep(1); // I know - it's bad - active waiting - but gets the job done :>
                }

                // #pragma omp ordered
                // {
                    a3 = Ival_read_ptr->a;
                    b3 = Ival_read_ptr->b;
                // }

                // actual algorithm - inlining gives superior performacne :O

                xmin = h3min (a3, b3);  //  x value for which h(x) is min 
                hmin = h3(xmin, a3, b3);  //  min(h(x))
                
                // printf("a=%f, b=%f, xmin=%f, hmin=%f, global_fmin=%f, fabs=%f\n", a, b, xmin, hmin, global_fmin, fabs(a-b));

                if (hmin > global_fmin)
                    xmin = NAN;
                else
                {
                    f_val = f3(xmin);
                    #pragma omp critical (update_globals)
                    {
                        if (global_fmin > f_val)
                        {
                            global_fmin = f_val; // change global fmin -- it should have mutex on it later
                            global_xmin = xmin;
                        }
                    }
                }

                if (!isnan(xmin))
                {
                    if (fabs(a3 - b3) < epsilon)
                    {
                        #pragma omp cancel for
                        // break;
                    }
                    
                    #pragma omp critical (update_write_pointer)
                    {
                        if (Ival_write_ptr != Ival_end)
                        {
                            Ival_write_ptr->a = a3;
                            Ival_write_ptr->b = xmin;
                            
                            ++Ival_write_ptr;
                            if (Ival_write_ptr != Ival_end)
                            {
                                Ival_write_ptr->a = xmin;
                                Ival_write_ptr->b = b3;

                                ++Ival_write_ptr;            
                            }
                        }
                    }
                }

            }
        }

        gettimeofday(&t2, NULL);
        elapsed_time = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
        elapsed_time += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
        avg_time += elapsed_time;
        if (min_time > elapsed_time)
            min_time = elapsed_time;
        if (max_time < elapsed_time)
            max_time = elapsed_time;
    }
    avg_time /= n_tests;
    printf("f3 average time=%.3fms\nf3 min time=%.3fms\nf3 max time=%.3fms\nx=%f\nf(x)=%f\n\n", avg_time, min_time, max_time, global_xmin, global_fmin);
    // printf("%ld\n", time_wasted);
    free(Ival_begin);
    return 0;
}


void populateInterval(Interval * insert, double a, double b, int slices)
{
    double diff = fabs (a - b) / slices;

    for (int i = 0; i < slices; ++i)
    {
        insert->a = a + diff * i;
        insert->b = b - diff * (slices - i - 1);
        printf ("a=%f\nb=%f\n", insert->a, insert->b);
        ++insert;

    }
    

    
    

}