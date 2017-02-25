#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lalg.h"

// Variaveis externas definidas pelo flex
extern int yylex();  // funcao que pega token
extern char *yytext; // texto casado
extern int yylineno; // numero de linha
extern int yyleng;   // tamanho (strlen)

// Estrutura de dados para as infos de um token
typedef struct {
	int num;      // codigo do token
	char *value;  // texto casado
	int line;     // numero da linha
	int reserved; // codigo de palavra reservada
	int error;    // codigo de erro
} token_t;

// Tabela de palavras reservadas (ordem alfabetica hard-coded)
const char *reserved_words[] = {
	"begin",
	"char",
	"const",
	"do",
	"else",
	"end",
	"for",
	"function",
	"if",
	"integer",
	"procedure",
	"program",
	"read",
	"real",
	"repeat",
	"then",
	"to",
	"until",
	"var",
	"while",
	"write"
};

// Quantidade de palavras reservadas
int n_rwords = 21;

/**
 * Busca binaria pela palavra reservada
 */
int is_reserved(char *str)
{
	int bottom = 0;
	int mid;
	int top = n_rwords - 1;

	while (bottom <= top) {
		mid = (bottom + top) / 2;

		if (strcmp(reserved_words[mid], str) == 0) {
			return mid;
		} else if (strcmp(reserved_words[mid], str) > 0) {
			top = mid - 1;
		} else if (strcmp(reserved_words[mid], str) < 0) {
			bottom = mid + 1;
		}
	}

	return -1;
}

/**
 * Dado um codigo de erro, retorna um mensagem
 */
const char *error_msg(int error)
{
	switch (error) {
		case ERR_BAD_IDENT:
			return "- ERRO: idenfiticador mal formado";
		case ERR_MAX_IDENT:
			return "- ERRO: idenfiticador muito grande";
		case ERR_BAD_INTEGER:
			return "- ERRO: numero inteiro mal formado";
		case ERR_MAX_INTEGER:
			return "- ERRO: numero inteiro muito grande";
		case ERR_BAD_REAL:
			return "- ERRO: numero real mal formado";
		case ERR_MAX_REAL:
			return "- ERRO: numero real muito grande";
		case ERR_BAD_CHAR:
			return "- ERRO: caractere mal formado";
		case ERR_MAX_CHAR:
			return "- ERRO: caractere muito grande";
		case ERR_CHAR_EMPTY:
			return "- ERRO: caractere vazio";
		case ERR_CHAR_BREAK:
			return "- ERRO: caractere nao inline";
		case ERR_CHAR_OPEN:
			return "- ERRO: caractere nao fechado";
		case ERR_COMMENT_BREAK:
			return "- ERRO: comentario com mais de uma linha";
		case ERR_COMMENT_OPEN:
			return "- ERRO: comentario nao fechado";
		case ERR_UNKNOWN:
			return "- ERRO: token desconhecido";
		default:
			return "";
	}
	return "";
}

/**
 * Dado o numero de um token, retorna um texto descritivo
 */
const char *token_desc(int token)
{
	switch (token) {
		case COLON:
			return "COLON (dois-pontos)";
		case SEMICOLON:
			return "SEMICOLON (ponto-e-virgula)";
		case DOT:
			return "DOT (ponto)";
		case COMMA:
			return "COMMA (virgula)";
		case OPAR:
			return "OPAR (abre parenteses)";
		case CPAR:
			return "CPAR (fecha parenteses)";
		case ATR:
			return "ATR (atribuicao)";
		case EQUAL:
			return "EQUAL (igual)";
		case DIFFERENT:
			return "DIFFERENT (diferente)";
		case GOE:
			return "GOE (maior ou igual)";
		case LOE:
			return "LOE (menor ou igual)";
		case GR:
			return "GR (maior)";
		case LE:
			return "LE (menor)";
		case PLUS:
			return "PLUS (adicao)";
		case MINUS:
			return "MINUS (subtracao)";
		case MULT:
			return "MULT (multiplicacao)";
		case DIV:
			return "DIV (divisao)";
		case IDENT:
			return "IDENT (identificador)";
		case INTEGER:
			return "INTEGER (numero inteiro)";
		case REAL:
			return "REAL (numero real)";
		case CHAR:
			return "CHAR (caractere)";
		case UNKNOWN:
			return "UNKNOWN (token desconhecido)";
		default:
			return "";
	}
	return "";
}

/**
 * Funcao que faz a analise lexica
 * Retorna por referencia os dados do token
 * Retorna 0 se fim, 1 se leu
 */
int lexical(token_t *token)
{
	// Guarda sempre ultima linha em que foi encontrado um token
	static int last_line = 0;

	// Inicializa valores do token
	token->num = 0;
	token->value = NULL;
	token->line = last_line;
	token->reserved = -1;
	token->error = SUCCESS;

	// Tenta pegar um token
	token->num   = yylex();
	token->value = yytext;
	token->line  = yylineno;
	last_line    = yylineno;

	if (token->num == NONE) {
		// Fim
		return 0;
	}

	// Confere por erros que JA' foram tratados no Lex
	// Quando ocorre um erro detectado no Lex, o codigo do erro e' retornado
	//     como sendo o numero de token, entao tem que redefinir apropriadamente
	//     o numero do token e o codigo de erro
	switch (token->num) {

		case ERR_BAD_IDENT:
			token->num = IDENT;
			token->error = ERR_BAD_IDENT;
			return 1;

		case ERR_MAX_IDENT:
			token->num = IDENT;
			token->error = ERR_MAX_IDENT;
			return 1;

		case ERR_BAD_INTEGER:
			token->num = INTEGER;
			token->error = ERR_BAD_INTEGER;
			return 1;
			
		case ERR_MAX_INTEGER:
			token->num = INTEGER;
			token->error = ERR_MAX_INTEGER;
			return 1;

		case ERR_BAD_REAL:
			token->num = REAL;
			token->error = ERR_BAD_REAL;
			return 1;
			
		case ERR_MAX_REAL:
			token->num = REAL;
			token->error = ERR_MAX_REAL;
			return 1;

		case ERR_BAD_CHAR:
			token->num = CHAR;
			token->error = ERR_BAD_CHAR;
			return 1;
			
		case ERR_MAX_CHAR:
			token->num = CHAR;
			token->error = ERR_MAX_CHAR;
			return 1;

		case ERR_CHAR_EMPTY:
			token->num = CHAR;
			token->error = ERR_CHAR_EMPTY;
			return 1;

		case ERR_CHAR_BREAK:
			token->num = CHAR;
			token->error = ERR_CHAR_BREAK;
			return 1;

		case ERR_CHAR_OPEN:
			token->num = CHAR;
			token->error = ERR_CHAR_OPEN;
			return 1;

		case ERR_COMMENT_BREAK:
			token->num = UNKNOWN;
			token->error = ERR_COMMENT_BREAK;
			return 1;

		case ERR_COMMENT_OPEN:
			token->num = UNKNOWN;
			token->error = ERR_COMMENT_OPEN;
			return 1;
			
		case ERR_UNKNOWN:
			token->num = UNKNOWN;
			token->error = ERR_UNKNOWN;
			return 1;

		default:
			token->error = SUCCESS;
	}

	/* Trata as particuliaridades para cada token */

	// IDENT
	if (token->num == IDENT) {

		// Procura na tabela pra ver se e' uma palavra reservada
		token->reserved = is_reserved(token->value);

		// Se e' palavra reservada
		if (token->reserved != -1) {
			token->num = RESERVED;
		}

		return 1;
	}

	return 1;
}

/**
 * Imprime infos de um token
 */
void token_print(token_t token)
{
	char buffer[200]; // buffer para a descricao

	if (token.reserved != -1) {
		strcpy(buffer, reserved_words[token.reserved]);
	} else {
		strcpy(buffer, token_desc(token.num));
	}

	printf("%2d: %20s - %-s %s\n", token.line, token.value, buffer, error_msg(token.error));
}

/**
 * Funcao principal
 */
int main(int argc, char **argv)
{
	token_t token;
	
	// Pega tokens
	while (lexical(&token)) {
		
		// Imprime token capturado
		token_print(token);
	}

	return EXIT_SUCCESS;
}

