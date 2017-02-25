/**
 * trabalho03
 * 2012/09/19
 * Elias Italiano Rodrigues, 7987251, ICC2, BCC-B, ICMC-USP
 */
#include <stdlib.h>
#include <stdio.h>

/**
 * Busca binária iterativa em um arquivo
 * @return int 0 para não encontrou, 1 para encontrou
 */
int buscaBinaria(FILE *fp, int chave, int *cmp) {
	long int tamanho;
	int comeco, meio, fim, chaveLida;

	*cmp = 0; // inicializa o contador de comparações em zero
	
	// Quantidade de chaves que o arquivo possui
	fseek(fp, 0, SEEK_END);
	tamanho = ftell(fp);
	tamanho /= sizeof(int);
	
	// Define comeco e fim
	comeco = 0;
	fim = (int) (tamanho-1);
	
	while (comeco <= fim) {
		meio = (comeco + fim) / 2;
		
		fseek(fp, (meio) * sizeof(int), SEEK_SET);
		fread(&chaveLida, sizeof(int), 1, fp);
		
		(*cmp)++; // incrementa o contador de comparações
		if (chave == chaveLida)
			return 1;
		
		if (chave < chaveLida)
			comeco = meio+1;
		else
			fim = meio-1;
	}
	
	return 0;
}

/**
 * Busca sequencial em um arquivo
 * @return int 0 para não encontrou, 1 para encontrou
 */
int buscaSequencial(FILE *fp, int chave, int *cmp) {
	int chaveLida;
	
	*cmp = 0; // inicializa o contador de comparações em zero
	fseek(fp, 0, SEEK_SET);

	while (!feof(fp)) {
		fread(&chaveLida, sizeof(int), 1, fp);
		if (!feof(fp)) {
			(*cmp)++; // incrementa o contador de comparações
			if (chave == chaveLida)
				return 1;
		}
	}
	
	return 0;
}

/**
 * Função para o método 1
 * @return int total de comparações feitas
 */
int metodo1(FILE *fp_principal, FILE *fp_tmp, int chave) {
	int cmp1, cmp2 = 0;
	
	if (!buscaSequencial(fp_principal, chave, &cmp1))
		buscaSequencial(fp_tmp, chave, &cmp2);
		
	return cmp1 + cmp2;
}

/**
 * Função para o método 2
 * @return int total de comparações feitas
 */
int metodo2(FILE *fp_principal, FILE *fp_tmp, int chave) {
	int cmp1, cmp2 = 0;
	
	if (!buscaBinaria(fp_principal, chave, &cmp1))
		buscaSequencial(fp_tmp, chave, &cmp2);
		
	return cmp1 + cmp2;
}

int main(int argc, char **argv) {
	char principal[100];
	char tmp[] = "tmp.dat";
	int K, X, chave, i;
	FILE *fp_principal = NULL, *fp_tmp = NULL;
	
	scanf("%s", principal); // lê o nome do arquivo
	
	// Abertura dos arquivos
	fp_principal = fopen(principal, "rb");
	fp_tmp = fopen(tmp, "wb+");
	
	// Inserções
	scanf("%d", &K);
	for (i = 0; i < K; i++) {
		scanf("%d", &chave);
		fwrite(&chave, sizeof(int), 1, fp_tmp);
	}
	
	// Aplicações dos Métodos e impressão dos resultados
	scanf("%d", &X);
	for (i = 0; i < X; i++) {
		scanf("%d", &chave);
		printf("%d ",  metodo1(fp_principal, fp_tmp, chave));
		printf("%d\n", metodo2(fp_principal, fp_tmp, chave));
	}

	// Fechamento dos arquivos	
	fclose(fp_principal);
	fclose(fp_tmp);

	return 0;
}
