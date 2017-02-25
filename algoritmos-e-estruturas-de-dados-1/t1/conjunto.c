#include <stdio.h>
#include "conjunto.h"

void criar(conjunto A) {
	int i;

	for (i = 0; i < MAX; i++)
		A[i] = 0;
}

void uniao(conjunto C, conjunto A, conjunto B) {
	int i;

	for (i = 0; i < MAX; i++)
		C[i] = A[i] || B[i];
}

void intersecao(conjunto C, conjunto A, conjunto B) {
	int i;

	for (i = 0; i < MAX; i++)
		C[i] = A[i] && B[i];
}

int pertence(conjunto A, elemento e) {
	return A[e];
}

void diferenca(conjunto C, conjunto A, conjunto B) {
	int i;

	for (i = 0; i < MAX; i++)
		C[i] = (A[i] && !B[i]);
}

void inserir(conjunto A, elemento e) {
	A[e] = 1;
}

void remover(conjunto A, elemento e) {
	A[e] = 0;
}

void imprimir(conjunto A) {
	int i;

	for (i = 0; i < MAX; i++)
		if (pertence(A, i))
			printf("%d ", i);
	
	printf("\n");
}

int vazio(conjunto A) {
	int i;

	for (i = 0; i < MAX; i++)
		if (A[i])
			return 0;
			
	return 1;
}

int contido(conjunto A, conjunto B) {
	int i;
	
	for (i = 0; i < MAX; i++)
		if (!pertence(B, A[i]))
			return 0;
			
	return 1;
}

int tamanho(conjunto A) {
	int i;
	int count = 0;
	
	for (i = 0; i < MAX; i++)
		if (A[i])
			count++;
			
	return count;
}
