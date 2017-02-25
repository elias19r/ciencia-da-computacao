/**
 * trabalho05
 * 2012/10/14
 * Elias Italiano Rodrigues, 7987251, ICC2, BCC-B, ICMC-USP
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ENTER 10

// Definindo valores numéricos para os tipos de campos
#define INT   0
#define CHAR  1
#define CCHAR 2

// Definindo valores numéricos para as palavras-chave
#define ORDER 0

// Definindo o tamanho dos tipos de campos
#define SIZEOFINT  4
#define SIZEOFCHAR 1

// Definindo valores numéricos para os comandos
#define SEARCH 0

// Tipos de informações para a função pegarInfoCampo
#define TIPO    0
#define TAMANHO 1
#define OFFSET  2

// Estrutura para guardar as informações de um campo
typedef struct {
	char *nome;
	int tipo;
	int *palavrasChave;
	int qtdePalavrasChave;
	int tamanho;
	int offset;
} Campo;

// Estrutura para guardar as informações de uma tabela
typedef struct {
	char *nome;
	Campo *campos;
	int qtdeCampos;
	int tamanho;
} Tabela;

// Estrutura para guardar as informações de um script
typedef struct {
	int comando;
	char **parametros;
	int qtdeParametros;
} Script;

// Estrutura para guardar os valores de um registro do arquivo IDX
typedef struct {
	void *dado;
	int offset;
} RegistroIDX;

/**
 * Função para escanear os arquivos de schema e script
 * @return char * palavra escaneada
 */
char *escanear(FILE *fp) {
	char c, *leitura = NULL;
	int aspas = 0, count = 0;

	while (!feof(fp)) {
		fscanf(fp, "%c", &c);
		if (!feof(fp)) {
			if (c == '"') {
				aspas = !(aspas);
				continue;
			}
			if (aspas || (c != ' ' && c != '\t' && c != '\n' && c != '\r' && c != '\v' && c != '\f')) {
				leitura = (char *) realloc(leitura, sizeof(char) * (count + 1));
				leitura[count] = c;
				count++;
			} else if (count > 0) {
				break;
			}
		}
	}
	if (leitura != NULL) {
		leitura = (char *) realloc(leitura, sizeof(char) * (count + 1));
		leitura[count] = '\0';
	}

	return leitura;
}

/**
 * Faz o parse do arquivo de schema
 * @return Tabela * contendo as informações das tabelas lidas
 */
Tabela *parseSchema(char *arquivo, int *qtde) {
	FILE *fp;
	char *leitura;
	int indiceT, indiceC;
	Tabela *tabelas = NULL;

	*qtde = 0;
	fp = fopen(arquivo, "r");

	if (fp == NULL) return NULL;
	
	while (!feof(fp)) {
		leitura = escanear(fp);

		if (leitura != NULL && !feof(fp)) {
			// "tabela"
			if (strstr(leitura, "tabela") != NULL || strstr(leitura, "TABELA") != NULL) {
				char *nome = escanear(fp);
				if (nome != NULL) {
					indiceT = *qtde;
					tabelas = (Tabela *) realloc(tabelas, sizeof(Tabela) * (*qtde + 1));
					tabelas[indiceT].nome = malloc(sizeof(char) * (strlen(nome) + 1));
					strcpy(tabelas[indiceT].nome, nome);
					tabelas[indiceT].campos = NULL;
					tabelas[indiceT].qtdeCampos = 0;
					tabelas[indiceT].tamanho = 0;
					(*qtde)++;
					free(nome);
				}

			// "order"
			} else if (strstr(leitura, "order") != NULL || strstr(leitura, "ORDER") != NULL) {
				tabelas[indiceT].campos[indiceC].palavrasChave = (int *) realloc(tabelas[indiceT].campos[indiceC].palavrasChave, sizeof(int) * (tabelas[indiceT].campos[indiceC].qtdePalavrasChave + 1));
				tabelas[indiceT].campos[indiceC].palavrasChave[(tabelas[indiceT].campos[indiceC].qtdePalavrasChave)] = ORDER;
				tabelas[indiceT].campos[indiceC].qtdePalavrasChave++;

			// "int"
			} else if (strstr(leitura, "int") != NULL || strstr(leitura, "INT") != NULL) {
				tabelas[indiceT].campos[indiceC].tipo = INT;
				tabelas[indiceT].campos[indiceC].tamanho = SIZEOFINT;
				tabelas[indiceT].campos[indiceC].offset = tabelas[indiceT].tamanho;
				tabelas[indiceT].tamanho += SIZEOFINT;

			// "char"
			} else if (strstr(leitura, "char") != NULL || strstr(leitura, "CHAR") != NULL) {
				tabelas[indiceT].campos[indiceC].tipo = CHAR;
				leitura = strtok(leitura, "[");
				if (leitura != NULL) {
					tabelas[indiceT].campos[indiceC].tipo = CCHAR;
					tabelas[indiceT].campos[indiceC].tamanho = atoi(strtok(NULL, "]")) * SIZEOFCHAR;
				} else {
					tabelas[indiceT].campos[indiceC].tamanho = SIZEOFCHAR;
				}
				tabelas[indiceT].campos[indiceC].offset = tabelas[indiceT].tamanho;
				tabelas[indiceT].tamanho += tabelas[indiceT].campos[indiceC].tamanho;
			
			// campo
			} else {
				indiceC = tabelas[indiceT].qtdeCampos;
				tabelas[indiceT].campos = (Campo *) realloc(tabelas[indiceT].campos, sizeof(Campo) * (tabelas[indiceT].qtdeCampos + 1));
				tabelas[indiceT].campos[indiceC].nome = NULL;
				tabelas[indiceT].campos[indiceC].tipo = -1;
				tabelas[indiceT].campos[indiceC].palavrasChave = NULL;
				tabelas[indiceT].campos[indiceC].qtdePalavrasChave = 0;
				tabelas[indiceT].campos[indiceC].tamanho = 0;
				tabelas[indiceT].campos[indiceC].offset = 0;

				tabelas[indiceT].campos[indiceC].nome = (char *) malloc(sizeof(char) * (strlen(leitura) + 1));
				strcpy(tabelas[indiceT].campos[indiceC].nome, leitura);
				
				tabelas[indiceT].qtdeCampos++;
			}
		free(leitura);			
		}		
	}
	fclose(fp);

	return tabelas;
}

/**
 * Faz o parse do arquivo de script
 * @return Script * contendo as informações dos scripts lidos
 */
Script *parseScript(char *arquivo, int *qtde) {
	FILE *fp;
	char *leitura;
	int indiceS, indiceP;
	Script *scripts = NULL;

	*qtde = 0;
	fp = fopen(arquivo, "r");

	if (fp == NULL) return NULL;
	
	while (!feof(fp)) {
		leitura = escanear(fp);
		
		if (leitura != NULL && !feof(fp)) {
		
			// "search"
			if (strstr(leitura, "search") != NULL || strstr(leitura, "SEARCH") != NULL) {
				indiceS = *qtde;
				scripts = (Script *) realloc(scripts, sizeof(Script) * (*qtde + 1));
				scripts[indiceS].comando = SEARCH;
				scripts[indiceS].qtdeParametros = 3;
				scripts[indiceS].parametros = (char **) malloc(sizeof(char *) * scripts[indiceS].qtdeParametros);

				// salvando parâmtros
				for (indiceP = 0; indiceP < scripts[indiceS].qtdeParametros; indiceP++) {
					char *parametro = escanear(fp);
					if (parametro != NULL) {
						scripts[indiceS].parametros[indiceP] = (char *) malloc(sizeof(char) * (strlen(parametro) + 1));
						strcpy(scripts[indiceS].parametros[indiceP], parametro);
						free(parametro);
					}
				}
				(*qtde)++;
			}
		free(leitura);
		}		
	}
	fclose(fp);
		
	return scripts;
}

/**
 * Algoritmo de ordenação mergesort para ordenar os registros lidos
 */
void mergesort(RegistroIDX *registros, int comeco, int fim, int tipoCampo, int tamanhoCampo) {
	int meio = (comeco + fim) / 2;

	if (comeco >= fim) return;

	mergesort(registros, comeco, meio, tipoCampo, tamanhoCampo);
	mergesort(registros, meio+1, fim, tipoCampo, tamanhoCampo);
	{
		RegistroIDX *direita, *esquerda;
		int i, j, k, tamanhoDireita, tamanhoEsquerda;

		tamanhoDireita = fim - meio;
		tamanhoEsquerda = meio - comeco + 1;

		direita  = (RegistroIDX *) malloc(sizeof(RegistroIDX) * tamanhoDireita);
		esquerda = (RegistroIDX *) malloc(sizeof(RegistroIDX) * tamanhoEsquerda);

		memcpy(direita, &registros[meio+1], sizeof(RegistroIDX) * tamanhoDireita);
		memcpy(esquerda, &registros[comeco], sizeof(RegistroIDX) * tamanhoEsquerda);

		i = j = 0;
		for (k = comeco; k < fim+1; k++) {
			if (j < tamanhoDireita && i < tamanhoEsquerda) {
			
				// tipo INT
				if (tipoCampo == INT) {
					int valorEsquerda = *((int *) esquerda[i].dado);
					int valorDireita  = *((int *) direita[j].dado);
					
					if (valorEsquerda > valorDireita) {
						registros[k] = direita[j];
						j++;
					} else {
						registros[k] = esquerda[i];
						i++;
					}				
					
				// tipo CHAR
				} else if (tipoCampo == CHAR) {
					char valorEsquerda = *((char *) esquerda[i].dado);
					char valorDireita  = *((char *) direita[j].dado);
					
					if (valorEsquerda > valorDireita) {
						registros[k] = direita[j];
						j++;
					} else {
						registros[k] = esquerda[i];
						i++;
					}				
				
				// tipo CCHAR
				} else if (tipoCampo == CCHAR) {
					char *valorEsquerda = (char *) esquerda[i].dado;
					char *valorDireita  = (char *) direita[j].dado;
					
					valorEsquerda = (char *) realloc(valorEsquerda, tamanhoCampo + 1);
					valorEsquerda[tamanhoCampo] = '\0';
					
					valorDireita = (char *) realloc(valorDireita, tamanhoCampo + 1);
					valorDireita[tamanhoCampo] = '\0';
					
					if (strcmp(valorEsquerda, valorDireita) > 0) {
						registros[k] = direita[j];
						j++;
					} else {
						registros[k] = esquerda[i];
						i++;
					}				
				}			
				
			} else if (j < tamanhoDireita) {
				registros[k] = direita[j];
				j++;
			} else if (i < tamanhoEsquerda) {
				registros[k] = esquerda[i];
				i++;
			}
		}

		free(direita);
		free(esquerda);
	}
}

/**
 * Informados o nome da tabela e o nome do campo, pega e uma informação do campo
 * @return int informação do campo
 */
int pegarInfoCampo(char *tabela, char *campo, Tabela *tabelas, int qtdeTabelas, int info) {
	int i, j;
	
	for (i = 0; i < qtdeTabelas; i++) {
		if (strcmp(tabelas[i].nome, tabela) == 0) {
			for (j = 0; j < tabelas[i].qtdeCampos; j++) {
				if (strcmp(tabelas[i].campos[j].nome, campo) == 0) {
					if (info == TIPO)
						return tabelas[i].campos[j].tipo;
					else if (info == TAMANHO)
						return tabelas[i].campos[j].tamanho;
					else if (info == OFFSET)
						return tabelas[i].campos[j].offset;
				}
			}
		}
	}	
	return -1;
}

/**
 * Função para ordenar o arquivo IDX
 */
void ordenarIDX(char *arquivo, Tabela *tabelas, int qtdeTabelas) {
	FILE *fp;
	char *nomeTabela, *nomeCampo;
	int tipoCampo, tamanhoCampo, count, i;
	void *dado;
	int offset;
	RegistroIDX *registros = NULL;

	fp = fopen(arquivo, "rb+");
	
	// Pegando o tipo e tamanho do campo
	nomeTabela = strtok(arquivo, "-");
	nomeCampo  = strtok(NULL, ".");
	tipoCampo    = pegarInfoCampo(nomeTabela, nomeCampo, tabelas, qtdeTabelas, TIPO);
	tamanhoCampo = pegarInfoCampo(nomeTabela, nomeCampo, tabelas, qtdeTabelas, TAMANHO);
	
	// Leitura dos registros
	count = 0;	
	dado = malloc(tamanhoCampo);
	while (!feof(fp)) {
		fread(dado, tamanhoCampo, 1, fp);
		fread(&offset, sizeof(int), 1, fp);
		if (!feof(fp)) {
			registros = (RegistroIDX *) realloc(registros, sizeof(RegistroIDX) * (count + 1));
			registros[count].dado = malloc(tamanhoCampo);
			memcpy(registros[count].dado, dado, tamanhoCampo);
			registros[count].offset = offset;
			count++;
		}
	}
	free(dado);
	
	// Ordenação dos registros
	mergesort(registros, 0, count-1, tipoCampo, tamanhoCampo);

	// Reescrevendo o arquivo agora com os registros ordenados
	fseek(fp, 0, SEEK_SET);
	for (i = 0; i < count; i++) {
		fwrite(registros[i].dado, tamanhoCampo, 1, fp);
		fwrite(&(registros[i].offset), sizeof(int), 1, fp);
		free(registros[i].dado);
	}
	free(registros);
	fclose(fp);
}

/**
 * Função que cria os arquivos IDX para as tabelas
 */
void criarIDX(Tabela *tabelas, int qtdeTabelas) {
	FILE *fpIDX, *fpDAT;
	char *arquivoIDX, *arquivoDAT;
	int i, j, k, offset;
	void *leitura;

	// Percorre todas tabelas
	for (i = 0; i < qtdeTabelas; i++) {
	
		arquivoDAT = (char *) malloc(sizeof(char) * (strlen(tabelas[i].nome) + 5));
		strcpy(arquivoDAT, tabelas[i].nome);
		strcat(arquivoDAT, ".dat");
	
		fpDAT = fopen(arquivoDAT, "rb");
	
		// Percorre os campos e encontra aqueles que tem palavra chave ORDER
		for (j = 0; j < tabelas[i].qtdeCampos; j++) {
			for (k = 0; k < tabelas[i].campos[j].qtdePalavrasChave; j++) {
				if (tabelas[i].campos[j].palavrasChave[k] == ORDER) {
					arquivoIDX = (char *) malloc(sizeof(char) * (strlen(tabelas[i].nome) + strlen(tabelas[i].campos[j].nome) + 6));
					strcpy(arquivoIDX, tabelas[i].nome);
					strcat(arquivoIDX, "-");
					strcat(arquivoIDX, tabelas[i].campos[j].nome);
					strcat(arquivoIDX, ".idx");
				
					// Criação do arquivo IDX
					fpIDX = fopen(arquivoIDX, "wb+");				
					offset = tabelas[i].campos[j].offset;
					fseek(fpDAT, offset, SEEK_SET);
					leitura = malloc(tabelas[i].campos[j].tamanho);
					while (!feof(fpDAT)) {
						fread(leitura, tabelas[i].campos[j].tamanho, 1, fpDAT);
						if (!feof(fpDAT)) {
							fwrite(leitura, tabelas[i].campos[j].tamanho, 1, fpIDX);
							fwrite(&offset, sizeof(int), 1, fpIDX);
							offset += tabelas[i].tamanho;
							fseek(fpDAT, offset, SEEK_SET);
						}
					}				
					free(leitura);
					fclose(fpIDX);

					// Ordenação do arquivo IDX recém criado
					ordenarIDX(arquivoIDX, tabelas, qtdeTabelas);
					free(arquivoIDX);
				}
			}
		}
		free(arquivoDAT);
		fclose(fpDAT);
	}
}

/**
 * Busca binária iterativa em um arquivo
 * @return int o número de iteracoes feitas na tentativa de busca
 */
int buscaBinaria(FILE *fp, int tipoCampo, int tamanhoCampo, char *chave, int *offset) {
	int comeco, meio, fim, iteracoes = 0; // inicializa o contador de iteracoes em zero
	int tamanhoRegistro, tamanhoArquivo;
	void *leitura;
	
	*offset = -1;

	// Quantidade de chaves que o arquivo possui
	tamanhoRegistro = tamanhoCampo + sizeof(int);
	fseek(fp, 0, SEEK_END);
	tamanhoArquivo = ftell(fp);	
	tamanhoArquivo /= tamanhoRegistro;
	
	// Define comeco e fim
	comeco = 0;
	fim = tamanhoArquivo-1;
	
	leitura = malloc(tamanhoCampo);
	while (comeco <= fim) {
		iteracoes++;
		meio = (comeco + fim) / 2;
		fseek(fp, meio * tamanhoRegistro, SEEK_SET);
		fread(leitura, tamanhoCampo, 1, fp);

		// tipo INT		
		if (tipoCampo == INT) {
			int chaveLida = *((int *) leitura);
			int chaveProcurada = atoi(chave);

			if (chaveProcurada == chaveLida) {
				fread(offset, sizeof(int), 1, fp);
				break;
			}
			if (chaveProcurada > chaveLida)
				comeco = meio+1;
			else
				fim = meio-1;
		
		// tipo CHAR
		} else if (tipoCampo == CHAR) {
			char chaveLida = *((char *) leitura);
			char chaveProcurada = *chave;

			if (chaveProcurada == chaveLida) {
				fread(offset, sizeof(int), 1, fp);
				break;
			}
			if (chaveProcurada > chaveLida)
				comeco = meio+1;
			else
				fim = meio-1;
		
		// tipo CCHAR
		} else if (tipoCampo == CCHAR) {
			char *chaveLida = (char *) leitura;
			char *chaveProcurada = chave;
			
			chaveLida = (char *) realloc(chaveLida, tamanhoCampo + 1);
			chaveLida[tamanhoCampo] = '\0';

			if (strcmp(chaveProcurada, chaveLida) == 0) {
				fread(offset, sizeof(int), 1, fp);
				break;
			}
			if (strcmp(chaveProcurada, chaveLida) > 0)
				comeco = meio+1;
			else
				fim = meio-1;		
		}
	}
	free(leitura);
	
	return iteracoes;
}

/**
 * Faz a execução de um script e imprimi na tela o resultado
 */
void executarScript(Script *script, Tabela *tabelas, int qtdeTabelas) {
	int offset, iteracoes;
	
	// comando SEARCH
	if (script->comando == SEARCH) {
		FILE *fp;
		char *nomeTabela, *nomeCampo;
		int i, j, tipoCampo, tamanhoCampo, offsetCampo;
		
		fp = fopen(script->parametros[0], "rb");
		
		// Arquivo IDX inexistente?
		if (fp == NULL) {
			printf("NULL 0\n");
			return;
		}

		nomeTabela = strtok(script->parametros[0], "-");
		nomeCampo  = strtok(NULL, ".");
		tipoCampo    = pegarInfoCampo(nomeTabela, nomeCampo, tabelas, qtdeTabelas, TIPO);
		tamanhoCampo = pegarInfoCampo(nomeTabela, nomeCampo, tabelas, qtdeTabelas, TAMANHO);
		offsetCampo  = pegarInfoCampo(nomeTabela, nomeCampo, tabelas, qtdeTabelas, OFFSET);

		// Campo inexistente?		
		if (tipoCampo == -1) {
			printf("NULL 0\n");
			return;		
		}
		
		// Campo procurado inexistente?
		if (pegarInfoCampo(nomeTabela, script->parametros[2], tabelas, qtdeTabelas, TIPO) == -1) {
			printf("NULL 0\n");
			return;
		}
		
		// Busca binária
		iteracoes = buscaBinaria(fp, tipoCampo, tamanhoCampo, script->parametros[1], &offset);
		fclose(fp);
		
		// Se encontrou
		if (offset != -1) {
			i = 0;
			while (strcmp(tabelas[i].nome, nomeTabela) != 0)
				i++;

			j = 0;
			while (strcmp(tabelas[i].campos[j].nome, script->parametros[2]) != 0)
				j++;

			strcat(nomeTabela, ".dat");		
			fp = fopen(nomeTabela, "rb");
			
			// tipo INT
			if (tabelas[i].campos[j].tipo == INT) {
				int *resultado = malloc(tabelas[i].campos[j].tamanho);
				fseek(fp, offset - offsetCampo + tabelas[i].campos[j].offset, SEEK_SET);
				fread(resultado, tabelas[i].campos[j].tamanho, 1, fp);
				printf("\"%d\" %d\n", *resultado, iteracoes);
				free(resultado);
				
			// tipo CHAR
			} else if (tabelas[i].campos[j].tipo == CHAR) {
				char *resultado = malloc(tabelas[i].campos[j].tamanho);
				fseek(fp, offset - offsetCampo + tabelas[i].campos[j].offset, SEEK_SET);
				fread(resultado, tabelas[i].campos[j].tamanho, 1, fp);
				printf("\"%c\" %d\n", *resultado, iteracoes);
				free(resultado);
				
			// tipo CCHAR
			} else if (tabelas[i].campos[j].tipo == CCHAR) {
				char *resultado = malloc(tabelas[i].campos[j].tamanho + 1);
				fseek(fp, offset - offsetCampo + tabelas[i].campos[j].offset, SEEK_SET);
				fread(resultado, tabelas[i].campos[j].tamanho, 1, fp);
				resultado[tabelas[i].campos[j].tamanho] = '\0';
				printf("\"%s\" %d\n", resultado, iteracoes);
				free(resultado);
			}
		
			fclose(fp);
		
		// Se NÃO encontrou
		} else {
			printf("NULL %d\n", iteracoes);
		}
	}
}

/**
 * Lê o teclado alocando dinâmicamente em uma string
 */
char *lerTeclado() {
	char *str = NULL, c;
	int count = 0;
	
	do {
		c = getchar();
		str = (char *) realloc(str, sizeof(char) * (count + 1));
		str[count++] = c != ENTER ? c : '\0';
	} while(c != ENTER);

	return str;
}

int main(int argc, char **argv) {
	char *arquivo;
	Tabela *tabelas;
	Script *scripts;
	int qtdeTabelas, qtdeScripts, i, j;

	// Leitura dos schemas
	arquivo = lerTeclado();
	tabelas = parseSchema(arquivo, &qtdeTabelas);
	free(arquivo);
	
	// Leitura dos scripts
	arquivo = lerTeclado();
	scripts = parseScript(arquivo, &qtdeScripts);
	free(arquivo);
	
	// Criação (e ordenação) dos arquivos IDX
	if (tabelas != NULL)
		criarIDX(tabelas, qtdeTabelas);

	// Execução dos scripts
	if (scripts != NULL) 
		for (i = 0; i < qtdeScripts; i++)
			executarScript(scripts+i, tabelas, qtdeTabelas);

	// Liberação de memória
	if (tabelas != NULL) {
		for (i = 0; i < qtdeTabelas; i++)
			free(tabelas[i].campos);
		free(tabelas);
	}
	if (scripts != NULL) {
		for (i = 0; i < qtdeScripts; i++) {
			for (j = 0; j < scripts[i].qtdeParametros; j++)
				free(scripts[i].parametros[j]);
			free(scripts[i].parametros);
		}
		free(scripts);
	}

	return 0;
}
