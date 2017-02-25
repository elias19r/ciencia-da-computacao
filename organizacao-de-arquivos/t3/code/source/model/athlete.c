#include <stdlib.h>
#include <string.h>
#include "../../include/lib/mylib.h"
#include "../../include/config/athlete.h"
#include "../../include/core/database.h"
#include "../../include/core/index.h"
#include "../../include/model/athlete.h"

/**
 * Faz leitura dos dados de um atleta para dentro de uma estrutura de dados
 * Retorna 0 para sucesso, ou um valor diferente para erro em cada campo
 */
int athlete_read(athlete_t *a) {
	char *buffer;

	buffer = scanln();
	if (strlen(buffer) > ATHLETE_CPF_SIZE) {
		free(buffer);
		return 1;
	}
	strcpy(a->cpf, buffer);
	free(buffer);

	buffer = scanln();
	if (strlen(buffer) > ATHLETE_NAME_SIZE) {
		free(buffer);
		return 2;
	}
	strcpy(a->name, buffer);
	free(buffer);
	
	buffer = scanln();
	if (strlen(buffer) > ATHLETE_RA_SIZE) {
		free(buffer);
		return 3;
	}
	strcpy(a->ra, buffer);
	free(buffer);
	
	buffer = scanln();
	if (strlen(buffer) > ATHLETE_UNI_SIZE) {
		free(buffer);
		return 4;
	}
	strcpy(a->uni, buffer);
	free(buffer);
	
	buffer = scanln();
	if (strlen(buffer) > ATHLETE_MOD_SIZE) {
		free(buffer);
		return 5;
	}
	strcpy(a->mod, buffer);
	free(buffer);
	
	return 0;
}

/**
 * Valida se um chave primaria eh unica
 * Para isso, faz a busca no arquivo de indices e confere se retornou um offset invalido
 */
int athlete_valid_key(const char *key) {
	return !index_exists(ATHLETE_IDX, key, ATHLETE_CPF_SIZE);
}

/**
 * Faz a insercao do atleta no arquivo de dados
 * Recebe a estrutura de dados e chama a funcao de insercao do banco de dados
 */
int athlete_insert(athlete_t *a) {
	char **values = (char **) malloc(5 * sizeof(char *));
	int ret, *sizes = (int *) malloc(5 * sizeof(int));

	// Valores dos campos
	values[0] = a->cpf;
	values[1] = a->name;
	values[2] = a->ra;
	values[3] = a->uni;
	values[4] = a->mod;

	// Tamanho de cada campo
	sizes[0] = ATHLETE_CPF_SIZE;
	sizes[1] = ATHLETE_NAME_SIZE;
	sizes[2] = ATHLETE_RA_SIZE;
	sizes[3] = ATHLETE_UNI_SIZE;
	sizes[4] = ATHLETE_MOD_SIZE;
	
	// Insere
	ret = database_insert_record(ATHLETE_DAT, values, sizes, 5);
	
	// Libera memoria
	free(values);
	free(sizes);
	
	return ret;
}

/**
 * Atualiza todos os arquivos de indice
 */
void athlete_indexes(athlete_t *a, int offset) {
	// Arquivo de indice por chave primaria	
	update_primary_index(ATHLETE_IDX, a->cpf, ATHLETE_CPF_SIZE, offset);
	
	// Arquivos de indice por chave secundaria
	update_secondary_index(ATHLETE_UNI_IDX, ATHLETE_UNI_LIST_IDX, a->uni, ATHLETE_UNI_SIZE, a->cpf, ATHLETE_CPF_SIZE);
	update_secondary_index(ATHLETE_MOD_IDX, ATHLETE_MOD_LIST_IDX, a->mod, ATHLETE_MOD_SIZE, a->cpf, ATHLETE_CPF_SIZE);
}

/**
 * Remove um atleta do arquivo de dados
 */
int athlete_delete(const char *key) {
	int offset, ret = 0;
	index_t *rec = index_get(ATHLETE_IDX, key, ATHLETE_CPF_SIZE, &offset);

	// Se o atleta existe, chama a funcao de remocao do banco de dados no offset indicado	
	if (rec != NULL) {
		ret = database_delete_record(ATHLETE_DAT, atoi(rec->offset));
		free(rec->value);
		free(rec->offset);
		free(rec);
	}
	
	return ret;
}

/**
 * Pesquisa por atletas no arquivo de dados
 * Retorna um vetor de atletas e retorna o tamanho em count (por parametro)
 */
athlete_t **athlete_search(const char *field, const char *value, int *count) {
	athlete_t **ret = NULL;
	*count = 0;

	// Campo cpf (indexada, primario)
	if (strcmp(field, "cpf") == 0) {
		int offset;
		index_t *rec = index_get(ATHLETE_IDX, value, ATHLETE_CPF_SIZE, &offset);
		
		// Se encontrou o atleta
		if (rec != NULL) {
			
			// Abre o arquivo de dados e le no offset indicado
			FILE *fp = fopen(ATHLETE_DAT, "r");
			if (fp != NULL) {
				char buffer[ATHLETE_CPF_SIZE+1];
				
				fseek(fp, atoi(rec->offset), SEEK_SET);
				fread(buffer, ATHLETE_CPF_SIZE * sizeof(char), 1, fp);
				buffer[ATHLETE_CPF_SIZE] = '\0';
				rmspaces(buffer);

				free(rec->value);
				free(rec->offset);
				free(rec);
				
				// Confere se o registro esta marcado como "removido"
				if (buffer[0] == '*') {
					fclose(fp);
					return ret;					
				}

				// Aloca um novo atleta
				ret = (athlete_t **) realloc(ret, sizeof(athlete_t *) * (*count + 1));
				ret[*count] = (athlete_t *) calloc(1, sizeof(athlete_t));
	
				// Lendo os dados...
				strcpy(ret[*count]->cpf, buffer);
				fseek(fp, sizeof(char), SEEK_CUR);
	
				fread(ret[*count]->name, ATHLETE_NAME_SIZE * sizeof(char), 1, fp);
				rmspaces(ret[*count]->name);
				fseek(fp, sizeof(char), SEEK_CUR);

				fread(ret[*count]->ra, ATHLETE_RA_SIZE * sizeof(char), 1, fp);
				rmspaces(ret[*count]->ra);
				fseek(fp, sizeof(char), SEEK_CUR);
	
				fread(ret[*count]->uni, ATHLETE_UNI_SIZE * sizeof(char), 1, fp);
				rmspaces(ret[*count]->uni);
				fseek(fp, sizeof(char), SEEK_CUR);
	
				fread(ret[*count]->mod, ATHLETE_MOD_SIZE * sizeof(char), 1, fp);
				rmspaces(ret[*count]->mod);

				fclose(fp);
				(*count)++;
				return ret;
			}
		}

	// Campo uni/mod (indexada, secundario)
	} else {
		int offset;
		index_t *rec;
		char file_list_name[255];
		
		// Se "univerdade"
		if (strcmp(field, "universidade") == 0) {
			rec = index_get(ATHLETE_UNI_IDX, value, ATHLETE_UNI_SIZE, &offset);
			strcpy(file_list_name, ATHLETE_UNI_LIST_IDX);
		
		// Se "modalidade"
		} else if (strcmp(field, "modalidade") == 0) {
			rec = index_get(ATHLETE_MOD_IDX, value, ATHLETE_MOD_SIZE, &offset);
			strcpy(file_list_name, ATHLETE_MOD_LIST_IDX);
		
		// Se nenhum dos dois
		} else {
			return ret;
		}
		
		// Confere se encontrou no indice secundario
		if (rec != NULL) {
			FILE *fp = fopen(file_list_name, "r");
			char cpf[ATHLETE_CPF_SIZE+1];
			int next = atoi(rec->offset); // pega o offset do primeiro item na lista invertida
			
			free(rec->value);
			free(rec->offset);
			free(rec);

			if (fp != NULL) {
				// Abre o arquivo de dados e vai lendo os atletas de acordo
				// 	com os valores das chaves recuperadas da lista invertida
				FILE *fpDAT = fopen(ATHLETE_DAT, "r");
				if (fpDAT != NULL) {
					char buffer[ATHLETE_CPF_SIZE+1];

					do {
						fseek(fp, next, SEEK_SET);
						fscanf(fp, "%s", cpf);
						fscanf(fp, "%d", &next); // pega offset do proximo item na lista invertida
						
						// Pesquisa o offset do atleta no arquivo de dados
						rec = index_get(ATHLETE_IDX, cpf, ATHLETE_CPF_SIZE, &offset);
						if (rec != NULL) {
							fseek(fpDAT, atoi(rec->offset), SEEK_SET);
							fread(buffer, ATHLETE_CPF_SIZE * sizeof(char), 1, fpDAT);
							buffer[ATHLETE_CPF_SIZE] = '\0';
							rmspaces(buffer);
							
							free(rec->value);
							free(rec->offset);
							free(rec);

							// Confere se o registro esta marcado como removido
							if (buffer[0] == '*') {
								if (next != -1)
									continue;
								else
									break;
							}

							// Aloca um novo atleta
							ret = (athlete_t **) realloc(ret, sizeof(athlete_t *) * (*count + 1));
							ret[*count] = (athlete_t *) calloc(1, sizeof(athlete_t));

							// Vai lendo os dados...
							strcpy(ret[*count]->cpf, buffer);
							fseek(fpDAT, sizeof(char), SEEK_CUR);
				
							fread(ret[*count]->name, ATHLETE_NAME_SIZE * sizeof(char), 1, fpDAT);
							rmspaces(ret[*count]->name);
							fseek(fpDAT, sizeof(char), SEEK_CUR);

							fread(ret[*count]->ra, ATHLETE_RA_SIZE * sizeof(char), 1, fpDAT);
							rmspaces(ret[*count]->ra);
							fseek(fpDAT, sizeof(char), SEEK_CUR);
				
							fread(ret[*count]->uni, ATHLETE_UNI_SIZE * sizeof(char), 1, fpDAT);
							rmspaces(ret[*count]->uni);
							fseek(fpDAT, sizeof(char), SEEK_CUR);
				
							fread(ret[*count]->mod, ATHLETE_MOD_SIZE * sizeof(char), 1, fpDAT);
							rmspaces(ret[*count]->mod);

							(*count)++;
						}
					} while (next != -1); // enquanto o proximo offset for diferente de -1
					fclose(fpDAT);
				}
				fclose(fp);
				return ret;
			}
		}
	}

	return ret;
}


/**
 * Ordena (em ordem crescente de cpf) uma lista de atletas
 */
void athlete_sort(athlete_t **list, int left, int right) {
	if (left < right) {
		int i = left, j = left + 1;
		athlete_t *tmp;
		
		while (j <= right) {
			// Comparacao de valores e troca (swap)
			if (strcmp(list[j]->cpf, list[left]->cpf) < 0) {
				++i;
				tmp = list[j];
				list[j] = list[i];
				list[i] = tmp;
			}
			++j;
		}
		// Troca (swap)
		tmp = list[left];
		list[left] = list[i];
		list[i] = tmp;

		// Chamadas recursivas para esquerda e para direita
		athlete_sort(list, left, i - 1);
		athlete_sort(list, i + 1, right);
	}
}

