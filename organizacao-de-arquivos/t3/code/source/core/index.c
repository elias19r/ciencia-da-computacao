#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../include/lib/mylib.h"
#include "../../include/config/index.h"
#include "../../include/core/index.h"

/**
 * Ordena os indices em memoria (quicksort recursivo) de acordo com o valor do campo
 */
void index_sort(index_t **idx, int left, int right) {
	if (left < right) {
		int i = left, j = left + 1;
		index_t *tmp;
		
		while (j <= right) {
			// Comparacao de valores e troca (swap)
			if (strcmp(idx[j]->value, idx[left]->value) < 0) {
				++i;
				tmp = idx[j];
				idx[j] = idx[i];
				idx[i] = tmp;
			}
			++j;
		}
		// Troca (swap)
		tmp = idx[left];
		idx[left] = idx[i];
		idx[i] = tmp;

		// Chamadas recursivas para esquerda e para direita
		index_sort(idx, left, i - 1);
		index_sort(idx, i + 1, right);
	}
}

/**
 * Atualiza um arquivo de indice de chave primaria
 */
int update_primary_index(const char *file_name, const char *value, int size, int offset) {
	FILE *fp;
	char file_path[255], format_value[20], format_offset[20];
	
	// Define nome do arquivo
	//strcpy(file_path, INDEX_PATH);
	//strcat(file_path, file_name);
	strcpy(file_path, file_name);

	// Formatos para os fprintf
	sprintf(format_value, "%%-%ds ", size);
	sprintf(format_offset, "%%%dd\n", INDEX_OFFSET_SIZE);
	
	fp = fopen(file_path, "a+");
	if (fp != NULL) {
		index_t **idx = NULL;
		int i, count = 0;
		char *value_buffer = (char *) calloc(size + 1, sizeof(char));
		char *offset_buffer = (char *) calloc(INDEX_OFFSET_SIZE + 1, sizeof(char));

		// Insere novo indice no arquivo
		fprintf(fp, format_value, value);
		fprintf(fp, format_offset, offset);
		fclose(fp);
		
		// Vai lendo e copiando o arquivo para a memoria
		fp = fopen(file_path, "r");
		while (!feof(fp)) {
			fscanf(fp, "%s", value_buffer);
			fscanf(fp, "%s", offset_buffer);
			
			if (!feof(fp)) {
				idx = (index_t **) realloc(idx, sizeof(index_t *) * (count + 1));
				idx[count] = (index_t *) calloc(1, sizeof(index_t));
				idx[count]->value = (char *) calloc(size + 1, sizeof(char));
				idx[count]->offset = (char *) calloc(INDEX_OFFSET_SIZE + 1, sizeof(char));
				strcpy(idx[count]->value, value_buffer);
				strcpy(idx[count]->offset, offset_buffer);
				++count;
			}
		}
		free(value_buffer);
		free(offset_buffer);
		fclose(fp);

		// Ordena todos os indices
		index_sort(idx, 0, count-1);
		
		// Reescreve o arquivo
		fp = fopen(file_path, "w+");
		for (i = 0; i < count; ++i) {
			fprintf(fp, format_value, idx[i]->value);
			fprintf(fp, format_offset, atoi(idx[i]->offset));
		}
		fclose(fp);
		
		// Libera a memoria
		while (count != 0) {
			--count;
			free(idx[count]->value);
			free(idx[count]->offset);
			free(idx[count]);
		}
		free(idx);
		
		return 1;
	}
	
	return 0;
}

/**
 * Confere se existe um registro em um arquivo de indice
 */
int index_exists(const char *file_name, const char *key, int size) {
	FILE *fp;
	char file_path[255];
	int record_size = size + INDEX_OFFSET_SIZE + 2;

	// Define nome do arquivo de indice
	//strcpy(file_path, INDEX_PATH);
	//strcat(file_path, file_name);
	strcpy(file_path, file_name);

	// Tenta abrir o arquivo
	fp = fopen(file_path, "r");
	if (fp != NULL) {
		int central, start, end;
		char *buffer = (char *) calloc(size + 1, sizeof(char));

		fseek(fp, 0, SEEK_END);
		start = 0;
		end = ftell(fp);
		end /= record_size;
		
		// Faz busca binaria pelo indice
		while (start <= end) {
			central = (start + end) / 2;

			fseek(fp, central * record_size, SEEK_SET);
			fscanf(fp, "%s", buffer);

			// Se menor			
			if (strcmp(key, buffer) < 0) {
				end = central - 1;

			// Se maior
			} else if (strcmp(key, buffer) > 0) {
				start = central + 1;

			// Se igual
			} else {
				free(buffer);
				fclose(fp);
				return 1;
			}
		}
		
		free(buffer);
		fclose(fp);
	}
	
	return 0;
}

/**
 * Procura e retorna um registro de um arquivo de indice
 */
index_t *index_get(const char *file_name, const char *value, int size, int *offset) {
	FILE *fp;
	char file_path[255];
	int record_size = size + INDEX_OFFSET_SIZE + 2;

	// Define nome do arquivo
	//strcpy(file_path, INDEX_PATH);
	//strcat(file_path, file_name);
	strcpy(file_path, file_name);

	// Tenta abrir o arquivo
	fp = fopen(file_path, "r");
	if (fp != NULL) {
		int central, start, end;
		index_t *buffer = (index_t *) calloc(1, sizeof(index_t));
		
		buffer->value = (char *) calloc(size + 1, sizeof(char));
		buffer->offset = (char *) calloc(INDEX_OFFSET_SIZE + 1, sizeof(char));

		fseek(fp, 0, SEEK_END);
		start = 0;
		end = ftell(fp);
		end /= record_size;
		
		// Faz busca binaria procurando pelo indice
		*offset = -1;
		while (start <= end) {
			central = (start + end) / 2;
			*offset = central * record_size;

			fseek(fp, central * record_size, SEEK_SET);

			fscanf(fp, "%s", buffer->value);
			fscanf(fp, "%s", buffer->offset);
			
			// Se menor
			if (strcmp(value, buffer->value) < 0) {
				end = central - 1;
				
			// Se maior
			} else if (strcmp(value, buffer->value) > 0) {
				start = central + 1;
			
			// Se igual
			} else {
				fclose(fp);
				return buffer;
			}
		}
		
		fclose(fp);
		free(buffer->value);
		free(buffer->offset);
		free(buffer);
	}
	
	return NULL;
}

/**
 * Atualiza um arquivo de indice de chave secundaria
 * e tambem seu respectivo arquivo de lista invertida
 */
void update_secondary_index(const char *file_name, const char *list_file_name, const char *value, int size, const char *key, int key_size) {
	FILE *fp;
	index_t *rec;
	char file_path[255], format_value[20], format_offset[20];
	int offset_file, offset_new, offset_list = -1;

	// Variaveis para fazer a ordenacao
	index_t **idx = NULL;
	int i, count = 0;
	char *value_buffer = (char *) calloc(size + 1, sizeof(char));
	char *offset_buffer = (char *) calloc(INDEX_OFFSET_SIZE + 1, sizeof(char));

	// Confere se ja existe um indice secundario com o mesmo valor
	// Se sim, pega o offset que ele aponta
	rec = index_get(file_name, value, size, &offset_file);
	if (rec != NULL) {
		offset_list = atoi(rec->offset);
	}

	// Define o nome do arquivo da lista invertida
	//strcpy(file_path, INDEX_PATH);
	//strcat(file_path, list_file_name);
	strcpy(file_path, list_file_name);
	
	// Formatos para os fprintf
	sprintf(format_value, "%%-%ds ", key_size);
	sprintf(format_offset, "%%%dd\n", INDEX_OFFSET_SIZE);
	
	// Escreve na lista invertida
	fp = fopen(file_path, "a+");
	if (fp != NULL) {
		fseek(fp, 0, SEEK_END);
		offset_new = ftell(fp);

		fprintf(fp, format_value, key);
		fprintf(fp, format_offset, offset_list);
	}
	fclose(fp);
	
	// Define nome do arquivo de indice secundario
	//strcpy(file_path, INDEX_PATH);
	//strcat(file_path, file_name);
	strcpy(file_path, file_name);

	// Formato para o fprintf (agora do indice secundario)
	sprintf(format_value, "%%-%ds ", size);
	
	// Atualiza o valor do offset no arquivo de indice secundario
	if (rec != NULL) {
		fp = fopen(file_path, "r+");
		if (fp != NULL) {
			fseek(fp, offset_file, SEEK_SET);
			fprintf(fp, format_value, value);
			fprintf(fp, format_offset, offset_new);
			fclose(fp);
		}
		free(rec->value);
		free(rec->offset);
		free(rec);
	// Ou, caso ainda nao exista, cria novo registro no indice secundario
	} else {
		fp = fopen(file_path, "a+");
		if (fp != NULL) {
			fprintf(fp, format_value, value);
			fprintf(fp, format_offset, offset_new);
			fclose(fp);
		}
	}
	
	// Vai lendo e copiando o arquivo de indice secundario para a memoria
	fp = fopen(file_path, "r");
	while (!feof(fp)) {
		fscanf(fp, "%s", value_buffer);
		fscanf(fp, "%s", offset_buffer);
		
		if (!feof(fp)) {
			idx = (index_t **) realloc(idx, sizeof(index_t *) * (count + 1));
			idx[count] = (index_t *) calloc(1, sizeof(index_t));
			idx[count]->value = (char *) calloc(size + 1, sizeof(char));
			idx[count]->offset = (char *) calloc(INDEX_OFFSET_SIZE + 1, sizeof(char));
			strcpy(idx[count]->value, value_buffer);
			strcpy(idx[count]->offset, offset_buffer);
			++count;
		}
	}
	free(value_buffer);
	free(offset_buffer);
	fclose(fp);

	// Ordena todos os indices
	index_sort(idx, 0, count-1);
	
	// Reescreve o arquivo
	fp = fopen(file_path, "w+");
	for (i = 0; i < count; ++i) {
		fprintf(fp, format_value, idx[i]->value);
		fprintf(fp, format_offset, atoi(idx[i]->offset));
	}
	fclose(fp);
	
	// Libera memoria
	while (count != 0) {
		--count;
		free(idx[count]->value);
		free(idx[count]->offset);
		free(idx[count]);
	}
	free(idx);
}

