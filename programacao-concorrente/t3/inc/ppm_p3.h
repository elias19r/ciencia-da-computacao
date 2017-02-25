#ifndef _PPM_P3_
#define _PPM_P3_

#include "image.h"

// Estrutura para guardar o cabecalho da imagem
typedef struct {
	char format[3];
	int width;
	int height;
	int max_value;
} ppm_header_t;

/**
 * Le uma imagem PPM P3 da entrada padrao
 */
pixel_t **ppm_p3_scan(ppm_header_t *ppm);

/**
 * Imprime uma imagem PPM P3 na saida padrao
 */
void ppm_p3_print(pixel_t **img, ppm_header_t *ppm);

#endif
