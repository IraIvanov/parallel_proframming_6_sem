#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "mpi.h"

#define TAG 1

double func(double x){
    return exp(-x*x);
}

double integrate(double begin, int count, double step){
    double acc = 0;
    for(int i = 0 ; i < count ; ++i){
        acc += func(begin + step * i) * step;
    }
    return acc;
}

int main(int argc, char** argv){

    int rank, size;

    MPI_Init(&argc, &argv);

    double start_time = MPI_Wtime();

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("size: %d, rank: %d\n", size, rank);

    double begin = -100.0;
    double end = 100.0; 
    double step = 0.0001;
    int count = (end - begin) / step;
    
    int node_count = count/size;
    double node_begin = begin + node_count * step * rank;
    double node_acc = integrate(node_begin, node_count, step);

    if(rank == 0){
        double acc = node_acc;
        for(int i = 0 ; i < size - 1 ; ++i){
            double temp;
            MPI_Status status;
            MPI_Recv(&temp, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            printf("recv from: %d tag: %d\n", status.MPI_SOURCE, status.MPI_TAG );

            acc += temp;
        }
        double stop_time = MPI_Wtime();

        printf("pi = %.16f, time = %f\n", acc * acc, stop_time - start_time);
    } else {
        srand(time(NULL) + rank);
        MPI_Send(&node_acc, 1, MPI_DOUBLE, 0, rand()%1000, MPI_COMM_WORLD);
    }
    

    MPI_Finalize();
}