#include "mpi.h"
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#define eps 0.0001
#define a 2.0

double Function(double x, double t)
{
    return x + t;
}

double BoundaryCondX(double t)
{
    return exp(-t);
}

double BoundaryCondT(double x)
{
    return cos( M_PI*x );
}

double* solve(double startX, double stepX, int32_t countX, double startT, double stepT, int32_t countT, char timeIsZeroFlag, int next_node )
{   
    //printf("c x %d, c t %d\n", countX, countT);
    double* solution = (double*)calloc(countX*countT , sizeof(double));
    if (solution == NULL)
    {
        printf("ALLOC ERROR\n");
        return NULL;
    }

    for (int n = 0; n < countX; n++)
    {
        double x = startX + n*stepX;
        solution[n] = BoundaryCondT(x);
    }

    if (next_node != -1)
    {
        //printf("send to next node %d\n", next_node);
        MPI_Send(&(solution[countX - 1]), 1, MPI_DOUBLE, next_node, 1, MPI_COMM_WORLD);
    }

    for (int j = 0; j < (countT - 1); j++)
    {   
        double t = startT + stepT*j;
        double y = 0.0;
        double y_j_n_1 = 0.0;
        if (timeIsZeroFlag)
        {
            y_j_n_1 = BoundaryCondX(t);
        }
        else
        {
            //printf("recieving by prev node, time %d out of %d\n", j, countT);
            MPI_Recv(&y_j_n_1, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, NULL);
            //printf("recieved by prev node\n");
        }
        y = y_j_n_1;
        for (int n = 1; n < countX; n++)
        {   
            if (n != 1)
            {
                y_j_n_1 = solution[j*countX + n - 1];
            }
            double x = startX + stepX*n;
            double y_j_n = solution[j*countX + n];
            double tmp = Function(x, t) - 2*(y_j_n - y_j_n_1)/stepX;
            y = tmp*stepT + y_j_n;
            solution[(j + 1)*countX + n] = y;
        }
        if (next_node != -1)
        {
            //printf("sending to node %d\n", next_node);
            MPI_Send(&y, 1, MPI_DOUBLE, next_node, 1, MPI_COMM_WORLD);
        }
    }

    return solution; 
}

int main(int argc, char** argv){

    int rank, size;

    MPI_Init(&argc, &argv);

    double start_time = MPI_Wtime();

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    double startX = 0.0, endX = 1.0;
    double startT = 0.0, endT = 0.5;
    double stepX = 0.01;
    double stepT = 0.005;
    
    int32_t CountX = (endX - startX)/stepX;
    int32_t CountT = (endT - startT)/stepT;
    
    int32_t nodeCountX = CountX/size;
    int32_t nodeCountT = CountT/size;
    double nodeStartX = startX + stepX*nodeCountX*rank;
    
    double nodeStartT = startT; //+ stepT*nodeCountT*rank;
    
    if (rank == 0) {

        double* solution = solve(nodeStartX, stepX, nodeCountX, nodeStartT, stepT, nodeCountT, 1, 1);

        FILE* file = fopen("data.txt", "w+");

        if (file == NULL)
        {
            printf("ERROR, can't create file\n");
            exit(1);
        }

        for ( int j = 0; j < nodeCountT; j++ )
        {
            for ( int i = 0; i < nodeCountX; i++ )
            {
                fprintf(file, "%g %g %g\n", startT + j*stepT,startX + i*stepX, solution[j*nodeCountT + i]);
            }
        }

        free(solution);

        for (int i = 1; i < size; i++)
        {   
            int32_t array_size = nodeCountT*nodeCountX;
            double* data = calloc(array_size, sizeof(double)); 
            MPI_Recv((void*)data, array_size, MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD, NULL);
            for ( int j = 0; j < nodeCountT; j++ )
            {
                for ( int n = 0; n < nodeCountX; n++ )
                {
                    fprintf(file, "%g %g %g\n", startT + j*stepT,startX + stepX*nodeCountX*i + n*stepX, data[j*nodeCountT + n]);
                }
            }
        }
        
        fclose(file);

    }

    else 
    {

        int nextNode = rank + 1;

        if (nextNode == size)
        {
            nextNode = -1;
        }
        double* solution = solve(nodeStartX, stepX, nodeCountX, nodeStartT, stepT, nodeCountT, 0, nextNode);
        int32_t size = nodeCountX*nodeCountT;
        MPI_Send(solution, size, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
        free(solution);
    }

    MPI_Finalize();
    return 0;
}
