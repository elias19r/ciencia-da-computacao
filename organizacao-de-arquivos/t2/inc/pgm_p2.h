#ifndef _PGM_P2_H_
#define _PGM_P2_H_

/**
 * Define a estrutura de dados usada para o armazenar o formato .pgm
 */
typedef struct {
	unsigned int maxcolor;    // valor máximo para uma cor
	unsigned int width;       // largura da imagem
	unsigned int height;      // altura da imagem
	char *data;               // string de dados do arquivo .pgm
	char *data_rl;            // string de dados do arquivo .pgm comprimido com run-length (.rl)
	char *header;             // informações do header do arquivo .pgm
	unsigned int size;        // tamanho de data
	unsigned int size_rl;     // tamanho de data_rl
	unsigned int size_header; // tamanho de header
} pgm_p2_data;

/**
 * Inicializa uma estrutura
 */
void pgm_p2_new(pgm_p2_data *img);

/**
 * Lê somente as informações do cabeçalho do arquivo .pgm ou .rl
 */
void pgm_p2_read_header(pgm_p2_data *img, FILE *fp);

/**
 * Lê os dados a partir de um arquivo .pgm e armazena na estrutura de dados
 */
void pgm_p2_read(pgm_p2_data *img, FILE *fp);

/**
 * Lê os dados a partir de um arquivo comprimido .rl e armazena na estrutura de dados
 */
void pgm_p2_read_rl(pgm_p2_data *img, FILE *fp);

/**
 * Compacta os dados com um algoritmo de run-length
 */
void pgm_p2_compress(pgm_p2_data *img);

/**
 * Descompacta os dados previamente compactados com um algoritmo de run-length
 */
void pgm_p2_decompress(pgm_p2_data *img);

/**
 * Libera toda a memoria alocada na estrutura de dados e a reinicializa
 */
void pgm_p2_delete(pgm_p2_data *img);

/**
 * Libera somente a memória referente ao arquivo .pgm
 */
void pgm_p2_free(pgm_p2_data *img);

/**
 * Libera somente a memória referente ao arquivo comprimido .rl
 */
void pgm_p2_free_rl(pgm_p2_data *img);

/**
 * Imprime os dados da imagem na tela
 */
void pgm_p2_print(pgm_p2_data *img);

/**
 * Imprime os dados da imagem comprimida na tela
 */
void pgm_p2_print_rl(pgm_p2_data *img);

#endif
