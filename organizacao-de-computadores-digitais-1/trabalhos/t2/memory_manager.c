/*
 * ATENCAO: NAO ALTERAR OS ARQUIVO "main.c" e "memory_manager.h"
 * Seu trabalho deve ser desenvolvido neste arquivo "memory_manager.c"
 * na funcao memory_access(int addr, int *data, int type).
 *
 * Para compilar o projeto execute o comando
 *     make
 * Para executar digite o comando:
 *    ./main
 *
 * */

/*
 * 
 * OBSERVACAO: Conforme conversamos com o professor, estamos entregando este
 *             trabalho de acordo com a *primeira versao da especificacao*.
 *             Acompanham este arquivo, a descricao e os arquivos .c e .h originais.
 * 
 * TURMA:  2
 * GRUPO:  07
 * ALUNOS: Anderson Caio Santos Silva, 7972630
 *         Elias Italiano Rodrigues,   7987251
 *         Rodolfo Megiato de Lima,    7987286
 */

#include <stdlib.h>
#include "memory_manager.h"

/**
 * Macros usadas pelas funcoes
 */
#define WORD    0
#define BLOCK   1

#define READ    0
#define WRITE   1
#define LOAD    2

#define SUCCESS 1
#define FAIL    0

/**
 * Tipo de dado para guardar os offsets que indexam uma cache L1
 */
typedef struct {
	unsigned int byte;
	unsigned int word;
	unsigned int set;
	unsigned int tag;
} offsets_L1;

/**
 * Tipo de dado para guardar os offsets que indexam uma cache L2
 */
typedef struct {
	unsigned int byte;
	unsigned int word;
	unsigned int set;
	unsigned int tag;
	unsigned int addr;
	unsigned int sub;
} offsets_L2;

/**
 * Tipo de dado para guardar os offsets que indexam uma RAM
 */
typedef struct {
	unsigned int addr;
	unsigned int addr_word;
	unsigned int addr_block;
} offsets_RAM;

/**
 * Calcula e retorna os offsets para indexar uma RAM
 */
static void get_offsets_RAM(offsets_RAM *RAM, unsigned int addr) {
	RAM->addr       = addr;
	RAM->addr_word  = (addr >> 2) << 2; // 1........00
	RAM->addr_block = (addr >> 4) << 4; // 1......0000
}

/**
 * Calcula e retorna os offsets para indexar uma cache L1
 */
static void get_offsets_L1(offsets_L1 *L1, unsigned int addr) {
	L1->byte =  addr & 3;         // 0..0000000011
     	L1->word = (addr & 4)   >> 2; // 0..0000000100
	L1->set  = (addr & 24)  >> 3; // 0..0000011000
	L1->tag  = (addr & 992) >> 5; // 0..1111100000
}

/**
 * Calcula e retorna os offsets para indexar uma cache L2
 */
static void get_offsets_L2(offsets_L2 *L2, unsigned int addr) {
	L2->byte =  addr & 3;         // 0..0000000011
     	L2->word = (addr & 12)  >> 2; // 0..0000001100
	L2->set  = (addr & 112) >> 4; // 0..0001110000
	L2->tag  = (addr & 896) >> 7; // 0..1110000000
	L2->addr = (addr >> 4)  << 4; // 1........0000
	L2->sub  = (L2->word  >> 1) == 0 ? 0 : 2;
}

/**
 * Le uma palavra ou um bloco da RAM a partir de um endereco dado
 */
static void read_RAM(unsigned int *content, unsigned int addr, int type) {
	int i, j, count = 0;
	if (type == WORD)  count = 1;
	if (type == BLOCK) count = 4;
	
	for (i = 0; i < count; i++) {
		content[i] = 0;
		for (j = 0; j < 4; j++)
			content[i] += (unsigned int) memory[addr++] << (3 - j) * 8;
	}
}

/**
 * Escreve uma palavra na RAM a partir de um endereco dado
 */
static void write_RAM(unsigned int *content, unsigned int addr, int type) {
	int i, j, count = 0;
	if (type == WORD)  count = 1;
	if (type == BLOCK) count = 4;

	for (i = 0; i < count; i++)	
		for (j = 0; j < 4; j++)
			memory[addr++] = (unsigned char) (content[i] >> (3 - j) * 8);
}

/**
 * Identifica um bloco de L1
 */
static int get_block_L1(offsets_L1 *L1) {
	int i;
	for (i = 0; i < BLOCKS_L1; i++)
		if (cache_L1[L1->set].blocks[i].valid == 1 && cache_L1[L1->set].blocks[i].tag == L1->tag)
			return i;
	
	return -1;
}

/**
 * Le da cache L1
 */
static int read_L1(unsigned int *content, offsets_L1 *L1, int type) {
	int i = get_block_L1(L1);
	if (i != -1) {
		if (type == WORD) {
			*content = cache_L1[L1->set].blocks[i].words[L1->word];
		} else if (type == BLOCK) {
			int j;
			for (j = 0; j < WORDS_L1; j++)
				content[j] = cache_L1[L1->set].blocks[i].words[j];
		}

		return SUCCESS;
	}

	return FAIL;
}

/**
 * Seleciona um bloco de L1
 */
static int select_block_L1(offsets_L1 *L1) {
	int i;
	
	// Procura pelo primeiro bloco nao valido
	for (i = 0; i < BLOCKS_L1; i++)
		if (cache_L1[L1->set].blocks[i].valid == 0)
			return i;
	
	// Procura pelo primeiro bloco "que entrou"
	for (i = 0; i < BLOCKS_L1; i++)
		if (cache_L1[L1->set].blocks[i].subst == 0)
			return i;
	
	return 0;
}

/**
 * Define os valores das flags de um bloco de L1 selecionado
 */
static void set_block_L1(offsets_L1 *L1, int i) {
	cache_L1[L1->set].blocks[i].valid = 1;
	cache_L1[L1->set].blocks[i].subst = 0;
	cache_L1[L1->set].blocks[i].tag = L1->tag;
	cache_L1[L1->set].blocks[!i].subst = !cache_L1[L1->set].blocks[!i].subst;
}

/**
 * Carrega um sub-bloco, ou escreve uma palavra em L1
 */
static void write_L1(offsets_L1 *L1, unsigned int *content, int type) {
	int i;
	if (type == LOAD) {
		int j;
		i = select_block_L1(L1);
		set_block_L1(L1, i);
		for (j = 0; j < WORDS_L1; j++)
			cache_L1[L1->set].blocks[i].words[j] = content[j];

		return;
	}
	if (type == WRITE) {
		i = get_block_L1(L1);
		cache_L1[L1->set].blocks[i].words[L1->word] = *content;

		return;
	}
}

/**
 * Identifica um bloco de L2
 */
static int get_block_L2(offsets_L2 *L2) {
	int i;
	for (i = 0; i < BLOCKS_L2; i++)
		if (cache_L2[L2->set].blocks[i].valid == 1 && cache_L2[L2->set].blocks[i].tag == L2->tag)
			return i;
	
	return -1;
}

/**
 * Le uma palavra de L2
 */
static int read_L2(unsigned int *content, offsets_L2 *L2, int type) {
	int i = get_block_L2(L2);
	if (i != -1) {
		if (type == WORD) {
			*content = cache_L2[L2->set].blocks[i].words[L2->word];			
		} else if (type == BLOCK) {
			int j;
			for (j = 0; j < WORDS_L2; j++)
				content[j] = cache_L2[L2->set].blocks[i].words[j];
		}

		return SUCCESS;
	}

	return FAIL;
}

/**
 * Seleciona um bloco de L2
 */
static int select_block_L2(offsets_L2 *L2) {
	int i;
	
	// Procura pelo primeiro bloco nao valido
	for (i = 0; i < BLOCKS_L2; i++)
		if (cache_L2[L2->set].blocks[i].valid == 0)
			return i;
	
	// Procura pelo primeiro bloco "que entrou"
	for (i = 0; i < BLOCKS_L2; i++)
		if (cache_L2[L2->set].blocks[i].subst == 0)
			return i;
	
	return 0;
}

/**
 * Define os valores das flags de um bloco de L2 selecionado
 */
static void set_block_L2(offsets_L2 *L2, int i) {
	cache_L2[L2->set].blocks[i].valid = 1;
	cache_L2[L2->set].blocks[i].subst = 0;
	cache_L2[L2->set].blocks[i].tag = L2->tag;
	cache_L2[L2->set].blocks[!i].subst = !cache_L2[L2->set].blocks[!i].subst;
}

/**
 * Carrega um bloco, ou escreve um sub-bloco em L2
 */
static void write_L2(offsets_L2 *L2, unsigned int *content, int type) {
	int i, j, k, count;
	i = get_block_L2(L2);
	
	// Confere flag de modified
	if (cache_L2[L2->set].blocks[i].modified == 1) {
		write_RAM(cache_L2[L2->set].blocks[i].words, L2->addr, BLOCK);
		cache_L2[L2->set].blocks[i].modified = 0;
	}

	// Define quantidade de palavras
	if (type == LOAD) {
		i = select_block_L2(L2);
		set_block_L2(L2, i);

		j = 0;
		count = WORDS_L2;
	} else if (type == WRITE) {
		i = get_block_L2(L2);

		j = L2->sub;
		count = WORDS_L2 / 2;
		cache_L2[L2->set].blocks[i].modified = 1;
	}
	
	// Copia dados em L2
	for (k = 0; k < count; k++, j++)
		cache_L2[L2->set].blocks[i].words[j] = content[k];

}

/**
 * Faz o acesso de leitura na memoria
 */
static int memory_access_read(unsigned int *data, offsets_L1 *L1, offsets_L2 *L2, offsets_RAM *RAM) {
	// Tenta ler da L1	
	if (read_L1(data, L1, WORD) == SUCCESS)
		return 1;
	
	// Tenta ler da L2
	if (read_L2(data, L2, WORD) == SUCCESS) {
		unsigned int words[4];
		
		read_L2(words, L2, BLOCK);
		write_L1(L1, words+L2->sub, LOAD);
		
		return 1;
	}

	// Le da RAM
	unsigned int words[4];
	read_RAM(data, RAM->addr_word, WORD);
	read_RAM(words, RAM->addr_block, BLOCK);
	write_L2(L2, words, LOAD);
	write_L1(L1, words+L2->sub, LOAD);
	
	return 1;
}

/**
 * Faz o acesso de escrita na memoria
 */
static int memory_access_write(unsigned int *data, offsets_L1 *L1, offsets_L2 *L2, offsets_RAM *RAM) {
	unsigned int buffer = 0;
	
	// Forca uma leitura e garante o dado em L1
	memory_access_read(&buffer, L1, L2, RAM);
	
	// Escreve em L1 e L2 (e possivelmente na RAM)
	unsigned int words[2];
	write_L1(L1, data, WRITE);
	read_L1(words, L1, BLOCK);
	write_L2(L2, words, WRITE);
	
	return 1;
}

/**
 * A funcao memory_access
 */
int memory_access(int addr, int *data, int type) {
	offsets_L1 L1;
	offsets_L2 L2;
	offsets_RAM RAM;

	// Pega o endereco a byte da palavra...
	addr <<= 2;
	
	// Pega os offsets	
	get_offsets_L1(&L1, addr);
	get_offsets_L2(&L2, addr);
	get_offsets_RAM(&RAM, addr);

	// Executa o tipo de acesso	
	if (type == READ)
		return memory_access_read((unsigned int *) data, &L1, &L2, &RAM);

	if (type == WRITE)
		return memory_access_write((unsigned int *) data, &L1, &L2, &RAM);

	return -1;
}

