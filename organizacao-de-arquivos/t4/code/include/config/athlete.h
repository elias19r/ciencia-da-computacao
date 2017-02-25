#ifndef _CONFIG_ATHLETE_H_
#define _CONFIG_ATHLETE_H_

/**
 * Configuracoes relacionadas ao dados do atleta
 */

// Tamanhos dos registros
#define ATHLETE_RECORD_SIZE         116
#define ATHLETE_UNI_IDX_RECORD_SIZE  42
#define ATHLETE_MOD_IDX_RECORD_SIZE  42
#define ATHLETE_LIST_RECORD_SIZE     23

// Tamanhos dos campos
#define ATHLETE_CPF_SIZE  11
#define ATHLETE_NAME_SIZE 30
#define ATHLETE_RA_SIZE   10
#define ATHLETE_UNI_SIZE  30
#define ATHLETE_MOD_SIZE  30

// Offset dos campos no registro
#define ATHLETE_CPF_OFFSET   0
#define ATHLETE_NAME_OFFSET 12
#define ATHLETE_RA_OFFSET   43
#define ATHLETE_UNI_OFFSET  54
#define ATHLETE_MOD_OFFSET  85

// Arquivos de dados
#define ATHLETE_DAT          "data.db"
#define ATHLETE_IDX          "prim.idx"
#define ATHLETE_UNI_IDX      "universidade.idx"
#define ATHLETE_MOD_IDX      "modalidade.idx"
#define ATHLETE_UNI_LIST_IDX "universidadeListaInvertida.idx"
#define ATHLETE_MOD_LIST_IDX "modalidadeListaInvertida.idx"

#endif
