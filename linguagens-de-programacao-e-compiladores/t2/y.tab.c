
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 1



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 4 "lalg.y"

	
	#include <stdlib.h>
	#include <stdio.h>
	#include <stdarg.h>

	extern int yylex(); /* funcao define no lalg.l */

	void yyerror(char *s, ...);

	/* Tabela de simbolos */
	/* ... fazer aqui (e' da parte semantica) ... */
	


/* Line 189 of yacc.c  */
#line 89 "y.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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

/* Line 214 of yacc.c  */
#line 32 "lalg.y"

	int intval;       /* valor inteiro   */
	double floatval;  /* valor real      */
	char charval;     /* valor caractere */
	char *strval;     /* valor texto     */



/* Line 214 of yacc.c  */
#line 224 "y.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

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


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 249 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  7
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   332

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  33
/* YYNRULES -- Number of rules.  */
#define YYNRULES  119
/* YYNRULES -- Number of states.  */
#define YYNSTATES  281

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   300

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     9,    15,    21,    27,    33,    39,    44,
      49,    54,    61,    62,    69,    76,    83,    90,    91,    98,
     105,   112,   114,   116,   118,   121,   124,   127,   128,   135,
     136,   143,   150,   158,   159,   167,   175,   183,   191,   195,
     196,   200,   205,   210,   215,   220,   224,   227,   228,   234,
     240,   246,   252,   254,   258,   259,   263,   266,   269,   270,
     273,   276,   277,   280,   284,   285,   290,   295,   302,   308,
     312,   316,   319,   323,   328,   339,   344,   349,   354,   359,
     364,   368,   372,   375,   379,   383,   388,   393,   398,   409,
     420,   431,   442,   446,   448,   450,   452,   454,   456,   458,
     461,   463,   465,   466,   470,   471,   473,   475,   479,   483,
     484,   486,   488,   490,   492,   494,   496,   500,   502,   504
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      47,     0,    -1,    14,    41,    25,    48,    26,    -1,     1,
      41,    25,    48,    26,    -1,    14,     1,    25,    48,    26,
      -1,    14,    41,     1,    48,    26,    -1,    14,    41,    25,
       1,    26,    -1,    14,    41,    25,    48,     1,    -1,    49,
       3,    66,     8,    -1,    49,     3,    66,     1,    -1,    50,
      51,    55,    56,    -1,     5,    41,    34,    78,    25,    50,
      -1,    -1,     1,    41,    34,    78,    25,    50,    -1,     5,
      41,     1,    78,    25,    50,    -1,     5,    41,    34,    78,
       1,    50,    -1,    21,    53,    24,    52,    25,    51,    -1,
      -1,    21,     1,    24,    52,    25,    51,    -1,    21,    53,
       1,    52,    25,    51,    -1,    21,    53,    24,     1,    25,
      51,    -1,    16,    -1,    12,    -1,     4,    -1,    41,    54,
      -1,     1,    25,    -1,    27,    53,    -1,    -1,    13,    41,
      57,    25,    60,    55,    -1,    -1,    13,     1,    57,    25,
      60,    55,    -1,    13,    41,     1,    25,    60,    55,    -1,
      10,    41,    57,    24,    52,    25,    56,    -1,    -1,    10,
       1,    57,    24,    52,    25,    56,    -1,    10,    41,    57,
      24,     1,    25,    56,    -1,    10,    41,    57,    24,    52,
       1,    56,    -1,    10,    41,    57,    24,    52,     1,    25,
      -1,    28,    58,    29,    -1,    -1,    28,     1,    29,    -1,
      53,    24,    52,    59,    -1,     1,    24,    52,    59,    -1,
      53,     1,    52,    59,    -1,    53,    24,     1,    59,    -1,
      53,     1,    59,    -1,    25,    58,    -1,    -1,    61,     3,
      66,     8,    25,    -1,    61,     3,    66,     1,    25,    -1,
      61,     3,    66,     1,     3,    -1,    61,     3,    66,     8,
       1,    -1,    51,    -1,    28,    63,    29,    -1,    -1,    28,
       1,    29,    -1,    41,    64,    -1,    25,    63,    -1,    -1,
       1,    63,    -1,     7,    67,    -1,    -1,     7,     1,    -1,
      67,    25,    66,    -1,    -1,    15,    28,    53,    29,    -1,
      23,    28,    53,    29,    -1,    22,    28,    68,    29,     6,
      67,    -1,    11,    68,    18,    67,    65,    -1,    41,    30,
      70,    -1,    41,    30,    44,    -1,    41,    62,    -1,     3,
      66,     8,    -1,    17,    66,    20,    68,    -1,     9,    41,
      30,    70,    19,    42,     6,     3,    66,     8,    -1,    15,
       1,    53,    29,    -1,    15,    28,     1,    29,    -1,    23,
       1,    53,    29,    -1,    23,    28,     1,    29,    -1,    11,
      68,    18,     1,    -1,    41,     1,    70,    -1,    41,     1,
      44,    -1,    41,     1,    -1,     3,    66,     1,    -1,     3,
      66,    25,    -1,    17,    66,     1,    68,    -1,    17,    66,
       1,    25,    -1,    17,    66,     1,     8,    -1,     9,     1,
      30,    70,    19,    42,     6,     3,    66,     8,    -1,     9,
      41,     1,    70,    19,    42,     6,     3,    66,     8,    -1,
       9,    41,    30,    70,    19,     1,     6,     3,    66,     8,
      -1,     9,    41,    30,    70,    19,    42,     1,     3,    66,
       8,    -1,    70,    69,    70,    -1,    34,    -1,    33,    -1,
      31,    -1,    32,    -1,    35,    -1,    36,    -1,    74,    72,
      -1,    37,    -1,    38,    -1,    -1,    73,    74,    72,    -1,
      -1,    37,    -1,    38,    -1,    71,    77,    75,    -1,    76,
      77,    75,    -1,    -1,     1,    -1,    39,    -1,    40,    -1,
       1,    -1,    41,    -1,    78,    -1,    28,    70,    29,    -1,
       1,    -1,    42,    -1,    43,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    96,    96,    98,    99,   100,   101,   102,   106,   108,
     112,   116,   117,   119,   120,   121,   125,   126,   128,   129,
     130,   134,   135,   136,   140,   142,   146,   147,   151,   152,
     154,   155,   159,   160,   162,   163,   164,   165,   169,   170,
     172,   176,   178,   179,   180,   181,   185,   186,   190,   192,
     193,   194,   198,   202,   203,   205,   209,   213,   214,   216,
     220,   221,   223,   227,   228,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   243,   244,   246,   247,   249,
     251,   252,   253,   255,   256,   258,   259,   260,   262,   263,
     264,   265,   269,   273,   274,   275,   276,   277,   278,   282,
     286,   287,   288,   292,   293,   297,   298,   302,   306,   307,
     309,   313,   314,   316,   320,   321,   322,   324,   328,   329
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "\"begin\"", "\"char\"", "\"const\"",
  "\"do\"", "\"else\"", "\"end\"", "\"for\"", "\"function\"", "\"if\"",
  "\"integer\"", "\"procedure\"", "\"program\"", "\"read\"", "\"real\"",
  "\"repeat\"", "\"then\"", "\"to\"", "\"until\"", "\"var\"", "\"while\"",
  "\"write\"", "\":\"", "\";\"", "\".\"", "\",\"", "\"(\"", "\")\"",
  "\":=\"", "\">=\"", "\"<=\"", "\"<>\"", "\"=\"", "\">\"", "\"<\"",
  "\"+\"", "\"-\"", "\"*\"", "\"/\"", "\"identificador\"",
  "\"valor inteiro\"", "\"valor real\"", "\"caractere\"",
  "\"token desconhecido\"", "$accept", "programa", "corpo", "dc", "dc_c",
  "dc_v", "tipo_var", "variaveis", "mais_var", "dc_p", "dc_func",
  "parametros", "lista_par", "mais_par", "corpo_p", "dc_loc", "lista_arg",
  "argumentos", "mais_ident", "pfalsa", "comandos", "cmd", "condicao",
  "relacao", "expressao", "op_un", "outros_termos", "op_ad", "termo",
  "mais_fatores", "op_mul", "fator", "numero", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    46,    47,    47,    47,    47,    47,    47,    48,    48,
      49,    50,    50,    50,    50,    50,    51,    51,    51,    51,
      51,    52,    52,    52,    53,    53,    54,    54,    55,    55,
      55,    55,    56,    56,    56,    56,    56,    56,    57,    57,
      57,    58,    58,    58,    58,    58,    59,    59,    60,    60,
      60,    60,    61,    62,    62,    62,    63,    64,    64,    64,
      65,    65,    65,    66,    66,    67,    67,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    67,    68,    69,    69,    69,    69,    69,    69,    70,
      71,    71,    71,    72,    72,    73,    73,    74,    75,    75,
      75,    76,    76,    76,    77,    77,    77,    77,    78,    78
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     5,     5,     5,     5,     5,     5,     4,     4,
       4,     6,     0,     6,     6,     6,     6,     0,     6,     6,
       6,     1,     1,     1,     2,     2,     2,     0,     6,     0,
       6,     6,     7,     0,     7,     7,     7,     7,     3,     0,
       3,     4,     4,     4,     4,     3,     2,     0,     5,     5,
       5,     5,     1,     3,     0,     3,     2,     2,     0,     2,
       2,     0,     2,     3,     0,     4,     4,     6,     5,     3,
       3,     2,     3,     4,    10,     4,     4,     4,     4,     4,
       3,     3,     2,     3,     3,     4,     4,     4,    10,    10,
      10,    10,     3,     1,     1,     1,     1,     1,     1,     2,
       1,     1,     0,     3,     0,     1,     1,     3,     3,     0,
       1,     1,     1,     1,     1,     1,     3,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     1,     0,     0,
       0,     0,     0,     0,     0,     0,    17,     0,     0,     0,
       0,     0,     0,     3,    64,     0,    29,     4,     5,     6,
       7,     2,     0,     0,     0,    64,     0,   102,     0,    64,
       0,     0,     0,     0,     0,     0,    27,     0,     0,    33,
     118,   119,     0,     0,     0,     0,     0,     0,   100,   101,
       0,     0,     0,   104,     0,     0,     0,   102,     0,     0,
     102,     0,   102,    71,     9,     8,    64,     0,    25,     0,
      24,     0,     0,    39,     0,     0,    10,     0,     0,     0,
       0,    83,    72,    84,   102,   102,   102,     0,    95,    96,
      94,    93,    97,    98,   102,   117,   102,   114,     0,   115,
     105,   106,    99,   102,     0,     0,     0,     0,   102,   102,
       0,     0,     0,     0,    81,    80,     0,     0,     0,    70,
      69,    63,    23,    22,    21,     0,    26,     0,     0,     0,
       0,     0,     0,     0,    39,    39,    13,    14,    15,    11,
       0,     0,     0,    79,    61,    92,     0,   110,   111,   112,
     107,     0,   104,    75,    76,    65,    87,    86,    85,    73,
       0,    77,    78,    66,    55,     0,     0,    56,    53,    17,
      17,    17,    17,     0,     0,     0,    17,    17,    17,     0,
       0,     0,     0,     0,     0,    68,   116,     0,   103,     0,
      59,    57,    18,    19,    20,    16,     0,    40,    47,     0,
      38,    52,    29,     0,    29,    29,     0,     0,     0,     0,
       0,     0,    62,    60,   108,    67,    47,     0,    47,    45,
      47,    47,    30,    64,    31,    28,     0,     0,     0,     0,
       0,     0,     0,     0,    42,     0,    46,    43,    44,    41,
       0,    33,    33,    33,    33,    64,    64,    64,    64,    64,
       0,     0,    34,    35,    37,    36,    32,     0,     0,     0,
       0,     0,    50,    49,    51,    48,    88,    89,    90,    91,
      74
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,    14,    15,    16,   211,   135,   184,    80,    49,
      86,   141,   185,   229,   212,   213,    73,   128,   177,   195,
      43,    44,    60,   104,    61,    62,   112,   113,    63,   160,
     161,   108,   109
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -93
static const yytype_int16 yypact[] =
{
     176,   -20,     8,    23,    61,    74,    97,   -93,   204,   204,
     204,   225,    79,    90,   130,   210,   151,   201,   211,    68,
      82,   222,    44,   -93,    78,    27,   232,   -93,   -93,   -93,
     -93,   -93,    49,    49,    49,    78,    28,   228,     9,    78,
     239,    96,   133,    12,   247,   194,   251,   169,    33,   267,
     -93,   -93,   254,   255,   158,   161,   252,    81,   -93,   -93,
     263,   208,    11,   231,    34,    39,    13,   228,    34,    47,
      98,    63,   138,   -93,   -93,   -93,    78,   199,   -93,    34,
     -93,   199,    15,   256,   116,    69,   -93,   204,   204,   204,
     204,   -93,   -93,   -93,   228,   228,   228,    21,   -93,   -93,
     -93,   -93,   -93,   -93,   228,   -93,   228,   -93,   114,   -93,
     -93,   -93,   -93,   228,   258,   257,   181,   259,    88,   228,
     260,   261,   207,   262,   -93,   -93,   264,    89,   265,   -93,
     -93,   -93,   -93,   -93,   -93,   270,   -93,   271,   272,   273,
      71,   274,   275,   276,   256,   256,   -93,   -93,   -93,   -93,
     266,   268,   283,   -93,   285,   -93,   277,    14,   -93,   -93,
     -93,    11,   231,   -93,   -93,   -93,   -93,   -93,   -93,   -93,
     297,   -93,   -93,   -93,   -93,   269,   269,   -93,   -93,   151,
     151,   151,   151,   149,   170,   278,   151,   151,   151,   280,
     281,   279,   282,    16,    62,   -93,   -93,   114,   -93,    78,
     -93,   -93,   -93,   -93,   -93,   -93,   199,   -93,   183,   184,
     -93,   -93,   232,   305,   232,   232,   199,   200,   303,   306,
     307,   196,   -93,   -93,   -93,   -93,   286,    75,   286,   -93,
     286,   286,   -93,    78,   -93,   -93,   289,   290,   159,   313,
     314,   315,   316,   317,   -93,   246,   -93,   -93,   -93,   -93,
      17,   267,   267,     1,   267,    78,    78,    78,    78,    78,
     103,   167,   -93,   -93,   -93,   -93,   -93,   318,   319,   320,
     321,   322,   -93,   -93,   -93,   -93,   -93,   -93,   -93,   -93,
     -93
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -93,   -93,   250,   -93,   160,   -15,   -79,   -18,   -93,    43,
       0,   -78,    95,     3,    86,   -93,   -93,   100,   -93,   -93,
     -35,   -92,   -59,   -93,    85,   -93,   163,   -93,   218,   126,
     -93,   171,   230
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -114
static const yytype_int16 yytable[] =
{
      55,    26,   137,   139,    66,   154,   143,    47,   120,     5,
      64,    85,   105,    74,   118,  -113,   138,   220,   260,   132,
      75,     4,   153,     7,    35,   261,   264,   133,    45,    56,
      36,   134,    37,   119,    83,   114,    38,    65,    39,   106,
     116,   131,  -113,    40,    41,    33,   115,   117,   122,     6,
     121,   123,   107,    50,    51,  -113,  -113,  -113,   221,   168,
     169,   136,    42,   222,   126,    35,   189,   190,    46,    57,
     144,    36,   183,    37,    84,    46,   245,    38,    34,    39,
      46,    35,    95,    30,    40,    41,     8,    36,    46,    37,
     175,    50,    51,    38,    29,    39,   166,    68,    10,     9,
      40,    41,   223,    42,   127,   -82,   272,   225,    31,    21,
     145,    96,    46,   167,   176,   157,    46,   142,   -58,    42,
      21,  -109,    11,   -82,    69,    58,    59,   226,   273,   228,
     231,    22,  -109,  -109,    70,    58,    59,   236,   238,  -109,
     -54,   -39,   124,  -109,   140,  -109,  -109,  -109,  -109,  -109,
    -109,  -109,  -109,   158,   159,   125,    23,   130,   -54,    89,
     253,    71,    91,    72,   202,   203,   204,   205,   274,    92,
      81,   208,    25,   206,    78,    58,    59,     1,   207,   150,
     151,   152,   129,    90,   254,   230,    93,   132,   132,   155,
       2,   156,   275,    82,   209,   133,   133,   242,   250,   134,
     134,   237,   243,   132,   132,    12,    78,   -12,   227,    13,
     164,   133,   133,    24,   -12,   134,   134,   -12,    77,    78,
     267,   268,   269,   270,   271,   -12,    19,    27,   -12,   244,
      13,   247,    78,   248,   249,   -12,   172,    28,   -12,    98,
      99,   100,   101,   102,   103,    48,   -12,   146,   147,   148,
     149,   262,   263,   265,   266,   232,    32,   234,   235,    17,
      18,    20,    52,    53,    54,    58,    59,    67,   110,   111,
     206,    78,    76,   214,   215,   200,   201,    85,    79,    87,
      88,    97,    94,    78,   140,   191,   163,   192,   165,   170,
     171,   173,   194,   174,   178,   179,   180,   181,   182,   186,
     187,   188,   193,   199,   216,   217,   196,   210,   233,   239,
     127,   227,   240,   241,   251,   252,   255,   256,   257,   258,
     259,   218,   246,   224,   219,   198,   276,   277,   278,   279,
     280,   162,   197
};

static const yytype_uint16 yycheck[] =
{
      35,    16,    81,    82,    39,    97,    84,    25,    67,     1,
       1,    10,     1,     1,     1,     1,     1,     1,     1,     4,
       8,    41,     1,     0,     3,     8,    25,    12,     1,     1,
       9,    16,    11,    20,     1,     1,    15,    28,    17,    28,
       1,    76,    28,    22,    23,     1,    64,    65,     1,    41,
      68,    69,    41,    42,    43,    41,    42,    43,    42,   118,
     119,    79,    41,     1,     1,     3,   144,   145,    41,    41,
       1,     9,     1,    11,    41,    41,     1,    15,    34,    17,
      41,     3,     1,     1,    22,    23,    25,     9,    41,    11,
       1,    42,    43,    15,    26,    17,     8,     1,     1,    25,
      22,    23,   194,    41,    41,     7,     3,   199,    26,    41,
      41,    30,    41,    25,    25,     1,    41,     1,    29,    41,
      41,     7,    25,    25,    28,    37,    38,   206,    25,   208,
     209,    41,    18,    19,     1,    37,    38,   216,   217,    25,
       7,    25,    44,    29,    28,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    70,    26,    72,    25,     1,
       1,    28,     1,    30,   179,   180,   181,   182,     1,     8,
       1,     1,    21,    24,    25,    37,    38,     1,    29,    94,
      95,    96,    44,    25,    25,     1,    25,     4,     4,   104,
      14,   106,    25,    24,    24,    12,    12,     1,   233,    16,
      16,     1,     6,     4,     4,     1,    25,     3,    25,     5,
      29,    12,    12,     3,    10,    16,    16,    13,    24,    25,
     255,   256,   257,   258,   259,    21,     1,    26,     3,   226,
       5,   228,    25,   230,   231,    10,    29,    26,    13,    31,
      32,    33,    34,    35,    36,    13,    21,    87,    88,    89,
      90,   251,   252,   253,   254,   212,    34,   214,   215,     9,
      10,    11,    32,    33,    34,    37,    38,    28,    37,    38,
      24,    25,    25,   187,   188,   175,   176,    10,    27,    25,
      25,    18,    30,    25,    28,    19,    29,    19,    29,    29,
      29,    29,     7,    29,    29,    25,    25,    25,    25,    25,
      25,    25,    19,     6,    24,    24,    29,    29,     3,     6,
      41,    25,     6,     6,    25,    25,     3,     3,     3,     3,
       3,    42,   227,   197,    42,   162,     8,     8,     8,     8,
       8,   113,   161
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,    14,    47,    41,     1,    41,     0,    25,    25,
       1,    25,     1,     5,    48,    49,    50,    48,    48,     1,
      48,    41,    41,    26,     3,    21,    51,    26,    26,    26,
       1,    26,    34,     1,    34,     3,     9,    11,    15,    17,
      22,    23,    41,    66,    67,     1,    41,    53,    13,    55,
      42,    43,    78,    78,    78,    66,     1,    41,    37,    38,
      68,    70,    71,    74,     1,    28,    66,    28,     1,    28,
       1,    28,    30,    62,     1,     8,    25,    24,    25,    27,
      54,     1,    24,     1,    41,    10,    56,    25,    25,     1,
      25,     1,     8,    25,    30,     1,    30,    18,    31,    32,
      33,    34,    35,    36,    69,     1,    28,    41,    77,    78,
      37,    38,    72,    73,     1,    53,     1,    53,     1,    20,
      68,    53,     1,    53,    44,    70,     1,    41,    63,    44,
      70,    66,     4,    12,    16,    52,    53,    52,     1,    52,
      28,    57,     1,    57,     1,    41,    50,    50,    50,    50,
      70,    70,    70,     1,    67,    70,    70,     1,    39,    40,
      75,    76,    74,    29,    29,    29,     8,    25,    68,    68,
      29,    29,    29,    29,    29,     1,    25,    64,    29,    25,
      25,    25,    25,     1,    53,    58,    25,    25,    25,    57,
      57,    19,    19,    19,     7,    65,    29,    77,    72,     6,
      63,    63,    51,    51,    51,    51,    24,    29,     1,    24,
      29,    51,    60,    61,    60,    60,    24,    24,    42,    42,
       1,    42,     1,    67,    75,    67,    52,    25,    52,    59,
       1,    52,    55,     3,    55,    55,    52,     1,    52,     6,
       6,     6,     1,     6,    59,     1,    58,    59,    59,    59,
      66,    25,    25,     1,    25,     3,     3,     3,     3,     3,
       1,     8,    56,    56,    25,    56,    56,    66,    66,    66,
      66,    66,     3,    25,     1,    25,     8,     8,     8,     8,
       8
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.
       `yyls': related to locations.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[2];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;

#if YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 1;
#endif

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);

	yyls = yyls1;
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
	YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 96 "lalg.y"
    { free((yyvsp[(2) - (5)].strval)); }
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 98 "lalg.y"
    { yyerrok; free((yyvsp[(2) - (5)].strval)); }
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 99 "lalg.y"
    { yyerrok; }
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 100 "lalg.y"
    { yyerrok; free((yyvsp[(2) - (5)].strval)); }
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 101 "lalg.y"
    { yyerrok; free((yyvsp[(2) - (5)].strval)); }
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 102 "lalg.y"
    { yyerrok; free((yyvsp[(2) - (5)].strval)); }
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 106 "lalg.y"
    {;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 108 "lalg.y"
    { yyerrok; }
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 112 "lalg.y"
    {;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 116 "lalg.y"
    { free((yyvsp[(2) - (6)].strval));}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 117 "lalg.y"
    {;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 119 "lalg.y"
    { yyerrok; free((yyvsp[(2) - (6)].strval)); }
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 120 "lalg.y"
    { yyerrok; free((yyvsp[(2) - (6)].strval)); }
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 121 "lalg.y"
    { yyerrok; free((yyvsp[(2) - (6)].strval)); }
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 125 "lalg.y"
    {;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 126 "lalg.y"
    {;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 128 "lalg.y"
    { yyerrok; }
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 129 "lalg.y"
    { yyerrok; }
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 130 "lalg.y"
    { yyerrok; }
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 134 "lalg.y"
    {;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 135 "lalg.y"
    {;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 136 "lalg.y"
    {;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 140 "lalg.y"
    { free((yyvsp[(1) - (2)].strval)); }
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 142 "lalg.y"
    { yyerrok; }
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 146 "lalg.y"
    {;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 147 "lalg.y"
    {;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 151 "lalg.y"
    { free((yyvsp[(2) - (6)].strval)); }
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 152 "lalg.y"
    {;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 154 "lalg.y"
    { yyerrok; }
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 155 "lalg.y"
    { yyerrok; free((yyvsp[(2) - (6)].strval)); }
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 159 "lalg.y"
    { free((yyvsp[(2) - (7)].strval)); }
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 160 "lalg.y"
    {;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 162 "lalg.y"
    { yyerrok; }
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 163 "lalg.y"
    { yyerrok; free((yyvsp[(2) - (7)].strval)); }
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 164 "lalg.y"
    { yyerrok; free((yyvsp[(2) - (7)].strval)); }
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 165 "lalg.y"
    { yyerrok; free((yyvsp[(2) - (7)].strval)); }
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 169 "lalg.y"
    {;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 170 "lalg.y"
    {;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 172 "lalg.y"
    { yyerrok; }
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 176 "lalg.y"
    {;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 178 "lalg.y"
    { yyerrok; }
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 179 "lalg.y"
    { yyerrok; }
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 180 "lalg.y"
    { yyerrok; }
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 181 "lalg.y"
    { yyerrok; }
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 185 "lalg.y"
    {;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 186 "lalg.y"
    {;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 190 "lalg.y"
    {;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 192 "lalg.y"
    { yyerrok; }
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 193 "lalg.y"
    { yyerrok; }
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 194 "lalg.y"
    { yyerrok; }
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 198 "lalg.y"
    {;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 202 "lalg.y"
    {;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 203 "lalg.y"
    {;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 205 "lalg.y"
    { yyerrok; }
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 209 "lalg.y"
    { free((yyvsp[(1) - (2)].strval)); }
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 213 "lalg.y"
    {;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 214 "lalg.y"
    {;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 216 "lalg.y"
    { yyerrok; }
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 220 "lalg.y"
    {;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 221 "lalg.y"
    {;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 223 "lalg.y"
    { yyerrok; }
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 227 "lalg.y"
    {;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 228 "lalg.y"
    {;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 232 "lalg.y"
    {;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 233 "lalg.y"
    {;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 234 "lalg.y"
    {;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 235 "lalg.y"
    {;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 236 "lalg.y"
    { free((yyvsp[(1) - (3)].strval)); }
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 237 "lalg.y"
    { free((yyvsp[(1) - (3)].strval)); }
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 238 "lalg.y"
    { free((yyvsp[(1) - (2)].strval)); }
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 239 "lalg.y"
    {;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 240 "lalg.y"
    {;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 241 "lalg.y"
    { free((yyvsp[(2) - (10)].strval)); }
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 243 "lalg.y"
    { yyerrok; }
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 244 "lalg.y"
    { yyerrok; }
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 246 "lalg.y"
    { yyerrok; }
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 247 "lalg.y"
    { yyerrok; }
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 249 "lalg.y"
    { yyerrok; }
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 251 "lalg.y"
    { yyerrok; free((yyvsp[(1) - (3)].strval)); }
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 252 "lalg.y"
    { yyerrok; free((yyvsp[(1) - (3)].strval)); }
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 253 "lalg.y"
    { yyerrok; free((yyvsp[(1) - (2)].strval)); }
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 255 "lalg.y"
    { yyerrok; }
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 256 "lalg.y"
    { yyerrok; }
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 258 "lalg.y"
    { yyerrok; }
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 259 "lalg.y"
    { yyerrok; }
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 260 "lalg.y"
    { yyerrok; }
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 262 "lalg.y"
    { yyerrok; }
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 263 "lalg.y"
    { yyerrok; free((yyvsp[(2) - (10)].strval)); }
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 264 "lalg.y"
    { yyerrok; free((yyvsp[(2) - (10)].strval)); }
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 265 "lalg.y"
    { yyerrok; free((yyvsp[(2) - (10)].strval)); }
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 269 "lalg.y"
    {;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 273 "lalg.y"
    {;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 274 "lalg.y"
    {;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 275 "lalg.y"
    {;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 276 "lalg.y"
    {;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 277 "lalg.y"
    {;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 278 "lalg.y"
    {;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 282 "lalg.y"
    {;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 286 "lalg.y"
    {;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 287 "lalg.y"
    {;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 288 "lalg.y"
    {;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 292 "lalg.y"
    {;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 293 "lalg.y"
    {;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 297 "lalg.y"
    {;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 298 "lalg.y"
    {;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 302 "lalg.y"
    {;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 306 "lalg.y"
    {;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 307 "lalg.y"
    {;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 309 "lalg.y"
    { yyerrok; }
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 313 "lalg.y"
    {;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 314 "lalg.y"
    {;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 316 "lalg.y"
    { yyerrok; }
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 320 "lalg.y"
    { free((yyvsp[(1) - (1)].strval)); }
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 321 "lalg.y"
    {;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 322 "lalg.y"
    {;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 324 "lalg.y"
    { yyerrok; }
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 328 "lalg.y"
    {;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 329 "lalg.y"
    {;}
    break;



/* Line 1455 of yacc.c  */
#line 2560 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }

  yyerror_range[0] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, &yylloc);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[0] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, &yylloc);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 331 "lalg.y"

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

