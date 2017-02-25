#ifndef _DATABASE_H_
#define _DATABASE_H_

/**
 * Insere um registro em um arquivo de dados e retorna o offset
 */
int database_insert_record(const char *file_name, char **values, int *sizes, int count);

/**
 * Remove um registro em um arquivo de dados
 */
int database_delete_record(const char *file_name, int offset);

#endif
