/**
 * ICMC-USP
 * Implementacao de backtracking para colorir mapas
 * Elias Italiano Rodrigues, 7987251
 * Rodolfo Megiato de Lima,  7987286
 * 2013/09
 */

#ifndef _COLOR_MAP_H_
#define _COLOR_MAP_H_

/**
 * Inicializa a estrutura de dados para o mapa
 * amount_colors quantidade de cores
 *           ... nome de cada cor
 *
 * Exemplo:
 * 	color_map_init(4, "Yellow", "Green", "Blue", "Red");
 */
void color_map_init(int amount_colors, ...);

/**
 * Recebe a entrada, trata e armazena na estrutura de dados interna
 * 
 * A entrada contem na primeira linha o numero de regioes do mapa
 * Nas linhas seguintes, o nome de cada regiao, dois pontos, nomes de suas
 * regioes vizinhas separadas por virgula e terminado com ponto final
 * 
 * Exemplo:
 * N
 * Region 1: Region 5, Region 3, Region 7.
 * Region 2: Region 10, Region 3, Region 6.
 * ...
 * Region N: Region 12.
 */
int color_map_in();

/**
 * Realiza a coloracao do mapa de acordo com o metodo escolhido
 * Eh um "stub" para a funcao recursiva de backtracking
 * 
 * method pode ser:
 * 	0 : backtracking simples, sem poda
 * 	1 : backtracking com forward-checking
 * 	2 : backtracking com forward-checking e MRV
 * 	3 : backtracking com forward-checking, MRV e desempate por grau (degree)
 */
int color_map_make(int method);

/**
 * Gera a saida com as cores definidas para cada regiao
 * 
 * Exemplo:
 * Region 1: Yellow.
 * Region 2: Blue.
 * Region 3: Green.
 * Region 4: Green.
 * ...
 * Region N: Red.
 */
void color_map_out();

/**
 * Libera toda memoria alocada para um caso
 */
void color_map_free();

/**
 * Imprime na tela todas as informacoes lidas para conferir se esta tudo OK
 */
void color_map_debug();

#endif
