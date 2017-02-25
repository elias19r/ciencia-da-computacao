#ifndef _ATHLETE_CONTROLLER_H_
#define _ATHLETE_CONTROLLER_H_

/**
 * Acao "cadastrar" um novo atleta
 * Faz leitura dos dados e valida a chave primaria
 * Se tudo OK, manda inserir e atualizar os indices
 */
int athlete_action_insert();

/**
 * Acao "remover" um atleta
 * Apenas executa a remocao invocando a funcao do model
 */
int athlete_action_delete(const char *cpf);

/**
 * Acao "buscar" por um atleta
 * Recebe o campo e o valor e manda buscar
 * Se encontrou resultados, imprime
 */
int athlete_action_search(const char *field, const char *value);

/**
 * Acao "buscar" por atletas combinada com E/OU
 * Recebe os campos e os valores e manda buscar
 * Se encontrou resultados, imprime
 */
int athlete_action_search_combined(const char *field1, const char *value1, const char *op, const char *field2, const char *value2);

/**
 * Acao "dump <arquivo>"
 * Apenas imprime o arquivo na tela, invocando a funcao da view
 */
void athlete_action_dump(const char *file_name);

#endif
