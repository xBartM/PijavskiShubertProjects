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

#include <mpi.h>

#include "../testfunctions/testfunctions.h"


typedef struct 
{
    double a;
    double b;

} Interval;

typedef struct 
{
    double a;
    double b;
    double xmin;
    double hmin;
    double f_val;
} Fvals;



Interval * Ival_begin;      // pointer pointing at the beginning of the array of Intervals
Interval * Ival_read_ptr;   // pointer at which to read next values
Interval * Ival_write_ptr;  // pointer at which to insert next values
Interval * Ival_end;        // pointer pointing at the end of the array of Intervals

const double epsilon = 0.0000001; // we'll see
double global_fmin = INFINITY;
double global_xmin = 0;

int main(int argc, char *argv[])
{
    // if (getenv("OMP_CANCELLATION") == NULL)
    // {
    //     fprintf(stderr, "No OMP_CANCELLATION env\n");
    //     fprintf(stderr, "Run: export OMP_CANCELLATION=\"true\"\n");
    //     return -1;
    // }

    if (argc != 3)
    {
        fprintf(stderr, "Need more args:\n");
        fprintf(stderr, "1. number of tests\n");
        fprintf(stderr, "2. max number of itarations\n");
        // fprintf(stderr, "x. TBA\n");

        return -1;
    }

    int rank, size;

    int n_tests = atoi(argv[1]);
    int max_iters = atoi(argv[2]);

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // custom datatype
    MPI_Datatype dt_interval;
    MPI_Type_contiguous(2, MPI_DOUBLE, &dt_interval);
    MPI_Type_commit(&dt_interval);

    MPI_Datatype dt_fvals;
    MPI_Type_contiguous(5, MPI_DOUBLE, &dt_fvals);
    MPI_Type_commit(&dt_fvals);

    if (rank == 0)
    {
        if (size == 1)
        {
            fprintf(stderr, "Not enough processes: Run with: mpiexec -n <number of processes> ./main <number of tests> <max number of iterations>\n");
            MPI_Finalize();
            return -1;
        }

        int sent = 0;   // number of sent messages
        int stop = 0;  



        struct timeval t1, t2;
        double avg_time, min_time, max_time;
        double elapsed_time;

        Ival_begin = Ival_read_ptr = Ival_write_ptr = (Interval *) malloc(sizeof (Interval) * max_iters);
        Ival_end = (&Ival_read_ptr[max_iters-1]) + 1;


        Fvals xhfmin_buffer;
        MPI_Request request;

        //----------------- f3 ------------------


        double a3 = __A3;
        double b3 = __B3;

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
            stop = 0;
            sent = 0;
            gettimeofday(&t1, NULL);

            for (; Ival_read_ptr < Ival_end;)// ++Ival_read_ptr)
            {
                if (stop == 1)
                    break;

                // send as much as possible
                while (sent < size-1 && Ival_read_ptr < Ival_write_ptr)
                {
                    MPI_Isend(Ival_read_ptr, 1, dt_interval, sent+1, 0, MPI_COMM_WORLD, &request);
                    // printf("sending to: %d\n", sent+1);
                    ++sent;
                    ++Ival_read_ptr;
                }

                // receive everything sent
                while (sent > 0)
                {
                    // get anything
                    MPI_Recv(&xhfmin_buffer, 1, dt_fvals, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    // printf("messages to receive: %d\n", sent);

                    --sent;

                    if (xhfmin_buffer.hmin > global_fmin)
                        xhfmin_buffer.xmin = NAN;
                    else
                    {
                        if (global_fmin > xhfmin_buffer.f_val)
                        {
                            global_fmin = xhfmin_buffer.f_val; // change global fmin -- it should have mutex on it later
                            global_xmin = xhfmin_buffer.xmin;
                        }
                        
                    }

                    if (!isnan(xhfmin_buffer.xmin))
                    {
                        if (fabs(xhfmin_buffer.a - xhfmin_buffer.b) < epsilon)
                        {
                            // im not sure about this :V
                            while(sent > 0)
                            {
                                MPI_Recv(&xhfmin_buffer, 1, dt_fvals, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                                // printf("(stop) messages to receive: %d\n", sent);

                                --sent;
                            }

                            Ival_read_ptr->a = NAN;
                            while(sent < size-1)
                            {
                                MPI_Send(Ival_read_ptr, 1, dt_interval, sent+1, 0, MPI_COMM_WORLD);
                                // printf("sending stop signal to: %d\n", sent+1);
                                
                                ++sent;
                            }
                            
                            stop = 1;
                            break;
                        }
                        
                        if (Ival_write_ptr != Ival_end)
                        {
                            Ival_write_ptr->a = xhfmin_buffer.a;
                            Ival_write_ptr->b = xhfmin_buffer.xmin;
                            
                            ++Ival_write_ptr;
                            if (Ival_write_ptr != Ival_end)
                            {
                                Ival_write_ptr->a = xhfmin_buffer.xmin;
                                Ival_write_ptr->b = xhfmin_buffer.b;

                                ++Ival_write_ptr;            
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


        //----------------- f4 ------------------

        Ival_read_ptr = Ival_write_ptr = Ival_begin;

        double a4 = __A4;
        double b4 = __B4;

        Ival_write_ptr->a = a4;
        Ival_write_ptr->b = b4;
        ++Ival_write_ptr;


        elapsed_time = 0;
        avg_time = 0;
        min_time = INFINITY;
        max_time = 0;
        for (int i = 0; i < n_tests; ++i) {
            global_fmin = INFINITY;
            Ival_read_ptr = Ival_write_ptr = Ival_begin;
            ++Ival_write_ptr;
            stop = 0;
            sent = 0;
            gettimeofday(&t1, NULL);

            for (; Ival_read_ptr < Ival_end;)
            {
                if (stop == 1)
                    break;

                // send as much as possible
                while (sent < size-1 && Ival_read_ptr < Ival_write_ptr)
                {
                    MPI_Isend(Ival_read_ptr, 1, dt_interval, sent+1, 0, MPI_COMM_WORLD, &request);
                    // printf("sending to: %d\n", sent+1);
                    ++sent;
                    ++Ival_read_ptr;
                }

                // receive everything sent
                while (sent > 0)
                {
                    // get anything
                    MPI_Recv(&xhfmin_buffer, 1, dt_fvals, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    // printf("messages to receive: %d\n", sent);

                    --sent;

                    if (xhfmin_buffer.hmin > global_fmin)
                        xhfmin_buffer.xmin = NAN;
                    else
                    {
                        if (global_fmin > xhfmin_buffer.f_val)
                        {
                            global_fmin = xhfmin_buffer.f_val; // change global fmin -- it should have mutex on it later
                            global_xmin = xhfmin_buffer.xmin;
                        }
                        
                    }

                    if (!isnan(xhfmin_buffer.xmin))
                    {
                        if (fabs(xhfmin_buffer.a - xhfmin_buffer.b) < epsilon)
                        {
                            // im not sure about this :V
                            while(sent > 0)
                            {
                                MPI_Recv(&xhfmin_buffer, 1, dt_fvals, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                                // printf("(stop) messages to receive: %d\n", sent);

                                --sent;
                            }

                            Ival_read_ptr->a = NAN;
                            while(sent < size-1)
                            {
                                MPI_Send(Ival_read_ptr, 1, dt_interval, sent+1, 0, MPI_COMM_WORLD);
                                // printf("sending stop signal to: %d\n", sent+1);
                                
                                ++sent;
                            }
                            
                            stop = 1;
                            break;
                        }
                        
                        if (Ival_write_ptr != Ival_end)
                        {
                            Ival_write_ptr->a = xhfmin_buffer.a;
                            Ival_write_ptr->b = xhfmin_buffer.xmin;
                            
                            ++Ival_write_ptr;
                            if (Ival_write_ptr != Ival_end)
                            {
                                Ival_write_ptr->a = xhfmin_buffer.xmin;
                                Ival_write_ptr->b = xhfmin_buffer.b;

                                ++Ival_write_ptr;            
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
        printf("f4 average time=%.3fms\nf4 min time=%.3fms\nf4 max time=%.3fms\nx=%f\nf(x)=%f\n\n", avg_time, min_time, max_time, global_xmin, global_fmin);



        free(Ival_begin);
    }
    else    // if your rank != 0
    {
        Interval ival_buffer;
        Fvals xhfmin_buffer;

        ival_buffer.a = INFINITY;
        ival_buffer.b = INFINITY;

        //----------------- f3 ------------------
       
        for (int i = 0; i < n_tests; ++i) 
        {
            while (1)
            {
                MPI_Recv(&ival_buffer, 1, dt_interval, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                
                if (isnan(ival_buffer.a))
                    break; 

                xhfmin_buffer.a = ival_buffer.a;
                xhfmin_buffer.b = ival_buffer.b;
                xhfmin_buffer.xmin = h3min (ival_buffer.a, ival_buffer.b);  //  x value for which h(x) is min 
                xhfmin_buffer.hmin = h3(xhfmin_buffer.xmin, ival_buffer.a, ival_buffer.b);  //  min(h(x))
                xhfmin_buffer.f_val = f3(xhfmin_buffer.xmin);

                MPI_Send(&xhfmin_buffer, 1, dt_fvals, 0, 0, MPI_COMM_WORLD);
            }
        }


        //----------------- f4 ------------------
       
        for (int i = 0; i < n_tests; ++i) 
        {
            while (1)
            {
                MPI_Recv(&ival_buffer, 1, dt_interval, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                
                if (isnan(ival_buffer.a))
                    break; 

                xhfmin_buffer.a = ival_buffer.a;
                xhfmin_buffer.b = ival_buffer.b;
                xhfmin_buffer.xmin = h4min (ival_buffer.a, ival_buffer.b);  //  x value for which h(x) is min 
                xhfmin_buffer.hmin = h4(xhfmin_buffer.xmin, ival_buffer.a, ival_buffer.b);  //  min(h(x))
                xhfmin_buffer.f_val = f4(xhfmin_buffer.xmin);

                MPI_Send(&xhfmin_buffer, 1, dt_fvals, 0, 0, MPI_COMM_WORLD);
            }
        }

    }
    
    MPI_Finalize();
    return 0;
}


