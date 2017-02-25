/**
 * Universidade de Sao Paulo
 * Instituto de Ciencias Matematicas e de Computacao
 * 
 * Trabalho 3: Smooth
 * 
 * SSC0143 Programacao Concorrente
 * 
 * Elias Italiano Rodrigues 7987251
 * Rodolfo Megiato de Lima  7987286
 * Vinicius Katata Biondo   6783972
 */
#include <stdlib.h>
#include <stdio.h>
#include "../inc/ppm_p3.h"
#include "../inc/image.h"

/**
 * Le uma imagem PPM P3 da entrada padrao
 */
pixel_t **ppm_p3_scan(ppm_header_t *ppm)
{
	int i, npixels;
	pixel_t **img;

	// Le o cabecalho
	scanf("%s", ppm->format);
	scanf("%d", &ppm->width);
	scanf("%d", &ppm->height);
	scanf("%d", &ppm->max_value);
	
	// Aloca matriz de pixels para guardar a imagem inteira
	img = (pixel_t **)malloc(sizeof(pixel_t *) * ppm->height);
	if (img == NULL) {
		fprintf(stderr, "ERROR: could not allocate memory for image\n");
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < ppm->height; i++) {
		img[i] = (pixel_t *)malloc(sizeof(pixel_t) * ppm->width);
		if (img[i] == NULL) {
			fprintf(stderr, "ERROR: could not allocate memory for image\n");
			exit(EXIT_FAILURE);
		}
	}

	// Le e carrega a imagem para a memoria
	npixels = ppm->width * ppm->height;
	for (i = 0; i < npixels; i++) {
		scanf("%hhu", &img[i / ppm->width][i % ppm->width][R]);
		scanf("%hhu", &img[i / ppm->width][i % ppm->width][G]);
		scanf("%hhu", &img[i / ppm->width][i % ppm->width][B]);
	}
	
	return img;
}

/**
 * Imprime uma imagem PPM P3 na saida padrao
 */
void ppm_p3_print(pixel_t **img, ppm_header_t *ppm)
{
	int i, j;

	printf("%s\n", ppm->format);
	printf("%d %d\n", ppm->width, ppm->height);
	printf("%d\n", ppm->max_value);

	for (i = 0; i < ppm->height; i++) {
		for (j = 0; j < ppm->width; j++) {
			printf("%hhu ", img[i][j][R]);
			printf("%hhu ", img[i][j][G]);
			printf("%hhu ", img[i][j][B]);
		}
	}
}

