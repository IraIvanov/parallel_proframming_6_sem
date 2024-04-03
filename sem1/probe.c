#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "mpi.h"

#define TAG 1

int main(int argc, char** argv){

    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    if(rank == 0){
        void* data;
        int32_t str_size;
        FILE *fd = fopen("probe.c", "rb");
        if(fd){

            fseek(fd, 0, SEEK_END);
            str_size = ftell(fd);
            fseek(fd, 0, SEEK_SET);

            data = malloc(str_size);

            if(data){
                fread(data, str_size, 1, fd);
            } else {
                fclose(fd);
                abort();
            }

            fclose(fd);
        } else {
            abort();
        }

        MPI_Send(&str_size, 1, MPI_INT, 1, TAG, MPI_COMM_WORLD);
        MPI_Send(data, str_size, MPI_CHAR, 1, TAG, MPI_COMM_WORLD);

        free(data);
    }
  
    if (rank == 1){
        int32_t str_size = -1;
        MPI_Recv(&str_size, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, NULL);

        // MPI_Status status;
        // MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        // MPI_Get_count(&status, MPI_CHAR, &str_size);

        void* data = malloc(str_size);
        if(data){
            MPI_Recv(data, str_size, MPI_CHAR, 0, TAG, MPI_COMM_WORLD, NULL);
            printf("%d\n", str_size);
        } else {
            abort();
        }

        free(data);
    }

    MPI_Finalize();
}