#ifndef _PGM_H_
#define _PGM_H_

// Definicao para simplificar a escrita do codigo
typedef unsigned char byte;

// Estrutura de dados para a imagem PGM
typedef struct {
	char magic_value[3]; // P2 ou P5
	int image_width;
	int image_height;
	int max_grey;

	byte **data; // dados da imagem: uma matriz de bytes
} pgm_t;

/**
 * Le uma imagem PGM e retorna
 */
pgm_t *pgm_read(const char *path);

/**
 * Escreve uma imagem PGM
 */
int pgm_write(const char *path, pgm_t *img);

/**
 * Confere se um pixel e' valido, ou seja, esta' contido na imagem
 */
int valid_pixel(pgm_t *img, int *p);

#endif
