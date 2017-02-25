#ifndef _LALG_H_
#define _LALG_H_

#define MAX_LENGTH_IDENT   13 /* comprimento maximo para identificador  */
#define MAX_LENGTH_INTEGER 13 /* comprimento maximo para numero inteiro */
#define MAX_LENGTH_REAL    13 /* comprimento maximo para numero real    */

enum {
	/* Codigos de tokens */
	NONE,      /*   nenhum token     */
	COLON,     /*        :           */
	SEMICOLON, /*        ;           */
	DOT,       /*        .           */
	COMMA,     /*        ,           */
	OPAR,      /*        (           */
	CPAR,      /*        )           */
	ATR,       /*        :=          */
	GOE,       /*        >=          */
	LOE,       /*        <=          */
	DIFFERENT, /*        <>          */
	EQUAL,     /*        =           */
	GR,        /*        >           */
	LE,        /*        <           */
	PLUS,      /*        +           */
	MINUS,     /*        -           */
	MULT,      /*        *           */
	DIV,       /*        /           */
	IDENT,     /*  identificador     */
	INTEGER,   /*  numero inteiro    */
	REAL,      /*   numero real      */
	CHAR,      /*    caractere       */
	RESERVED,  /* palavra reservada  */
	UNKNOWN,   /* token desconhecido */

	/* Codigos de erros */	
	SUCCESS,           /*       nenhum erro           */
	ERR_BAD_IDENT,     /* idenfiticador mal formado   */
	ERR_MAX_IDENT,     /* idenfiticador muito grande  */
	ERR_BAD_INTEGER,   /* numero inteiro mal formado  */
	ERR_MAX_INTEGER,   /* numero inteiro muito grande */
	ERR_BAD_REAL,      /* numero real mal formado     */
	ERR_MAX_REAL,      /* numero real muito grande    */
	ERR_BAD_CHAR,      /* caractere mal formado       */
	ERR_MAX_CHAR,      /* caractere muito grande      */
	ERR_CHAR_EMPTY,    /* caractere vazio             */
	ERR_CHAR_BREAK,    /* caractere nao inline        */
	ERR_CHAR_OPEN,     /* caractere nao fechado       */
	ERR_COMMENT_BREAK, /* comentario nao inline       */
	ERR_COMMENT_OPEN,  /* comentraio nao fechado      */
	ERR_UNKNOWN        /*    token desconhecido       */
};

#endif
