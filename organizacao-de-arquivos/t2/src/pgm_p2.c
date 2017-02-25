#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <myfunc.h>

#include <pgm_p2.h>

/**
 * Inicializa uma estrutura
 */
void pgm_p2_new(pgm_p2_data *img) {
	img->maxcolor = 0;
	img->width = 0;
	img->height = 0;
	img->data = NULL;
	img->data_rl = NULL;
	img->header = NULL;
	img->size = 0;
	img->size_rl = 0;
	img->size_header = 0;
}

/**
 * Lê somente as informações do cabeçalho do arquivo .pgm ou .rl
 */
void pgm_p2_read_header(pgm_p2_data *img, FILE *fp) {
	int i, bufferlen;
	char *buffer;

	// Leitura do cabeçalho
	i = 0;
	while (i < 3) {
		// Lê uma linha do cabeçalho do arquivo
		buffer = fscanln(fp);
		bufferlen = strlen(buffer);

		// Salva a linha na estrutura de dados
		img->header = (char *) realloc(img->header, sizeof(char) * (img->size_header + bufferlen + 1));
		memcpy(img->header + img->size_header, buffer, bufferlen);
		img->size_header += bufferlen;
		img->header[img->size_header++] = '\n';
		
		// Confere se não é comentário e salva separadamente os demais valores
		if (buffer[0] != '#') {

			// Altura e largura
			if (i == 1) {
				char *value = nextw(buffer);
				img->width = atoi(value);
				free(value);
				
				value = nextw(NULL);
				img->height = atoi(value);
				free(value);

			// Tons de cinza
			} else if (i == 2) {
				char *value = nextw(buffer);
				img->maxcolor = atoi(value);
				free(value);		
			}

			i++;
		}
	
		free(buffer);
	}
	img->header = (char *) realloc(img->header, sizeof(char) * (img->size_header + 1));
	img->header[img->size_header] = '\0';
}

/**
 * Lê os dados a partir de um arquivo .pgm e armazena na estrutura de dados
 */
void pgm_p2_read(pgm_p2_data *img, FILE *fp) {
	char c;
	
	// Lê cabeçalho
	pgm_p2_read_header(img, fp);
	
	// Leitura do restante do arquivo
	while (!feof(fp)) {
		fread(&c, sizeof(char), 1, fp);
		if (!feof(fp)) {
			img->data = (char *) realloc(img->data, sizeof(char) * (img->size + 1));
			img->data[img->size++] = c;
		}
	}
	img->data = (char *) realloc(img->data, sizeof(char) * (img->size + 1));
	img->data[img->size] = '\0';
}

/**
 * Lê os dados a partir de um arquivo comprimido .rl e armazena na estrutura de dados
 */
void pgm_p2_read_rl(pgm_p2_data *img, FILE *fp) {
	char c;
	
	// Lê cabeçalho
	pgm_p2_read_header(img, fp);

	// Leitura do restante do arquivo
	while (!feof(fp)) {
		fread(&c, sizeof(char), 1, fp);
		if (!feof(fp)) {
			img->data_rl = (char *) realloc(img->data_rl, sizeof(char) * (img->size_rl + 1));
			img->data_rl[img->size_rl++] = c;
		}
	}
	img->data_rl = (char *) realloc(img->data_rl, sizeof(char) * (img->size_rl + 1));
	img->data_rl[img->size_rl] = '\0';
}


/**
 * Compacta os dados com um algoritmo de run-length
 */
void pgm_p2_compress(pgm_p2_data *img) {
	char *cur, *next;
	int count, i, j, bufferlen;
	char buffer[21];
	
	// Adiciona "P8" no header
	img->header[1] = '8';

	// Lê uma palavra dos dados não-comprimidos
	cur = nextw(img->data);

	// Laço que confere o final dos dados
	while (*cur != '\0') {
		
		// Laço que confere o final de uma linha dos dados
		i = 0;
		while (i < img->width) {
			count = 1;
			
			// Lê a palavra seguinte a cur
			next = nextw(NULL);
			i++;
			
			// Enquanto houver palavras iguais e ainda não alcançar o final da linha
			// 	vai lendo a próxima palavra e incrementando a variável count
			// 	e o contador i
			while (i < img->width && strcmp(cur, next) == 0) {
				count++;
				free(next);
				next = nextw(NULL);
				i++;
			}
			
			// Se houve mais de 3 repetições: omite a sequência repetida
			// 	e escreve na string de dados comprimidos
			if (count > 3) {
				sprintf(buffer, "ff %s %d ", cur, count);
				bufferlen = strlen(buffer);

				img->data_rl = (char *) realloc(img->data_rl, sizeof(char) * (img->size_rl + bufferlen));

				memcpy(img->data_rl + img->size_rl, buffer, bufferlen);
				img->size_rl += bufferlen;
			
			// Caso contrário, menor ou igual a 3 repetições: não omite a senquência repetida
			// 	e escreve a palavra count vezes na string de dados comprimidos
			} else {
				int at = 0;
				for (j = 0; j < count; j++) {
					at += sprintf(buffer + at, "%s ", cur);
				}
				bufferlen = strlen(buffer);
				img->data_rl = (char *) realloc(img->data_rl, sizeof(char) * (img->size_rl + bufferlen + 1));

				memcpy(img->data_rl + img->size_rl, buffer, bufferlen);
				img->size_rl += bufferlen;
			}

			// Faz cur apontar para next
			free(cur);
			cur = next;
		}
		img->data_rl[img->size_rl-1] = '\n';
	}
	free(next);
	img->data_rl = (char *) realloc(img->data_rl, sizeof(char) * (img->size_rl + 1));
	img->data_rl[img->size_rl] = '\0';
}

/**
 * Descompacta os dados previamente compactados com um algoritmo de run-length
 */
void pgm_p2_decompress(pgm_p2_data *img) {
	char *cur, *times;
	int i, j, count, curlen;

	// Adiciona "P2" no header
	img->header[1] = '2';

	// Lê uma palavra dos dados comprimidos
	cur = nextw(img->data_rl);
	
	// Laço que confere o final dos dados
	while (*cur != '\0') {
		
		// Laço que confere o final de uma linha dos dados
		i = 0;
		while (i < img->width) {
			count = 1;
			
			// Caso encontre uma palavra de repetição 'ff',
			// 	lê novamente a palavra a ser repetida em cur
			// 	e salva a quantidade de vezes em count
			if (strcmp(cur, "ff") == 0) {
				free(cur);
				cur = nextw(NULL);
				times = nextw(NULL);
				count = atoi(times);
				free(times);
			}
			
			// Escreve a palavra count vezes na string de dados do arquivo não-comprimido
			curlen = strlen(cur);
			for (j = 0; j < count; j++) {
				img->data = (char *) realloc(img->data, sizeof(char) * (img->size + curlen + 1));
				memcpy(img->data + img->size, cur, curlen);
				img->size += curlen + 1;
				img->data[img->size-1] = ' ';
			}
			
			// Lê próxima palavra e incrementa contador com count
			free(cur);
			cur = nextw(NULL);
			i += count;
		}
		img->data[img->size-1] = '\n';
	}
	free(cur);
	img->data = (char *) realloc(img->data, sizeof(char) * (img->size + 1));
	img->data[img->size] = '\0';
}

/**
 * Libera toda a memoria alocada na estrutura de dados e a reinicializa
 */
void pgm_p2_delete(pgm_p2_data *img) {
	if (img->header != NULL)
		free(img->header);

	if (img->data != NULL)
		free(img->data);
		
	if (img->data_rl != NULL)
		free(img->data_rl);

	pgm_p2_new(img);
}

/**
 * Libera somente a memória referente ao arquivo .pgm
 */
void pgm_p2_free(pgm_p2_data *img) {
	if (img->data != NULL) {
		free(img->data);
		img->data = NULL;
	}
	img->size = 0;
}

/**
 * Libera somente a memória referente ao arquivo comprimido .rl
 */
void pgm_p2_free_rl(pgm_p2_data *img) {
	if (img->data_rl != NULL) {
		free(img->data_rl);
		img->data_rl = NULL;
	}
	img->size_rl = 0;
}

/**
 * Imprime os dados da imagem na tela
 */
void pgm_p2_print(pgm_p2_data *img) {
	printf("%s", img->header);
	printf("%s", img->data);
}

/**
 * Imprime os dados da imagem comprimida na tela
 */
void pgm_p2_print_rl(pgm_p2_data *img) {
	printf("%s", img->header);
	printf("%s", img->data_rl);
}

