#include <stdlib.h>
#include <stdio.h>
#include <Graph.h>

int main(int argc, char *argv[]) {

	Graph *g = new Graph(10);
	
	g->addEdge(1, 3);
	g->addEdge(4, 7);

	g->print();
	
	delete g;

	return 0;
}


