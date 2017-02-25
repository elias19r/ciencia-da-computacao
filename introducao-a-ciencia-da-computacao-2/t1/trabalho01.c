/**
 * trabalho01
 * 2012/08/25
 * Elias Italiano Rodrigues, 7987251, ICC2, BCC-B, ICMC-USP
 */
#include <stdlib.h>
#include <stdio.h>

/**
 * A função lê o arquivo de audio .raw e aloca seus valores em um vetor que é
 * retornado pela função
 *
 * @return int* vetor com os valores do arquivo lido
 */
int *lerArquivo(char *nome, int *observacoes) {
	FILE *arquivo = NULL;
	int count = 0, *vetor = NULL;
	unsigned char byte;

	arquivo = fopen(nome, "rb");

	while (!feof(arquivo)) {
		fread(&byte, 1, 1, arquivo); // lê um byte
		if (!feof(arquivo)) {
			vetor = realloc(vetor, sizeof(int) * (count + 1));
			vetor[count] = (int) byte; // guarda o byte lido numa posição do vetor
			count++;
		}
	}
	*observacoes = count;

	fclose(arquivo);
	return vetor;
}

/**
 * A partir do número de observações, a função retorna o tempo do arquivo.
 * Sendo 8000 Hz a frequência do arquivo de entrada, o tempo é dado pela
 * divisão do números de observações por 8000
 *
 * @return float o tempo do arquivo
 */
float calcularTempo(int observacoes) {
	return observacoes/8000.0;
}


/**
 * Calcula as médias das faixas e substitui esses valores no vetor.
 * Então cria o novo arquivo de áudio tmp.raw com esse vetor
 * 
 * @return int* vetor com os números de observações por faixa
 */
int *novoArquivo(int *vetor, int observacoes, int n) {
	FILE *arquivo = NULL;
	unsigned char byte;
	int i, k, *observacoesFaixas = NULL, media;
	float faixa = 256.0/n, comeco = 0.0, fim;
	
	observacoesFaixas = (int *) malloc(sizeof(int) * n);

	for (k = 0; k < n; k++) {
		int *indices = NULL, countI = 0;

		if (k == (n - 1))
			fim = 256.0;
		else
			fim = (k + 1) * faixa;
		
		observacoesFaixas[k] = media = 0;

		for (i = 0; i < observacoes; i++) {
			if (vetor[i] >= comeco && vetor[i] < fim) {
				indices = (int *) realloc(indices, sizeof(int) * (countI + 1));
				indices[countI++] = i; // guarda o índice dessa posição para substituir depois
				media += vetor[i];
				observacoesFaixas[k]++;
			}
		}
		if (observacoesFaixas[k])
			media /= observacoesFaixas[k];

		for (i = 0; i < countI; i++)
			vetor[(indices[i])] = media; // faz a substituição pela média

		comeco = fim;
		
		free(indices);
	}

	arquivo = fopen("tmp.raw", "wb+");
	for (i = 0; i < observacoes; i++) {
		byte = (unsigned char) vetor[i]; // recebe um byte do vetor
		fwrite(&byte, 1, 1, arquivo); // escreve um byte no arquivo
	}
	fclose(arquivo);

	return observacoesFaixas;
}

int main(int argc, char **argv) {
	int *vetor = NULL, *observacoesFaixas = NULL;
	int i, n, observacoes;
	char nome[100];

	// Entrada dos dados
	scanf("%s", nome); while (getchar() != '\n');
	scanf("%d", &n);
	
	// Operações
	vetor = lerArquivo(nome, &observacoes);
	observacoesFaixas = novoArquivo(vetor, observacoes, n);

	// Saídas
	printf("%d\n", observacoes);
	for (i = 0; i < n; i++)
		printf("%d ", observacoesFaixas[i]);
	printf("\n%.1f\n", calcularTempo(observacoes));

	free(vetor);
	free(observacoesFaixas);

	return 0;
}
