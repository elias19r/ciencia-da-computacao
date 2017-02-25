#ifndef _DIJKSTRA_
#define _DIJKSTRA_

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

/**
 * Estruturas para um TAD de listas
 */
typedef struct list List;
typedef struct no No;

struct list {
	int qtd;
	No *first;
};

struct no {
	int value;
	No *next;
};

/**
 * Funcoes para manipulacao de listas
 */
void createList(List* list);
void addList(List* list, int id);
int minList(List *list);
int exists(List *list, int number);
void removeList(List *list, int id);
int getListQtd(List *list);

typedef struct graph Graph;
typedef struct node Node;
typedef struct edge Edge;

/**
 * Estrutura para um vertice do grafo
 */
struct node {
	int ID;
	int visited;
	Edge *edge;
	Node *next;
};

/**
 * Estrutura para uma aresta do grafo
 */
struct edge {
	int w;
	Node *node;
	Edge *next;
};

/**
 * Estrutura para grafo
 */
struct graph {
	Node *first;
	int qtd;
};

/**
 * Inicializa um grafo
 */
void createGraph(Graph *graph);

/**
 * Libera um grafo
 */
void destroyGraph(Graph *graph);

/**
 * Retorna o ponteiro para um vertice do grafo
 */
Node *getNode(Graph* graph, int ID);

/**
 * Remove aresta entre dois vertices A e B
 */
void removeEdge(Graph *graph, int A, int B);
/**
 * Remove arestas de A para B e de B para A
 */
void removeBothEdge(Graph *graph, int A, int B);

/**
 * Adiciona um novo vertice no grafo
 */
void addNode(Graph *graph, int ID);

/**
 * Remove um vertice especifico do grafo
 */
void removeNode(Graph *graph, int ID);

/**
 * Adiciona aresta de A para B com peso W
 */
void addEdge(Graph *graph, int A, int B, int W);
/**
 * Adiciona arestas de A para B e de B para A com peso W
 */
void addBothEdge(Graph *graph, int A, int B, int W);

/**
 * Implementacao do algoritmo de Dijkstra
 */
int dijkstra(Graph *graph, int init);

int min(int *array, List *list, int N);

#endif
