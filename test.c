#include <stdio.h>
#include <stdlib.h>

int main(){
	double* n = (double*)calloc(200, sizeof(double));
	
	for (int i=0; i < 20; i++){
		for (int j=0; j < 10; j++){
			n[i*10 + j] = i + 1;
		}

	}

	for (int i=0; i < 20; i++){
                for (int j=0; j < 10; j++){
                        printf("%g ", n[i*10 + j]);
                }
		printf("\n");

        }
	return 0;
}
