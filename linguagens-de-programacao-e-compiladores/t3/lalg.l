	/* ============================================================ */
	/* Opcoes do Lex */
	/* ============================================================ */

	/* Habilita a contagem de linhas e nao usa funcao yywrap */
%option    noyywrap yylineno

	/* ============================================================ */
	/* Definicoes do codigo */
	/* ============================================================ */
%{
	#include "y.tab.h" /* inclui tokens definidos no bison */

	/* "Adding Locations to the Lexer" do livro "flex & bison" */
	int yycolumn = 1;
	#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno; \
		yylloc.first_column = yycolumn; yylloc.last_column = yycolumn+yyleng-1; \
		yycolumn += yyleng;

	#define MAX_LENGTH_IDENT   13 /* comprimento maximo para identificador  */
	#define MAX_LENGTH_INTEGER 13 /* comprimento maximo para numero inteiro */
	#define MAX_LENGTH_REAL    13 /* comprimento maximo para numero real    */

	/* Tabela de palavras reservadas (ordem alfabetica hard-coded) */
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

	/* Codigo dos tokens das palavras reservadas */
	const int RESERVED[] = {
		W_BEGIN,
		W_CHAR,
		W_CONST,
		W_DO,
		W_ELSE,
		W_END,
		W_FOR,
		W_FUNCTION,
		W_IF,
		W_INTEGER,
		W_PROCEDURE,
		W_PROGRAM,
		W_READ,
		W_REAL,
		W_REPEAT,
		W_THEN,
		W_TO,
		W_UNTIL,
		W_VAR,
		W_WHILE,
		W_WRITE
	};

	/* Quantidade de palavras reservadas */
	const int n_rwords = 21;

	/* Imprime erro (funcao externa definida no lalg.y) */
	extern void yyerror(char *s, ...);

	/* Busca binaria pela palavra reservada */
	int is_reserved(char *str);
	
%}

%%
	/* ============================================================ */
	/* Regras */
	/* ============================================================ */

	/* Ignora comentarios */
\{[^}\n\r]*\}    {;}

	/* Tokens que podem ser capturados aqui no Lex */
:               {return COLON;}
;               {return SEMICOLON;}
\.              {return DOT;}
,               {return COMMA;}
\(              {return OPAR;}
\)              {return CPAR;}
:=              {return ATR;}
\>=             {return GOE;}
\<=             {return LOE;}
\<\>            {return DIFFERENT;}
=               {return EQUAL;}
\>              {return GR;}
\<              {return LE;}
\+              {return PLUS;}
\-              {return MINUS;}
\*              {return MULT;}
\/              {return DIV;}

	/* Erros de caractere reconhecidos com regexp */
''     { yyerror("empty character: %s", yytext); yylval.data.type = TYPE_CHAR; yylval.data.value.charval = '\0'; return CHAR; }
'''    { yyerror("bad character: %s", yytext); yylval.data.type = TYPE_CHAR; yylval.data.value.charval = '\0'; return CHAR; } /* ' */

	/* Caractere escapados */
'(\\([abfnrtv'"0?\\]|[0-7]{3}|x[0-9abcdef]{2})|.)'    {
		/* Tratamento de caracteres escapados */
	if (yytext[1] == '\\') {
		if (yytext[2] == 'x') {
			yytext[5] = '\0';
			yylval.data.value.charval = (char)strtoul(yytext+3, NULL, 16);
		} else if (
				yytext[2] == '0' ||
				yytext[2] == '1' ||
				yytext[2] == '2' ||
				yytext[2] == '3' ||
				yytext[2] == '4' ||
				yytext[2] == '5' ||
				yytext[2] == '6' ||
				yytext[2] == '7'
			) {
				yytext[5] = '\0';
				yylval.data.value.charval = (char)strtoul(yytext+2, NULL, 8);
		} else {
			switch (yytext[2]) {
				case 'a':
					yylval.data.value.charval = 0x07;
					break;
				case 'b':
					yylval.data.value.charval = 0x08;
					break;
				case 'f':
					yylval.data.value.charval = 0x0c;
					break;
				case 'n':
					yylval.data.value.charval = 0x0a;
					break;
				case 'r':
					yylval.data.value.charval = 0x0d;
					break;
				case 't':
					yylval.data.value.charval = 0x09;
					break;
				case 'v':
					yylval.data.value.charval = 0x0b;
					break;
				case '\\':
					yylval.data.value.charval = 0x5c;
					break;
				case '\'':
					yylval.data.value.charval = 0x27;
					break;
				case '"':
					yylval.data.value.charval = 0x22;
					break;
				case '0':
					yylval.data.value.charval = 0;
					break;
				case '?':
					yylval.data.value.charval = 0x3f;
					break;
				default:
					yylval.data.value.charval = '\0';
			}
		}
	} else {
		yylval.data.value.charval = yytext[1];
	}
	yylval.data.type = TYPE_CHAR;
	return CHAR;
} /* http://en.wikipedia.org/wiki/Escape_sequences_in_C */

[a-zA-Z][_a-zA-Z0-9]*    {
	/* Confere erro de comprimento de identificador */
	if (yyleng > MAX_LENGTH_IDENT) {
		yyerror("identifier is too big: %s", yytext);
		yylval.data.value.strval = NULL;
	} else {
		/* Faz busca para tratar se e' mesmo uma IDENT ou uma palavra reservada */
		int rword = is_reserved(yytext);

		/* Se e' palavra reservada */
		if (rword != -1) {
			yylval.data.value.strval = NULL;
			return RESERVED[rword];
		} else {
			yylval.data.value.strval = strdup(yytext);
		}
	}
	yylval.data.type = TYPE_STRING;
	return IDENT;
}
0|[1-9][0-9]*    {
	/* Confere erro de comprimento de numero inteiro */
	if (yyleng > MAX_LENGTH_INTEGER) {
		yyerror("integer number is too big: %s", yytext);
	}
	yylval.data.value.intval = atoi(yytext); /* pega o valor inteiro numerico */
	yylval.data.type = TYPE_INTEGER;
	return INTEGER;
}
[0-9]+\.[0-9]+    {
	/* Confere erro de comprimento de numero real */
	if (yyleng > MAX_LENGTH_REAL) {
		yyerror("real number is too big: %s", yytext);
	}
	yylval.data.value.floatval = atof(yytext); /* pega o valor real numerico */
	yylval.data.type = TYPE_FLOAT;
	return REAL;
}

	/* Ignora espacos em branco */
[ \t]      {;}
[ \n\r]    { yycolumn = 1; }

	/* Erros de ma' formacao que podem ser reconhecidos com regexp */
[a-zA-Z][_a-zA-Z0-9]*[^ \t\n\r:;.,()=></+*-]+      { yyerror("bad identifier: %s", yytext); yylval.data.type = TYPE_STRING; yylval.data.value.strval = NULL; return IDENT;}
[0-9]+\.[0-9]*[^ \t\n\r:;,)=></+*-]+[0-9]*         { yyerror("bad real number: %s", yytext); yylval.data.type = TYPE_FLOAT; yylval.data.value.floatval = 0.0; return REAL; }
0[0-9]+|[1-9][0-9]*[^ \t\n\r:;,)=></+*-]+[0-9]*    { yyerror("bad integer number: %s", yytext); yylval.data.type = TYPE_INTEGER; yylval.data.value.intval = 0; return INTEGER; }

	/* Mais erros de caractere reconhecidos com regexp */
'[^'\n\r]*[\n\r]+[^']*'    { yyerror("character not inline: %s", yytext); yylval.data.type = TYPE_CHAR; yylval.data.value.charval = '\0'; return CHAR; }
'[^']*                     { yyerror("unclosed character: %s", yytext); yylval.data.type = TYPE_CHAR; yylval.data.value.charval = '\0'; return CHAR; }
'.{2,}'                    { yyerror("big character: %s", yytext); yylval.data.type = TYPE_CHAR; yylval.data.value.charval = '\0'; return CHAR; }

	/* Erros de comentarios que podem ser reconhecidos com regexp */
\{[^}\n\r]*[\n\r]+[^}]*\}    { yyerror("comment with more than one line: %s", yytext); yycolumn = 1; }
\{[^}]*                      { yyerror("unclosed comment: %s", yytext); }

	/* Erro de token desconhecido */
.    { yyerror("unkown token: %s", yytext); }


%%
	/* ============================================================ */
	/* Codigo do usuario */
	/* ============================================================ */

	/* Busca binaria pela palavra reservada */
int is_reserved(char *str) {
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
