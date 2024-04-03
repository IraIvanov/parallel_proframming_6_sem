#include "config.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

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

double* recieve(){
    return NULL;
}

double* solve(double startX, double stepX, int32_t countX, double startT, double stepT, int32_t countT, char timeIsZeroFlag )
{   
    double* solution = (double*)calloc(countX*countT , sizeof(double));
    if (solution == NULL)
    {
        return NULL;
    }
    if (timeIsZeroFlag)
    {
        for (int j = 0; j < countT; j++)
        {
            double tau = startT + stepT*j;
            solution[j*countX + 0] = BoundaryCondX(tau);
        }

    }   else {

        recieve();
    }
    for (int n = 0; n < countX; n++)
    {
        double x = startX + n*stepX;
        solution[n] = BoundaryCondT(x);
    }

    for (int n = 1; n < countX; n++)
    {   
        double x = startX + stepX*n;
        for (int j = 0; j < (countT - 1); j++)
        {      
            double t = startT + stepT*j;
            double y_j_n = solution[j*countX + n];
            double y_j_n_1 = solution[j*countX + n - 1];
            double tmp = Function(x, t) - 2*(y_j_n - y_j_n_1)/stepX;
            double y = tmp*stepT + y_j_n;
            solution[(j + 1)*countX + n] = y;
        }

    }

    return solution; 
}