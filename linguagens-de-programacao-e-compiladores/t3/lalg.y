	/* ============================================================ */
	/* Definicoes do codigo */
	/* ============================================================ */

%code requires {

	#define MAX_LENGTH_IDENT   13 /* comprimento maximo para identificador  */
	#define MAX_LENGTH_INTEGER 13 /* comprimento maximo para numero inteiro */
	#define MAX_LENGTH_REAL    13 /* comprimento maximo para numero real    */

	/* Tipos de dados */	
	enum type_e {
		TYPE_PROGRAM,
		TYPE_FUNCTION,
		TYPE_STRING,
		TYPE_CONST,
		TYPE_INTEGER,
	 	TYPE_FLOAT,
		TYPE_CHAR,
		TYPE_VAR,
		TYPE_PROC,
		TYPE_NONE
	};

	/* Valores de dados */
	union val_u {
		int    intval;
		double floatval;
		char   charval;
		char   *strval;
	};

	/* Estrutura de dado que tem tipo e valor */
	struct data_s {
		enum type_e type;
		union val_u value;
	};

	/* Estrutura para guardar um simbolo */
	struct sym_s {
		int           scope;
		enum type_e   type; /* TYPE_VAR ou TYPE_PROC */
		char          id[MAX_LENGTH_IDENT + 1];
		struct data_s data;
		struct sym_s  **parlist;
		int           n_parlist;
	};

	/* Funcoes que manipulam tabelas de simbolos */
	void print_sym(const struct sym_s *sym);
	void set_sym_var(enum type_e type, struct sym_s *sym, const char *id, struct data_s data);
	void free_sym(struct sym_s *sym);
	struct entry *exist_sym(const char *id);
	int add_sym_var_s(struct sym_s *new_sym);
	void add_sym_var(enum type_e type, const char *id, struct data_s data);
	void update_sym_var(const char *id, struct data_s data);
	struct data_s get_sym_data(const char *id);
	int add_sym_proc(const char *id);
	void get_sym_and_print(const char *id, int scope);
	void free_symtables(int scope);

	/* Manipulacao de escopo */
	void sym_inc_scope();
	void sym_dec_scope();

	/* Lista de declaracao de variaveis */
	void add_varlist(enum type_e type, const char *id);
	void setnsave_varlist(enum type_e datatype);
	void free_varlist();
	void check_varlist_type();

	/* Lista de declaracao de parametros */
	void add_parlist(enum type_e type, const char *id);
	void set_parlist(enum type_e datatype);
	void save_parlist();
	void free_parlist();
	void empty_parlist();

	/* Lista de argumentos (usado em chamada de procedimento ou comando) */
	void add_arglist(const char *id);
	void check_arglist_proc(const char *id);
	void free_arglist();
}
%{
	#define _GNU_SOURCE

	#include <stdlib.h>
	#include <stdio.h>
	#include <stdarg.h>
	#include <string.h>
	#include <search.h>

	#define SIZE_SYM_TABLE 1000

	/* Funcao definida no lalg.l */
	extern int yylex();

	/* Funcao para imprimir mensagem de erro */
	void yyerror(char *s, ...);

	/* Tabelas de simbolos: uma por escopo */
	struct hsearch_data *symtables = NULL;
	int scope = 0;
	
	/* Lista temporaria de variaveis (simbolos) */
	struct sym_s **varlist = NULL;
	int n_varlist = 0;

	/* Lista temporaria de parametros (simbolos) */
	struct sym_s **parlist = NULL;
	int n_parlist = 0;
	int last_par  = -1;

	/* Lista temporaria de argumentos (strings) */
	char **arglist = NULL;
	int n_arglist = 0;

	/* Flag que sinaliza se ocorreu operacao de divisao nas contas */
	int div_flag = 0;
%}

	/* ============================================================ */
	/* Estrutura de dados para um token                             */
	/* ============================================================ */
%union {
	struct data_s data;
}

	/* Usa localizacao (linha, coluna) */
%locations

	/* Imprime mensagem de erro padrao do bison */
%error-verbose

	/* Deixa o Bison tratar o unico shift/reduce conflict (que e' do if/else) */
%expect 1

	/* ============================================================ */
	/* Tokens e nao-terminais */
	/* ============================================================ */

	/* O nao-terminal inicial da gramatica */
%start programa

	/* Codigos de tokens de palavras reservadas */
%token W_BEGIN     "begin"
%token W_CHAR      "char"
%token W_CONST     "const"
%token W_DO        "do"
%token W_ELSE      "else"
%token W_END       "end"
%token W_FOR       "for"
%token W_FUNCTION  "function"
%token W_IF        "if"
%token W_INTEGER   "integer"
%token W_PROCEDURE "procedure"
%token W_PROGRAM   "program"
%token W_READ      "read"
%token W_REAL      "real"
%token W_REPEAT    "repeat"
%token W_THEN      "then"
%token W_TO        "to"
%token W_UNTIL     "until"
%token W_VAR       "var"
%token W_WHILE     "while"
%token W_WRITE     "write"

	/* Codigos dos demais tokens */
%token COLON     ":"
%token SEMICOLON ";"
%token DOT       "."
%token COMMA     ","
%token OPAR      "("
%token CPAR      ")"
%token ATR       ":="
%token GOE       ">="
%token LOE       "<="
%token DIFFERENT "<>"
%token EQUAL     "="
%token GR        ">"
%token LE        "<"
%token PLUS      "+"
%token MINUS     "-"
%token MULT      "*"
%token DIV       "/"
%token <data> IDENT   "identificador"
%token <data> INTEGER "valor inteiro"
%token <data> REAL    "valor real"
%token <data> CHAR    "caractere"
%token UNKNOWN        "token desconhecido"

%type <data> numero
%type <data> tipo_var
%type <data> expressao
%type <data> fator
%type <data> termo
%type <data> termo0
%type <data> outros_termos
%type <data> outros_termos0
%type <data> mais_fatores	
%type <data> mais_fatores0
%type <data> op_ad
%type <data> op_un
%type <data> op_mul

%%
	/* ============================================================ */
	/* Gramatica no formato Bison */
	/* ============================================================ */
programa      : W_PROGRAM IDENT SEMICOLON corpo DOT {
                    /* Cria procedimento para o nome do programa */
                    struct data_s data;
                    data.type = TYPE_PROGRAM;
                    memset(&data.value, 0, sizeof(union val_u));

                    /* Adiciona procedimento com o nome do programa */
                    add_sym_var(TYPE_PROC, $2.value.strval, data); free($2.value.strval);
              }
              /* erros */
              | error     IDENT SEMICOLON corpo DOT   { yyerrok; free($2.value.strval); }
              | W_PROGRAM error SEMICOLON corpo DOT   { yyerrok; }
              | W_PROGRAM IDENT error     corpo DOT   { yyerrok; free($2.value.strval); }
              | W_PROGRAM IDENT SEMICOLON error DOT   { yyerrok; free($2.value.strval); }
              | W_PROGRAM IDENT SEMICOLON corpo error { yyerrok; free($2.value.strval); }
              ;


corpo         : dc W_BEGIN comandos W_END {;}
              /* erros */
              | dc W_BEGIN comandos error { yyerrok; }
              ;


dc            : dc_c dc_v dc_p dc_func {;}
              ;

dc_c          : dc_c0 dc_c {;}
              | {;}
              ;

dc_c0         : W_CONST IDENT EQUAL numero SEMICOLON {
                    /* Adiciona uma constante na tabela de simbolos    */
                    /* O datatype da constante e' o mesmo de "numbero" */
                    add_sym_var(TYPE_CONST, $2.value.strval, $4); free($2.value.strval);
              }
              /* erros */
              | error   IDENT EQUAL numero SEMICOLON { yyerrok; free($2.value.strval); }
              | W_CONST IDENT error numero SEMICOLON { yyerrok; free($2.value.strval); }
              | W_CONST IDENT EQUAL numero error     { yyerrok; free($2.value.strval); }
              ;

dc_v          : dc_v0 dc_v {;}
              | {;}
              ;

dc_v0         : W_VAR variaveis COLON tipo_var SEMICOLON {
                    /* Define datatype da lista de variaveis */
                    setnsave_varlist($4.type);
              }
              /* erros */
              | W_VAR error     COLON tipo_var SEMICOLON { yyerrok; }
              | W_VAR variaveis error tipo_var SEMICOLON { yyerrok; }
              | W_VAR variaveis COLON error    SEMICOLON { yyerrok; }
              ;

              /* "tipo_var" so' usa o seu atribuito .type */
tipo_var      : W_REAL    { $$.type = TYPE_FLOAT;   }
              | W_INTEGER { $$.type = TYPE_INTEGER; }
              | W_CHAR    { $$.type = TYPE_CHAR;    }
              ;


variaveis     : IDENT mais_var {
                    /* Adiciona uma variavel numa lista temporaria */
                    add_varlist(TYPE_VAR, $1.value.strval);
                    free($1.value.strval);
              }
              ;

mais_var      : COMMA variaveis {;}
              | {;}
              ;


              /* Este "variavies0" foi criado especialmente para tratar parametros */
variaveis0    : IDENT mais_var0 {
                    /* Adiciona parametros numa lista temporaria */
                    add_parlist(TYPE_VAR, $1.value.strval);
                    free($1.value.strval);
              }
              ;

mais_var0     : COMMA variaveis0 {;}
              | {;}
              ;

dc_p          : dc_p0 dc_p {;}
              | {;}
              ;

              /* Decrementa escopo e adiciona procedimento */
dc_p0         : W_PROCEDURE IDENT parametros SEMICOLON corpo_p {
                    sym_dec_scope();
                    add_sym_proc($2.value.strval);
                    free($2.value.strval);
              }
              /* erros */
              | W_PROCEDURE error parametros SEMICOLON corpo_p { yyerrok; }
              | W_PROCEDURE IDENT error      SEMICOLON corpo_p { yyerrok; free($2.value.strval); }
              ;	


dc_func       : W_FUNCTION IDENT parametros COLON tipo_var SEMICOLON dc_func { free($2.value.strval); }
              | {;}
              /* erros */
              | W_FUNCTION error parametros COLON tipo_var SEMICOLON dc_func   { yyerrok; }
              | W_FUNCTION IDENT parametros COLON error    SEMICOLON dc_func   { yyerrok; free($2.value.strval); }
              | W_FUNCTION IDENT parametros COLON tipo_var error     dc_func   { yyerrok; free($2.value.strval); }
              | W_FUNCTION IDENT parametros COLON tipo_var error     SEMICOLON { yyerrok; free($2.value.strval); }
              ;

parametros    : parametros0 {
                    sym_inc_scope();
                    save_parlist(); /* salva parametros na tabela de simbolos */
              }
              ;

parametros0   : OPAR lista_par CPAR {;}
              | { free_parlist(); /* caso nao tenha parametros, libera lista */ }
              /* erros */
              | OPAR error CPAR { yyerrok; }
              ;

lista_par     : lista_par0 mais_par {;}
              ;


lista_par0    : variaveis0 COLON tipo_var {
                    set_parlist($3.type); /* define tipo dos parametros ate agora */
              }
              /* erros */
              | error      COLON tipo_var { yyerrok; }
              | variaveis0 error tipo_var { yyerrok; }
              | variaveis0 COLON error    { yyerrok; }
              | variaveis0       error    { yyerrok; }
              ;


mais_par      : SEMICOLON lista_par {;}
              | {;}
              ;


corpo_p       : dc_loc W_BEGIN comandos W_END SEMICOLON {;}
              /* erros */
              | dc_loc W_BEGIN comandos error SEMICOLON { yyerrok; }
              | dc_loc W_BEGIN comandos error W_BEGIN   { yyerrok; }
              | dc_loc W_BEGIN comandos W_END error     { yyerrok; }
              ;


dc_loc        : dc_loc0 dc_loc {;}
              | {;}
              ;


              /* Aqui sao as variaveis locais de um procedimento */
dc_loc0       : W_VAR variaveis COLON tipo_var SEMICOLON {
                    setnsave_varlist($4.type);
              }
              /* erros */
              | W_VAR error     COLON tipo_var SEMICOLON { yyerrok; }
              | W_VAR variaveis error tipo_var SEMICOLON { yyerrok; }
              | W_VAR variaveis COLON error    SEMICOLON { yyerrok; }
              ;


lista_arg     : OPAR argumentos CPAR {;}
              | { free_arglist(); /* se nao tem argumentos, libera lista */ }
              /* erros */
              | OPAR error CPAR { yyerrok; }
              ;


argumentos    : IDENT mais_ident {
                    /* Adiciona um argumento (string) na lista */
                    add_arglist($1.value.strval);
                    free($1.value.strval);
              }
              ;


mais_ident    : SEMICOLON argumentos {;}
              | {;}
              /* erros */
              | error argumentos { yyerrok; }
              ;


pfalsa        : W_ELSE cmd {;}
              | {;}
              /* erros */
              | W_ELSE error { yyerrok; }
              ;


comandos      : comandos0 comandos {;}
              | {;}
              ;

comandos0     : cmd SEMICOLON {;}
              ;


cmd           : W_READ OPAR variaveis CPAR {
                    check_varlist_type();
              }
              | W_WRITE OPAR variaveis CPAR {
                    check_varlist_type();
              }
              | W_WHILE OPAR condicao CPAR W_DO cmd {;}
              | W_IF condicao W_THEN cmd pfalsa {;}
              | IDENT ATR expressao {
                    /* Atribuicao de expressao */
                    /* "expressao" foi resolvido, portanto tem tipo e valor */
              	    update_sym_var($1.value.strval, $3);
                    free($1.value.strval);
              }
              | IDENT ATR CHAR  {
                    /* Atribuicao de char */
                    update_sym_var($1.value.strval, $3);
                    free($1.value.strval);
              }
              | IDENT lista_arg {
                    /* Checa se argumentos informados casam com os do procedimento */
                    check_arglist_proc($1.value.strval);
                    free_arglist();
                    free($1.value.strval);
              }
              | W_BEGIN comandos W_END {;}
              | W_REPEAT comandos W_UNTIL condicao {;}
              | W_FOR IDENT ATR expressao W_TO INTEGER W_DO W_BEGIN comandos W_END {
                    update_sym_var($2.value.strval, $4);
                    free($2.value.strval);
              }
              /* erros */
              | W_READ error variaveis CPAR { yyerrok; }
              | W_READ OPAR error CPAR      { yyerrok; }

              | W_WRITE error variaveis CPAR { yyerrok; }
              | W_WRITE OPAR error CPAR      { yyerrok; }

              | W_IF condicao W_THEN error { yyerrok; }

              | IDENT error SEMICOLON { yyerrok; free($1.value.strval); }
              | IDENT error expressao { yyerrok; free($1.value.strval); }
              | IDENT error CHAR      { yyerrok; free($1.value.strval); }

              | W_BEGIN comandos error  { yyerrok; }
              | W_BEGIN comandos SEMICOLON  { yyerrok; }

              | W_REPEAT comandos error condicao  { yyerrok; }
              | W_REPEAT comandos error SEMICOLON { yyerrok; }
              | W_REPEAT comandos error W_END     { yyerrok; }

              | W_FOR error ATR   expressao W_TO INTEGER W_DO  W_BEGIN comandos W_END { yyerrok; }
              | W_FOR IDENT error expressao W_TO INTEGER W_DO  W_BEGIN comandos W_END { yyerrok; free($2.value.strval); }
              | W_FOR IDENT ATR   expressao W_TO error   W_DO  W_BEGIN comandos W_END { yyerrok; free($2.value.strval); }
              | W_FOR IDENT ATR   expressao W_TO INTEGER error W_BEGIN comandos W_END { yyerrok; free($2.value.strval); }
              ;


condicao      : expressao relacao expressao {;}
              ;


relacao       : EQUAL {;}
              | DIFFERENT {;}
              | GOE {;}
              | LOE {;}
              | GR {;}
              | LE {;}
              ;


expressao     : termo outros_termos {
                    if ($2.type == TYPE_NONE) {
                        $$ = $1;

                    } else {

	                    $$.type = $2.type;

	                    if ($1.type == TYPE_INTEGER && $2.type == TYPE_INTEGER) {
	                        $$.value.intval = $1.value.intval + $2.value.intval;

	                    } else if ($1.type == TYPE_FLOAT && $2.type == TYPE_FLOAT) {
	                        $$.value.floatval = $1.value.floatval + $2.value.floatval;

	                    } else {
	                        if ($1.type == TYPE_INTEGER) {
	                            $1.value.floatval = (double)$1.value.intval;
	                        }
	                        if ($2.type == TYPE_INTEGER) {
	                            $2.value.floatval = (double)$2.value.intval;
	                        }

	                        $$.type = TYPE_FLOAT;
	                        $$.value.floatval = $1.value.floatval + $2.value.floatval;
	                    }
	                }
              }
              ;


op_un         : PLUS  { $$.type = TYPE_INTEGER; $$.value.intval =  1; }
              | MINUS { $$.type = TYPE_INTEGER; $$.value.intval = -1; }
              | { $$.type = TYPE_INTEGER; $$.value.intval =  1; }
              ;

outros_termos : outros_termos0 outros_termos {
                    if ($2.type == TYPE_NONE) {
                        $$ = $1;

                    } else {

	                    $$.type = $2.type;

	                    if ($1.type == TYPE_INTEGER && $2.type == TYPE_INTEGER) {
	                        $$.value.intval = $1.value.intval + $2.value.intval;

	                    } else if ($1.type == TYPE_FLOAT && $2.type == TYPE_FLOAT) {
	                        $$.value.floatval = $1.value.floatval + $2.value.floatval;

	                    } else {
	                        if ($1.type == TYPE_INTEGER) {
	                            $1.value.floatval = (double)$1.value.intval;
	                        }
	                        if ($2.type == TYPE_INTEGER) {
	                            $2.value.floatval = (double)$2.value.intval;
	                        }

	                        $$.type = TYPE_FLOAT;
	                        $$.value.floatval = $1.value.floatval + $2.value.floatval;
	                    }
	                }
              }
              | { $$.type = TYPE_NONE; $$.value.floatval = 0.0; }
              ;

outros_termos0: op_ad termo {
                    $$ = $2;
                    if ($1.value.intval == -1 && $2.type == TYPE_INTEGER) {
                        $$.value.intval = -$2.value.intval;
                    } else if ($1.value.intval == -1 && $2.type == TYPE_FLOAT) {
                    	$$.value.floatval = -$2.value.floatval;
                    }
              }
              ;


op_ad         : PLUS  { $$.type = TYPE_INTEGER; $$.value.intval =  1; }
              | MINUS { $$.type = TYPE_INTEGER; $$.value.intval = -1; }
              ;

termo         : termo0 mais_fatores {
                    if ($2.type == TYPE_NONE) {
                        $$ = $1;

                    } else {

	                    $$.type = $2.type;

	                    if ($1.type == TYPE_INTEGER && $2.type == TYPE_INTEGER) {
	                        $$.value.intval = $1.value.intval * $2.value.intval;

	                    } else if ($1.type == TYPE_FLOAT && $2.type == TYPE_FLOAT) {
	                        $$.value.floatval = $1.value.floatval * $2.value.floatval;
	                        if (div_flag == 1) {
	                        	yyerror("division with non-integer numerator");
	                        	div_flag = 0;
	                        }

	                    } else {
	                        if ($1.type == TYPE_INTEGER) {
	                            $1.value.floatval = (double)$1.value.intval;
	                        }
	                        if ($2.type == TYPE_INTEGER) {
	                            $2.value.floatval = (double)$2.value.intval;
	                        }

	                        $$.type = TYPE_FLOAT;
	                        $$.value.floatval = $1.value.floatval * $2.value.floatval;
	                        if (div_flag == 1) {
	                        	yyerror("division with non-integer numerator");
	                        	div_flag = 0;
	                        }
	                    }
	                }
              }
              ;

termo0        : op_un fator {

                    if ($1.value.intval == -1 && $2.type == TYPE_INTEGER) {
                    	$2.value.intval = -$2.value.intval;
                    }
                    if ($1.value.intval == -1 && $2.type == TYPE_FLOAT) {
                        $2.value.floatval = -$2.value.floatval;
                    }
                    
                    $$ = $2;
              }
              ;


mais_fatores  : mais_fatores0 mais_fatores {
                    if ($2.type == TYPE_NONE) {
                        $$ = $1;

                    } else {
	                    $$.type = $2.type;

	                    if ($1.type == TYPE_INTEGER && $2.type == TYPE_INTEGER) {
	                        $$.value.intval = $1.value.intval * $2.value.intval;

	                    } else if ($1.type == TYPE_FLOAT && $2.type == TYPE_FLOAT) {
	                        $$.value.floatval = $1.value.floatval * $2.value.floatval;
	                        if (div_flag == 1) {
	                        	yyerror("division with non-integer numerator");
	                        	div_flag = 0;
	                        }

	                    } else {
	                        if ($1.type == TYPE_INTEGER) {
	                            $1.value.floatval = (double)$1.value.intval;
	                        }
	                        if ($2.type == TYPE_INTEGER) {
	                            $2.value.floatval = (double)$2.value.intval;
	                        }

	                        $$.type = TYPE_FLOAT;
	                        $$.value.floatval = $1.value.floatval * $2.value.floatval;
	                        if (div_flag == 1) {
	                        	yyerror("division with non-integer numerator");
	                        	div_flag = 0;
	                        }
	                    }
	                }
              }
              | { $$.type = TYPE_NONE; $$.value.floatval = 1.0; }
              ;

mais_fatores0 : op_mul fator {
                    $$ = $2;
                    if ($1.value.intval == -1 && $2.type == TYPE_INTEGER) {
                        $$.value.intval = 1/$2.value.intval;
                    } else if ($1.value.intval == -1 && $2.type == TYPE_FLOAT) {
                    	$$.value.floatval = 1.0/$2.value.floatval;
                    	yyerror("division with non-integer denominator");
                    }
              }
              /* erros */
              | error { yyerrok; }
              ;


op_mul        : MULT { $$.type = TYPE_INTEGER; $$.value.intval =  1; div_flag = 0; }
              | DIV  { $$.type = TYPE_INTEGER; $$.value.intval = -1; div_flag = 1; }
              ;


fator         : IDENT  { $$ = get_sym_data($1.value.strval); free($1.value.strval); }
              | numero { $$ = $1; }
              | OPAR expressao CPAR { $$ = $2;}
              /* erros */
              | error { yyerrok; }
              ;


numero        : INTEGER { $$ = $1; }
              | REAL    { $$ = $1; }
              ;
%%
	/* ============================================================ */
	/* Codigo do usuario */
	/* ============================================================ */
int main()
{
	int i;

	/* Aloca primeira tabela de simbolos (escopo 0) */
	sym_inc_scope();

	/* Executa o analisador semantico */
	yyparse();

	/* DEBUG para o arquivo "./test/sim/test1.lalg" */
	/*
	get_sym_and_print("test1", 0);
	get_sym_and_print("a",  0);
	get_sym_and_print("b",  0);
	get_sym_and_print("k",  0);
	get_sym_and_print("x",  0);
	get_sym_and_print("y",  0);
	get_sym_and_print("c",  0);
	get_sym_and_print("i",  0);
	get_sym_and_print("j",  0);
	get_sym_and_print("my_proc",  0);
	*/

	/* Libera a tabela de variaveis */
	free_symtables(0);

	return 0;
}

	/* Imprime mensagem de erro com localizacao (linha-coluna) */
void yyerror(char *s, ...) {
	va_list ap;
	va_start(ap, s);

	if (yylloc.first_line)
		fprintf(stderr, "[%2d,%-2d]: ", yylloc.first_line, yylloc.first_column);
	
	vfprintf(stderr, s, ap);
	fprintf(stderr, "\n");
}

/* ================================================= */
/*  Funcoes que manipulam tabelas de simbolos        */
/* ================================================= */

	/* Imprime um simbolo. Usado para debug */
void print_sym(const struct sym_s *sym)
{
	printf("sym.scope      : %d\n", sym->scope);
	printf("sym.type       : ");
	     if (sym->type == TYPE_VAR)   printf("var\n");
	else if (sym->type == TYPE_CONST) printf("const\n");
	else if (sym->type == TYPE_PROC)  printf("proc\n");
	printf("sym.id         : %s\n", sym->id);
	printf("sym.data.type  : ");

	switch (sym->data.type) {
		case TYPE_PROGRAM:
			printf("program\n");
			printf("sym.data.value : nenhum\n");
			break;

		case TYPE_CONST:
			printf("const\n");
			printf("sym.data.value : %d\n", sym->data.value.intval);
			break;

		case TYPE_INTEGER:
			printf("integer\n");
			printf("sym.data.value : %d\n", sym->data.value.intval);
			break;

		case TYPE_FLOAT:
			printf("float\n");
			printf("sym.data.value : %lf\n", sym->data.value.floatval);
			break;

		case TYPE_CHAR:
			printf("char\n");
			printf("sym.data.value : %c\n", sym->data.value.charval);
			break;
		
		default:
			printf("nao definido\b");
			printf("sym.data.value : nenhum\n");
			break;
	}
	printf("\n");
}

	/* Define valores de uma variavel */
void set_sym_var(enum type_e type, struct sym_s *sym, const char *id, struct data_s data)
{
	sym->scope = scope-1;
	sym->type  = type;
	strcpy(sym->id, id);
	sym->data = data;
}

	/* Libera dados de um simbolo */
void free_sym(struct sym_s *sym)
{
	int i;
	if (sym != NULL && sym->parlist != NULL) {
		for (i = 0; i < sym->n_parlist; i++) {
			if (sym->parlist[i] != NULL) {
				free(sym->parlist[i]);
			}
		}
		free(sym->parlist);
	}
}

	/* Confere se simbolo ja existe na tabela */
struct entry *exist_sym(const char *id)
{
	struct entry item, *ret;

	item.key = strdup(id);
	hsearch_r(item, FIND, &ret, &symtables[scope-1]);
	free(item.key);

	return ret;
}

	/* Adiciona uma variavel na tabela de simbolos */
int add_sym_var_s(struct sym_s *new_sym)
{
	ENTRY item, *ret;

	/* Confere se variavel ja existe na tabela */
	if (exist_sym(new_sym->id) != NULL) {
		if (new_sym->type == TYPE_CONST) {
			yyerror("constant '%s' has already been declared", new_sym->id);
		} else {
			yyerror("variable '%s' has already been declared", new_sym->id);
		}
		return 0;
	}

	item.key  = strdup(new_sym->id);
	item.data = new_sym;

	/* Tenta inserir na tabela de simbolos */
	if (hsearch_r(item, ENTER, &ret, &symtables[scope-1]) == 0) {
		yyerror("couldn't add '%s' to the symbols' table", new_sym->id);
		return 0;
	}

	return 1;
}

	/* Adiciona uma variavel na tabela de simbolos */
void add_sym_var(enum type_e type, const char *id, struct data_s data)
{
	/* Cria nova variavel */
	struct sym_s *new_sym = (struct sym_s *)malloc(sizeof(struct sym_s));

	/* Definie valores */
	set_sym_var(type, new_sym, id, data);

	/* Tenta adicionar */
	if (!add_sym_var_s(new_sym)) {
		free_sym(new_sym);
	}
}

	/* Atualiza valor de um variavel */
void update_sym_var(const char *id, struct data_s data)
{
	ENTRY *ret = exist_sym(id);

	/* Recupera variavel da tabela de simbolos */
	if (ret == NULL) {
		yyerror("undeclared variable '%s'", id);
		return;
	}

	struct sym_s *sym = (struct sym_s *)(ret->data);

	/* Confere se tipos de dados sao iguais */
	if (sym->data.type != data.type) {
		yyerror("incompatible type for variable '%s'", sym->id);
		return;
	}

	/* Atribui valor */
	sym->data = data;
}

	/* Retorna dado de uma variavel */
struct data_s get_sym_data(const char *id)
{
	struct entry  *ret = exist_sym(id);
	struct data_s data;

	/* Recupera variavel da tabela de simbolos */
	if (ret == NULL) {
		yyerror("undeclared variable '%s'\n", id);
		return data;
	}

	/* Retorna copia de seu dado */
	struct sym_s *sym = (struct sym_s *)(ret->data);
	data = sym->data;

	return data;
}

	/* Adiciona procedimento na tabela de simbolos */
int add_sym_proc(const char *id)
{
	struct sym_s *new_sym = (struct sym_s *)malloc(sizeof(struct sym_s));
	ENTRY item, *ret;

	new_sym->type  = TYPE_PROC;
	new_sym->scope = scope-1;
	strcpy(new_sym->id, id);

	/* Pega lista atual de parametros e atribui ao procedimento */
	new_sym->parlist = parlist;
	new_sym->n_parlist = n_parlist;
	
	empty_parlist();

	/* Confere se procedimento ja existe */
	if (exist_sym(new_sym->id) != NULL) {
		yyerror("procedure '%s' has already been declared", new_sym->id);
		return 0;
	}

	/* Tenta adicionar */
	item.key  = strdup(new_sym->id);
	item.data = new_sym;

	if (hsearch_r(item, ENTER, &ret, &symtables[scope-1]) == 0) {
		yyerror("couldn't add '%s' to the symbols' table", new_sym->id);
		return 0;
	}

	return 1;
}

	/* Busca um simbolo e imprime. Usado para debug */
void get_sym_and_print(const char *id, int scope)
{
	ENTRY e, *ep;

	e.key = strdup(id);
	hsearch_r(e, FIND, &ep, &symtables[scope]);
	if (ep != NULL && ep->data != NULL)
		print_sym((const struct sym_s *)ep->data);
	free(e.key);
}

	/* Libera dados de um escopo da tabela de simbolos */
void free_symtables(int scope)
{
	/* TODO */
}

/* ================================================= */
/*  Manipulacao de escopo                            */
/* ================================================= */

	/* Incrementa o escopo, ou seja, aloca mais uma tabela de simbolos */
void sym_inc_scope()
{
	symtables = (struct hsearch_data *)realloc(symtables, sizeof(struct hsearch_data) * (scope + 1));
	memset(&symtables[scope], 0, sizeof(struct hsearch_data));
	hcreate_r(SIZE_SYM_TABLE, &symtables[scope]);
	scope++;
}

	/* Incrementa o escopo, ou seja, libera a ultima tabela de simbolos alocada */
void sym_dec_scope()
{
	scope--;
	free_symtables(scope);
	hdestroy_r(&symtables[scope]);
	symtables = (struct hsearch_data *)realloc(symtables, sizeof(struct hsearch_data) * scope);
}

/* ================================================= */
/*  Lista de declaracao de variaveis                 */
/* ================================================= */

	/* Adiciona uma variavel na lista temporaria */
void add_varlist(enum type_e type, const char *id)
{
	varlist = (struct sym_s **)realloc(varlist, sizeof(struct sym_s *) * (n_varlist + 1));
	varlist[n_varlist] = (struct sym_s *)malloc(sizeof(struct sym_s));
	
	varlist[n_varlist]->type  = type;
	strcpy(varlist[n_varlist]->id, id);

	n_varlist++;
}

	/* Define tipo da variaveis da lista e salva elas na tabela de simbolos */
void setnsave_varlist(enum type_e datatype)
{
	int i;
	for (i = 0; i < n_varlist; i++) {
		varlist[i]->scope = scope-1;
	    varlist[i]->data.type = datatype;
	    add_sym_var_s(varlist[i]);
	}

	free_varlist();
}

	/* Libera a lista de variaveis */
void free_varlist()
{
	/* Libera lista de simbolos */
	free(varlist);
	varlist = NULL;
	n_varlist = 0;
}

	/* Confere se as variaveis da lista sao do mesmo tipo */
void check_varlist_type()
{
	struct entry  *ret;
	struct sym_s  *sym;
	struct data_s data;
	int i, flag = 0;
	enum type_e type_prev;

	if (n_varlist > 1) {
		for (i = 0; i < n_varlist; i++) {
			
			/* Recupera variavel */
			ret = exist_sym(varlist[i]->id);

			if (ret == NULL) {
				yyerror("undeclared variable '%s'\n", varlist[i]->id);
			
			} else {
				sym  = (struct sym_s *)(ret->data);
				
				if (flag == 0) {
					type_prev = sym->data.type;
					flag = 1;
				}

				/* Confere tipo */
				if (type_prev != sym->data.type) {
					yyerror("read/write command with different variable types");
				}

				type_prev = sym->data.type;
			}
		}
	}

	free_varlist();
}

/* ================================================= */
/*  Lista de declaracao de parametros                */
/* ================================================= */

void add_parlist(enum type_e type, const char *id)
{
	parlist = (struct sym_s **)realloc(parlist, sizeof(struct sym_s *) * (n_parlist + 1));
	parlist[n_parlist] = (struct sym_s *)malloc(sizeof(struct sym_s));
	
	parlist[n_parlist]->type  = type;
	strcpy(parlist[n_parlist]->id, id);

	n_parlist++;
}

	/* Define tipo dos parametros da lista, mas ainda nao salva eles */
void set_parlist(enum type_e datatype)
{
	int i;
	for (i = last_par+1; i < n_parlist; i++) {
	    parlist[i]->data.type = datatype;
	}
	last_par = i-1; /* guarda a posicao do ultimo */
}

	/* Salva lista de parametros na tabela de simbolos */
void save_parlist()
{
	int i;
	for (i = 0; i < n_parlist; i++) {
		parlist[i]->scope = scope-1;
	    add_sym_var_s(parlist[i]);
	}
}

	/* Libera lista de parametros */
void free_parlist()
{
	/* TODO */
	empty_parlist();
}

void empty_parlist()
{
	parlist = NULL;
	n_parlist = 0;
	last_par  = -1;
}

/* ================================================= */
/*  Lista de argumentos                              */
/* ================================================= */

void add_arglist(const char *id)
{
	arglist = (char **)realloc(arglist, sizeof(char *) * (n_arglist + 1));
	arglist[n_arglist] = (char *)malloc(sizeof(MAX_LENGTH_IDENT + 1));

	strcpy(arglist[n_arglist], id);

	n_arglist++;
}

	/* Confere se argumentos casam com os paramentros do procedimento informado */
void check_arglist_proc(const char *id)
{
	ENTRY *ret;
	struct sym_s **parlist;
	int n_parlist;

	ret = exist_sym(id);

	if (ret == NULL) {
		yyerror("undeclared procedure '%s'", id);
		return;
	}

	parlist = ((struct sym_s *)(ret->data))->parlist;
	n_parlist = ((struct sym_s *)(ret->data))->n_parlist;

	if (n_arglist < n_parlist) {
		yyerror("insufficient number of arguments for procedure '%s'", id);
		return;
	}

	if (n_arglist > n_parlist) {
		yyerror("too much arguments for procedure '%s'", id);
	}

	int i;
	for (i = 0; i < n_parlist; i++) {
		ret = exist_sym(arglist[n_arglist-1-i]);
		if (ret == NULL) {
			yyerror("undeclared variable '%s' in argument %d for '%s' procedure", arglist[n_arglist-1-i], i+1, id);
			continue;
		}

		if (parlist[i]->data.type != ((struct sym_s *)(ret->data))->data.type) {
			yyerror("incompatible type in argument %d for '%s' procedure", i+1, id);
			continue;
		}
	}
}

	/* Libera lista de argumentos */
void free_arglist()
{
	int i;
	if (arglist != NULL) {
		for (i = 0; i < n_arglist; i++) {
			if (arglist[i] != NULL) {
				free(arglist[i]);
			}
		}
		free(arglist);
	}

	arglist = NULL;
	n_arglist = 0;
}
