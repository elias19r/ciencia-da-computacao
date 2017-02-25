/**
 * ICMC-USP
 * Implementacao de backtracking para colorir mapas
 * Elias Italiano Rodrigues, 7987251
 * Rodolfo Megiato de Lima,  7987286
 * 2013/09
 */

#include <stdlib.h>
#include <color_map.h>

int main(int argc, char **argv) {
	int method;

	if (argc < 2) exit(0);

	// Define o metodo para o algoritmo de colorir
	method = atoi(argv[1]);

	// Inicializa estruturas de dados e define os nomes das cores
	color_map_init(4, "Vermelho", "Azul", "Verde", "Amarelo");

	// Recebe entrada com os dados
	if (color_map_in())

		// Colore o mapa usando o metodo escolhido
		if (color_map_make(method))

			// Cria arquivo de saida
			color_map_out();

	// Descomente a linha abaixo para ver as informacoes de debug
	//color_map_debug();

	// Libera os dados alocados para o mapa
	color_map_free();

	return 0;
}

