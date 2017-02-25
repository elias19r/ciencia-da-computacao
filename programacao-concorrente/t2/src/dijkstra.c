#include <dijkstra.h>

/**
 * Funcoes para manipulacao de listas
 */
void createList(List* list) 
{
	list->qtd = 0;
	list->first = NULL;
}

void destroyGraph(Graph *graph)
{
	Node *node, *old_node; Edge *edge, *old_edge;
	node = graph->first;

	while (node != NULL) {
		edge = node->edge;

		while (edge != NULL) {
			old_edge = edge->next;
			free(edge);
			edge = old_edge;
		}
		old_node = node->next;
		free(node);
		node = old_node;
	}
}

void addList(List* list, int id)
{
	No *no;
	no = (No *)malloc(sizeof(No));

	no->value = id;
	no->next = list->first;

	list->qtd++;
	list->first = no;
}

int minList(List *list)
{
	int aux;

	No *no;
	no = (No *)malloc(sizeof(No));

	no = list->first;
	aux = no->value;

	while (no != NULL) {
		if (no->value < aux) {
			aux = no->value;
		}
		no = no->next;
	}

	return aux;
}

int exists(List *list, int number) 
{
	No *no;
	no = (No *)malloc(sizeof(No));

	while (no != NULL) {
		if (no->value == number) {
			return 1;
		}
		no = no->next;
	}

	return 0;
}

void removeList(List * list, int id)
{
	No *aux, *old, *removed;

	aux = list->first;

	if (aux->value == id) {
		list->qtd--;
		removed = list->first;
		list->first = aux->next;
	} else {
		old = aux;
		aux = aux->next;
		while (aux != NULL) {
			if (aux->value == id) {
				list->qtd--;
				removed = aux;
				old->next = aux->next;
			}
			old = aux;
			aux = aux->next;
		}
	}

	free(removed);

}

int getListQtd(List *list)
{
	return list->qtd;
}

/**
 * Inicializa um grafo
 */
void createGraph(Graph *graph) 
{
	graph->first = NULL;
	graph->qtd = 0;
}

/**
 * Retorna o ponteiro para um vertice do grafo
 */
Node *getNode(Graph* graph, int ID)
{
	Node *aux;
	aux = graph->first;

	while (aux != NULL) {
		if (aux->ID == ID) {
			return aux;
		}
		aux = aux->next;
	}
	return aux;
}

/**
 * Remove aresta entre dois vertices A e B
 */
void removeEdge(Graph *graph, int A, int B)
{
	Edge *aux, *old;
	Node *node;

	node = (Node*)malloc(sizeof(Node));
	aux = (Edge*)malloc(sizeof(Edge));
	old = (Edge*)malloc(sizeof(Edge));

	node = getNode(graph, A);

	aux = node->edge;

	if (aux->node->ID == B) {
		node->edge = node->edge->next;
	} else {
		old = aux;
		aux = aux->next;
		while (aux != NULL) {
			if (aux->node->ID == B) {
				old->next = aux->next;
			}
			old = aux;
			aux = aux->next;
		}	
	}	
}

/**
 * Remove arestas de A para B e de B para A
 */
void removeBothEdge(Graph *graph, int A, int B)
{
	removeEdge(graph, A, B);
	removeEdge(graph, B, A);
}

/**
 * Adiciona um novo vertice no grafo
 */
void addNode(Graph *graph, int ID)
{
	Node *node = (Node *)malloc(sizeof(Node));
	
	node->ID = ID;
	node->visited = 0;
	node->edge = NULL;
	node->next = graph->first;
	
	graph->first = node;
	graph->qtd++;
}

/**
 * Remove um vertice especifico do grafo
 */
void removeNode(Graph *graph, int ID)
{
	Edge *edge = (Edge *)malloc(sizeof(Edge));
	Node *old = (Node *)malloc(sizeof(Node));
	Node *aux = (Node *)malloc(sizeof(Node));

	aux = graph->first;

	if (graph->first->ID == ID) {
		edge = aux->edge;
		while (edge != NULL) {
			removeBothEdge(graph, aux->ID, edge->node->ID);
			edge = edge->next;
		}

		graph->first = graph->first->next;

	} else {
		old = aux;
		aux = aux->next;
		while (aux != NULL) {
			if (aux->ID == ID) {
				edge = aux->edge;
				while (edge != NULL) {
					removeBothEdge(graph, aux->ID, edge->node->ID);
					edge = edge->next;
				}
				old->next = aux->next;
				graph->qtd--;
			}
			old = aux;
			aux = aux->next;
		}	
	}
}

/**
 * Adiciona aresta de A para B com peso W
 */
void addEdge(Graph *graph, int A, int B, int W)
{
	Node *aux, *temp;
	Edge *edge;

	edge = (Edge *)malloc(sizeof(Edge));

	aux = graph->first;
	temp = graph->first;

	while (aux != NULL) {
		if (aux->ID == A) {
			break;
		}
		aux = aux->next;
	}
	while (temp != NULL) {
		if (temp->ID == B) {
			break;
		}
		temp = temp->next;
	}

	edge->w = W;
	edge->next = aux->edge;
	aux->edge = edge;
	edge->node = temp;
}

/**
 * Adiciona arestas de A para B e de B para A com peso W
 */
void addBothEdge(Graph *graph, int A, int B, int W)
{
	addEdge(graph, A, B, W);
	addEdge(graph ,B, A, W);
}

/**
 * Implementacao do algoritmo de Dijkstra
 */
int dijkstra(Graph *graph, int init)
{
	int *distancia, *predecessor;
	int N = graph->qtd;
	int i, u, alt, k;
	List list;

	Node *node;
	Edge *edge;

	distancia = (int *)malloc(sizeof(int) * N);
	predecessor = (int *)malloc(sizeof(int) * N);
	createList(&list);

	distancia[init-1] = 0;

	for (i = 0; i < N; i++) {
		if (i != init-1) {
			distancia[i] = INT_MAX;
			predecessor[i] = -1;
		}
		addList(&list, i+1);
	}

	while (getListQtd(&list)) {
		u = min(distancia, &list, N);

		removeList(&list, u+1);
		node = getNode(graph, u+1);

		edge = node->edge;
		k = edge->node->ID;

		while (edge != NULL) {
			k = edge->node->ID;
			alt = distancia[u] + edge->w;
			
			if (alt < distancia[k-1]) {
				distancia[k-1] = alt;
				predecessor[k-1] = u;
			}
			edge = edge->next;
		}
	}

	alt = distancia[0];

	for (i = 0; i < N; i++) {
		if (distancia[i] > alt) {
			alt = distancia[i];
		}
	}

	free(distancia);
	free(predecessor);

	return alt;
}

int min(int *array, List *list, int N)
{	
	No *no;
	int min, last;

	no = list->first;
	min = INT_MAX;

	while (no != NULL) {
		if (array[no->value-1] < min) {
			min = array[no->value-1];
			last = no->value-1;
		}
		no = no->next;
	}
	return last;
}

