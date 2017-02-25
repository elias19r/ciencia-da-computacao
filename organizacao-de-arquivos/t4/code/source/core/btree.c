#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../include/core/btree.h"

FILE *btree_fp = NULL;
int btree_root = NIL;

/**
 * Define o arquivo que contem a arvore-b
 */
int btree_set(const char *file_name) {
	// Tenta abrir em r+
	btree_fp = fopen(file_name, "r+");

	// Se falhou, abre em w+
	if (btree_fp == NULL) {
		btree_fp = fopen(file_name, "w+");

		if (btree_fp == NULL)
			return FAIL;
	}
	
	// Le o offset da pagina raiz
	if (fread(&btree_root, sizeof(int), 1, btree_fp) < 1)
		btree_update_root(NIL);
	
	return SUCCESS;
}

/**
 * Atualiza a raiz da arvore
 */
void btree_update_root(const int offset) {
	fseek(btree_fp, 0, SEEK_SET);
	fwrite(&offset, sizeof(int), 1, btree_fp);
	btree_root = offset;
}

/**
 * Efetua pesquisa em uma pagina
 */
int btree_page_search(const page_t *page, const char *key, int *pos) {
	int i;

	// Pesquisa sequencialmente no arquivo (ordem e' pequena)
	for (i = 0; i < page->n; i++) {
		if (strcmp(key, page->elems[i].key) == 0) {
			*pos = i;
			return SUCCESS;

		} else if (strcmp(key, page->elems[i].key) < 0)
			break;
	}

	// Caso nao encontrou, retorna a posicao que deveria estar (i)
	*pos = i;
	return FAIL;
}

/**
 * Carrega uma pagina do arquivo num determinado offset
 */
int btree_load_page(page_t **page, const int offset) {
	*page = (page_t *) malloc(sizeof(page_t));

	fseek(btree_fp, offset, SEEK_SET);
	if (fread(*page, sizeof(page_t), 1, btree_fp) < 1) {
		if (*page != NULL) {
			free(*page);
			*page = NULL;
		}

		return FAIL;
	}

	return SUCCESS;
}

/**
 * Salva uma pagina da memoria no arquivo da arvore-b
 */
void btree_save_page(const int offset, const page_t *page) {
	// Se um offset NIL for passado, posiciona no fim do arquivo
	// 	senao, posiciona no offset dado
	if (offset == NIL)
		fseek(btree_fp, 0, SEEK_END);
	else
		fseek(btree_fp, offset, SEEK_SET);

	fwrite(page, sizeof(page_t), 1, btree_fp);
}

/**
 * Aloca uma pagina na memoria
 */
void btree_alloc_page(page_t **page) {
	*page = (page_t *) malloc(sizeof(page_t));
	btree_reset_page(*page);
}

/**
 * Reseta valores de uma pagina
 */
void btree_reset_page(page_t *page) {
	int i;

	for (i = 0; i < ORDER-1; i++) {
		page->elems[i].key[0] = '\0';
		page->elems[i].offset = NIL;
		page->child[i] = NIL;
	}
	page->child[i] = NIL;
	page->n = 0;
}

/**
 * Reseta valores de uma pagina buffer
 */
static void btree_reset_page_buffer(page_buffer_t *page_buf) {
	int i;

	for (i = 0; i < ORDER; i++) {
		page_buf->elems[i].key[0] = '\0';
		page_buf->elems[i].offset = NIL;
		page_buf->child[i] = NIL;
	}
	page_buf->child[i] = NIL;
	page_buf->n = 0;
}

/**
 * Faz uma busca na arvore-b a partir de uma dado offset de pagina (funcao recursiva)
 */
int btree_search_recursive(const int offset, const char *key, int *offset_found, int *pos_found) {
	if (offset == NIL)
		return FAIL;

	else {
		int next, pos = 0;
		page_t *cur_page;
		btree_load_page(&cur_page, offset);

		if (cur_page != NULL && btree_page_search(cur_page, key, &pos) == SUCCESS) {
			// retorna as posicoes do elemento
			*offset_found = offset;
			*pos_found = pos;
			free(cur_page);

			return SUCCESS;
		}

		// Nao encontrou, vai para a proxima pagina
		*offset_found = offset;
		*pos_found = pos;
		next = cur_page->child[pos]; // pega o offset da proxima pagina que deveria estar o elemento procurado
		free(cur_page);

		return btree_search_recursive(next, key, offset_found, pos_found);
	}
}

/**
 * Faz uma busca na arvore-b. Um "stub" para a chamada recursiva
 */
int btree_search(const char *key, int *offset_found, int *pos_found) {
	return btree_search_recursive(btree_root, key, offset_found, pos_found);
}

/**
 * Insere (em ordem) um elemento numa pagina buffer
 */
static void btree_page_buffer_insert(page_buffer_t *page_buf, const elem_t *elem, const int right_child) {
	int i;

	if (page_buf->n == 0) {
		strcpy(page_buf->elems[0].key, elem->key);
		page_buf->elems[0].offset = elem->offset;
		page_buf->child[1] = right_child;
		page_buf->n++;
		return;
	}

	for (i = page_buf->n-1; i >= 0; i--) {
		if (strcmp(elem->key, page_buf->elems[i].key) < 0) {
			strcpy(page_buf->elems[i+1].key, page_buf->elems[i].key);
			page_buf->elems[i+1].offset = page_buf->elems[i].offset;
			page_buf->child[i+2] = page_buf->child[i+1];

			if (i == 0) {
				strcpy(page_buf->elems[i].key, elem->key);
				page_buf->elems[i].offset = elem->offset;
				page_buf->child[1] = right_child;
				page_buf->n++;
			}
		} else {
			strcpy(page_buf->elems[i+1].key, elem->key);
			page_buf->elems[i+1].offset = elem->offset;
			page_buf->child[i+2] = right_child;
			page_buf->n++;
			break;
		}
	}
}

/**
 * Insere (em ordem) um elemento numa pagina da arvore-b
 */
void btree_page_insert(page_t *page, const elem_t *elem, const int right_child) {
	int i;

	if (page->n == 0) {
		strcpy(page->elems[0].key, elem->key);
		page->elems[0].offset = elem->offset;
		page->child[1] = right_child;
		page->n++;
		return;
	}

	for (i = page->n-1; i >= 0; i--) {
		if (strcmp(elem->key, page->elems[i].key) < 0) {
			strcpy(page->elems[i+1].key, page->elems[i].key);
			page->elems[i+1].offset = page->elems[i].offset;
			page->child[i+2] = page->child[i+1];

			if (i == 0) {
				strcpy(page->elems[i].key, elem->key);
				page->elems[i].offset = elem->offset;
				page->child[1] = right_child;
				page->n++;
			}
		} else {
			strcpy(page->elems[i+1].key, elem->key);
			page->elems[i+1].offset = elem->offset;
			page->child[i+2] = right_child;
			page->n++;
			break;
		}
	}
}

/**
 * Insere um elemento na arvore-b. Um "stub" para a insercao
 */
int btree_insert(const elem_t *elem) {
	return btree_insert_at(btree_root, elem);
}

/**
 * Insere um elemento na arvore-b em um determinado offset
 */
int btree_insert_at(const int offset, const elem_t *elem) {
	elem_t promoted_elem;
	int ret = ERROR, promoted_right_child = NIL;

	promoted_elem.key[0] = '\0';
	promoted_elem.offset = NIL;

	if ((ret = btree_insert_elem(offset, elem, &promoted_elem, &promoted_right_child)) == PROMOTED) {
		int new_root = NIL;
		page_t *page;

		// Cria nova raiz com o elemento promovido
		btree_alloc_page(&page);
		btree_page_insert(page, &promoted_elem, promoted_right_child);
		page->child[0] = offset;

		// Salva no arquivo e atualiza btree_root
		fseek(btree_fp, 0, SEEK_END);
		new_root = ftell(btree_fp);
		btree_save_page(new_root, page);
		btree_update_root(new_root);
		free(page);
	}
	
	return ret;
}

/**
 * Insere um elemento na arvore-b
 */
int btree_insert_elem(const int offset, const elem_t *elem, elem_t *promoted_elem, int *promoted_right_child) {
	page_t *cur_page = NULL, *new_page = NULL;
	elem_t promotion_elem;
	int promotion_right_child = NIL, pos = 0, ret = ERROR;

	// Se offset NIL, promove
	if (offset == NIL) {
		strcpy(promoted_elem->key, elem->key);
		promoted_elem->offset = elem->offset;
		*promoted_right_child = NIL;
		return PROMOTED;
	}

	// Senao, carrega a pagina a partir de offset e procura:

	// Se encontrou, ERROR: chave ja existe
	btree_load_page(&cur_page, offset);
	if (cur_page != NULL && btree_page_search(cur_page, elem->key, &pos) == SUCCESS) {
		free(cur_page);
		return ERROR;
	}

	// Se nao encontrou, usa a posicao que deveria estar para tentar novamente a insercao
	ret = btree_insert_elem(cur_page->child[pos], elem, &promotion_elem, &promotion_right_child);

	// Se nao houve promocao, fim.
	if (ret == NOT_PROMOTED || ret == ERROR) {
		free(cur_page);
		return ret;
	}
 
 	// Se houve promocao, confere se ha espaco na pagina atual e insere   
	if (cur_page->n < ORDER-1) {
		btree_page_insert(cur_page, &promotion_elem, promotion_right_child);
		btree_save_page(offset, cur_page);
		free(cur_page);
		return NOT_PROMOTED;
	}
    
    	// Se nao ha espaco, faz split e retorna outro elemento promovido
	btree_split(&promotion_elem, promotion_right_child, cur_page, promoted_elem, promoted_right_child, &new_page);
	btree_save_page(offset, cur_page);
	btree_save_page(*promoted_right_child, new_page);
	
	free(cur_page);
	free(new_page);

	return PROMOTED;
}

/**
 * Operacao de split em pagina da arvore-b
 */
void btree_split(const elem_t *promotion_elem, const int promotion_right_child, page_t *cur_page, elem_t *promoted_elem, int *promoted_right_child, page_t **new_page) {
	int i, mid = (ORDER-1)/2, left_child = NIL;
	page_buffer_t page_buf;

	// Reseta valores da pagina de buffer (page_buf)
	btree_reset_page_buffer(&page_buf);

	// Insere todos elementos da pagina atual (cur_page) mais o elemento a ser inserido (promotion_elem)
	for (i = 0; i < ORDER-1; i++)
		btree_page_buffer_insert(&page_buf, &cur_page->elems[i], cur_page->child[i+1]);
	btree_page_buffer_insert(&page_buf, promotion_elem, promotion_right_child);
	page_buf.child[0] = cur_page->child[0];

	// Aloca a nova pagina (new_page) e copia para ela metade-1 dos elementos da pagina de buffer (page_buf)
	*new_page = (page_t *) malloc(sizeof(page_t));
	btree_reset_page(*new_page);

	for (i = mid+2; i < ORDER; i++)
		btree_page_insert(*new_page, &page_buf.elems[i], page_buf.child[i+1]);
	(*new_page)->child[0] = page_buf.child[mid+2];

	// Define dados para retornar
	fseek(btree_fp, 0, SEEK_END);
	page_buf.child[mid+2] = ftell(btree_fp);
	*promoted_elem = page_buf.elems[mid+1];
	*promoted_right_child = page_buf.child[mid+2];

	// Reseta e cria novamente a pagina atual (cur_page)
	left_child = cur_page->child[0];
	btree_reset_page(cur_page);
	for (i = 0; i < mid+1; i++)
		btree_page_insert(cur_page, &page_buf.elems[i], page_buf.child[i+1]);
	cur_page->child[0] = left_child;
}

/**
 * Confere se uma chave existe em um dado arquivo estruturado em arvore-b
 */
int btree_exists(const char *key) {
	int offset_found, pos_found;

	if (btree_search(key, &offset_found, &pos_found) == SUCCESS)
		return 1;
	
	return 0;
}

/**
 * Encerra o uso da arvore-b
 */
void btree_close() {
	fclose(btree_fp);
	btree_root = NIL;
}

/**
 * Imprime a arvore-b (recursivo) segundo a especificacao do trabalho
 */
static void btree_print_recursive(const int offset, const int h) {
	if (offset == NIL)
		return;

	int i;
	page_t *next_page;

	btree_load_page(&next_page, offset);
	printf("Altura:  %d | num. Chaves:  %d | chaves = [ ", h, next_page->n);

	for (i = 0; i < next_page->n; i++)
		printf("%s ", next_page->elems[i].key);

	printf("]\n");

	for (i = 0; i < ORDER; i++)
		btree_print_recursive(next_page->child[i], h + 1);
	
	free(next_page);
}

/**
 * Imprime a arvore-b segundo a especificacao do trabalho
 * Um "stub" para a funcao recursiva
 */
void btree_print() {
	btree_print_recursive(btree_root, 1);
}

