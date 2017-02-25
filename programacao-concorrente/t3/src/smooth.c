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
#include <string.h>
#include "../inc/smooth.h"
#include "../inc/image.h"

/**
 * Funcao que aplica o algoritmo smooth para remocao de ruidos em um imagem
 */
void smooth(pixel_t **img, int width, int height, int flag)
{
	int i, j, start = 0, bottom = height;
	int k, l;
	int a, b;
	int mean[3];
	
	pixel_t **img_buffer;
	
	// Define que tipo de fatia esta' lidando
	//	1: ultima fatia
	// 	2: primeira fatia
	// 	3: fatia do meio
	if (flag == 1) {
		start = BLOCK_OFFSET;
		height += BLOCK_OFFSET;
	} else if (flag == 2) {
		height += BLOCK_OFFSET;
		bottom = height + BLOCK_OFFSET;
	} else if (flag == 3) {
		start = BLOCK_OFFSET;
		height += BLOCK_OFFSET;
		bottom = height + BLOCK_OFFSET;
	}

	// Faz uma copia da imagem original para um buffer
	img_buffer = (pixel_t **)malloc(sizeof(pixel_t *) * bottom);
	if (img_buffer == NULL) {
		fprintf(stderr, "ERROR: could not allocate memory for image buffer\n");
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < bottom; i++) {
		img_buffer[i] = (pixel_t *)malloc(sizeof(pixel_t) * width);
		if (img_buffer[i] == NULL) {
			fprintf(stderr, "ERROR: could not allocate memory for image buffer\n");
			exit(EXIT_FAILURE);
		}
		memcpy(img_buffer[i], img[i], sizeof(pixel_t) * width);
	}

	// Para cada pixel da imagem	
	for (i = start; i < height; i++) {
		for (j = 0; j < width; j++) {

			// Soma um bloco considerando o n-esimo pixel como centro desse bloco
			// 	k, l iteram no bloco sendo montado
			mean[R] = 0;
			mean[G] = 0;
			mean[B] = 0;
			for (a = 0, k = i - BLOCK_OFFSET; a < BLOCK_SIZE; a++, k++) {
				for (b = 0, l = j - BLOCK_OFFSET; b < BLOCK_SIZE; b++, l++) {
					if (k < 0 || l < 0 || k >= bottom || l >= width) {
						// Quando nao houver pixel valido (problema da borda)
						//	coloca o pixel padrao
						mean[R] += DEFAULT_R;
						mean[G] += DEFAULT_G;
						mean[B] += DEFAULT_B;
					} else {
						mean[R] += (int)img_buffer[k][l][R];
						mean[G] += (int)img_buffer[k][l][G];
						mean[B] += (int)img_buffer[k][l][B];
					}
				}
			}

			// Substitui o valor do n-esimo pixel
			img[i][j][R] = (unsigned char)(mean[R] / BLOCK_NPIXELS);
			img[i][j][G] = (unsigned char)(mean[G] / BLOCK_NPIXELS);
			img[i][j][B] = (unsigned char)(mean[B] / BLOCK_NPIXELS);
		}
	}
	
	// Libera memoria do buffer
	if (img_buffer != NULL) {
		for (i = 0; i < bottom; i++) {
			if (img_buffer[i] != NULL) {
				free(img_buffer[i]);
			}
		}
		free(img_buffer);
	}
}

