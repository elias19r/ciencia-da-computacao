/*
	TO-DO:
		- remover aresta
		- remover vértice
		- retornar vértices adjacentes
		- obter transposta do grafo
		- BFS
		- DFS
		- Implementar TAD lista para auxiliar a BFS e DFS
		- Diferencia postOrder e preOrder nas buscas
*/

/**
 * Implementação de grafo em C++ usando o modelo proposto por Tanenbaum
 * 2013/04/16
 * Elias Italiano Rodrigues, 7987251, Grafos, BCC-A, ICMC-USP
 */

#ifndef _GRAPH_H_
#define _GRAPH_H_

// Constantes para as travessias BFS e DFS no grafo
#define WHITE 0
#define GRAY  1
#define BLACK 2
#define INF  -1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

template <class T>
class Item<T> {
	public:
		Item<T> *prev;
		Item<T> *next;
		T data;

		Item(T data) {
			this->prev = NULL;
			this->next = NULL;
			this->data = data;
		}

		virtual ~Item() {}
};

template <class T>
class List {
	private:
		int count;
		Item<T> *first;
		Item<T> *last;
		
		void initialize() {
			this->count = 0;
			this->first = NULL;
			this->last = NULL;
		}
	public:	
		List() {
			this->initialize();
		}
	
		virtual ~List() {
			if (this->count > 0) {
				Item<T> *i = this->first;
				do {
					Item<T> *tmp = i;
					i = i->next;
					delete tmp;
				} while (i != NULL);
			}
		}
	
		void add(Item<T> item) {
			if (this->count == 0) {
				this->first = new Item<T>(item);
				this->last = first;
				this->count++;

			} else if (this->count == 1) {
				this->last = new Item<T>(item);
				this->first->next = this->last;
				this->last->prev = this->first;
				this->count++;

			} else {
				Item<T> *i = new Item<T>(item);
				this->last->next = i;
				i->prev = this->last;
				this->last = i;
				this->count++;
			}
		}
	
		void addBegin(Item<T> item) {
			if (this->count == 0) {
				this->first = new Item<T>(item);
				this->last = first;
				this->count++;

			} else {
				Item<T> *i = new Item<T>(item);
				i->next = this->first;
				this->first->prev = i;
				this->first = i;
				this->count++;
			}
		}
	
		T pop() {
			if (this->count > 0) {
				Item<T> *i = this->first;
				this->first = this->first->next;
				this->count--;
				
				T ret = i->data;
				delete i;
				return ret;
			}
			return NULL;
		}
		
		int empty() {
			return this->count;
		}
	
};

class Vertice;
class Edge;

/**
 * Classe do vértice
 */
class Vertice : public Item {
	private:
		/**
		 * Inicializa os valores do objeto
		 */
		void initialize() {
			this->index = 0;
			this->label = NULL;
			this->first = NULL;
			this->last = NULL;
			this->prev = NULL;
			this->next = NULL;
			this->edgesIn = 0;
			this->edgesOut = 0;

			this->color = WHITE;
			this->distance = INF;
			this->predecessorsList = NULL;
			this->predecessorsCount = 0;
		}
	
	public:
		int index;     // índice associado ao vértice
		char *label;   // rótulo opcional
		Edge *first;   // ponteiro para o primeiro nó da lista de arestas
		Edge *last;    // ponteiro para o último nó da lista de arestas
		Vertice *prev; // ponteiro para o vértice anterior
		Vertice *next; // ponteiro para o próximo vértice
		int edgesIn;   // quantidade de arestas que chegam neste vértice
		int edgesOut;  // quantidade de arestas que saem deste vértice
		
		// Varíaves para BFS e DFS
		int color;
		int distance;
		Vertice **predecessorsList;
		int predecessorsCount;

		/**
		 * Construtor
		 */
		Vertice(int index, const char *label = NULL) {
			this->initialize();
			
			this->index = index;
			if (label != NULL)
				this->setLabel(label);
		}
		
		/**
		 * Destrutor
		 */
		virtual ~Vertice() {
			this->predecessors(NULL);

			if (this->label != NULL)
				free(this->label);
		}
		
		/**
		 * Define um rótulo para o vértice
		 */
		void setLabel(const char *label) {
			this->label = (char *) realloc(this->label, sizeof(char) * (strlen(label) + 1));
			strcpy(this->label, label);
		}
		
		/**
		 * Adiciona ou zera a lista de vértices predecessores a este em uma busca BFS ou DFS
		 */
		void predecessors(Vertice *v) {
			if (v == NULL) {
				if (this->predecessorsList != NULL) {
					free(this->predecessorsList);
					this->predecessorsList = NULL;
				}
			} else {
				this->predecessorsList = (Vertice **) realloc(this->predecessorsList, sizeof(Vertice *) * (this->predecessorsCount + 1));
				this->predecessorsList[this->predecessorsCount++] = v;
			}
		}
		
		void printPredecessors() {
			printf("Predecessors of vertice %d: ", this->index);
			for (int i = 0; i < this->predecessorsCount; i++) {
				printf("%d ", this->predecessorsList[i]->index);
			}
			printf("\n");
		}
};

/**
 * Classe da aresta
 */
class Edge : public Item {
	private:
		/**
		 * Inicializa os valores do objeto
		 */
		void initialize() {
			this->vertice = NULL;
			this->weight = 0.0;
			this->prev = NULL;
			this->next = NULL;
		}

	public:
		Vertice *vertice; // vértice que esta aresta aponta
		double weight;    // peso associado à aresta
		Edge *prev;       // ponteiro para a aresta anterior
		Edge *next;       // ponteiro para a próxima aresta
		
		/**
		 * Construtor
		 */
		Edge(Vertice *v, double weight = 0.0) {
			this->initialize();
			
			this->vertice = v;
			this->weight = weight;
		}
		
		/**
		 * Destrutor
		 */
		virtual ~Edge() {}
};

/**
 * Classe Graph
 */
class Graph : public List<Vertice *> {
	private:
		Vertice *first;        // ponteiro para o primeiro nó da lista de vértices
		Vertice *last;         // ponteiro para o último nó da lista de vértices
		int vertices;          // quantidade total de vértices neste grafo
		int edges;             // quantidade total de arestas neste grafo
		int error;             // código do erro ocorrido (ainda não implementado...)
		List<Vertice *> *list; // fila/pilha de vértices auxiliar usada para BFS e DFS

		/**
		 * Inicializa os valores do objeto
		 */
		void initialize() {
			this->first = NULL;
			this->last = NULL;
			this->vertices = 0;
			this->edges = 0;
			this->error = 0;
			this->list = NULL;
		}
		
		/**
		 * Retorna a quantidade de dígitos de um número inteiro
		 */
		int numberOfDigits(int num) {
			int digits = 0;

			if (num < 0)
				digits = 1; // inclui sinal de negativo

			while (num != 0) {
				num /= 10;
				digits++;
			}
			return digits;
		}
		
	public:
		/**
		 * Construtor
		 * Recebe o número de vértices como parâmetro
		 */
		Graph() {
			this->initialize();
		}

		/**
		 * Destrutor
		 * Libera toda a memória alocada para o grafo
		 */
		virtual ~Graph() {
			// Libera os vértices
			if (this->vertices > 0) {
				Vertice *v = this->first;
				Vertice *tmp;
				do {
					// Libera as arestas do vértice
					if (v->edgesOut > 0) {
						Edge *e = v->first;
						Edge *tmp;
						do {
							tmp = e;
							e = e->next;							
							delete tmp;
						} while (e != NULL);
					}
					tmp = v;
					v = v->next;
					delete tmp;
				} while (v != NULL);
			}
		}

		/**
		 * Adiciona um novo vértice no grafo
		 */
		void add(int index, const char *label = NULL) {
			if (this->vertices == 0) {
				this->first = new Vertice(index, label);
				this->last = first;
				this->vertices++;

			} else if (this->vertices == 1) {
				this->last = new Vertice(index, label);
				this->first->next = this->last;
				this->last->prev = this->first;
				this->vertices++;

			} else {
				Vertice *v = new Vertice(index, label);
				this->last->next = v;
				v->prev = this->last;
				this->last = v;
				this->vertices++;
			}
		}

		/**
		 * Confere se um vértice existe
		 */
		bool exist(int index) {
			if (this->vertices > 0) {
				Vertice *v = this->first;
				do {
					if (v->index == index)
						return true;
					else
						v = v->next;
				} while (v != NULL);
			}
			return false;
		}

		/**
		 * Confere se um vértice existe (passado um ponteiro)
		 */
		bool exist(Vertice *v = NULL) {
			if (v != NULL && this->vertices > 0) {
				Vertice *p = this->first;
				do {
					if (p == v)
						return true;
					else
						p = p->next;
				} while (p != NULL);
			}
			return false;
		}

		/**
		 * Retorna o ponteiro para um vértice
		 */
		Vertice *get(int index) {
			if (this->vertices > 0) {
				Vertice *v = this->first;
				do {
					if (v->index == index)
						return v;
					else
						v = v->next;
				} while (v != NULL);
			}
			return NULL;
		}

		/**
		 * Adiciona uma nova aresta no grafo
		 */
		bool addEdge(int a, int b, double weight = 0.0) {
			Vertice *vA, *vB;

			vA = this->get(a);
			vB = this->get(b);
			
			return this->addEdge(vA, vB, weight);
		}
		
		bool addEdge(Vertice *vA = NULL, Vertice *vB = NULL, double weight = 0.0) {
			if (vA != NULL && vB != NULL && this->vertices > 0) {
				this->edges++;
				vB->edgesIn++;

				if (vA->edgesOut == 0) {
					vA->first = new Edge(vB, weight);
					vA->edgesOut++;

					return true;

				} else if (vA->edgesOut == 1) {
					vA->last = new Edge(vB, weight);
					vA->last->prev = vA->first;
					vA->first->next = vA->last;
					vA->edgesOut++;

					return true;

				} else {
					Edge *e = new Edge(vB, weight);
					vA->last->next = e;
					e->prev = vA->last;
					vA->last = e;
					vA->edgesOut++;

					return true;
				}
			}
			return false;
		}

		/**
		 * Confere se uma aresta existe
		 */
		bool existEdge(int a, int b) {
			Vertice *vA, *vB;

			vA = this->get(a);
			vB = this->get(b);

			return this->existEdge(vA, vB);
		}

		bool existEdge(Vertice *vA = NULL, Vertice *vB = NULL) {
			if (vA != NULL && vB != NULL && this->vertices > 0) {
				// Confere se existe de vA para vB
				if (vA->edgesOut > 0) {
					Edge *e = vA->first;
					do {
						if (e->vertice == vB)
							return true;
						else
							e = e->next;
					} while (e != NULL);
				}
			}
			return false;
		}

		/**
		 * Retorna o ponteiro de uma aresta
		 */
		Edge *getEdge(int a, int b) {
			Vertice *vA, *vB;

			vA = this->get(a);
			vB = this->get(b);
			
			return this->getEdge(vA, vB);
		}
		
		Edge *getEdge(Vertice *vA = NULL, Vertice *vB = NULL) {
			if (vA != NULL && vB != NULL && this->vertices > 0) {
				// Confere se existe de vA para vB
				if (vA->edgesOut > 0) {
					Edge *e = vA->first;
					do {
						if (e->vertice == vB)
							return e;
						else
							e = e->next;
					} while (e != NULL);
				}
			}
			return NULL;
		}
		
		/**
		 * Remove uma aresta
		 */
		bool removeEdge(int a, int b) {
			Vertice *vA, *vB;

			vA = this->get(a);
			vB = this->get(b);

			return this->removeEdge(vA, vB);
		}

		bool removeEdge(Vertice *vA = NULL, Vertice *vB = NULL) {
			if (vA != NULL && vB != NULL && this->vertices > 0) {
				// Confere se existe de vA para vB
				if (vA->edgesOut > 0) {
					Edge *e = vA->first;
					do {
						// Se achou o vértice
						if (e->vertice == vB) {
							// A FAZER...
							return true;
						}
						else
							e = e->next;
					} while (e != NULL);
				}
			}
			return false;
		}

		/**
		 * Atualiza o peso de uma aresta (definida pelos índices dos vértices)
		 */
		bool updateWeight(int a, int b, double weight) {
			Vertice *vA, *vB;
			
			vA = this->get(a);
			vB = this->get(b);
			
			return this->updateWeight(vA, vB);
		}
		
		bool updateWeight(Vertice *vA = NULL, Vertice *vB = NULL, double weight = 0.0) {
			Edge *e = this->getEdge(vA, vB);

			if (e != NULL) {
				e->weight = weight;
				return true;
			}
			return false;
		}
		
		/**
		 * Retorna a quantidade de vértices no grafo
		 */
		int amountOfVertices() {
			return this->vertices;
		}

		/**
		 * Retorna a quantidade de arestas no grafo
		 */
		int amountOfEdges() {
			return this->edges;
		}

		/**
		 * Imprime o grafo
		 */
		void print(char dot = 'X') {
			char *str = this->toStr(dot);
			if (str != NULL) {
				printf("%s\n", str);
				free(str);
			}
		}

		/**
		 * Retorna um vetor de inteiros com os índices do vértices do grafo
		 */
		int *getAllVerticesIndexes() {
			if (this->vertices > 0) {
				int *all = (int *) malloc(sizeof(int) * this->vertices);
				Vertice *v = this->first;
				int i = 0;

				do {
					all[i++] = v->index;
					v = v->next;
				} while (v != NULL);

				return all;
			}
			return NULL;
		}

		/**
		 * Retorna um vetor de ponteiros para os vértices do grafo
		 */
		Vertice **getAllVertices() {
			if (this->vertices > 0) {
				Vertice **all = (Vertice **) malloc(sizeof(Vertice *) * this->vertices);
				Vertice *v = this->first;
				int i = 0;

				do {
					all[i++] = v;
					v = v->next;

				} while (v != NULL);

				return all;
			}
			return NULL;
		}

		Vertice **getAdjacentes(int index) {
			Vertice *v = this->get(index);
			return this->getAdjacentes(v);
		}
		
		Vertice **getAdjacentes(Vertice *v = NULL) {
			if (v != NULL && this->vertices > 0) {
				if (v->edgesOut > 0) {
					Vertice **all = (Vertice **) malloc(sizeof(Vertice *) * v->edgesOut);
					Edge *e = v->first;
					int i = 0;
					
					do {
						all[i++] = e->vertice;
						e = e->next;
					} while (e != NULL);
					
					return all;
				}
			}
			return NULL;
		}

		bool BFS(int start, bool postOrder = false) {
			Vertice *v = this->get(start);
			
			return this->BFS(v, postOrder);
		}

		bool BFS(Vertice *start = NULL, bool postOrder = false) {
			if (start != NULL && this->vertices > 0) {
				Vertice *v;
				Vertice **vAdj;
				
				// Reseta cor, distância e predecessores de todos os vértices
				v = this->first;
				do {
					v->color = WHITE;
					v->distance = INF;
					v->predecessors(NULL);

					v = v->next;
				} while (v != NULL);
				
				// Coloca o vértice start na lista
				start->color = GRAY;
				start->distance = 0;
				this->list = new List<Vertice *>();
				this->list->add(start);
				
				// Laço onde processa e enfila os vértices adjacentes começando pelo inicial (start)
				while (!this->list->empty()) {
					v = this->list->pop();
					vAdj = getAdjacentes(v);
					
					for (int i = 0, n = v->edgesOut; i < n; i++) {
						if (vAdj[i]->color == WHITE) {
							vAdj[i]->color = GRAY;
							vAdj[i]->distance = v->distance + 1;
							vAdj[i]->predecessors(v);
							this->list->add(vAdj[i]);
						}
					}
					v->color = BLACK;
					free(vAdj);
				}
				delete this->list;
				return true;
			}
			return false;
		}
		
		bool DFS(int start, bool postOrder = false) {
			Vertice *v = this->get(start);
			
			return this->DFS(v, postOrder);
		}

		bool DFS(Vertice *start = NULL, bool postOrder = false) {
			// ... A FAZER
			return false;
		}

		/**
		 * Retorna uma string com o grafo representado em uma tabela
		 */
		char *toStr(char dot = 'X') {
			if (this->vertices > 0) {
				char *str = NULL;
				char formatNum[5];   // formatação usada para os números dos índices do vértices
				char formatSpace[5]; // formatação usada para o espaço em branco
				int count = 1; // contador usado para alocar a string
				int at = 0;    // variável auxiliar usada para apontar o fim da string
				int i, j;      // iteradores

				int *all = this->getAllVerticesIndexes();
				int amount = this->amountOfVertices();

				// Recebe a quantidade de dígitos do possível maior índice
				int digits = this->numberOfDigits(amount);

				// Define as formatações com base na quantidade de dígitos do possível maior índice
				sprintf(formatNum, "%%%dd ", digits);
				sprintf(formatSpace, "%%%dc ", digits);

				// Imprime um espaço em branco
				count += digits+1;
				str = (char *) realloc(str, sizeof(char) * count);
				at += sprintf(str+at, formatSpace, ' ');

				// Imprime os índices da parte de cima da tabela
				for (i = 0; i < amount; i++) {
					count += digits+1;
					str = (char *) realloc(str, sizeof(char) * count);
					at += sprintf(str+at, formatNum, all[i]);
				}

				// Quebra de linha
				count += 1;
				str = (char *) realloc(str, sizeof(char) * count);
				at += sprintf(str+at, "\n");

				// Imprime as linhas da tabela
				for (i = 0; i < amount; i++) {

					// Índice da coluna esquerda da tabela
					count += digits+1;
					str = (char *) realloc(str, sizeof(char) * count);
					at += sprintf(str+at, formatNum, all[i]);

					for (j = 0; j < amount; j++) {
						count += digits+1;
						str = (char *) realloc(str, sizeof(char) * count);

						// Se existe aresta, imprime um caractere dot
						// Se não, imprime apenas um espaço "em branco"
						if (this->existEdge(all[i], all[j]))
							at += sprintf(str+at, formatSpace, dot);
						else
							at += sprintf(str+at, formatSpace, '.');
					}

					// Quebra de linha
					count += 1;
					str = (char *) realloc(str, sizeof(char) * count);
					at += sprintf(str+at, "\n");
				}
				free(all);

				return str;
			}
			return NULL;
		}
};

#endif
