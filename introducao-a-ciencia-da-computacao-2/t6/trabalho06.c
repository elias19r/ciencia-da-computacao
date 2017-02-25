/**
 * trabalho06
 * 2012/10/26
 * Elias Italiano Rodrigues, 7987251, ICC2, BCC-B, ICMC-USP
 */
#include <stdlib.h>
#include <stdio.h>

// Definindo valores numéricos para as cores
#define VERMELHO 0
#define LARANJA  1
#define AMARELO  2
#define VERDE    3
#define AZUL     4

// Definindo o tempo médio para atendimento de cada cor
#define TM_VERMELHO 50.0
#define TM_LARANJA  20.0
#define TM_AMARELO  15.0
#define TM_VERDE    8.0
#define TM_AZUL     5.0

// Definindo macro para calcular pais e filhos na fila (heap)
#define PAI(posicao)            ((posicao-1)/2)
#define FILHO_DIREITA(posicao)  ((posicao+1)*2)
#define FILHO_ESQUERDA(posicao) ((posicao*2)+1)

// Estrutura de dados para o paciente
typedef struct {
	char nome[255];
	int cor;
	int ordem;
	float TM;
	float chegada;
	float atendimento;
	float estimado;
	float espera;
} Paciente;

// Estrutura de dados para os grupos de pacientes
typedef struct {
	float T;
	int N;
	Paciente *pacientes;
} GrupoPacientes;

// Estrutura de dados para o médico
typedef struct {
	int registro;
	Paciente *paciente;
} Medico;

// Estrutura para definir uma fila (heap usando vetor)
typedef struct {
	Paciente *paciente;
} Item;

typedef struct {
	int F;
	Item *itens;
	float tempoTotal;
} Fila;

/**
 * Faz a troca de dois itens na fila
 */
void swap(Fila fila, int i, int j) {
	Item tmp = fila.itens[i];
	fila.itens[i] = fila.itens[j];
	fila.itens[j] = tmp;
}

/**
 * Função heapfy que refaz a estrutura heap da fila
 */
void heapify(Fila fila, int posicao) {
	int menor = posicao;
	int direita  = FILHO_DIREITA(posicao);
	int esquerda = FILHO_ESQUERDA(posicao);

	if (esquerda < fila.F && fila.itens[esquerda].paciente->cor < fila.itens[menor].paciente->cor)
		menor = esquerda;

	if (direita < fila.F && fila.itens[direita].paciente->cor < fila.itens[menor].paciente->cor)
		menor = direita;

	if (menor != posicao) {
		swap(fila, posicao, menor);
		heapify(fila, menor);
	}
}

/**
 * Insere um paciente na fila
 */
void inserir(Fila *fila, Item item) {
	fila->itens = (Item *) realloc(fila->itens, sizeof(Item) * (fila->F + 1));
	fila->itens[(fila->F)++] = item;
	heapify(*fila, PAI(fila->F-1));
}

/**
 * Faz o pop removendo e retornando o primeiro da fila
 */
Item pop(Fila *fila) {
	Item min = fila->itens[0];
	(fila->F)--;
	fila->itens[0] = fila->itens[fila->F];
	fila->itens = (Item *) realloc(fila->itens, sizeof(Item) * (fila->F));
	heapify(*fila, 0);
	
	return min;
}

/**
 * Libera os médicos que já terminaram seus atendimentos
 */
void liberarMedicos(float tempoTotal, Medico *medicos, int M) {
	int i;
	
	for (i = 0; i < M; i++) {
		if (medicos[i].paciente != NULL) {
			if (tempoTotal - medicos[i].paciente->atendimento >= medicos[i].paciente->TM) {
				printf("S %d %s %.1f\n", medicos[i].registro, medicos[i].paciente->nome, medicos[i].paciente->espera);
				medicos[i].paciente = NULL;
			}
		}
	}
}

/**
 * Aloca os pacientes da fila para os médicos livres
 */
void alocarMedicos(Fila *fila, Medico *medicos, int M) {
	int i;
	Item item;
	
	for (i = 0; i < M && fila->F > 0; i++) {
		if (medicos[i].paciente == NULL) {
			item = pop(fila);
			medicos[i].paciente = item.paciente;
			medicos[i].paciente->atendimento = fila->tempoTotal;
			medicos[i].paciente->espera = medicos[i].paciente->atendimento - medicos[i].paciente->chegada;
		}
	}
}

/**
 * Calcula e retonar o menor tempo médio de atendimento dos pacientes que já estão
 * sendo atendidos
 */
float calcularMenorTM(Medico *medicos, int M) {
	int i;
	float menorTM = 720.0;
	
	for (i = 0; i < M; i++) {
		if (medicos[i].paciente != NULL) {
			if (medicos[i].paciente->TM < menorTM) {
				menorTM = medicos[i].paciente->TM;
			}
		}
	}
	
	return menorTM;
}

/**
 * Percorre a fila de espera calculando o tempo estimado que cada
 * paciente terá que esperar para ser atendido.
 */
void calcularEstimado(Fila fila, int posicao, float menorTM) {
	int direita = FILHO_DIREITA(posicao);
	int esquerda = FILHO_ESQUERDA(posicao);

	if (fila.F > 0) {

		fila.itens[posicao].paciente->estimado = menorTM;
		printf("E %s %.1f\n", fila.itens[posicao].paciente->nome, fila.itens[posicao].paciente->estimado);
	
		if (esquerda < fila.F && direita < fila.F) {
			if (fila.itens[direita].paciente->cor < fila.itens[esquerda].paciente->cor)
				calcularEstimado(fila, direita, menorTM + fila.itens[posicao].paciente->TM);
			else
				calcularEstimado(fila, esquerda, menorTM + fila.itens[posicao].paciente->TM);
		} else {
			if (direita < fila.F)
				calcularEstimado(fila, direita, menorTM + fila.itens[posicao].paciente->TM);
			else if (esquerda < fila.F)
				calcularEstimado(fila, esquerda, menorTM + fila.itens[posicao].paciente->TM);
		}
	}
}

/**
 * Ordena os médicos pelo registro usando o algoritmo Quicksort
 */
void ordenarMedicos(Medico *medicos, int esquerda, int direita) {
	if (direita - esquerda > 0) {
		int i, j;
		Medico tmp;

		i = esquerda;
		for (j = esquerda+1; j <= direita; j++) {
			if (medicos[j].registro < medicos[esquerda].registro) {
				i++;
				tmp = medicos[i];
				medicos[i] = medicos[j];
				medicos[j] = tmp;
			}
		}
		tmp = medicos[esquerda];
		medicos[esquerda] = medicos[i];
		medicos[i] = tmp;

		ordenarMedicos(medicos, esquerda, i-1);
		ordenarMedicos(medicos, i+1, direita);
	}
}


int main(int argc, char **argv) {
	int M, N, G = 0, i, j, ordem = 1;
	float T;
	char arquivo[255], sintomas[18][2];
	FILE *fp;
	Medico *medicos;
	GrupoPacientes *gruposPacientes = NULL;
	Fila fila;
	Item item;
	
	// Inicializando fila
	fila.F = 0;
	fila.itens = NULL;
	fila.tempoTotal = 0.0;
	
	// Leitura e ordenação dos médicos
	scanf("%d", &M);
	medicos = (Medico *) malloc(sizeof(Medico) * M);
	for (i = 0; i < M; i++) {
		scanf("%d", &(medicos[i].registro));
		medicos[i].paciente = NULL;
	}
	ordenarMedicos(medicos, 0, M-1);

	// Leitura dos grupos de pacientes
	scanf("%s", arquivo);
	fp = fopen(arquivo, "r");
	while (!feof(fp)) {
		fscanf(fp, "%f", &T);
		if (!feof(fp)) {
			fscanf(fp, "%d", &N);
			gruposPacientes = (GrupoPacientes *) realloc(gruposPacientes, sizeof(GrupoPacientes) * (G + 1));
			gruposPacientes[G].T = T;
			gruposPacientes[G].N = N;
			gruposPacientes[G].pacientes = (Paciente *) malloc(sizeof(Paciente) * N);
			fila.tempoTotal += T;
			
			// Lendo o nome do paciente e inicializando seu atributos
			for (i = 0; i < N; i++) {
				fscanf(fp, "%s", gruposPacientes[G].pacientes[i].nome);
				gruposPacientes[G].pacientes[i].chegada = fila.tempoTotal;
				gruposPacientes[G].pacientes[i].atendimento = 0.0;
				gruposPacientes[G].pacientes[i].estimado = 0.0;
				gruposPacientes[G].pacientes[i].espera = 0.0;
				gruposPacientes[G].pacientes[i].ordem = ordem++;
				
				// Lendo os sintomas
				for (j = 0; j < 18; j++)
					fscanf(fp, "%s", sintomas[j]);

				// Obtendo a cor para o paciente
				if (sintomas[0][0] == 'S' || sintomas[1][0] == 'S' || sintomas[2][0] == 'S' ||
					sintomas[3][0] == 'S' || sintomas[4][0] == 'S') {
					gruposPacientes[G].pacientes[i].cor = VERMELHO;
					gruposPacientes[G].pacientes[i].TM  = TM_VERMELHO;
				}
				else
					if (sintomas[5][0] == 'S' || sintomas[6][0] == 'S' || sintomas[7][0] == 'S' ||
						sintomas[8][0] == 'S' || sintomas[9][0] == 'S') {
						gruposPacientes[G].pacientes[i].cor = LARANJA;
						gruposPacientes[G].pacientes[i].TM  = TM_LARANJA;
					}
					else
						if (sintomas[10][0] == 'S' || sintomas[11][0] == 'S' || sintomas[12][0] == 'S' ||
							sintomas[13][0] == 'S' || sintomas[14][0] == 'S') {
							gruposPacientes[G].pacientes[i].cor = AMARELO;
							gruposPacientes[G].pacientes[i].TM  = TM_AMARELO;
						}
						else
							if (sintomas[15][0] == 'S' || sintomas[16][0] == 'S' || sintomas[17][0] == 'S') {
								gruposPacientes[G].pacientes[i].cor = VERDE;
								gruposPacientes[G].pacientes[i].TM  = TM_VERDE;
							}
							else {
								gruposPacientes[G].pacientes[i].cor = AZUL;
								gruposPacientes[G].pacientes[i].TM  = TM_AZUL;
							}
				// Inserindo o paciente na fila
				item.paciente = gruposPacientes[G].pacientes+i;
				inserir(&fila, item);
			}
			// Executando as operações para cada grupo de pacientes
			liberarMedicos(fila.tempoTotal, medicos, M);
			alocarMedicos(&fila, medicos, M);
			calcularEstimado(fila, 0, calcularMenorTM(medicos, M));
			
			G++;
		}
	}
	fclose(fp);
	
	// Continuando a execução das operações, quando não houver mais pacientes a chegar
	while (fila.F > 0) {
		float menorTM = calcularMenorTM(medicos, M);
		fila.tempoTotal += menorTM;
		liberarMedicos(fila.tempoTotal, medicos, M);
		alocarMedicos(&fila, medicos, M);
	}
	liberarMedicos(fila.tempoTotal + calcularMenorTM(medicos, M), medicos, M);
	
	// Liberação de memória
	free(medicos);
	for (i = 0; i < G; i++)
		free(gruposPacientes[i].pacientes);
	free(gruposPacientes);

	return 0;
}
