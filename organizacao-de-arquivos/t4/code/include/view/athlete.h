#ifndef _ATHLETE_VIEW_H_
#define _ATHLETE_VIEW_H_

/**
 * Imprime na tela um vetor de atletas
 */
void athlete_print_results(athlete_t **res, int count);

/**
 * Imprime na tela os dados de um atleta
 */
void athlete_print_data(athlete_t *a);

/**
 * Imprime na tela um dos arquivos (de dados ou de indice) do atleta
 */
int athlete_print_file(const char *file_name);

#endif
