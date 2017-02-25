/*
 * ATENCAO: NAO ALTERAR ESTE ARQUIVO
 * Seu trabalho deve ser desenvolvido no arquivo memory_manager.c
 * na funcao memory_access(int addr, int *data, int type).
 *
 * Para compilar o projeto execute o comando
 *     make
 * Para executar digite o comando:
 *    ./main
 *
 * */

#ifndef MEMORY_MANAGER_H_INCLUDED
#define MEMORY_MANAGER_H_INCLUDED

#define RAM_SIZE 1024     // A RAM possui 256 words dividida em 1024 bytes (4 bytes por word)
#define SETS_L1 4         // Quantidade de conjuntos para cache L1
#define SETS_L2 8         // Quantidade de conjuntos para cache L2
#define BLOCKS_L1 2       // Quantidade de blocos para cache L1
#define BLOCKS_L2 2       // Quantidade de blocos para cache L2
#define WORDS_L1 2        // Quantidade de palavras da cache L1
#define WORDS_L2 4        // Quantidade de palavras da cache L2


typedef struct {
  int valid;              // bit de validade
  int subst;              // bit de controle para politica de substituicao
  int tag;                // tag do bloco
  unsigned int words[WORDS_L1];    // palavras do bloco
} block_L1;

typedef struct {
  int valid;              // bit de validade
  int modified;           // bit de modificado
  int subst;              // bit de controle para politica de substituicao
  int tag;                // tag do bloco
  unsigned int words[WORDS_L2];    // palavras do bloco
} block_L2;

typedef struct {
  block_L1 blocks[BLOCKS_L1];
} set_L1;

typedef struct {
  block_L2 blocks[BLOCKS_L2];
} set_L2;

unsigned char memory[RAM_SIZE];    // char representa 1 byte de 8 bits (desconsidera bit de sinal)
set_L1 cache_L1[SETS_L1];
set_L2 cache_L2[SETS_L2];

int memory_access(int addr, int *data, int type);

#endif
