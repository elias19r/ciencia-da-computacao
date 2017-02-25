	/* Habilita a contagem de linhas */
%option    yylineno

	/* Definicoes */
%{
	#include <stdlib.h>
	#include <stdio.h>

	#include "lalg.h"
%}

%%
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
''     {return ERR_CHAR_EMPTY;}
'''    {return ERR_BAD_CHAR;} /* ' */

'\\([abfnrtv'"?\\]|[0-9]{3}|x[0-9]{2})|.'    {return CHAR;} /* http://en.wikipedia.org/wiki/Escape_sequences_in_C */

[a-zA-Z][_a-zA-Z0-9]*    {
	/* Confere erro de comprimento de identificador */
	if (yyleng > MAX_LENGTH_IDENT) {
		return ERR_MAX_IDENT;
	}
	return IDENT;
}
0|[1-9][0-9]*    {
	/* Confere erro de comprimento de numero inteiro */
	if (yyleng > MAX_LENGTH_INTEGER) {
		return ERR_MAX_INTEGER;
	}
	return INTEGER;
}
[0-9]+\.[0-9]+    {
	/* Confere erro de comprimento de numero real */
	if (yyleng > MAX_LENGTH_REAL) {
		return ERR_MAX_REAL;
	}
	return REAL;
}

	/* Ignora espacos em branco */
[ \t\n\r]    {;}

	/* Erros de ma' formacao que podem ser reconhecidos com regexp */
[a-zA-Z][_a-zA-Z0-9]*[^ \t\n\r:;.,()=></+*-]+      {return ERR_BAD_IDENT;}
[0-9]+\.[0-9]*[^ \t\n\r:;,)=></+*-]+[0-9]*         {return ERR_BAD_REAL;}
0[0-9]+|[1-9][0-9]*[^ \t\n\r:;,)=></+*-]+[0-9]*    {return ERR_BAD_INTEGER;}

	/* Mais erros de caractere reconhecidos com regexp */
'[^'\n\r]*[\n\r]+[^']*'    {return ERR_CHAR_BREAK;}
'[^']*                     {return ERR_CHAR_OPEN;}
'.{2,}'                    {return ERR_MAX_CHAR;}

	/* Erros de comentarios que podem ser reconhecidos com regexp */
\{[^}\n\r]*[\n\r]+[^}]*\}    {return ERR_COMMENT_BREAK;}
\{[^}]*                      {return ERR_COMMENT_OPEN;}

	/* Erro de token desconhecido */
.    {return ERR_UNKNOWN;}

%%
	/* Codigo do usuario */
int yywrap(void)
{
	return 1;
}

