
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     W_BEGIN = 258,
     W_CHAR = 259,
     W_CONST = 260,
     W_DO = 261,
     W_ELSE = 262,
     W_END = 263,
     W_FOR = 264,
     W_FUNCTION = 265,
     W_IF = 266,
     W_INTEGER = 267,
     W_PROCEDURE = 268,
     W_PROGRAM = 269,
     W_READ = 270,
     W_REAL = 271,
     W_REPEAT = 272,
     W_THEN = 273,
     W_TO = 274,
     W_UNTIL = 275,
     W_VAR = 276,
     W_WHILE = 277,
     W_WRITE = 278,
     COLON = 279,
     SEMICOLON = 280,
     DOT = 281,
     COMMA = 282,
     OPAR = 283,
     CPAR = 284,
     ATR = 285,
     GOE = 286,
     LOE = 287,
     DIFFERENT = 288,
     EQUAL = 289,
     GR = 290,
     LE = 291,
     PLUS = 292,
     MINUS = 293,
     MULT = 294,
     DIV = 295,
     IDENT = 296,
     INTEGER = 297,
     REAL = 298,
     CHAR = 299,
     UNKNOWN = 300
   };
#endif
/* Tokens.  */
#define W_BEGIN 258
#define W_CHAR 259
#define W_CONST 260
#define W_DO 261
#define W_ELSE 262
#define W_END 263
#define W_FOR 264
#define W_FUNCTION 265
#define W_IF 266
#define W_INTEGER 267
#define W_PROCEDURE 268
#define W_PROGRAM 269
#define W_READ 270
#define W_REAL 271
#define W_REPEAT 272
#define W_THEN 273
#define W_TO 274
#define W_UNTIL 275
#define W_VAR 276
#define W_WHILE 277
#define W_WRITE 278
#define COLON 279
#define SEMICOLON 280
#define DOT 281
#define COMMA 282
#define OPAR 283
#define CPAR 284
#define ATR 285
#define GOE 286
#define LOE 287
#define DIFFERENT 288
#define EQUAL 289
#define GR 290
#define LE 291
#define PLUS 292
#define MINUS 293
#define MULT 294
#define DIV 295
#define IDENT 296
#define INTEGER 297
#define REAL 298
#define CHAR 299
#define UNKNOWN 300




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 32 "lalg.y"

	int intval;       /* valor inteiro   */
	double floatval;  /* valor real      */
	char charval;     /* valor caractere */
	char *strval;     /* valor texto     */



/* Line 1676 of yacc.c  */
#line 151 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

extern YYLTYPE yylloc;

