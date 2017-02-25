/**
 * trabalho02
 * 2012/09/11
 * Elias Italiano Rodrigues, 7987251, ICC2, BCC-B, ICMC-USP
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <complex.h>

// Estrutura para um Coeficiente
typedef struct {
	int k;
	double complex valor;
	double magnitude;
} Coeficiente;

/**
 * Função dada pelo enunciado do trabalho apenas com uma pequena
 * alteração devido à estrutura Coeficiente utilizada neste programa
 */
unsigned char *inverseFourier(Coeficiente *coefficients, int N) {
	// N é o total de coeficientes
	// M_PI é uma macro que define o valor do PI (math.h)
	double complex *audio = (double complex *) calloc(N, sizeof(double complex));
	unsigned char *audio8ubits = (unsigned char *) calloc(N, sizeof(unsigned char));
	int n, k;
	
	for (n = 0; n < N; n++) {
		for (k = 0; k < N; k++)
			audio[n] = audio[n] + coefficients[k].valor * cexp((2.0 * M_PI * (((k+1) * n * 1.0) / (N * 1.0))) * _Complex_I);

		audio[n] = audio[n] / (N * 1.0);
		audio8ubits[n] = (int) __real__ audio[n];
	}
	
	free(audio);
	return audio8ubits;
}

/**
 * A função lê o arquivo de audio .raw e aloca seus valores em um vetor que é
 * retornado pela função (é a mesma função do trabalho01)
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
			vetor = (int *) realloc(vetor, sizeof(int) * (count + 1));
			vetor[count] = (int) byte; // guarda o byte lido numa posição do vetor
			count++;
		}
	}
	*observacoes = count;

	fclose(arquivo);
	return vetor;
}

/**
 * A função recebe o vetor com as observações do arquivo .raw e calcula
 * os coeficientes da Transformada de Fourier
 *
 * @return Coeficiente* com os coeficientes calculados
 */
Coeficiente *transformadaFourier(int *vetor, int N, int *valoresMenorIgualZero) {
	Coeficiente *coeficientes = NULL;
	double complex somatorio;
	double real, imag;
	int k, n;
	
	*valoresMenorIgualZero = 0;
	
	for (k = 1; k <= N; k++) {
		coeficientes = (Coeficiente *) realloc(coeficientes, sizeof(Coeficiente) * k);

		somatorio = 0;
		for (n = 0; n < N; n++)
			somatorio += vetor[n] * cexp(-2.0 * _Complex_I * M_PI * ((double) k)/((double) N) * ((double) n));

		real = __real__ somatorio;
		imag = __imag__ somatorio;
		
		coeficientes[k-1].k = k;
		coeficientes[k-1].valor = somatorio;
		coeficientes[k-1].magnitude = sqrt(real*real + imag*imag);
		
		if (real <= 0 && imag <= 0)
			(*valoresMenorIgualZero)++;
	}
	
	return coeficientes;
}

/**
 * Ordena os coeficientes de acordo com suas magnitudes em ordem decrescente
 * utilizando o algoritmo do Insertion Sort
 */ 
void ordenarMagnitudes(Coeficiente *coeficientes, int N) {
	int i, j;
	Coeficiente tmp;
	
	for (i = 1; i < N; i++) {
		j = i-1;
		while (j >= 0 && coeficientes[j+1].magnitude > coeficientes[j].magnitude) {
			tmp = coeficientes[j];
			coeficientes[j] = coeficientes[j+1];
			coeficientes[j+1] = tmp;
			j--;
		}
	}
}

/**
 * Zera os valores dos coeficientes das posições maiores do que T
 */
void zerarCoeficientes(Coeficiente *coeficientes, int N, int T) {
	int i;
	
	for (i = T; i < N; i++) {
		coeficientes[i].valor = 0;
		coeficientes[i].magnitude = 0;
	}
}

/**
 * Ordena os coeficientes de acordo com seus índices k em ordem crescente
 * utilizando o algoritmo do Insertion Sort
 */
void ordenarCoeficientes(Coeficiente *coeficientes, int N) {
	int i, j;
	Coeficiente tmp;
	
	for (i = 1; i < N; i++) {
		j = i-1;
		while (j >= 0 && coeficientes[j+1].k < coeficientes[j].k) {
			tmp = coeficientes[j];
			coeficientes[j] = coeficientes[j+1];
			coeficientes[j+1] = tmp;
			j--;
		}
	}
}

/**
 * Cria o novo arquivo de áudio tmp.raw com o vetor retornado da Transformada
 * Inversa de Fourier
 */
void novoArquivo(unsigned char *audio, int N) {
	FILE *arquivo = NULL;
	unsigned char byte;
	int i;
	
	arquivo = fopen("tmp.raw", "wb+");
	for (i = 0; i < N; i++) {
		byte = (unsigned char) audio[i]; // recebe um byte do vetor
		fwrite(&byte, 1, 1, arquivo); // escreve um byte no arquivo
	}
	fclose(arquivo);
}

int main(int argc, char **argv) {
	char nome[100];
	unsigned char *audio = NULL;
	int i, T, N, valoresMenorIgualZero, *vetor = NULL;
	Coeficiente *coeficientes = NULL;

	// Entradas	
	scanf("%s", nome);
	scanf("%d", &T);
	
	// Operações e Saídas
	vetor = lerArquivo(nome, &N);
	coeficientes = transformadaFourier(vetor, N, &valoresMenorIgualZero);
	
	printf("%d\n", N);
	printf("%d\n", valoresMenorIgualZero);
	
	ordenarMagnitudes(coeficientes, N);
	
	for (i = 0; i < T; i++)
		printf("%d ", (int) coeficientes[i].magnitude);
	printf("\n");
	
	zerarCoeficientes(coeficientes, N, T);
	ordenarCoeficientes(coeficientes, N);
	audio = inverseFourier(coeficientes, N);
	novoArquivo(audio, N);
	
	// Liberação de memória
	free(vetor);
	free(audio);
	free(coeficientes);

	return 0;
}
