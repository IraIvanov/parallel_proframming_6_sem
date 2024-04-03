#include <math.h>
#include <stdint.h>

#define eps 0.0001
#define a 2.0

double Function(double x, double t); 

double BoundaryCondX(double t);

double BoundaryCondT(double x);

double* solve(double startX, double stepX, int32_t countX, double startT, double stepT, int32_t countT, char timeIsZeroFlag );

double* recieve();