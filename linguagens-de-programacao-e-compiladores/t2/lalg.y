	/* ============================================================ */
	/* Definicoes do codigo */
	/* ============================================================ */
%{
	
	#include <stdlib.h>
	#include <stdio.h>
	#include <stdarg.h>

	extern int yylex(); /* funcao definida no lalg.l */

	void yyerror(char *s, ...);

	/* Tabela de simbolos */
	/* ... fazer aqui (e' da parte semantica) ... */
	
%}

/* Usa localizacao (linha, coluna) */
%locations

/* Imprime mensagem de erro do bison */
%error-verbose

/* Deixa o Bison tratar o unico shift/reduce conflict (que e' do if/else) */
%expect 1

	/* ============================================================ */
	/* Estrutura de dados para um token (a ser usada na parte semantica) */
	/* ============================================================ */

%union {
	int intval;       /* valor inteiro   */
	double floatval;  /* valor real      */
	char charval;     /* valor caractere */
	char *strval;     /* valor texto     */
}

	/* ============================================================ */
	/* Tokens e nao-terminais */
	/* ============================================================ */

%start programa /* o nao-terminal inicial da gramatica */

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
%token <strval> IDENT   "identificador"
%token <intval> INTEGER "valor inteiro"
%token <floatval> REAL  "valor real"
%token <charval> CHAR   "caractere"
%token UNKNOWN          "token desconhecido"

%%
	/* ============================================================ */
	/* Gramatica no formato Bison */
	/* ============================================================ */
programa      : W_PROGRAM IDENT SEMICOLON corpo DOT { free($2); }
              /* erros */
              | error     IDENT SEMICOLON corpo DOT   { yyerrok; free($2); }
              | W_PROGRAM error SEMICOLON corpo DOT   { yyerrok; }
              | W_PROGRAM IDENT error     corpo DOT   { yyerrok; free($2); }
              | W_PROGRAM IDENT SEMICOLON error DOT   { yyerrok; free($2); }
              | W_PROGRAM IDENT SEMICOLON corpo error { yyerrok; free($2); }
              ;


corpo         : dc W_BEGIN comandos W_END {;}
              /* erros */
              | dc W_BEGIN comandos error { yyerrok; }
              ;


dc            : dc_c dc_v dc_p dc_func {;}
              ;


dc_c          : W_CONST IDENT EQUAL numero SEMICOLON dc_c  { free($2);}
              | {;}
              /* erros */
              | error   IDENT EQUAL numero SEMICOLON dc_c { yyerrok; free($2); }
              | W_CONST IDENT error numero SEMICOLON dc_c { yyerrok; free($2); }
              | W_CONST IDENT EQUAL numero error     dc_c { yyerrok; free($2); }
              ;


dc_v          : W_VAR variaveis COLON tipo_var SEMICOLON dc_v {;}
              | {;}
              /* erros */
              | W_VAR error     COLON tipo_var SEMICOLON dc_v { yyerrok; }
              | W_VAR variaveis error tipo_var SEMICOLON dc_v { yyerrok; }
              | W_VAR variaveis COLON error    SEMICOLON dc_v { yyerrok; }
              ;


tipo_var      : W_REAL    {;}
              | W_INTEGER {;}
              | W_CHAR    {;}
              ;


variaveis     : IDENT mais_var { free($1); }
              /* erros */
              | error SEMICOLON { yyerrok; }
              ;


mais_var      : COMMA variaveis {;}
              | {;}
              ;


dc_p          : W_PROCEDURE IDENT parametros SEMICOLON corpo_p dc_p { free($2); }
              | {;}
              /* erros */
              | W_PROCEDURE error parametros SEMICOLON corpo_p dc_p { yyerrok; }
              | W_PROCEDURE IDENT error      SEMICOLON corpo_p dc_p { yyerrok; free($2); }
              ;	


dc_func       : W_FUNCTION IDENT parametros COLON tipo_var SEMICOLON dc_func { free($2); }
              | {;}
              /* erros */
              | W_FUNCTION error parametros COLON tipo_var SEMICOLON dc_func   { yyerrok; }
              | W_FUNCTION IDENT parametros COLON error    SEMICOLON dc_func   { yyerrok; free($2); }
              | W_FUNCTION IDENT parametros COLON tipo_var error     dc_func   { yyerrok; free($2); }
              | W_FUNCTION IDENT parametros COLON tipo_var error     SEMICOLON { yyerrok; free($2); }
              ;


parametros    : OPAR lista_par CPAR {;}
              | {;}
              /* erros */
              | OPAR error CPAR { yyerrok; }
              ;


lista_par     : variaveis COLON tipo_var mais_par {;}
              /* erros */
              | error     COLON tipo_var mais_par { yyerrok; }
              | variaveis error tipo_var mais_par { yyerrok; }
              | variaveis COLON error    mais_par { yyerrok; }
              | variaveis       error    mais_par { yyerrok; }
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


dc_loc        : dc_v {;}
              ;


lista_arg     : OPAR argumentos CPAR {;}
              | {;}
              /* erros */
              | OPAR error CPAR { yyerrok; }
              ;


argumentos    : IDENT mais_ident { free($1); }
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


comandos      : cmd SEMICOLON comandos {;}
              | {;}
              ;


cmd           : W_READ OPAR variaveis CPAR {;}
              | W_WRITE OPAR variaveis CPAR {;}
              | W_WHILE OPAR condicao CPAR W_DO cmd {;}
              | W_IF condicao W_THEN cmd pfalsa {;}
              | IDENT ATR expressao { free($1); }
              | IDENT ATR CHAR  { free($1); }
              | IDENT lista_arg { free($1); }
              | W_BEGIN comandos W_END {;}
              | W_REPEAT comandos W_UNTIL condicao {;}
              | W_FOR IDENT ATR expressao W_TO INTEGER W_DO W_BEGIN comandos W_END { free($2); }
              /* erros */
              | W_READ error variaveis CPAR { yyerrok; }
              | W_READ OPAR error CPAR      { yyerrok; }

              | W_WRITE error variaveis CPAR { yyerrok; }
              | W_WRITE OPAR error CPAR      { yyerrok; }

              | W_IF condicao W_THEN error { yyerrok; }

              | IDENT error expressao { yyerrok; free($1); }
              | IDENT error CHAR      { yyerrok; free($1); }
              | IDENT error           { yyerrok; free($1); }

              | W_BEGIN comandos error  { yyerrok; }
              | W_BEGIN comandos SEMICOLON  { yyerrok; }

              | W_REPEAT comandos error condicao  { yyerrok; }
              | W_REPEAT comandos error SEMICOLON { yyerrok; }
              | W_REPEAT comandos error W_END     { yyerrok; }

              | W_FOR error ATR   expressao W_TO INTEGER W_DO  W_BEGIN comandos W_END { yyerrok; }
              | W_FOR IDENT error expressao W_TO INTEGER W_DO  W_BEGIN comandos W_END { yyerrok; free($2); }
              | W_FOR IDENT ATR   expressao W_TO error   W_DO  W_BEGIN comandos W_END { yyerrok; free($2); }
              | W_FOR IDENT ATR   expressao W_TO INTEGER error W_BEGIN comandos W_END { yyerrok; free($2); }
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


expressao     : termo outros_termos {;}
              ;


op_un         : PLUS {;}
              | MINUS {;}
              | {;}
              ;


outros_termos : op_ad termo outros_termos {;}
              | {;}
              ;


op_ad         : PLUS {;}
              | MINUS {;}
              ;


termo         : op_un fator mais_fatores {;}
              ;


mais_fatores  : op_mul fator mais_fatores {;}
              | {;}
              /* erros */
              | error { yyerrok; }
              ;


op_mul        : MULT {;}
              | DIV {;}
              /* erros */
              | error { yyerrok; }
              ;


fator         : IDENT { free($1); }
              | numero {;}
              | OPAR expressao CPAR {;}
              /* erros */
              | error { yyerrok; }
              ;


numero        : INTEGER {;}
              | REAL {;}
              ;
%%
	/* ============================================================ */
	/* Codigo do usuario */
	/* ============================================================ */
int main()
{
	/* Executa o analisador sintatico */
	yyparse();

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


	/* Funcoes que gerenciam tabela de simbolos */
	/* ... fazer aqui (e' da parte semantica) ... */
