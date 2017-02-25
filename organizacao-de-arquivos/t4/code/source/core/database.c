#include <stdio.h>
#include <string.h>
#include "../../include/config/database.h"
#include "../../include/core/database.h"

/**
 * Insere um registro em um arquivo de dados e retorna o offset
 */
int database_insert_record(const char *file_name, char **values, int *sizes, int count) {
	FILE *fp;
	char file_path[255], format[20];
	int i, offset = -1;
	
	// Define nome do arquivo
	//strcpy(file_path, DATABASE_PATH);
	//strcat(file_path, file_name);
	strcpy(file_path, file_name);

	// Tenta abrir o arquivo	
	fp = fopen(file_path, "a+");
	if (fp != NULL) {
		fseek(fp, 0, SEEK_END);
		offset = ftell(fp);

		// Faz todas as insercoes requisitadas
		for (i = 0; i < count; ++i) {
			sprintf(format, "%%-%ds|", sizes[i]); // formata a impressao para cada campo
			fprintf(fp, format, values[i]); // imprime no arquivo
		}
		fclose(fp);
	}
	
	return offset;
}

/**
 * Remove um registro em um arquivo de dados
 */
int database_delete_record(const char *file_name, int offset) {
	FILE *fp;
	char file_path[255];
	
	// Define nome do arquivo	
	//strcpy(file_path, DATABASE_PATH);
	//strcat(file_path, file_name);
	strcpy(file_path, file_name);
	
	// Abre o arquivo e faz a remocao no offset indicado
	fp = fopen(file_path, "r+");
	if (fp != NULL) {
		fseek(fp, offset, SEEK_SET);
		fprintf(fp, "***********");
		fclose(fp);
		return 1;
	}
	
	return 0;
}

