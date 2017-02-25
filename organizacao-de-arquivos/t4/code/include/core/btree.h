#ifndef _BTREE_H_
#define _BTREE_H_

/**
 * Define macros usados pelos algoritmos da arvore-b
 */
#define FAIL    0
#define SUCCESS 1

#define NIL         -1
#define NOT_PROMOTED 0
#define PROMOTED     1
#define ERROR        2

/**
 * Define aqui a ordem (quantidade descendentes) da arvore-b
 */
#define ORDER 4

/**
 * Estrutura de dados para um elemento de uma pagina da arvore-b
 */
typedef struct elem_s {
	char key[12];
	int offset;
} elem_t;

/**
 * Estrutura de dados para uma pagina da arvore-b
 */
typedef struct page_s {
	int n;
	elem_t elems[ORDER-1];
	int child[ORDER];
} page_t;

typedef struct page_buffer_s {
	int n;
	elem_t elems[ORDER];
	int child[ORDER+1];
} page_buffer_t;

/**
 * Define o arquivo que contem a arvore-b
 */
int btree_set(const char *file_name);

/**
 * Atualiza a raiz da arvore
 */
void btree_update_root(const int offset);

/**
 * Efetua pesquisa em uma pagina
 */
int btree_page_search(const page_t *page, const char *key, int *pos);

/**
 * Carrega uma pagina do arquivo num determinado offset
 */
int btree_load_page(page_t **page, const int offset);

/**
 * Salva uma pagina da memoria no arquivo da arvore-b
 */
void btree_save_page(const int offset, const page_t *page);

/**
 * Aloca uma pagina na memoria
 */
void btree_alloc_page(page_t **page);

/**
 * Reseta valores de uma pagina
 */
void btree_reset_page(page_t *page);

/**
 * Faz uma busca na arvore-b a partir de uma dado offset de pagina (funcao recursiva)
 */
int btree_search_recursive(const int offset, const char *key, int *offset_found, int *pos_found);

/**
 * Faz uma busca na arvore-b. Um "stub" para a chamada recursiva
 */
int btree_search(const char *key, int *offset_found, int *pos_found);

/**
 * Insere (em ordem) um elemento numa pagina da arvore-b
 */
void btree_page_insert(page_t *page, const elem_t *elem, const int right_child);

/**
 * Insere um elemento na arvore-b. Um "stub" para a insercao
 */
int btree_insert(const elem_t *elem);

/**
 * Insere um elemento na arvore-b em um determinado offset
 */
int btree_insert_at(const int offset, const elem_t *elem);

/**
 * Insere um elemento na arvore-b
 */
int btree_insert_elem(const int offset, const elem_t *elem, elem_t *promoted_elem, int *promoted_right_child);

/**
 * Operacao de split em pagina da arvore-b
 */
void btree_split(const elem_t *promotion_elem, const int promotion_right_child, page_t *cur_page, elem_t *promoted_elem, int *promoted_right_child, page_t **new_page);

/**
 * Confere se uma chave existe em um dado arquivo estruturado em arvore-b
 */
int btree_exists(const char *key);

/**
 * Encerra o uso da arvore-b
 */
void btree_close();

/**
 * Imprime a arvore-b segundo a especificacao do trabalho
 * Um "stub" para a funcao recursiva
 */
void btree_print();

#endif
