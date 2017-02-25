#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../include/lib/mylib.h"
#include "../../include/config/database.h"
#include "../../include/config/index.h"
#include "../../include/config/athlete.h"
#include "../../include/model/athlete.h"
#include "../../include/view/athlete.h"

/**
 * Imprime na tela um vetor de atletas
 */
void athlete_print_results(athlete_t **res, int count) {
	int i;
	
	for (i = 0; i < count; ++i)
		athlete_print_data(res[i]);
}

/**
 * Imprime na tela os dados de um atleta
 */
void athlete_print_data(athlete_t *a) {
	printf("%s - %s\n", a->cpf, a->name);
	printf("\tRegistro Academico: %s\n", a->ra);
	printf("\tUniversidade: %s\n", a->uni);
	printf("\tModalidade: %s\n", a->mod);
}

/**
 * Imprime na tela um dos arquivos (de dados ou de indice) do atleta
 */
int athlete_print_file(const char *file_name) {
	// Arquivo de dados (caractere por caractere)
	if (strcmp(file_name, ATHLETE_DAT) == 0) {
		//FILE *fp = fopen(DATABASE_PATH ATHLETE_DAT, "r");
		FILE *fp = fopen(ATHLETE_DAT, "r");
		char c;
		
		if (fp != NULL) {
			while (!feof(fp)) {
				fscanf(fp, "%c", &c);
				if (!feof(fp)) {
					printf("%c", c);
				}
			}
			fclose(fp);
			printf("\n");
			return 1;
		} else
			return 0;


	// Arquivo de indice por chave primaria (cpf)
	} else if (strcmp(file_name, ATHLETE_IDX) == 0) {
		char cpf[ATHLETE_CPF_SIZE+1], offset[INDEX_OFFSET_SIZE+1];

		//FILE *fp = fopen(INDEX_PATH ATHLETE_IDX, "r");
		FILE *fp = fopen(ATHLETE_IDX, "r");
		if (fp != NULL) {
			while (!feof(fp)) {
				fscanf(fp, "%s", cpf);
				fscanf(fp, "%s", offset);
				if (!feof(fp)) {
					printf("%s - %d\n", cpf, atoi(offset));
				}
			}
			fclose(fp);
			return 1;
		} else
			return 0;
		
	
	// Arquivo de indice por chave secundaria (uni)
	} else if (strcmp(file_name, ATHLETE_UNI_IDX) == 0) {
		char uni[ATHLETE_UNI_SIZE+1], offset[INDEX_OFFSET_SIZE+1];
		
		//FILE *fp = fopen(INDEX_PATH ATHLETE_UNI_IDX, "r");
		FILE *fp = fopen(ATHLETE_UNI_IDX, "r");
		if (fp != NULL) {
			while (!feof(fp)) {
				fscanf(fp, "%s", uni);
				fscanf(fp, "%s", offset);
				if (!feof(fp)) {
					printf("%s - %d\n", uni, atoi(offset));
				}
			}
			fclose(fp);
			return 1;
		} else
			return 0;
	
	// Arquivo de indice por chave secundaria (mod)
	} else if (strcmp(file_name, ATHLETE_MOD_IDX) == 0) {
		char mod[ATHLETE_MOD_SIZE+1], offset[INDEX_OFFSET_SIZE+1];
		
		//FILE *fp = fopen(INDEX_PATH ATHLETE_MOD_IDX, "r");
		FILE *fp = fopen(ATHLETE_MOD_IDX, "r");
		if (fp != NULL) {
			while (!feof(fp)) {
				fscanf(fp, "%s", mod);
				fscanf(fp, "%s", offset);
				if (!feof(fp)) {
					printf("%s - %d\n", mod, atoi(offset));
				}
			}
			fclose(fp);
			return 1;
		} else
			return 0;
	
	// Arquivo de lista invertida (uni)
	} else if (strcmp(file_name, ATHLETE_UNI_LIST_IDX) == 0) {
		char cpf[ATHLETE_CPF_SIZE+1], offset[INDEX_OFFSET_SIZE+1];
		
		//FILE *fp = fopen(INDEX_PATH ATHLETE_UNI_LIST_IDX, "r");
		FILE *fp = fopen(ATHLETE_UNI_LIST_IDX, "r");
		if (fp != NULL) {
			while (!feof(fp)) {
				fscanf(fp, "%s", cpf);
				fscanf(fp, "%s", offset);
				if (!feof(fp)) {
					printf("%s - %d\n", cpf, atoi(offset));
				}
			}
			fclose(fp);
			return 1;
		} else
			return 0;

	// Arquivo de lista invertida (mod)
	} else if (strcmp(file_name, ATHLETE_MOD_LIST_IDX) == 0) {
		char cpf[ATHLETE_CPF_SIZE+1], offset[INDEX_OFFSET_SIZE+1];
		
		//FILE *fp = fopen(INDEX_PATH ATHLETE_MOD_LIST_IDX, "r");
		FILE *fp = fopen(ATHLETE_MOD_LIST_IDX, "r");
		if (fp != NULL) {
			while (!feof(fp)) {
				fscanf(fp, "%s", cpf);
				fscanf(fp, "%s", offset);
				if (!feof(fp)) {
					printf("%s - %d\n", cpf, atoi(offset));
				}
			}
			fclose(fp);
			return 1;
		} else
			return 0;
	}
	
	return 0;
}

