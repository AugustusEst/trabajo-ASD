/*
* Algoritmo serie de Monte Carlo para estimar el valor de pi
*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>

int main (int argc, char *argv[]) {
unsigned long long count = 0;
unsigned long long total = 0;
long long i;
long long samples;
double x, y;
int mi_rango, tamanno;
double tiempo_total;

MPI_Init(&argc, &argv);               
MPI_Comm_rank(MPI_COMM_WORLD, &mi_rango);
MPI_Comm_size(MPI_COMM_WORLD, &tamanno);
tiempo_total = MPI_Wtime();
samples=3000000;
if (argc >1)
	samples = atoll(argv[1]);
srand(time(NULL) + mi_rango);
long long n_per_thread = samples / tamanno;
for (i = 0; i < n_per_thread; ++i) {
	x = ((double)rand()) / ((double)RAND_MAX);
	y = ((double)rand()) / ((double)RAND_MAX);
if (x*x+y*y <= 1.0)
	++count;
}
MPI_Reduce(&count, &total, 1, MPI_UNSIGNED_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
if (mi_rango == 0){
	double result = 4.0 * total / samples;
	printf("ERROR DE PRECISION %.10f %\n", fabs(M_PI-result) / M_PI * 100);
	tiempo_total = MPI_Wtime() - tiempo_total;
	printf("TIEMPO DE EJECUCION = %lf s\n", tiempo_total);
	printf ("Valor estimado de pi: %.7f\n", result);
}
MPI_Finalize();
return 0;
}
