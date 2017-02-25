class Edge;

#define WHITE 0
#define GRAY  1
#define BLACK 2

#define INF -1
#define NIL NULL

/**
 * Classe do vértice
 */
class Vertice {
	public:
		/**
		 * Construtor
		 */
		Vertice(const char *label = NULL, void *info = NULL) {
			this->index = 0;
			this->label = NULL;
			this->info = info;
			this->first = NULL;
			this->last = NULL;
			this->prev = NULL;
			this->next = NULL;
			this->edges = 0;
			this->edgesIn = 0;
			this->edgesOut = 0;
			
			if (label != NULL)
				this->setLabel(label);
				
			this->color = WHITE;
			this->distance = INF;
			this->predecessorsList = NIL;
			this->predecessorsCount = 0;
		}
		
		/**
		 * Destrutor
		 */
		~Vertice() {
			if (this->label != NULL)
				free(this->label);
		}
		
		int index;     // índice associado ao vértice
		char *label;   // rótulo opcional
		void *info;    // informação anexada ao vértice
		Edge *first;   // ponteiro para o primeiro nó da lista de arestas
		Edge *last;    // ponteiro para o último nó da lista de arestas
		Vertice *prev; // ponteiro para o vértice anterior
		Vertice *next; // ponteiro para o próximo vértice
		int edges;     // quantidade total de arestas (in + out)
		int edgesIn;   // quantidade de arestas que chegam neste vértice
		int edgesOut;  // quantidade de arestas que saem deste vértice
		
		int color;
		int distance;
		Vertice **predecessorsList;
		int predecessorsCount;
		
		/**
		 * Define um rótulo para o vértice
		 */
		void setLabel(const char *label) {
			this->label = (char *) realloc(this->label, sizeof(char) * (strlen(label) + 1));
			strcpy(this->label, label);
		}
		
		void predecessors(Vertice *v) {
			if (v == NIL) {
				if (this->predecessorsList != NULL)
					free(this->predecessorsList);
				this->predecessorsList = NULL;
			} else {
				this->predecessorsList = (Vertice **) realloc(this->predecessorsList, sizeof(Vertice *) * (this->predecessorsCount + 1));
				this->predecessorsList[this->predecessorsCount++] = v;
			}
		}
};
