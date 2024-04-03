//include "mpi.h"
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include "config.h"

int main(){

    double startX = 0.0, endX = 1.0;
    double startT = 0.0, endT = 0.25;
    int32_t countX = 100, countT = 100;
    double stepX = (endX - startX)/countX;
    double stepT = (endT - startT)/countT;

    double* solution = solve(startX, stepX, countX, startT, stepT, countT, 1);

    FILE* file = fopen("data.txt", "w+");

    if (file == NULL)
    {
        printf("ERROR, can't create file\n");
        exit(1);
    }

    if (solution == NULL)
    {
        printf("ERROR, can't create solution array\n");
        exit(1);
    }

    for ( int j = 0; j < countT; j++ )
    {
        for ( int i = 0; i < countX; i++ )
        {   
            fprintf(file, "%g %g %g\n", startT + j*stepT,startX + i*stepX, solution[j*countT + i]);
        }
    }

    fclose(file);
    free(solution);
    return 0;

}
