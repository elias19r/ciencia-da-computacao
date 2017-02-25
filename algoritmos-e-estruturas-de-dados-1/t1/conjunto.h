#ifndef CONJUNTO_H

#define CONJUNTO_H
#define MAX 2000001

typedef int conjunto[MAX];
typedef int elemento;

void criar(conjunto A);
void uniao(conjunto C, conjunto A, conjunto B);
void intersecao(conjunto C, conjunto A, conjunto B);
int pertence(conjunto A, elemento e);
void diferenca(conjunto C, conjunto A, conjunto B);
void inserir(conjunto A, elemento e);
void remover(conjunto A, elemento e);
void imprimir(conjunto A);
int vazio(conjunto A);
int contido(conjunto A, conjunto B);
int tamanho(conjunto A);

#endif
