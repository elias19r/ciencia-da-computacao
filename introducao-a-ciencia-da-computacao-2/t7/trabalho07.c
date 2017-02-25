/**
 * trabalho07
 * 2012/11/19
 * Elias Italiano Rodrigues, 7987251, ICC2, BCC-B, ICMC-USP
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Tamanho máximo para os nomes das tabelas e campos
#define NAMELENGTH 65

// Valores numéricos para os tipos de campos
#define CHAR   0 // caracter
#define CCHAR  1 // cadeia de caractereres
#define INT    2 // inteiro
#define FLOAT  3 // real
#define DOUBLE 4 // real com mais precisão

// Valores numéricos para os comandos de script
#define CREATETABLE 0
#define CREATEINDEX 1
#define SORT        2
#define INSERT      3
#define SELECT      4
#define STATS       5
#define SHOWALL     6
#define SHOWALLIDX  7

// Estados para os comandos de script
#define OK                        0
#define TABLEDOESNOTEXIST         1
#define FIELDDOESNOTEXIST         2
#define FIELDSANDVALUESDONOTMATCH 3
#define SYNTAXERRORVALUES         4
#define INDEXDOESNOTEXIST         5

// Estrutura para as infos de um índice
typedef struct {
	char tablename[NAMELENGTH];
	char fieldname[NAMELENGTH];
} Index;

// Estrutura para as infos das estatísticas
typedef struct {
	int ntables;
	Index *indexes;
	int nindexes;
	int ninserts;
	int nselects;
	int nsorts;
	int nshowalltables;
	int nshowallindexes;
	int recordsinlastselectbinary;
	int recordsinlastselectsequential;
} Statistics;

// Estrutura para as infos de um comando "create table"
typedef struct {
	char tablename[NAMELENGTH];
	char **fieldnames;
	int *fieldtypes;
	char **fieldtypesdesc;
	int *fieldsizes;
	int nfields;
} CmdCreateTable;

// Estrutura para as infos de um comando "create index"
typedef struct {
	char tablename[NAMELENGTH];
	char fieldname[NAMELENGTH];
} CmdCreateIndex;

// Estrutura para as infos de um comando "sort"
typedef struct {
	char tablename[NAMELENGTH];
	char fieldname[NAMELENGTH];
} CmdSort;

// Estrutura para as infos de um comando "insert"
typedef struct {
	char tablename[NAMELENGTH];
	char **fieldnames;
	char **values;
	int nfields;
} CmdInsert;

// Estrutura para as infos de um comando "select"
typedef struct {
	char tablename[NAMELENGTH];
	char fieldname[NAMELENGTH];
	char *value;
} CmdSelect;

// Estrutura para as infos de um comando qualquer
typedef struct {
	int cmdstatus;
	int cmdtype;
	void *cmd;
} Command;

// Lista de dados dos comandos
typedef struct {
	Command *commands;
	int ncommands;
	int last;
} CommandList;

// Estrutura para as infos de um campo
typedef struct {
	char name[NAMELENGTH];
	int type;
	char typedesc[NAMELENGTH];
	int size;
	int offset;
	int index;
} Field;

// Estrutura para as infos de uma tabela
typedef struct {
	char name[NAMELENGTH];
	char dat[NAMELENGTH+4];
	char tmp[NAMELENGTH+4];
	int size;
	Field *fields;
	int nfields;
	int last;
} Table;

// Lista de dados das tabelas
typedef struct {
	Table *tables;
	int ntables;
	int last;
} TableList;

// Estrutura para um registro do arquivo ".idx"
typedef struct {
	void *data;
	int offset;
} IDX;

/**
 * Função para escanear o arquivo de script
 * @return char * palavra escaneada
 */
char *scanfile(FILE *fp) {
	char c, *str = NULL;
	int quotes = 0, brackets = 0, count = 0;

	while (!feof(fp)) {
		fscanf(fp, "%c", &c);
		if (!feof(fp)) {
			if (c == '(' || c == ')') { // confere abertura/fechamento de parênteses
				if (count > 0 && c == '(') {
					fseek(fp, -sizeof(char), SEEK_CUR);
					break;
				}
				brackets = !(brackets);
				continue;
			} else if (c == '\'' || c == '"') { // confere abertura/fechamento de aspas
				quotes = !(quotes);
			}
			if ((quotes || (c != ' ' && c != '\t' && c != '\n' && c != '\r' && c != '\v' && c != '\f' && c != '(' && c != ')' && c != ';')) || brackets) {
				str = (char *) realloc(str, sizeof(char) * (count + 1));
				str[count] = c;
				count++;
			} else if (count > 0) {
				break;
			}
		}
	}
	str = (char *) realloc(str, sizeof(char) * (count + 1));
	str[count] = '\0';
	return str;
}

/**
 * Função para escanear uma string pegando as palavras
 * @return char * palavra escaneada
 */
char *scanstr(char *str) {
	char *aux = NULL;
	int count = 0, quotes = 0;
	static char *str__;

	// Quando str é igual a NULL, é usada a mesma string da última chamada da função
	//     semelhante a função strtok
	if (str != NULL)
		str__ = str;

	while (*str__ != '\0') {
		if (*str__ == '\'' || *str__ == '"') { // confere abertura/fechamento de aspas
			quotes = !(quotes);
			str__++;
			continue;
		}
		if (quotes || (*str__ != ' ' && *str__ != '\t' && *str__ != '\n' && *str__ != '\r' && *str__ != '\v' && *str__ != '\f' && *str__ != ',' && *str__ != ';')) {
			aux = (char *) realloc(aux, sizeof(char) * (count + 1));
			aux[count] = *str__;
			count++;
		} else if (count > 0) {
			str__++;
			break;
		}
		str__++;
	}
	aux = (char *) realloc(aux, sizeof(char) * (count + 1));
	aux[count] = '\0';
	return aux;
}

/**
 * Algoritmo de ordenação mergesort para ordenar os registros lidos
 */
void mergesort(IDX *records, int start, int end, int type, int size) {
	int central = (start + end) / 2;

	if (start >= end) return;

	mergesort(records, start, central, type, size);
	mergesort(records, central+1, end, type, size);
	{
		IDX *right, *left;
		int i, j, k, rightsize, leftsize;

		rightsize = end - central;
		leftsize  = central - start + 1;

		right = (IDX *) malloc(sizeof(IDX) * rightsize);
		left  = (IDX *) malloc(sizeof(IDX) * leftsize);

		memcpy(right, &records[central+1], sizeof(IDX) * rightsize);
		memcpy(left, &records[start], sizeof(IDX) * leftsize);

		i = j = 0;
		for (k = start; k < end+1; k++) {
			if (j < rightsize && i < leftsize) {

				// tipo INT
				if (type == INT) {
					int leftvalue = *((int *) left[i].data);
					int rightvalue  = *((int *) right[j].data);

					if (leftvalue > rightvalue) {
						records[k] = right[j];
						j++;
					} else {
						records[k] = left[i];
						i++;
					}

				// tipo CHAR
				} else if (type == CHAR) {
					char leftvalue = *((char *) left[i].data);
					char rightvalue  = *((char *) right[j].data);

					if (leftvalue > rightvalue) {
						records[k] = right[j];
						j++;
					} else {
						records[k] = left[i];
						i++;
					}

				// tipo CCHAR
				} else if (type == CCHAR) {
					char *leftvalue = (char *) left[i].data;
					char *rightvalue  = (char *) right[j].data;

					leftvalue = (char *) realloc(leftvalue, size + 1);
					leftvalue[size] = '\0';

					rightvalue = (char *) realloc(rightvalue, size + 1);
					rightvalue[size] = '\0';

					if (strcmp(leftvalue, rightvalue) > 0) {
						records[k] = right[j];
						j++;
					} else {
						records[k] = left[i];
						i++;
					}

				// tipo FLOAT
				} else if (type == FLOAT) {
					float leftvalue = *((float *) left[i].data);
					float rightvalue  = *((float *) right[j].data);

					if (leftvalue > rightvalue) {
						records[k] = right[j];
						j++;
					} else {
						records[k] = left[i];
						i++;
					}

				// tipo DOUBLE
				} else if (type == DOUBLE) {
					double leftvalue = *((double *) left[i].data);
					double rightvalue  = *((double *) right[j].data);

					if (leftvalue > rightvalue) {
						records[k] = right[j];
						j++;
					} else {
						records[k] = left[i];
						i++;
					}
				}

			} else if (j < rightsize) {
				records[k] = right[j];
				j++;
			} else if (i < leftsize) {
				records[k] = left[i];
				i++;
			}
		}
		free(right);
		free(left);
	}
}

/**
 * Função para criar o arquivo ".idx"
 */
void createidx(char *idxname, TableList *tablelist, int i, int j) {
	int offset, offsetrecord;
	unsigned char byte;
	FILE *fpidx, *fpdat, *fptmp;
	void *data;

	// Anexa o conteúdo do arquivo ".tmp" da tabela ao arquivo ".dat" da tabela
	// O arquivo é copiado byte a byte
	fpdat = fopen(tablelist->tables[i].dat, "ab+");
	fptmp = fopen(tablelist->tables[i].tmp, "rb");
	while (!feof(fptmp)) {
		fread(&byte, sizeof(unsigned char), 1, fptmp);
		if (!feof(fptmp)) {
			fwrite(&byte, sizeof(unsigned char), 1, fpdat);
		}
	}
	fclose(fptmp);
	fclose(fpdat);

	// Trunca o arquivo ".tmp" da tabela
	fptmp = fopen(tablelist->tables[i].tmp, "wb");
	fclose(fptmp);
	
	// Abre o arquivo ".idx" e copia para dentro dele o valor do campo
	//     que está sendo indexado e o offset do registro ao qual pertence
	fpidx = fopen(idxname, "wb+");
	fpdat = fopen(tablelist->tables[i].dat, "rb");
	offset = tablelist->tables[i].fields[j].offset;
	fseek(fpdat, offset, SEEK_SET);
	data = malloc(tablelist->tables[i].fields[j].size);
	while (!feof(fpdat)) {
		fread(data, tablelist->tables[i].fields[j].size, 1, fpdat);
		if (!feof(fpdat)) {
			// O offset do registro (offsetrecord) ao qual o campo pertence é
			//     a posição atual (offset) menos o offset do campo em
			//     relação a tabela
			offsetrecord = offset - tablelist->tables[i].fields[j].offset;
			fwrite(data, tablelist->tables[i].fields[j].size, 1, fpidx);
			fwrite(&offsetrecord, sizeof(int), 1, fpidx);
			offset += tablelist->tables[i].size; // avança de registro em registro
			fseek(fpdat, offset, SEEK_SET);
		}
	}
	free(data);
	fclose(fpdat);
	fclose(fpidx);
}

/**
 * Função para ordenar o arquivo ".idx"
 */
void sortidx(char *idxname, int type, int size) {
	FILE *fp;
	int i, count, offset;
	void *data;
	IDX *records = NULL;

	fp = fopen(idxname, "rb+");

	// Leitura dos registros
	count = 0;
	data = malloc(size);
	while (!feof(fp)) {
		fread(data, size, 1, fp);
		fread(&offset, sizeof(int), 1, fp);
		if (!feof(fp)) {
			records = (IDX *) realloc(records, sizeof(IDX) * (count + 1));
			records[count].data = malloc(size);
			memcpy(records[count].data, data, size);
			records[count].offset = offset;
			count++;
		}
	}
	free(data);

	// Ordenação dos registros
	mergesort(records, 0, count-1, type, size);

	// Reescrevendo o arquivo agora com os registros ordenados
	fseek(fp, 0, SEEK_SET);
	for (i = 0; i < count; i++) {
		fwrite(records[i].data, size, 1, fp);
		fwrite(&(records[i].offset), sizeof(int), 1, fp);
		free(records[i].data);
	}
	free(records);
	fclose(fp);
}

/**
 * Busca binária iterativa em um arquivo ".idx"
 * A quantidade de registros encontrados é retornada por referência em matches
 * @return int * vetor de inteiros contendo os offset dos registros encontrados
 */
int *binarysearch(FILE *fp, char *key, int type, int size, int *matches) {
	int start, central, end, recordsize, filesize, found, *result = NULL;
	void *data;

	// Quantidade de chaves que o arquivo possui
	recordsize = size + sizeof(int);
	fseek(fp, 0, SEEK_END);
	filesize = ftell(fp);
	filesize /= recordsize;

	// Define start e end
	start = 0;
	end = filesize-1;
	*matches = 0;

	data = malloc(size);
	while (start <= end) {
		central = (start + end) / 2;
		fseek(fp, central * recordsize, SEEK_SET);
		fread(data, size, 1, fp);

		// tipo INT
		if (type == INT) {
			int keyread = *((int *) data);
			int keywanted = atoi(key);

			if (keywanted == keyread) {
				found = central;
				// Confere se existem chaves anteriores com o mesmo valor procurado
				if (central > 0) {
					found = central-1;
					while (found >= 0) {
						fseek(fp, found * recordsize, SEEK_SET);
						fread(data, size, 1, fp);
						keyread = *((int *) data);
						if (keywanted == keyread) {
							found--;
						} else {
							break;
						}
					}
					found++;
				}
				// Aloca em result os offsets dos registros encontrados em ordem
				while (found <= end) {
					fseek(fp, found * recordsize, SEEK_SET);
					fread(data, size, 1, fp);
					keyread = *((int *) data);
					if (keywanted == keyread) {
						result = (int *) realloc(result, sizeof(int) * (*matches + 1));
						fread(result+(*matches), sizeof(int), 1, fp);
						(*matches)++;
						found++;
					} else {
						break;
					}
				}
				break;
			}
			if (keywanted > keyread)
				start = central+1;
			else
				end = central-1;

		// tipo CHAR
		} else if (type == CHAR) {
			char keyread = *((char *) data);
			char keywanted = *key;

			if (keywanted == keyread) {
				found = central;
				// Confere se existem chaves anteriores com o mesmo valor procurado
				if (central > 0) {
					found = central-1;
					while (found >= 0) {
						fseek(fp, found * recordsize, SEEK_SET);
						fread(data, size, 1, fp);
						keyread = *((char *) data);
						if (keywanted == keyread) {
							found--;
						} else {
							break;
						}
					}
					found++;
				}
				// Aloca em result os offsets dos registros encontrados em ordem
				while (found <= end) {
					fseek(fp, found * recordsize, SEEK_SET);
					fread(data, size, 1, fp);
					keyread = *((char *) data);
					if (keywanted == keyread) {
						result = (int *) realloc(result, sizeof(int) * (*matches + 1));
						fread(result+(*matches), sizeof(int), 1, fp);
						(*matches)++;
						found++;
					} else {
						break;
					}
				}
				break;
			}
			if (keywanted > keyread)
				start = central+1;
			else
				end = central-1;

		// tipo CCHAR
		} else if (type == CCHAR) {
			char *keyread = (char *) data;
			char *keywanted = key;

			keyread = (char *) realloc(keyread, size + 1);
			keyread[size] = '\0';

			if (strcmp(keywanted, keyread) == 0) {
				found = central;
				// Confere se existem chaves anteriores com o mesmo valor procurado
				if (central > 0) {
					found = central-1;
					while (found >= 0) {
						fseek(fp, found * recordsize, SEEK_SET);
						fread(data, size, 1, fp);
						keyread = (char *) data;
						keyread = (char *) realloc(keyread, size + 1);
						keyread[size] = '\0';
						if (strcmp(keywanted, keyread) == 0) {
							found--;
						} else {
							break;
						}
					}
					found++;
				}
				// Aloca em result os offsets dos registros encontrados em ordem
				while (found <= end) {
					fseek(fp, found * recordsize, SEEK_SET);
					fread(data, size, 1, fp);
					keyread = (char *) data;
					keyread = (char *) realloc(keyread, size + 1);
					keyread[size] = '\0';
					if (strcmp(keywanted, keyread) == 0) {
						result = (int *) realloc(result, sizeof(int) * (*matches + 1));
						fread(result+(*matches), sizeof(int), 1, fp);
						(*matches)++;
						found++;
					} else {
						break;
					}
				}
				break;
			}
			if (strcmp(keywanted, keyread) > 0)
				start = central+1;
			else
				end = central-1;

		// tipo FLOAT
		} else if (type == FLOAT) {
			float keyread = *((float *) data);
			float keywanted = atof(key);

			if (keywanted == keyread) {
				found = central;
				// Confere se existem chaves anteriores com o mesmo valor procurado
				if (central > 0) {
					found = central-1;
					while (found >= 0) {
						fseek(fp, found * recordsize, SEEK_SET);
						fread(data, size, 1, fp);
						keyread = *((float *) data);
						if (keywanted == keyread) {
							found--;
						} else {
							break;
						}
					}
					found++;
				}
				// Aloca em result os offsets dos registros encontrados em ordem
				while (found <= end) {
					fseek(fp, found * recordsize, SEEK_SET);
					fread(data, size, 1, fp);
					keyread = *((float *) data);
					if (keywanted == keyread) {
						result = (int *) realloc(result, sizeof(int) * (*matches + 1));
						fread(result+(*matches), sizeof(int), 1, fp);
						(*matches)++;
						found++;
					} else {
						break;
					}
				}
				break;
			}
			if (keywanted > keyread)
				start = central+1;
			else
				end = central-1;

		// tipo DOUBLE
		} else if (type == DOUBLE) {
			double keyread = *((double *) data);
			double keywanted = atof(key);

			if (keywanted == keyread) {
				found = central;
				// Confere se existem chaves anteriores com o mesmo valor procurado
				if (central > 0) {
					found = central-1;
					while (found >= 0) {
						fseek(fp, found * recordsize, SEEK_SET);
						fread(data, size, 1, fp);
						keyread = *((double *) data);
						if (keywanted == keyread) {
							found--;
						} else {
							break;
						}
					}
					found++;
				}
				// Aloca em result os offsets dos registros encontrados em ordem
				while (found <= end) {
					fseek(fp, found * recordsize, SEEK_SET);
					fread(data, size, 1, fp);
					keyread = *((double *) data);
					if (keywanted == keyread) {
						result = (int *) realloc(result, sizeof(int) * (*matches + 1));
						fread(result+(*matches), sizeof(int), 1, fp);
						(*matches)++;
						found++;
					} else {
						break;
					}
				}
				break;
			}
			if (keywanted > keyread)
				start = central+1;
			else
				end = central-1;
		}
	}
	free(data);
	return result;
}

/**
 * Busca sequencial em um arquivo ".idx"
 * A quantidade de registros encontrados é retornada por referência em matches
 * @return int * vetor de inteiros contendo os offset dos registros encontrados
 */
int *sequentialsearch(FILE *fp, char *key, int type, int size, int offset, int recordsize, int *matches) {
	int *result = NULL, i, filesize;
	void *data;

	*matches = 0;
	fseek(fp, 0, SEEK_END);
	filesize = ftell(fp);
	filesize /= recordsize;

	data = malloc(size);
	for (i = 0; i < filesize; i++) {
		fseek(fp, (i * recordsize) + offset, SEEK_SET);
		fread(data, size, 1, fp);

		// tipo INT
		if (type == INT) {
			int keyread = *((int *) data);
			int keywanted = atoi(key);

			// Aloca em result os offsets dos registros encontrados em ordem
			if (keywanted == keyread) {
				result = (int *) realloc(result, sizeof(int) * (*matches + 1));
				result[(*matches)] = i * recordsize;
				(*matches)++;
			}

		// tipo CHAR
		} else if (type == CHAR) {
			char keyread = *((char *) data);
			char keywanted = *key;

			// Aloca em result os offsets dos registros encontrados em ordem
			if (keywanted == keyread) {
				result = (int *) realloc(result, sizeof(int) * (*matches + 1));
				result[(*matches)] = i * recordsize;
				(*matches)++;
			}

		// tipo CCHAR
		} else if (type == CCHAR) {
			char *keyread = (char *) data;
			char *keywanted = key;

			keyread = (char *) realloc(keyread, size + 1);
			keyread[size] = '\0';

			// Aloca em result os offsets dos registros encontrados em ordem
			if (strcmp(keywanted, keyread) == 0) {
				result = (int *) realloc(result, sizeof(int) * (*matches + 1));
				result[(*matches)] = i * recordsize;
				(*matches)++;
			}

		// tipo FLOAT
		} else if (type == FLOAT) {
			float keyread = *((float *) data);
			float keywanted = atof(key);

			// Aloca em result os offsets dos registros encontrados em ordem
			if (keywanted == keyread) {
				result = (int *) realloc(result, sizeof(int) * (*matches + 1));
				result[(*matches)] = i * recordsize;
				(*matches)++;
			}

		// tipo DOUBLE
		} else if (type == DOUBLE) {
			double keyread = *((double *) data);
			double keywanted = atof(key);

			// Aloca em result os offsets dos registros encontrados em ordem
			if (keywanted == keyread) {
				result = (int *) realloc(result, sizeof(int) * (*matches + 1));
				result[(*matches)] = i * recordsize;
				(*matches)++;
			}
		}
	}
	free(data);
	return result;
}

/**
 * Imprime os resultados se um comando "select"
 */
void printselect(Table *table, FILE *fp, int *result, int matches) {
	int i, j;
	void *data;

	// Para cada match, posiciona o ponteiro no arquivo e lê seus campos
	//     de acordo com cada tipo de dado
	for (i = 0; i < matches; i++) {
		fseek(fp, result[i], SEEK_SET);
		for (j = 0; j < table->nfields; j++) {
			data = malloc(table->fields[j].size);
			fread(data, table->fields[j].size, 1, fp);

			// tipo INT
			if (table->fields[j].type == INT) {
				int value = *((int *) data);
				printf("%d", value);

			// tipo CHAR
			} else if (table->fields[j].type == CHAR) {
				char value = *((char *) data);
				printf("'%c'", value);

			// tipo CCHAR
			} else if (table->fields[j].type == CCHAR) {
				char *value = (char *) data;
				printf("'%s'", value);

			// tipo FLOAT
			} else if (table->fields[j].type == FLOAT) {
				float value = *((float *) data);
				printf("%f", value);

			// tipo DOUBLE
			} else if (table->fields[j].type == DOUBLE) {
				double value = *((double *) data);
				printf("%lf", value);
			}

			free(data);
			if (j != table->nfields-1) printf(", ");
		}
		printf("\n");
	}
}

/**
 * Executa o último comando inserido na lista de comandos
 */
void exec(CommandList *cmdlist, TableList *tablelist, Statistics *stats) {

	// Confere o tipo do comando e o executa
	switch (cmdlist->commands[cmdlist->last].cmdtype) {

		// comando "create table"
		case CREATETABLE: {
			int i;
			FILE *fp;
			CmdCreateTable *pointer = (CmdCreateTable *) cmdlist->commands[cmdlist->last].cmd;

			// Aloca uma nova tabela na lista de tabelas
			tablelist->ntables++;
			tablelist->last++;
			tablelist->tables = (Table *) realloc(tablelist->tables, sizeof(Table) * tablelist->ntables);

			// Inicializa seus valores
			strcpy(tablelist->tables[tablelist->last].name, pointer->tablename);
			strcpy(tablelist->tables[tablelist->last].dat, tablelist->tables[tablelist->last].name);
			strcat(tablelist->tables[tablelist->last].dat, ".dat");
			strcpy(tablelist->tables[tablelist->last].tmp, tablelist->tables[tablelist->last].name);
			strcat(tablelist->tables[tablelist->last].tmp, ".tmp");
			tablelist->tables[tablelist->last].nfields = pointer->nfields;
			tablelist->tables[tablelist->last].last = pointer->nfields-1;
			tablelist->tables[tablelist->last].size = 0;

			// Aloca cada campo e preenche suas infos
			for (i = 0; i < pointer->nfields; i++) {
				tablelist->tables[tablelist->last].fields = (Field *) realloc(tablelist->tables[tablelist->last].fields, sizeof(Field) * (i + 1));
				strcpy(tablelist->tables[tablelist->last].fields[i].name, pointer->fieldnames[i]);
				tablelist->tables[tablelist->last].fields[i].type = pointer->fieldtypes[i];
				strcpy(tablelist->tables[tablelist->last].fields[i].typedesc, pointer->fieldtypesdesc[i]);
				tablelist->tables[tablelist->last].fields[i].size = pointer->fieldsizes[i];
				tablelist->tables[tablelist->last].fields[i].offset = tablelist->tables[tablelist->last].size;
				tablelist->tables[tablelist->last].size += pointer->fieldsizes[i];
				tablelist->tables[tablelist->last].fields[i].index = 0;
			}

			// Atualiza as estatísticas incrementando o número de tabelas
			stats->ntables++;

			// Apenas cria e trunca os arquivos ".dat" e ".tmp" da tabela
			fp = fopen(tablelist->tables[tablelist->last].dat, "wb"); fclose(fp);
			fp = fopen(tablelist->tables[tablelist->last].tmp, "wb"); fclose(fp);
		} break;

		// comando "showalltables"
		case SHOWALL: {
			int i, j;

			// Imprime na tela as infos de cada tabela na lista de tabelas
			for (i = 0; i < tablelist->ntables; i++) {
				printf("\nTablename: %s\n", tablelist->tables[i].name);
				for (j = 0; j < tablelist->tables[i].nfields; j++) {
					printf("\tField: %s Type: %s Size %d\n", tablelist->tables[i].fields[j].name, tablelist->tables[i].fields[j].typedesc, tablelist->tables[i].fields[j].size);
				}
				printf("\n");
			}

			// Atualiza as estatísticas incrementando o número de comandos "showalltables"
			stats->nshowalltables++;
		} break;

		// comando "showallindexes"
		case SHOWALLIDX: {
			int i;

			// Imprime na tela as infos de cada índice criado até o momento
			for (i = 0; i < stats->nindexes; i++) {
				printf("\nIndex information\n");
				printf("\tTablename: %s\n", stats->indexes[i].tablename);
				printf("\tFieldname: %s\n\n", stats->indexes[i].fieldname);
			}

			// Atualiza as estatísticas incrementando o número de comandos "showallindexes"
			stats->nshowallindexes++;
		} break;

		// comando "insert into"
		case INSERT: {
			int i, j;
			FILE *fp;
			CmdInsert *pointer = (CmdInsert *) cmdlist->commands[cmdlist->last].cmd;

			// Encontra na lista de tabelas a tabela onde serão inseridos os dados
			i = 0;
			while (strcmp(pointer->tablename, tablelist->tables[i].name) != 0)
				i++;

			// Anexa o registro ao arquivo ".tmp" da tabela
			// O valor de cada campo é gravado de acordo com seu tipo
			fp = fopen(tablelist->tables[i].tmp, "ab+");
			for (j = 0; j < tablelist->tables[i].nfields; j++) {
				switch (tablelist->tables[i].fields[j].type) {

					// tipo CHAR
					case CHAR:
						fwrite(&(pointer->values[j][0]), tablelist->tables[i].fields[j].size, 1,fp);
					break;

					// tipo CCHAR
					case CCHAR:
						fwrite(pointer->values[j], tablelist->tables[i].fields[j].size, 1,fp);
					break;

					// tipo INT
					case INT: {
						int value = atoi(pointer->values[j]);
						fwrite(&value, tablelist->tables[i].fields[j].size, 1,fp);
					} break;

					// tipo FLOAT
					case FLOAT: {
						float value = atof(pointer->values[j]);
						fwrite(&value, tablelist->tables[i].fields[j].size, 1,fp);
					} break;

					// tipo DOUBLE
					case DOUBLE: {
						double value = atof(pointer->values[j]);
						fwrite(&value, tablelist->tables[i].fields[j].size, 1,fp);
					} break;

					default: break;
				}
			}
			fclose(fp);

			// Atualiza as estatísticas incrementando o número de comandos "insert into"
			stats->ninserts++;
		} break;

		// comando "create index"
		case CREATEINDEX: {
			int i, j;
			char idxname[NAMELENGTH+NAMELENGTH+5];
			CmdCreateIndex *pointer = (CmdCreateIndex *) cmdlist->commands[cmdlist->last].cmd;

			// Encontra na lista de tabelas a tabela para qual será criado um índice
			i = 0;
			while (strcmp(pointer->tablename, tablelist->tables[i].name) != 0)
				i++;

			// Encontra na lista de tabelas o campo da tabela para qual será criado um índice
			j = 0;
			while (strcmp(pointer->fieldname, tablelist->tables[i].fields[j].name) != 0)
				j++;
				
			// Monta o nome do arquivo ".idx"
			strcpy(idxname, tablelist->tables[i].name);
			strcat(idxname, "-");
			strcat(idxname, tablelist->tables[i].fields[j].name);
			strcat(idxname, ".idx");

			// Cria o arquivo ".idx"
			createidx(idxname, tablelist, i, j);

			// Executa a ordenação do arquivo ".idx" recém-criado
			sortidx(idxname, tablelist->tables[i].fields[j].type, tablelist->tables[i].fields[j].size);

			// Atualiza as estatísticas incrementando o número de comandos "create index"
			stats->nindexes++;

			// Aloca as infos do índice para as estatísticas
			stats->indexes = (Index *) realloc(stats->indexes, sizeof(Index) * stats->nindexes);
			strcpy(stats->indexes[stats->nindexes-1].tablename, tablelist->tables[i].name);
			strcpy(stats->indexes[stats->nindexes-1].fieldname, tablelist->tables[i].fields[j].name);
		} break;

		// comando "sort"
		case SORT: {
			char idxname[NAMELENGTH+NAMELENGTH+5];
			CmdSort *pointer = (CmdSort *) cmdlist->commands[cmdlist->last].cmd;
			int i, j;

			// Encontra na lista de tabelas a tabela para qual será ordenado um índice
			i = 0;
			while (strcmp(pointer->tablename, tablelist->tables[i].name) != 0)
				i++;

			// Encontra na lista de tabelas o campo da tabela para qual será ordenado o índice
			j = 0;
			while (strcmp(pointer->fieldname, tablelist->tables[i].fields[j].name) != 0)
				j++;
				
			// Monta o nome do arquivo ".idx"
			strcpy(idxname, pointer->tablename);
			strcat(idxname, "-");
			strcat(idxname, pointer->fieldname);
			strcat(idxname, ".idx");
			
			// Cria novamente o arquivo ".idx"
			createidx(idxname, tablelist, i, j);

			// Executa a ordenação do arquivo ".idx"
			sortidx(idxname, tablelist->tables[i].fields[j].type, tablelist->tables[i].fields[j].size);

			// Atualiza as estatísticas incrementando o número de comandos "sort"
			stats->nsorts++;
		} break;

		// comando "select"
		case SELECT: {
			char idxname[NAMELENGTH+NAMELENGTH+5];
			CmdSelect *pointer = (CmdSelect *) cmdlist->commands[cmdlist->last].cmd;
			int i, j, matches = 0, *result = NULL;
			FILE *fpidx = NULL, *fpdat = NULL, *fptmp = NULL;

			// Encontra na lista de tabelas a tabela em que será selecionado os registros
			i = 0;
			while (strcmp(pointer->tablename, tablelist->tables[i].name) != 0)
				i++;

			// Encontra na lista de tabelas o campo da tabela que será usado para fazer a consulta
			j = 0;
			while (strcmp(pointer->fieldname, tablelist->tables[i].fields[j].name) != 0)
				j++;

			// Monta o nome do arquivo ".idx"
			strcpy(idxname, pointer->tablename);
			strcat(idxname, "-");
			strcat(idxname, pointer->fieldname);
			strcat(idxname, ".idx");

			fpdat = fopen(tablelist->tables[i].dat, "rb");
			fpidx = fopen(idxname, "rb");

			// Caso não exista o arquivo ".idx", é feita a busca sequencial no arquivo ".dat"
			if (fpidx == NULL) {
				fclose(fpidx);
				result = sequentialsearch(fpdat, pointer->value, tablelist->tables[i].fields[j].type, tablelist->tables[i].fields[j].size, tablelist->tables[i].fields[j].offset, tablelist->tables[i].size, &matches);
				if (result != NULL) {
					// Imprime os resultados na tela
					printselect(&(tablelist->tables[i]), fpdat, result, matches);
					free(result);
				}

				// Atualiza as estatísticas salvando o número de encontrados na busca sequencial
				stats->recordsinlastselectsequential = matches;
			} else {
				// Caso o arquivo ".idx" exista, é feita a busca binária nele
				result = binarysearch(fpidx, pointer->value, tablelist->tables[i].fields[j].type, tablelist->tables[i].fields[j].size, &matches);
				if (result != NULL) {
					// Imprime os resultados na tela
					printselect(&(tablelist->tables[i]), fpdat, result, matches);
					free(result);
				}
				fclose(fpidx);

				// Atualiza as estatísticas salvando o número de encontrados na busca binária
				stats->recordsinlastselectbinary = matches;
			}
			fclose(fpdat);

			// É sempre feita a busca sequencial no arquivo ".tmp"
			fptmp = fopen(tablelist->tables[i].tmp, "rb");
			result = sequentialsearch(fptmp, pointer->value, tablelist->tables[i].fields[j].type, tablelist->tables[i].fields[j].size, tablelist->tables[i].fields[j].offset, tablelist->tables[i].size, &matches);
			if (result != NULL) {
				printselect(&(tablelist->tables[i]), fptmp, result, matches);
				free(result);
			}
			fclose(fptmp);

			// Atualiza as estatísticas salvando o número de encontrados na busca sequencial
			stats->recordsinlastselectsequential = matches;

			// Atualiza as estatísticas incrementando o número de comandos "select"
			stats->nselects++;
		} break;

		// comando "statistics"
		case STATS: {
			// Imprime as infos das estatísticas na tela
			printf("#Tables: %d\n", stats->ntables);
			printf("#Indexes: %d\n", stats->nindexes);
			printf("#Inserts: %d\n", stats->ninserts);
			printf("#Selects: %d\n", stats->nselects);
			printf("#Sorts: %d\n", stats->nsorts);
			printf("#ShowAllTables: %d\n", stats->nshowalltables);
			printf("#ShowAllIndexes: %d\n", stats->nshowallindexes);
			printf("#Records in last select (binary search): %d\n", stats->recordsinlastselectbinary);
			printf("#Records in last select (sequential search): %d\n", stats->recordsinlastselectsequential);
		} break;

		default: break;
	}
}

/**
 * Faz a leitura e o parsing do arquivo de script
 */
void parse(char *file, CommandList *cmdlist, TableList *tablelist, Statistics *stats) {
	FILE *fp;
	char *str;

	fp = fopen(file, "r");
	while (!feof(fp)) {
		str = scanfile(fp); // lê uma palavra do script
		if (!feof(fp)) {

			// "create"
			if (strcmp(str, "create") == 0 || strcmp(str, "CREATE") == 0) {
				free(str);
				str = scanfile(fp);

				// comando "create table"
				if (strcmp(str, "table") == 0 || strcmp(str, "TABLE") == 0) {
					CmdCreateTable *pointer;
					char *aux;

					// Aloca e inicializa um novo comando na lista de comandos
					cmdlist->ncommands++;
					cmdlist->last++;
					cmdlist->commands = (Command *) realloc(cmdlist->commands, sizeof(Command) * cmdlist->ncommands);
					cmdlist->commands[cmdlist->last].cmdtype = CREATETABLE;
					cmdlist->commands[cmdlist->last].cmdstatus = OK;
					pointer = (CmdCreateTable *) malloc(sizeof(CmdCreateTable));
					pointer->fieldnames = NULL;
					pointer->fieldtypes = NULL;
					pointer->fieldtypesdesc = NULL;
					pointer->fieldsizes = NULL;
					pointer->nfields = 0;

					// Lê o nome da tabela
					free(str);
					str = scanfile(fp);
					strcpy(pointer->tablename, str);

					// Faz a leitura dos nomes dos campos
					free(str);
					str = scanfile(fp);
					aux = scanstr(str);
					while (*aux != '\0') {
						// Aloca cada campo com suas infos
						pointer->fieldnames = (char **) realloc(pointer->fieldnames, sizeof(char *) * (pointer->nfields + 1));
						pointer->fieldnames[pointer->nfields] = (char *) malloc(sizeof(char) * NAMELENGTH);
						pointer->fieldtypes = (int *) realloc(pointer->fieldtypes, sizeof(int) * (pointer->nfields + 1));
						pointer->fieldtypesdesc = (char **) realloc(pointer->fieldtypesdesc, sizeof(char *) * (pointer->nfields + 1));
						pointer->fieldtypesdesc[pointer->nfields] = (char *) malloc(sizeof(char) * NAMELENGTH);
						pointer->fieldsizes = (int *) realloc(pointer->fieldsizes, sizeof(int) * (pointer->nfields + 1));

						strcpy(pointer->fieldnames[pointer->nfields], aux);
						free(aux);
						aux = scanstr(NULL);

						// Confere e seleciona o tipo de dado do campo

						// tipo CHAR e CCHAR
						if (strstr(aux, "char") != NULL || strstr(aux, "CHAR") != NULL) {
							strcpy(pointer->fieldtypesdesc[pointer->nfields], "char");
							if (strstr(aux, "[") != NULL) {
								strtok(aux, "[]");
								pointer->fieldtypes[pointer->nfields] = CCHAR;
								pointer->fieldsizes[pointer->nfields] = atoi(strtok(NULL, "[]")) * sizeof(char);
							} else {
								pointer->fieldtypes[pointer->nfields] = CHAR;
								pointer->fieldsizes[pointer->nfields] = sizeof(char);
							}

						// tipo INT
						} else if (strcmp(aux, "int") == 0 || strcmp(aux, "INT") == 0) {
							strcpy(pointer->fieldtypesdesc[pointer->nfields], "int");
							pointer->fieldtypes[pointer->nfields] = INT;
							pointer->fieldsizes[pointer->nfields] = sizeof(int);

						// tipo FLOAT
						} else if (strcmp(aux, "float") == 0 || strcmp(aux, "FLOAT") == 0) {
							strcpy(pointer->fieldtypesdesc[pointer->nfields], "float");
							pointer->fieldtypes[pointer->nfields] = FLOAT;
							pointer->fieldsizes[pointer->nfields] = sizeof(float);

						// tipo DOUBLE
						} else if (strcmp(aux, "double") == 0 || strcmp(aux, "DOUBLE") == 0) {
							strcpy(pointer->fieldtypesdesc[pointer->nfields], "double");
							pointer->fieldtypes[pointer->nfields] = DOUBLE;
							pointer->fieldsizes[pointer->nfields] = sizeof(double);
						}
						pointer->nfields++;
						free(aux);
						aux = scanstr(NULL);
					}
					free(aux);

					// Confere o estado do comando e executa se estiver OK
					cmdlist->commands[cmdlist->last].cmd = (void *) pointer;
					if (cmdlist->commands[cmdlist->last].cmdstatus == OK)
						exec(cmdlist, tablelist, stats);
				} else

				// comando "create index"
				if (strcmp(str, "index") == 0 || strcmp(str, "INDEX") == 0) {
					CmdCreateIndex *pointer;
					int i, j;

					// Aloca e inicializa um novo comando na lista de comandos
					cmdlist->ncommands++;
					cmdlist->last++;
					cmdlist->commands = (Command *) realloc(cmdlist->commands, sizeof(Command) * cmdlist->ncommands);
					cmdlist->commands[cmdlist->last].cmdtype = CREATEINDEX;
					cmdlist->commands[cmdlist->last].cmdstatus = OK;
					pointer = (CmdCreateIndex *) malloc(sizeof(CmdCreateIndex));

					// Lê o nome da tabela
					free(str);
					str = scanfile(fp);
					strcpy(pointer->tablename, str);

					// Lê o nome do campo
					free(str);
					str = scanfile(fp);
					strcpy(pointer->fieldname, str);

					// Confere se a tabela e o campo existem
					cmdlist->commands[cmdlist->last].cmdstatus = TABLEDOESNOTEXIST;
					for (i = 0; i < tablelist->ntables; i++) {
						if (strcmp(tablelist->tables[i].name, pointer->tablename) == 0) {
							cmdlist->commands[cmdlist->last].cmdstatus = FIELDDOESNOTEXIST;
							for (j = 0; j < tablelist->tables[i].nfields; j++) {
								if (strcmp(tablelist->tables[i].fields[j].name, pointer->fieldname) == 0) {
									cmdlist->commands[cmdlist->last].cmdstatus = OK;
									break;
								}
							}
						}
					}

					// Confere o estado do comando e executa se estiver OK
					cmdlist->commands[cmdlist->last].cmd = (void *) pointer;
					if (cmdlist->commands[cmdlist->last].cmdstatus == OK)
						exec(cmdlist, tablelist, stats);
				}
			} else

			// comando "showalltables"
			if (strcmp(str, "showalltables") == 0 || strcmp(str, "SHOWALLTABLES") == 0) {
				// Aloca e inicializa um novo comando na lista de comandos
				cmdlist->ncommands++;
				cmdlist->last++;
				cmdlist->commands = (Command *) realloc(cmdlist->commands, sizeof(Command) * cmdlist->ncommands);
				cmdlist->commands[cmdlist->last].cmdtype = SHOWALL;
				cmdlist->commands[cmdlist->last].cmdstatus = OK;
				cmdlist->commands[cmdlist->last].cmd = NULL;

				// Confere o estado do comando e executa se estiver OK
				if (cmdlist->commands[cmdlist->last].cmdstatus == OK)
					exec(cmdlist, tablelist, stats);
			} else

			// "insert"
			if (strcmp(str, "insert") == 0 || strcmp(str, "INSERT") == 0) {
				free(str);
				str = scanfile(fp);

				// comando "insert into"
				if (strcmp(str, "into") == 0 || strcmp(str, "INTO") == 0) {
					CmdInsert *pointer;
					char *aux;

					// Aloca e inicializa um novo comando na lista de comandos
					cmdlist->ncommands++;
					cmdlist->last++;
					cmdlist->commands = (Command *) realloc(cmdlist->commands, sizeof(Command) * cmdlist->ncommands);
					cmdlist->commands[cmdlist->last].cmdtype = INSERT;
					cmdlist->commands[cmdlist->last].cmdstatus = OK;
					pointer = (CmdInsert *) malloc(sizeof(CmdInsert));
					pointer->fieldnames = NULL;
					pointer->values = NULL;
					pointer->nfields = 0;

					// Lê o nome da tabela
					free(str);
					str = scanfile(fp);
					strcpy(pointer->tablename, str);

					// Lê os nomes dos campos
					//     Não muito útil por enquanto, pois as inserções estão sendo
					//     sempre na ordem correta dos campos e eu ainda não implementei
					//     a possibilidades de fazê-las em qualquer ordem
					free(str);
					str = scanfile(fp);
					aux = scanstr(str);
					while (*aux != '\0') {
						pointer->fieldnames = (char **) realloc(pointer->fieldnames, sizeof(char *) * (pointer->nfields + 1));
						pointer->fieldnames[pointer->nfields] = (char *) malloc(sizeof(char) * NAMELENGTH);
						strcpy(pointer->fieldnames[pointer->nfields], aux);
						pointer->nfields++;
						free(aux);
						aux = scanstr(NULL);
					}
					free(aux);

					// Escaneia a próxima palavra no script
					free(str);
					str = scanfile(fp);

					if (strcmp(str, "values") == 0 || strcmp(str, "VALUES") == 0) {
						int i;

						// Lê o valores a serem inseridos
						free(str);
						str = scanfile(fp);
						aux = scanstr(str);
						i = 0;
						while (*aux != '\0') {
							pointer->values = (char **) realloc(pointer->values, sizeof(char *) * (i + 1));
							pointer->values[i] = (char *) malloc(sizeof(char) * (strlen(aux) + 1));
							strcpy(pointer->values[i++], aux);
							free(aux);
							aux = scanstr(NULL);
						}
						free(aux);

						// Confere se a quantidade de valores é igual a quantidade de campos
						if (i != pointer->nfields) {
							cmdlist->commands[cmdlist->last].cmdstatus = FIELDSANDVALUESDONOTMATCH;
						} else {
							// Confere se a tabela existe
							cmdlist->commands[cmdlist->last].cmdstatus = TABLEDOESNOTEXIST;
							for (i = 0; i < tablelist->ntables; i++) {
								if (strcmp(tablelist->tables[i].name, pointer->tablename) == 0) {
									cmdlist->commands[cmdlist->last].cmdstatus = OK;
									break;
								}
							}
						}

					} else {
						cmdlist->commands[cmdlist->last].cmdstatus = SYNTAXERRORVALUES;
					}

					// Confere o estado do comando e executa se estiver OK
					cmdlist->commands[cmdlist->last].cmd = (void *) pointer;
					if (cmdlist->commands[cmdlist->last].cmdstatus == OK)
						exec(cmdlist, tablelist, stats);
				}
			} else

			// comando "statistics"
			if (strcmp(str, "statistics") == 0 || strcmp(str, "STATISTICS") == 0) {
				// Aloca e inicializa um novo comando na lista de comandos
				cmdlist->ncommands++;
				cmdlist->last++;
				cmdlist->commands = (Command *) realloc(cmdlist->commands, sizeof(Command) * cmdlist->ncommands);
				cmdlist->commands[cmdlist->last].cmdtype = STATS;
				cmdlist->commands[cmdlist->last].cmdstatus = OK;
				cmdlist->commands[cmdlist->last].cmd = NULL;

				// Confere o estado do comando e executa se estiver OK
				if (cmdlist->commands[cmdlist->last].cmdstatus == OK)
					exec(cmdlist, tablelist, stats);
			} else

			// comando "showallindexes"
			if (strcmp(str, "showallindexes") == 0 || strcmp(str, "SHOWALLINDEXES") == 0) {
				// Aloca e inicializa um novo comando na lista de comandos
				cmdlist->ncommands++;
				cmdlist->last++;
				cmdlist->commands = (Command *) realloc(cmdlist->commands, sizeof(Command) * cmdlist->ncommands);
				cmdlist->commands[cmdlist->last].cmdtype = SHOWALLIDX;
				cmdlist->commands[cmdlist->last].cmdstatus = OK;
				cmdlist->commands[cmdlist->last].cmd = NULL;

				// Confere o estado do comando e executa se estiver OK
				if (cmdlist->commands[cmdlist->last].cmdstatus == OK)
					exec(cmdlist, tablelist, stats);
			} else

			// comando "sort"
			if (strcmp(str, "sort") == 0 || strcmp(str, "SORT") == 0) {
				CmdSort *pointer;
				int i;

				// Aloca e inicializa um novo comando na lista de comandos
				cmdlist->ncommands++;
				cmdlist->last++;
				cmdlist->commands = (Command *) realloc(cmdlist->commands, sizeof(Command) * cmdlist->ncommands);
				cmdlist->commands[cmdlist->last].cmdtype = SORT;
				cmdlist->commands[cmdlist->last].cmdstatus = OK;
				pointer = (CmdSort *) malloc(sizeof(CmdSort));

				// Lê o nome da tabela
				free(str);
				str = scanfile(fp);
				strcpy(pointer->tablename, str);

				// Lê o nome do campo
				free(str);
				str = scanfile(fp);
				strcpy(pointer->fieldname, str);

				// Confere se o índice existe
				cmdlist->commands[cmdlist->last].cmdstatus = INDEXDOESNOTEXIST;
				for (i = 0; i < stats->nindexes; i++) {
					if (strcmp(pointer->tablename, stats->indexes[i].tablename) == 0 && strcmp(pointer->fieldname, stats->indexes[i].fieldname) == 0) {
						cmdlist->commands[cmdlist->last].cmdstatus = OK;
						break;
					}
				}

				// Confere o estado do comando e executa se estiver OK
				cmdlist->commands[cmdlist->last].cmd = (void *) pointer;
				if (cmdlist->commands[cmdlist->last].cmdstatus == OK)
					exec(cmdlist, tablelist, stats);
			} else

			// comando "select"
			if (strcmp(str, "select") == 0 || strcmp(str, "SELECT") == 0) {
				CmdSelect *pointer;
				char *aux;
				int i, j;

				// Aloca e inicializa um novo comando na lista de comandos
				cmdlist->ncommands++;
				cmdlist->last++;
				cmdlist->commands = (Command *) realloc(cmdlist->commands, sizeof(Command) * cmdlist->ncommands);
				cmdlist->commands[cmdlist->last].cmdtype = SELECT;
				cmdlist->commands[cmdlist->last].cmdstatus = OK;
				pointer = (CmdSelect *) malloc(sizeof(CmdSelect));

				// Lê o nome da tabela
				free(str);
				str = scanfile(fp);
				strcpy(pointer->tablename, str);

				// Lê o nome do campo
				free(str);
				str = scanfile(fp);
				strcpy(pointer->fieldname, str);

				// Lê o valor
				free(str);
				str = scanfile(fp);
				aux = scanstr(str);
				pointer->value = (char *) malloc(sizeof(char) * (strlen(aux) + 1));
				strcpy(pointer->value, aux);
				free(aux);

				// Confere se a tabela e o campo existem
				cmdlist->commands[cmdlist->last].cmdstatus = TABLEDOESNOTEXIST;
				for (i = 0; i < tablelist->ntables; i++) {
					if (strcmp(tablelist->tables[i].name, pointer->tablename) == 0) {
						cmdlist->commands[cmdlist->last].cmdstatus = FIELDDOESNOTEXIST;
						for (j = 0; j < tablelist->tables[i].nfields; j++) {
							if (strcmp(tablelist->tables[i].fields[j].name, pointer->fieldname) == 0) {
								cmdlist->commands[cmdlist->last].cmdstatus = OK;
								break;
							}
						}
					}
				}

				// Confere o estado do comando e executa se estiver OK
				cmdlist->commands[cmdlist->last].cmd = (void *) pointer;
				if (cmdlist->commands[cmdlist->last].cmdstatus == OK)
					exec(cmdlist, tablelist, stats);
			}
		}
		free(str);
	}
	fclose(fp);
}

int main(int argc, char *argv[]) {
	char file[101];
	CommandList cmdlist;
	TableList tablelist;
	Statistics stats;
	int i, j;

	// Inicializando listas
	cmdlist.commands = NULL;
	cmdlist.ncommands = 0;
	cmdlist.last = -1;
	tablelist.tables = NULL;
	tablelist.ntables = 0;
	tablelist.last = -1;

	// Inicializando stats
	stats.ntables = 0;
	stats.indexes = NULL;
	stats.nindexes = 0;
	stats.ninserts = 0;
	stats.nselects = 0;
	stats.nsorts = 0;
	stats.nshowalltables = 0;
	stats.nshowallindexes = 0;
	stats.recordsinlastselectbinary = 0;
	stats.recordsinlastselectsequential = 0;

	// Parsing e execução do arquivo de script
	scanf("%s", file);
	parse(file, &cmdlist, &tablelist, &stats);

	// Liberação de memória
	if (tablelist.tables != NULL) {
		for (i = 0; i < tablelist.ntables; i++) {
			free(tablelist.tables[i].fields);
		}
		free(tablelist.tables);
	}
	if (cmdlist.commands != NULL) {
		for (i = 0; i < cmdlist.ncommands; i++) {
			if (cmdlist.commands[i].cmdtype == CREATETABLE) {
				CmdCreateTable *pointer = (CmdCreateTable *) cmdlist.commands[i].cmd;
				for (j = 0; j < pointer->nfields; j++) {
					free(pointer->fieldnames[j]);
				}
				free(pointer->fieldnames);
				free(pointer->fieldtypes);
				free(pointer->fieldsizes);
				free(pointer);
			} else if (cmdlist.commands[i].cmdtype == INSERT) {
				CmdInsert *pointer = (CmdInsert *) cmdlist.commands[i].cmd;
				for (j = 0; j < pointer->nfields; j++) {
					free(pointer->fieldnames[j]);
					free(pointer->values[j]);
				}
				free(pointer->fieldnames);
				free(pointer->values);
				free(pointer);
			} else if (cmdlist.commands[i].cmdtype == SELECT) {
				CmdSelect *pointer = (CmdSelect *) cmdlist.commands[i].cmd;
				free(pointer->value);
				free(pointer);
			} else {
				free(cmdlist.commands[i].cmd);
			}
		}
		free(cmdlist.commands);
	}
	if (stats.indexes != NULL) {
		free(stats.indexes);
	}

	return 0;
}
