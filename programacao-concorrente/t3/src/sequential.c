/**
 * Universidade de Sao Paulo
 * Instituto de Ciencias Matematicas e de Computacao
 * 
 * Trabalho 3: Smooth
 * Algoritmo Sequencial
 * 
 * SSC0143 Programacao Concorrente
 * 
 * Elias Italiano Rodrigues 7987251
 * Rodolfo Megiato de Lima  7987286
 * Vinicius Katata Biondo   6783972
 */
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <sys/time.h>
#include "../inc/image.h"
#include "../inc/ppm_p3.h"
#include "../inc/smooth.h"

/**
 * Programa principal
 */
int main(int argc, char **argv)
{
	ppm_header_t ppm; // cabecalho do arquivo PPM P3
	pixel_t **img; // imagem inteira
	int i;
	
	struct timeval start, end;

	// Opcoes do programa
	int c;
	int do_output = 1, do_time = 0;
	struct option long_options[] = {
		{"no-output", no_argument,  &do_output, 0 },
		{"time",      no_argument,  &do_time,   1 },
		{0, 0, 0, 0}
	};

	// Tratamento dos argumentos usando optget
	while ((c = getopt_long(argc, argv, ":nt", long_options, NULL)) != -1) {
		switch (c) {
			case 'n':
				do_output = 0;
				break;
			case 't':
				do_time = 1;
				break;
			case 0:
			case '?':
			case ':':
			default:
				break;
		}
	}
	
	// Le imagem da entrada padrao
	img = ppm_p3_scan(&ppm);

	gettimeofday(&start, NULL);

	// Aplica o algoritmo smooth para cada pixel lido da imagem
	smooth(img, ppm.width, ppm.height, 0);

	gettimeofday(&end, NULL);

	// Imprime tempo de processamento
	if (do_time) {
		printf("%.2lf\n", (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec) / 1000000.0 );
	}

	// Imprime imagem apos aplicado o smooth
	if (do_output) {
		ppm_p3_print(img, &ppm);
	}

	// Libera memoria
	for (i = 0; i < ppm.height; i++) {
		free(img[i]);
	}
	free(img);
	
	return EXIT_SUCCESS;
}

