#ifndef _FLOYD_
#define _FLOYD_

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <omp.h>

/**
 * Algoritmo Floyd-Warshall
 * 
 * Retorna a menor distancia entre duas juncoes mais distantes
 */
int floyd(int **town, int n, int nthreads);

#endif
