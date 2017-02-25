#ifndef _SMOOTH_
#define _SMOOTH_

#include "image.h"

// Tamanho de um bloco para aplicar o smooth
#define BLOCK_SIZE 5
#define BLOCK_NPIXELS 25
#define BLOCK_OFFSET 2

/**
 * Funcao que aplica o algoritmo smooth para remocao de ruidos em um imagem
 */
void smooth(pixel_t **img, int width, int height, int flag);

#endif
