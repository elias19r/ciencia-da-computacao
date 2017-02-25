#ifndef _TXT_H_
#define _TXT_H_

/**
 * Estrutura de dados de um nó da árvore de Huffman
 */
typedef struct node_t {
	char *term;
	unsigned int frequence;
	struct node_t *left;
	struct node_t *right;
} node;

/**
 * Estrutura de dados do TAD
 */
typedef struct {
	char *data;                    // dados do texto
	unsigned char *data_huff;               // dados do texto compactado com Huffman
	char *header_huff;             // cabeçalho do texto compactado com Huffman
	unsigned int size;             // tamanho dos dados do texto
	unsigned int size_huff;        // tamanho dos dados do texto compactado com Huffman
	unsigned int size_header_huff; // tamanho do cabeçalho do texto compactado com Huffman
	node *huff_tree;               // nó raiz da árvore de Huffman
	char *symbols;                 // símbolos da tabela de símbolos
	char **paths;                  // caminhos dos símbolos
	unsigned int count;            // quantidade de símbolos
} txt_data;

/**
 * Inicializa a estrutura de dados
 */
void txt_new(txt_data *txt);

/**
 * Retorna uma string representativa do caminho para um símbolo
 * de acordo com a árvore de Huffman da estrutura de dados
 */
char *txt_search_huff(txt_data *txt, char *term);

/**
 * Lê o texto de um arquivo .txt
 */
void txt_read(txt_data *txt, FILE *fp);

/**
 * Lê o conteúdo um arquivo .huff
 */
void txt_read_huff(txt_data *txt, FILE *fp);

/**
 * Lê somente o cabeçalho de uma arquivo .huff
 */
void txt_read_header_huff(txt_data *txt, FILE *fp);

/**
 * Compacta o texto usando o algoritmo de Huffman
 */
void txt_compress(txt_data *txt);

/**
 * Descompacta o texto comprimido com base na árvore de Huffman
 */
void txt_decompress(txt_data *txt);

/**
 * Imprime na tela o texto não-comprimido
 */
void txt_print(txt_data *txt);

/**
 * Imprime na tela o texto compactado com o algoritmo de Huffman
 */
void txt_print_huff(txt_data *txt);

/**
 * Imprime na tela a árvore de Huffman percorrida em ordem
 */
void txt_print_huff_tree(txt_data *txt);

/**
 * Libera toda a memória alocada na estrutura de dados e reinicializa
 */
void txt_delete(txt_data *txt);

/**
 * Libera somente a memória alocada pelo dados do texto não-comprimido
 */
void txt_free(txt_data *txt);

/**
 * Libera somente a memória alocada pelos dados da compactação de Huffman
 */
void txt_free_huff(txt_data *txt);

/**
 * Libera somente a memória alocada pelo cabeçalho da compactação de Huffman
 */
void txt_free_header_huff(txt_data *txt);

/**
 * Libera somente a memória alocada para a tabela de símbolos de Huffman
 */
void txt_free_huff_symbols(txt_data *txt);

/**
 * Libera somente a árvore de Huffman gerada pela compactação
 */
void txt_free_huff_tree(txt_data *txt);

#endif
