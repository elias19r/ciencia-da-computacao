#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../inc/pgm.h"

/**
 * Procura por comentarios e os ignora
 */
void pgm_comment(FILE *fp)
{
	char c;

	fread(&c, 1, 1, fp);
	while (isspace(c)) {
		fread(&c, 1, 1, fp);
	}

	while (c == '#') {
		while (c != '\n' && c != '\r') {
			fread(&c, 1, 1, fp);
		}
		while (isspace(c)) {
			fread(&c, 1, 1, fp);
		}
	}
	fseek(fp, -1, SEEK_CUR);
}

/**
 * Le uma imagem PGM e retorna
 */
pgm_t *pgm_read(const char *path)
{
	FILE *fp = NULL;
	pgm_t *img = NULL;
	int i, k, nvalues, value;
	char tmp[3] = {'\0', '\0', '\0'};

	// Olha os primeiros 2 bytes do arquivo para conferir se e' P2 ou P5
	fp = fopen(path, "rb");
	if (fp == NULL) {
		fprintf(stderr, "ERROR: could not read PGM file\n");
		return NULL;
	}
	fread(tmp, sizeof(byte), 2, fp);
	if (ferror(fp)) {
		perror("ERROR: invalid PGM MagicValue");
		fclose(fp);
		return NULL;
	}
	if (feof(fp)) {
		fprintf(stderr, "ERROR: invalid end-of-file reached for PGM file (header)\n");
		fclose(fp);
		return NULL;
	}
	fclose(fp);
	
	// Se P5 abre no modo "rb" (acho que so' faz diferenca no Windows, anyway...)
	if (strcmp(tmp, "P2") == 0) {
		fp = fopen(path, "r");
	} else if (strcmp(tmp, "P5") == 0) {
		fp = fopen(path, "rb");
	} else {
		fprintf(stderr, "ERROR: invalid PGM MagicValue\n");
		return NULL;
	}

	if (fp == NULL) {
		fprintf(stderr, "ERROR: could not read PGM file\n");
		return NULL;
	} else {
		// Aloca estrutura de dados para imagem PGM
		img = (pgm_t *)malloc(sizeof(pgm_t));
		
		// Le as infos do cabecalho
		
		// MagicValue
		fscanf(fp, "%s", img->magic_value);
		if (ferror(fp)) {
			perror("ERROR: invalid PGM MagicValue");
			free(img);
			fclose(fp);
			return NULL;
		}
		if (feof(fp)) {
			fprintf(stderr, "ERROR: invalid end-of-file reached for PGM file (header)\n");
			free(img);
			fclose(fp);
			return NULL;
		}
		pgm_comment(fp);
		
		// ImageWidth
		fscanf(fp, "%d", &img->image_width);
		if (ferror(fp)) {
			perror("ERROR: invalid PGM ImageWidth");
			free(img);
			fclose(fp);		
			return NULL;
		}
		if (feof(fp)) {
			fprintf(stderr, "ERROR: invalid end-of-file reached for PGM file (header)\n");
			free(img);
			fclose(fp);
			return NULL;
		}
		if (img->image_width < 1) {
			perror("ERROR: invalid PGM ImageWidth");
			free(img);
			fclose(fp);		
			return NULL;
		}
		pgm_comment(fp);

		// ImageHeight
		fscanf(fp, "%d", &img->image_height);
		if (ferror(fp)) {
			perror("ERROR: invalid PGM ImageHeight");
			free(img);
			fclose(fp);
			return NULL;
		}
		if (feof(fp)) {
			fprintf(stderr, "ERROR: invalid end-of-file reached for PGM file (header)\n");
			free(img);
			fclose(fp);
			return NULL;
		}
		if (img->image_height < 1) {
			perror("ERROR: invalid PGM ImageHeight");
			free(img);
			fclose(fp);		
			return NULL;
		}
		pgm_comment(fp);
		
		// MaxGrey
		fscanf(fp, "%d", &img->max_grey);
		if (ferror(fp)) {
			perror("ERROR: invalid PGM MaxGrey");
			free(img);
			fclose(fp);
			return NULL;
		}
		if (feof(fp)) {
			fprintf(stderr, "ERROR: invalid end-of-file reached for PGM file (header)\n");
			free(img);
			fclose(fp);
			return NULL;
		}
		if (img->max_grey < 1 || img->max_grey > 255) {
			perror("ERROR: invalid PGM MaxGrey");
			free(img);
			fclose(fp);		
			return NULL;
		}
		pgm_comment(fp);
		
		// Alocando matriz de bytes para os dados da imagem PGM
		img->data = (byte **)malloc(sizeof(byte *) * img->image_height);
		for (i = 0; i < img->image_height; i++) {
			img->data[i] = (byte *)malloc(sizeof(byte) * img->image_width);
		}

		// Leitura dos dados
		
		// Se P2
		nvalues = img->image_width * img->image_height;
		if (strcmp(img->magic_value, "P2") == 0) {
			for (k = 0; k < nvalues; k++) {
				fscanf(fp, "%d", &value);
				if (feof(fp)) {
					fprintf(stderr, "ERROR: invalid end-of-file reached for PGM file (data)\n");
					for (i = 0; i < img->image_height; i++) {
						free(img->data[i]);
					}
					free(img->data);
					free(img);
					fclose(fp);
					return NULL;
				}
				if (value < 0) {
					fprintf(stderr, "ERROR: invalid negative value found in PGM P2 data\n");
					img->data[k / img->image_width][k % img->image_width] = 0;
				} else if (value > 255) {
					fprintf(stderr, "ERROR: invalid overflow value found in PGM P2 data\n");
					img->data[k / img->image_width][k % img->image_width] = 255;
				} else {
					img->data[k / img->image_width][k % img->image_width] = (byte)value;
				}
			}

		// Se P5
		} else if (strcmp(img->magic_value, "P5") == 0) {
			for (k = 0; k < nvalues; k++) {
				fread(&img->data[k / img->image_width][k % img->image_width], sizeof(byte), 1, fp);
				if (feof(fp)) {
					fprintf(stderr, "ERROR: invalid end-of-file reached for PGM file (data)\n");
					for (i = 0; i < img->image_height; i++) {
						free(img->data[i]);
					}
					free(img->data);
					free(img);
					fclose(fp);
					return NULL;
				}
			}

		} else {
			fprintf(stderr, "ERROR: invalid PGM format\n");
			for (i = 0; i < img->image_height; i++) {
				free(img->data[i]);
			}
			free(img->data);
			free(img);
			fclose(fp);
			return NULL;			
		}

		fclose(fp);

		return img;
	}
}

/**
 * Escreve uma imagem PGM
 */
int pgm_write(const char *path, pgm_t *img)
{
	FILE *fp;
	int i, j;
	
	if (img == NULL) {
		fprintf(stderr, "ERROR: invalid pgm_t pointer\n");
		return 0;
	}
	
	// Se P2
	if (strcmp(img->magic_value, "P2") == 0) {
		fp = fopen(path, "w");
		if (fp == NULL) {
			fprintf(stderr, "ERROR: could not write PGM P2 file\n");
			return 0;
		}
		// Escreve cabecalho
		fprintf(fp, "%s\n%d\n%d\n%d\n", img->magic_value, img->image_width, img->image_height, img->max_grey);

		// Escreve dados
		for (i = 0; i < img->image_height; i++) {
			for (j = 0; j < img->image_width; j++) {
				fprintf(fp, "%d ", img->data[i][j]);
			}
		}

	// Se P5
	} else if (strcmp(img->magic_value, "P5") == 0) {
		fp = fopen(path, "wb");
		if (fp == NULL) {
			fprintf(stderr, "ERROR: could not write PGM P5 file\n");
			return 0;
		}
		// Escreve cabecalho
		fprintf(fp, "%s\n%d\n%d\n%d\n", img->magic_value, img->image_width, img->image_height, img->max_grey);
		
		// Escreve dados
		for (i = 0; i < img->image_height; i++) {
			fwrite(img->data[i], sizeof(byte), img->image_width, fp);
		}	

	} else {
		fprintf(stderr, "ERROR: invalid PGM MagicValue\n");
		return 0;
	}
	
	fclose(fp);
	return 1;
}

/**
 * Confere se um pixel e' valido, ou seja, esta' contido na imagem
 */
int valid_pixel(pgm_t *img, int *p)
{
	if (p[0] < 0 || p[0] >= img->image_width || p[1] < 0 || p[1] >= img->image_height) {
		return 0;
	}
	return 1;
}
