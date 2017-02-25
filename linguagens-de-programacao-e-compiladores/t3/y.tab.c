
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
#line 84 "lalg.y"

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


/* Line 189 of yacc.c  */
#line 112 "y.tab.c"

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

/* "%code requires" blocks.  */

/* Line 209 of yacc.c  */
#line 5 "lalg.y"


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



/* Line 209 of yacc.c  */
#line 218 "y.tab.c"

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
#line 125 "lalg.y"

	struct data_s data;



/* Line 214 of yacc.c  */
#line 331 "y.tab.c"
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
#line 356 "y.tab.c"

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
#define YYLAST   323

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  45
/* YYNRULES -- Number of rules.  */
#define YYNRULES  134
/* YYNRULES -- Number of states.  */
#define YYNSTATES  296

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
      49,    54,    57,    58,    64,    70,    76,    82,    85,    86,
      92,    98,   104,   110,   112,   114,   116,   119,   122,   123,
     126,   129,   130,   133,   134,   140,   146,   152,   160,   161,
     169,   177,   185,   193,   195,   199,   200,   204,   207,   211,
     215,   219,   223,   226,   229,   230,   236,   242,   248,   254,
     257,   258,   264,   270,   276,   282,   286,   287,   291,   294,
     297,   298,   301,   304,   305,   308,   311,   312,   315,   320,
     325,   332,   338,   342,   346,   349,   353,   358,   369,   374,
     379,   384,   389,   394,   398,   402,   406,   410,   414,   419,
     424,   429,   440,   451,   462,   473,   477,   479,   481,   483,
     485,   487,   489,   492,   494,   496,   497,   500,   501,   504,
     506,   508,   511,   514,   517,   518,   521,   523,   525,   527,
     529,   531,   535,   537,   539
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      47,     0,    -1,    14,    41,    25,    48,    26,    -1,     1,
      41,    25,    48,    26,    -1,    14,     1,    25,    48,    26,
      -1,    14,    41,     1,    48,    26,    -1,    14,    41,    25,
       1,    26,    -1,    14,    41,    25,    48,     1,    -1,    49,
       3,    74,     8,    -1,    49,     3,    74,     1,    -1,    50,
      52,    59,    61,    -1,    51,    50,    -1,    -1,     5,    41,
      34,    90,    25,    -1,     1,    41,    34,    90,    25,    -1,
       5,    41,     1,    90,    25,    -1,     5,    41,    34,    90,
       1,    -1,    53,    52,    -1,    -1,    21,    55,    24,    54,
      25,    -1,    21,     1,    24,    54,    25,    -1,    21,    55,
       1,    54,    25,    -1,    21,    55,    24,     1,    25,    -1,
      16,    -1,    12,    -1,     4,    -1,    41,    56,    -1,    27,
      55,    -1,    -1,    41,    58,    -1,    27,    57,    -1,    -1,
      60,    59,    -1,    -1,    13,    41,    62,    25,    67,    -1,
      13,     1,    62,    25,    67,    -1,    13,    41,     1,    25,
      67,    -1,    10,    41,    62,    24,    54,    25,    61,    -1,
      -1,    10,     1,    62,    24,    54,    25,    61,    -1,    10,
      41,    62,    24,     1,    25,    61,    -1,    10,    41,    62,
      24,    54,     1,    61,    -1,    10,    41,    62,    24,    54,
       1,    25,    -1,    63,    -1,    28,    64,    29,    -1,    -1,
      28,     1,    29,    -1,    65,    66,    -1,    57,    24,    54,
      -1,     1,    24,    54,    -1,    57,     1,    54,    -1,    57,
      24,     1,    -1,    57,     1,    -1,    25,    64,    -1,    -1,
      68,     3,    74,     8,    25,    -1,    68,     3,    74,     1,
      25,    -1,    68,     3,    74,     1,     3,    -1,    68,     3,
      74,     8,     1,    -1,    69,    68,    -1,    -1,    21,    55,
      24,    54,    25,    -1,    21,     1,    24,    54,    25,    -1,
      21,    55,     1,    54,    25,    -1,    21,    55,    24,     1,
      25,    -1,    28,    71,    29,    -1,    -1,    28,     1,    29,
      -1,    41,    72,    -1,    25,    71,    -1,    -1,     1,    71,
      -1,     7,    76,    -1,    -1,     7,     1,    -1,    75,    74,
      -1,    -1,    76,    25,    -1,    15,    28,    55,    29,    -1,
      23,    28,    55,    29,    -1,    22,    28,    77,    29,     6,
      76,    -1,    11,    77,    18,    76,    73,    -1,    41,    30,
      79,    -1,    41,    30,    44,    -1,    41,    70,    -1,     3,
      74,     8,    -1,    17,    74,    20,    77,    -1,     9,    41,
      30,    79,    19,    42,     6,     3,    74,     8,    -1,    15,
       1,    55,    29,    -1,    15,    28,     1,    29,    -1,    23,
       1,    55,    29,    -1,    23,    28,     1,    29,    -1,    11,
      77,    18,     1,    -1,    41,     1,    25,    -1,    41,     1,
      79,    -1,    41,     1,    44,    -1,     3,    74,     1,    -1,
       3,    74,    25,    -1,    17,    74,     1,    77,    -1,    17,
      74,     1,    25,    -1,    17,    74,     1,     8,    -1,     9,
       1,    30,    79,    19,    42,     6,     3,    74,     8,    -1,
       9,    41,     1,    79,    19,    42,     6,     3,    74,     8,
      -1,     9,    41,    30,    79,    19,     1,     6,     3,    74,
       8,    -1,     9,    41,    30,    79,    19,    42,     1,     3,
      74,     8,    -1,    79,    78,    79,    -1,    34,    -1,    33,
      -1,    31,    -1,    32,    -1,    35,    -1,    36,    -1,    84,
      81,    -1,    37,    -1,    38,    -1,    -1,    82,    81,    -1,
      -1,    83,    84,    -1,    37,    -1,    38,    -1,    85,    86,
      -1,    80,    89,    -1,    87,    86,    -1,    -1,    88,    89,
      -1,     1,    -1,    39,    -1,    40,    -1,    41,    -1,    90,
      -1,    28,    79,    29,    -1,     1,    -1,    42,    -1,    43,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   210,   210,   220,   221,   222,   223,   224,   228,   230,
     234,   237,   238,   241,   247,   248,   249,   252,   253,   256,
     261,   262,   263,   267,   268,   269,   273,   280,   281,   286,
     293,   294,   297,   298,   302,   308,   309,   313,   314,   316,
     317,   318,   319,   322,   328,   329,   331,   334,   338,   342,
     343,   344,   345,   349,   350,   354,   356,   357,   358,   362,
     363,   368,   372,   373,   374,   378,   379,   381,   385,   393,
     394,   396,   400,   401,   403,   407,   408,   411,   415,   418,
     421,   422,   423,   429,   434,   440,   441,   442,   447,   448,
     450,   451,   453,   455,   456,   457,   459,   460,   462,   463,
     464,   466,   467,   468,   469,   473,   477,   478,   479,   480,
     481,   482,   486,   516,   517,   518,   521,   548,   551,   562,
     563,   566,   603,   617,   651,   654,   664,   668,   669,   673,
     674,   675,   677,   681,   682
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
  "dc_c0", "dc_v", "dc_v0", "tipo_var", "variaveis", "mais_var",
  "variaveis0", "mais_var0", "dc_p", "dc_p0", "dc_func", "parametros",
  "parametros0", "lista_par", "lista_par0", "mais_par", "corpo_p",
  "dc_loc", "dc_loc0", "lista_arg", "argumentos", "mais_ident", "pfalsa",
  "comandos", "comandos0", "cmd", "condicao", "relacao", "expressao",
  "op_un", "outros_termos", "outros_termos0", "op_ad", "termo", "termo0",
  "mais_fatores", "mais_fatores0", "op_mul", "fator", "numero", 0
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
      49,    50,    50,    51,    51,    51,    51,    52,    52,    53,
      53,    53,    53,    54,    54,    54,    55,    56,    56,    57,
      58,    58,    59,    59,    60,    60,    60,    61,    61,    61,
      61,    61,    61,    62,    63,    63,    63,    64,    65,    65,
      65,    65,    65,    66,    66,    67,    67,    67,    67,    68,
      68,    69,    69,    69,    69,    70,    70,    70,    71,    72,
      72,    72,    73,    73,    73,    74,    74,    75,    76,    76,
      76,    76,    76,    76,    76,    76,    76,    76,    76,    76,
      76,    76,    76,    76,    76,    76,    76,    76,    76,    76,
      76,    76,    76,    76,    76,    77,    78,    78,    78,    78,
      78,    78,    79,    80,    80,    80,    81,    81,    82,    83,
      83,    84,    85,    86,    86,    87,    87,    88,    88,    89,
      89,    89,    89,    90,    90
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     5,     5,     5,     5,     5,     5,     4,     4,
       4,     2,     0,     5,     5,     5,     5,     2,     0,     5,
       5,     5,     5,     1,     1,     1,     2,     2,     0,     2,
       2,     0,     2,     0,     5,     5,     5,     7,     0,     7,
       7,     7,     7,     1,     3,     0,     3,     2,     3,     3,
       3,     3,     2,     2,     0,     5,     5,     5,     5,     2,
       0,     5,     5,     5,     5,     3,     0,     3,     2,     2,
       0,     2,     2,     0,     2,     2,     0,     2,     4,     4,
       6,     5,     3,     3,     2,     3,     4,    10,     4,     4,
       4,     4,     4,     3,     3,     3,     3,     3,     4,     4,
       4,    10,    10,    10,    10,     3,     1,     1,     1,     1,
       1,     1,     2,     1,     1,     0,     2,     0,     2,     1,
       1,     2,     2,     2,     0,     2,     1,     1,     1,     1,
       1,     3,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     1,     0,     0,
       0,     0,     0,     0,     0,     0,    18,     0,     0,     0,
       0,     0,     0,     0,     3,    76,     0,    33,    18,    11,
       4,     5,     6,     7,     2,     0,     0,     0,    76,     0,
     115,     0,    76,     0,     0,     0,     0,    76,     0,     0,
      28,     0,     0,    38,    33,    17,   133,   134,     0,     0,
       0,     0,     0,     0,   113,   114,     0,     0,     0,   117,
       0,     0,     0,     0,   115,     0,     0,   115,     0,   115,
      84,     9,     8,    75,    77,     0,     0,    26,     0,     0,
      45,     0,     0,    10,    32,    14,    15,    16,    13,    96,
      85,    97,   115,   115,   115,     0,   108,   109,   107,   106,
     110,   111,   115,   132,   115,   129,   122,   130,   119,   120,
     112,   117,   115,   126,   127,   128,   121,     0,     0,     0,
       0,     0,   115,   115,     0,     0,     0,     0,    93,    95,
      94,     0,     0,     0,    83,    82,    25,    24,    23,     0,
      27,     0,     0,     0,     0,     0,    43,     0,     0,    45,
      45,     0,     0,     0,    92,    73,   105,     0,   116,   118,
     123,   125,    88,    89,    78,   100,    99,    98,    86,     0,
      90,    91,    79,    67,     0,     0,    68,    65,    20,    21,
      22,    19,     0,    31,     0,     0,    54,    60,    60,    60,
       0,     0,     0,     0,     0,     0,    81,   131,     0,    71,
      69,     0,    46,     0,    29,    52,     0,    44,     0,    47,
       0,    35,     0,    60,    36,    34,     0,     0,     0,     0,
       0,     0,    74,    72,    80,    49,    30,    50,    51,    48,
       0,    53,     0,     0,    76,    59,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    38,    38,
      38,    38,    76,    76,    76,    76,    76,     0,     0,     0,
       0,     0,     0,    39,    40,    42,    41,    37,     0,     0,
       0,     0,     0,    62,    63,    64,    61,    57,    56,    58,
      55,   101,   102,   103,   104,    87
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,    14,    15,    16,    17,    27,    28,   149,    51,
      87,   194,   214,    53,    54,    93,   155,   156,   195,   196,
     219,   221,   222,   223,    80,   143,   186,   206,    46,    47,
      48,    66,   112,    67,    68,   120,   121,   122,    69,    70,
     126,   127,   128,   116,   117
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -103
static const yytype_int16 yypact[] =
{
     152,   -26,    19,    17,     0,     2,    85,  -103,   169,   169,
     169,   175,    -8,     4,    51,   102,   139,   169,   107,   177,
     116,    71,   149,    12,  -103,    76,    21,   199,   139,  -103,
    -103,  -103,  -103,  -103,  -103,   168,   168,   168,    76,    27,
     206,    30,    76,   211,    33,    93,   157,    76,   193,   221,
     225,   103,    28,   243,   199,  -103,  -103,  -103,   229,   230,
      89,   101,   226,    18,  -103,  -103,   239,   198,    11,   209,
     112,   217,    34,   155,   206,   217,    35,   156,    39,    78,
    -103,  -103,  -103,  -103,  -103,   204,   217,  -103,   204,   183,
     231,    22,    40,  -103,  -103,  -103,  -103,  -103,  -103,  -103,
    -103,  -103,   206,   206,   206,    15,  -103,  -103,  -103,  -103,
    -103,  -103,   206,  -103,   206,  -103,  -103,  -103,  -103,  -103,
    -103,   209,   206,  -103,  -103,  -103,  -103,   112,    11,   232,
     233,   234,    70,   206,   235,   236,   237,   238,  -103,  -103,
    -103,   240,    63,   241,  -103,  -103,  -103,  -103,  -103,   246,
    -103,   247,   248,   249,    41,   250,  -103,   251,   252,   231,
     231,   259,   260,   261,  -103,   253,  -103,   254,  -103,  -103,
    -103,  -103,  -103,  -103,  -103,  -103,  -103,  -103,  -103,   262,
    -103,  -103,  -103,  -103,   244,   244,  -103,  -103,  -103,  -103,
    -103,  -103,   135,   255,   110,   257,   256,   263,   263,   263,
     264,   265,   245,   258,    13,    48,  -103,  -103,    76,  -103,
    -103,   204,  -103,   266,  -103,   204,   185,  -103,    42,  -103,
      43,  -103,   287,   263,  -103,  -103,   204,   201,   285,   286,
     288,   208,  -103,  -103,  -103,  -103,  -103,  -103,  -103,  -103,
     269,  -103,   271,   111,    76,  -103,   272,   273,    95,   293,
     296,   298,   299,   300,   204,   204,   203,   190,   243,   243,
     167,   243,    76,    76,    76,    76,    76,   279,   280,   281,
     283,   129,   100,  -103,  -103,  -103,  -103,  -103,   301,   302,
     303,   304,   305,  -103,  -103,  -103,  -103,  -103,  -103,  -103,
    -103,  -103,  -103,  -103,  -103,  -103
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -103,  -103,   212,  -103,   297,  -103,   289,  -103,   -87,   -65,
    -103,   105,  -103,   267,  -103,   -23,   -86,  -103,    97,  -103,
    -103,    50,    96,  -103,  -103,    66,  -103,  -103,   -38,  -103,
    -102,   -66,  -103,    59,  -103,   195,  -103,  -103,   200,  -103,
     196,  -103,  -103,   192,   205
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -125
static const yytype_int16 yytable[] =
{
      61,   151,   153,   165,    73,   158,   129,   131,   134,    83,
     135,   137,   113,    36,   230,     4,   164,     7,    38,   103,
       5,   150,    49,   157,    39,     8,    40,     9,    62,    90,
      41,    71,    42,    22,    75,   130,   136,    43,    44,   114,
     141,   159,   192,   240,   242,    23,    37,   -45,   104,   232,
     154,    38,   115,    56,    57,   231,    45,    39,    72,    40,
       6,    76,    50,    41,   184,    42,   177,   178,    63,    91,
      43,    44,    33,   200,   201,    50,    50,    24,   175,    38,
     142,   160,   193,   193,    50,    39,    10,    40,   185,    45,
      97,    41,   -70,    42,    77,   176,   260,    34,    43,    44,
     -66,   289,    99,   233,    88,    25,   234,    64,    65,   100,
      11,   215,   255,   123,    98,    64,    65,    45,   -66,  -124,
     261,    78,   144,    79,   235,   290,   101,    89,   237,   239,
    -124,  -124,   287,    30,   216,   256,   140,  -124,   145,   246,
     248,  -124,    32,  -124,  -124,  -124,  -124,  -124,  -124,  -124,
    -124,   124,   125,     1,   288,   243,   132,    22,    81,   211,
      26,   161,   162,   163,   212,    82,     2,   267,   268,   270,
      12,   166,   -12,   167,    13,   133,    20,    92,   -12,   -12,
      13,   138,   -12,    35,   152,   -12,   238,   146,   -12,   146,
     -12,   271,   275,    64,    65,   147,   -12,   147,   272,   148,
     139,   148,   247,    31,   269,   146,   257,   146,   146,   252,
      56,    57,    52,   147,   253,   147,   147,   148,    84,   148,
     148,    18,    19,    21,   278,   279,   280,   281,   282,   106,
     107,   108,   109,   110,   111,   273,   274,   276,   277,    74,
      58,    59,    60,    64,    65,    85,   118,   119,   224,   225,
     209,   210,    86,    92,    95,    96,   102,   105,    50,   154,
     205,   172,   173,   174,   179,   180,   181,   182,   208,   183,
     187,   188,   189,   190,   191,   197,   198,   199,   202,   203,
     204,   218,   213,   207,   220,   142,   217,   228,   226,   227,
     244,   249,   250,   211,   251,   254,   262,   258,   259,   263,
     229,   264,   265,   266,   283,   284,   285,   193,   286,   291,
     292,   293,   294,   295,    29,   241,   168,    55,   236,   245,
     171,    94,   169,   170
};

static const yytype_uint16 yycheck[] =
{
      38,    88,    89,   105,    42,    91,    71,    72,    74,    47,
      75,    76,     1,     1,     1,    41,     1,     0,     3,     1,
       1,    86,     1,     1,     9,    25,    11,    25,     1,     1,
      15,     1,    17,    41,     1,     1,     1,    22,    23,    28,
       1,     1,     1,     1,     1,    41,    34,    25,    30,     1,
      28,     3,    41,    42,    43,    42,    41,     9,    28,    11,
      41,    28,    41,    15,     1,    17,   132,   133,    41,    41,
      22,    23,     1,   159,   160,    41,    41,    26,     8,     3,
      41,    41,    41,    41,    41,     9,     1,    11,    25,    41,
       1,    15,    29,    17,     1,    25,     1,    26,    22,    23,
       7,     1,     1,   205,     1,     3,   208,    37,    38,     8,
      25,     1,     1,     1,    25,    37,    38,    41,    25,     7,
      25,    28,    44,    30,   211,    25,    25,    24,   215,   216,
      18,    19,     3,    26,    24,    24,    77,    25,    79,   226,
     227,    29,    26,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,     1,    25,   220,     1,    41,     1,    24,
      21,   102,   103,   104,    29,     8,    14,   254,   255,   256,
       1,   112,     3,   114,     5,    20,     1,    10,     3,    10,
       5,    25,    13,    34,     1,    10,     1,     4,    13,     4,
      21,     1,    25,    37,    38,    12,    21,    12,     8,    16,
      44,    16,     1,    26,     1,     4,   244,     4,     4,     1,
      42,    43,    13,    12,     6,    12,    12,    16,    25,    16,
      16,     9,    10,    11,   262,   263,   264,   265,   266,    31,
      32,    33,    34,    35,    36,   258,   259,   260,   261,    28,
      35,    36,    37,    37,    38,    24,    37,    38,   198,   199,
     184,   185,    27,    10,    25,    25,    30,    18,    41,    28,
       7,    29,    29,    29,    29,    29,    29,    29,     6,    29,
      29,    25,    25,    25,    25,    25,    25,    25,    19,    19,
      19,    25,    27,    29,    21,    41,    29,    42,    24,    24,
       3,     6,     6,    24,     6,    24,     3,    25,    25,     3,
      42,     3,     3,     3,    25,    25,    25,    41,    25,     8,
       8,     8,     8,     8,    17,   218,   121,    28,   213,   223,
     128,    54,   122,   127
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,    14,    47,    41,     1,    41,     0,    25,    25,
       1,    25,     1,     5,    48,    49,    50,    51,    48,    48,
       1,    48,    41,    41,    26,     3,    21,    52,    53,    50,
      26,    26,    26,     1,    26,    34,     1,    34,     3,     9,
      11,    15,    17,    22,    23,    41,    74,    75,    76,     1,
      41,    55,    13,    59,    60,    52,    42,    43,    90,    90,
      90,    74,     1,    41,    37,    38,    77,    79,    80,    84,
      85,     1,    28,    74,    28,     1,    28,     1,    28,    30,
      70,     1,     8,    74,    25,    24,    27,    56,     1,    24,
       1,    41,    10,    61,    59,    25,    25,     1,    25,     1,
       8,    25,    30,     1,    30,    18,    31,    32,    33,    34,
      35,    36,    78,     1,    28,    41,    89,    90,    37,    38,
      81,    82,    83,     1,    39,    40,    86,    87,    88,    55,
       1,    55,     1,    20,    77,    55,     1,    55,    25,    44,
      79,     1,    41,    71,    44,    79,     4,    12,    16,    54,
      55,    54,     1,    54,    28,    62,    63,     1,    62,     1,
      41,    79,    79,    79,     1,    76,    79,    79,    81,    84,
      86,    89,    29,    29,    29,     8,    25,    77,    77,    29,
      29,    29,    29,    29,     1,    25,    72,    29,    25,    25,
      25,    25,     1,    41,    57,    64,    65,    25,    25,    25,
      62,    62,    19,    19,    19,     7,    73,    29,     6,    71,
      71,    24,    29,    27,    58,     1,    24,    29,    25,    66,
      21,    67,    68,    69,    67,    67,    24,    24,    42,    42,
       1,    42,     1,    76,    76,    54,    57,    54,     1,    54,
       1,    64,     1,    55,     3,    68,    54,     1,    54,     6,
       6,     6,     1,     6,    24,     1,    24,    74,    25,    25,
       1,    25,     3,     3,     3,     3,     3,    54,    54,     1,
      54,     1,     8,    61,    61,    25,    61,    61,    74,    74,
      74,    74,    74,    25,    25,    25,    25,     3,    25,     1,
      25,     8,     8,     8,     8,     8
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
#line 210 "lalg.y"
    {
                    /* Cria procedimento para o nome do programa */
                    struct data_s data;
                    data.type = TYPE_PROGRAM;
                    memset(&data.value, 0, sizeof(union val_u));

                    /* Adiciona procedimento com o nome do programa */
                    add_sym_var(TYPE_PROC, (yyvsp[(2) - (5)].data).value.strval, data); free((yyvsp[(2) - (5)].data).value.strval);
              }
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 220 "lalg.y"
    { yyerrok; free((yyvsp[(2) - (5)].data).value.strval); }
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 221 "lalg.y"
    { yyerrok; }
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 222 "lalg.y"
    { yyerrok; free((yyvsp[(2) - (5)].data).value.strval); }
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 223 "lalg.y"
    { yyerrok; free((yyvsp[(2) - (5)].data).value.strval); }
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 224 "lalg.y"
    { yyerrok; free((yyvsp[(2) - (5)].data).value.strval); }
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 228 "lalg.y"
    {;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 230 "lalg.y"
    { yyerrok; }
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 234 "lalg.y"
    {;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 237 "lalg.y"
    {;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 238 "lalg.y"
    {;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 241 "lalg.y"
    {
                    /* Adiciona uma constante na tabela de simbolos    */
                    /* O datatype da constante e' o mesmo de "numbero" */
                    add_sym_var(TYPE_CONST, (yyvsp[(2) - (5)].data).value.strval, (yyvsp[(4) - (5)].data)); free((yyvsp[(2) - (5)].data).value.strval);
              }
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 247 "lalg.y"
    { yyerrok; free((yyvsp[(2) - (5)].data).value.strval); }
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 248 "lalg.y"
    { yyerrok; free((yyvsp[(2) - (5)].data).value.strval); }
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 249 "lalg.y"
    { yyerrok; free((yyvsp[(2) - (5)].data).value.strval); }
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 252 "lalg.y"
    {;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 253 "lalg.y"
    {;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 256 "lalg.y"
    {
                    /* Define datatype da lista de variaveis */
                    setnsave_varlist((yyvsp[(4) - (5)].data).type);
              }
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 261 "lalg.y"
    { yyerrok; }
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 262 "lalg.y"
    { yyerrok; }
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 263 "lalg.y"
    { yyerrok; }
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 267 "lalg.y"
    { (yyval.data).type = TYPE_FLOAT;   }
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 268 "lalg.y"
    { (yyval.data).type = TYPE_INTEGER; }
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 269 "lalg.y"
    { (yyval.data).type = TYPE_CHAR;    }
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 273 "lalg.y"
    {
                    /* Adiciona uma variavel numa lista temporaria */
                    add_varlist(TYPE_VAR, (yyvsp[(1) - (2)].data).value.strval);
                    free((yyvsp[(1) - (2)].data).value.strval);
              }
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 280 "lalg.y"
    {;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 281 "lalg.y"
    {;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 286 "lalg.y"
    {
                    /* Adiciona parametros numa lista temporaria */
                    add_parlist(TYPE_VAR, (yyvsp[(1) - (2)].data).value.strval);
                    free((yyvsp[(1) - (2)].data).value.strval);
              }
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 293 "lalg.y"
    {;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 294 "lalg.y"
    {;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 297 "lalg.y"
    {;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 298 "lalg.y"
    {;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 302 "lalg.y"
    {
                    sym_dec_scope();
                    add_sym_proc((yyvsp[(2) - (5)].data).value.strval);
                    free((yyvsp[(2) - (5)].data).value.strval);
              }
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 308 "lalg.y"
    { yyerrok; }
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 309 "lalg.y"
    { yyerrok; free((yyvsp[(2) - (5)].data).value.strval); }
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 313 "lalg.y"
    { free((yyvsp[(2) - (7)].data).value.strval); }
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 314 "lalg.y"
    {;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 316 "lalg.y"
    { yyerrok; }
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 317 "lalg.y"
    { yyerrok; free((yyvsp[(2) - (7)].data).value.strval); }
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 318 "lalg.y"
    { yyerrok; free((yyvsp[(2) - (7)].data).value.strval); }
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 319 "lalg.y"
    { yyerrok; free((yyvsp[(2) - (7)].data).value.strval); }
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 322 "lalg.y"
    {
                    sym_inc_scope();
                    save_parlist(); /* salva parametros na tabela de simbolos */
              }
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 328 "lalg.y"
    {;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 329 "lalg.y"
    { free_parlist(); /* caso nao tenha parametros, libera lista */ }
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 331 "lalg.y"
    { yyerrok; }
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 334 "lalg.y"
    {;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 338 "lalg.y"
    {
                    set_parlist((yyvsp[(3) - (3)].data).type); /* define tipo dos parametros ate agora */
              }
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 342 "lalg.y"
    { yyerrok; }
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 343 "lalg.y"
    { yyerrok; }
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 344 "lalg.y"
    { yyerrok; }
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 345 "lalg.y"
    { yyerrok; }
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 349 "lalg.y"
    {;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 350 "lalg.y"
    {;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 354 "lalg.y"
    {;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 356 "lalg.y"
    { yyerrok; }
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 357 "lalg.y"
    { yyerrok; }
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 358 "lalg.y"
    { yyerrok; }
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 362 "lalg.y"
    {;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 363 "lalg.y"
    {;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 368 "lalg.y"
    {
                    setnsave_varlist((yyvsp[(4) - (5)].data).type);
              }
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 372 "lalg.y"
    { yyerrok; }
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 373 "lalg.y"
    { yyerrok; }
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 374 "lalg.y"
    { yyerrok; }
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 378 "lalg.y"
    {;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 379 "lalg.y"
    { free_arglist(); /* se nao tem argumentos, libera lista */ }
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 381 "lalg.y"
    { yyerrok; }
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 385 "lalg.y"
    {
                    /* Adiciona um argumento (string) na lista */
                    add_arglist((yyvsp[(1) - (2)].data).value.strval);
                    free((yyvsp[(1) - (2)].data).value.strval);
              }
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 393 "lalg.y"
    {;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 394 "lalg.y"
    {;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 396 "lalg.y"
    { yyerrok; }
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 400 "lalg.y"
    {;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 401 "lalg.y"
    {;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 403 "lalg.y"
    { yyerrok; }
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 407 "lalg.y"
    {;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 408 "lalg.y"
    {;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 411 "lalg.y"
    {;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 415 "lalg.y"
    {
                    check_varlist_type();
              }
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 418 "lalg.y"
    {
                    check_varlist_type();
              }
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 421 "lalg.y"
    {;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 422 "lalg.y"
    {;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 423 "lalg.y"
    {
                    /* Atribuicao de expressao */
                    /* "expressao" foi resolvido, portanto tem tipo e valor */
              	    update_sym_var((yyvsp[(1) - (3)].data).value.strval, (yyvsp[(3) - (3)].data));
                    free((yyvsp[(1) - (3)].data).value.strval);
              }
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 429 "lalg.y"
    {
                    /* Atribuicao de char */
                    update_sym_var((yyvsp[(1) - (3)].data).value.strval, (yyvsp[(3) - (3)].data));
                    free((yyvsp[(1) - (3)].data).value.strval);
              }
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 434 "lalg.y"
    {
                    /* Checa se argumentos informados casam com os do procedimento */
                    check_arglist_proc((yyvsp[(1) - (2)].data).value.strval);
                    free_arglist();
                    free((yyvsp[(1) - (2)].data).value.strval);
              }
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 440 "lalg.y"
    {;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 441 "lalg.y"
    {;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 442 "lalg.y"
    {
                    update_sym_var((yyvsp[(2) - (10)].data).value.strval, (yyvsp[(4) - (10)].data));
                    free((yyvsp[(2) - (10)].data).value.strval);
              }
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 447 "lalg.y"
    { yyerrok; }
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 448 "lalg.y"
    { yyerrok; }
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 450 "lalg.y"
    { yyerrok; }
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 451 "lalg.y"
    { yyerrok; }
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 453 "lalg.y"
    { yyerrok; }
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 455 "lalg.y"
    { yyerrok; free((yyvsp[(1) - (3)].data).value.strval); }
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 456 "lalg.y"
    { yyerrok; free((yyvsp[(1) - (3)].data).value.strval); }
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 457 "lalg.y"
    { yyerrok; free((yyvsp[(1) - (3)].data).value.strval); }
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 459 "lalg.y"
    { yyerrok; }
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 460 "lalg.y"
    { yyerrok; }
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 462 "lalg.y"
    { yyerrok; }
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 463 "lalg.y"
    { yyerrok; }
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 464 "lalg.y"
    { yyerrok; }
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 466 "lalg.y"
    { yyerrok; }
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 467 "lalg.y"
    { yyerrok; free((yyvsp[(2) - (10)].data).value.strval); }
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 468 "lalg.y"
    { yyerrok; free((yyvsp[(2) - (10)].data).value.strval); }
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 469 "lalg.y"
    { yyerrok; free((yyvsp[(2) - (10)].data).value.strval); }
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 473 "lalg.y"
    {;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 477 "lalg.y"
    {;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 478 "lalg.y"
    {;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 479 "lalg.y"
    {;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 480 "lalg.y"
    {;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 481 "lalg.y"
    {;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 482 "lalg.y"
    {;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 486 "lalg.y"
    {
                    if ((yyvsp[(2) - (2)].data).type == TYPE_NONE) {
                        (yyval.data) = (yyvsp[(1) - (2)].data);

                    } else {

	                    (yyval.data).type = (yyvsp[(2) - (2)].data).type;

	                    if ((yyvsp[(1) - (2)].data).type == TYPE_INTEGER && (yyvsp[(2) - (2)].data).type == TYPE_INTEGER) {
	                        (yyval.data).value.intval = (yyvsp[(1) - (2)].data).value.intval + (yyvsp[(2) - (2)].data).value.intval;

	                    } else if ((yyvsp[(1) - (2)].data).type == TYPE_FLOAT && (yyvsp[(2) - (2)].data).type == TYPE_FLOAT) {
	                        (yyval.data).value.floatval = (yyvsp[(1) - (2)].data).value.floatval + (yyvsp[(2) - (2)].data).value.floatval;

	                    } else {
	                        if ((yyvsp[(1) - (2)].data).type == TYPE_INTEGER) {
	                            (yyvsp[(1) - (2)].data).value.floatval = (double)(yyvsp[(1) - (2)].data).value.intval;
	                        }
	                        if ((yyvsp[(2) - (2)].data).type == TYPE_INTEGER) {
	                            (yyvsp[(2) - (2)].data).value.floatval = (double)(yyvsp[(2) - (2)].data).value.intval;
	                        }

	                        (yyval.data).type = TYPE_FLOAT;
	                        (yyval.data).value.floatval = (yyvsp[(1) - (2)].data).value.floatval + (yyvsp[(2) - (2)].data).value.floatval;
	                    }
	                }
              }
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 516 "lalg.y"
    { (yyval.data).type = TYPE_INTEGER; (yyval.data).value.intval =  1; }
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 517 "lalg.y"
    { (yyval.data).type = TYPE_INTEGER; (yyval.data).value.intval = -1; }
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 518 "lalg.y"
    { (yyval.data).type = TYPE_INTEGER; (yyval.data).value.intval =  1; }
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 521 "lalg.y"
    {
                    if ((yyvsp[(2) - (2)].data).type == TYPE_NONE) {
                        (yyval.data) = (yyvsp[(1) - (2)].data);

                    } else {

	                    (yyval.data).type = (yyvsp[(2) - (2)].data).type;

	                    if ((yyvsp[(1) - (2)].data).type == TYPE_INTEGER && (yyvsp[(2) - (2)].data).type == TYPE_INTEGER) {
	                        (yyval.data).value.intval = (yyvsp[(1) - (2)].data).value.intval + (yyvsp[(2) - (2)].data).value.intval;

	                    } else if ((yyvsp[(1) - (2)].data).type == TYPE_FLOAT && (yyvsp[(2) - (2)].data).type == TYPE_FLOAT) {
	                        (yyval.data).value.floatval = (yyvsp[(1) - (2)].data).value.floatval + (yyvsp[(2) - (2)].data).value.floatval;

	                    } else {
	                        if ((yyvsp[(1) - (2)].data).type == TYPE_INTEGER) {
	                            (yyvsp[(1) - (2)].data).value.floatval = (double)(yyvsp[(1) - (2)].data).value.intval;
	                        }
	                        if ((yyvsp[(2) - (2)].data).type == TYPE_INTEGER) {
	                            (yyvsp[(2) - (2)].data).value.floatval = (double)(yyvsp[(2) - (2)].data).value.intval;
	                        }

	                        (yyval.data).type = TYPE_FLOAT;
	                        (yyval.data).value.floatval = (yyvsp[(1) - (2)].data).value.floatval + (yyvsp[(2) - (2)].data).value.floatval;
	                    }
	                }
              }
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 548 "lalg.y"
    { (yyval.data).type = TYPE_NONE; (yyval.data).value.floatval = 0.0; }
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 551 "lalg.y"
    {
                    (yyval.data) = (yyvsp[(2) - (2)].data);
                    if ((yyvsp[(1) - (2)].data).value.intval == -1 && (yyvsp[(2) - (2)].data).type == TYPE_INTEGER) {
                        (yyval.data).value.intval = -(yyvsp[(2) - (2)].data).value.intval;
                    } else if ((yyvsp[(1) - (2)].data).value.intval == -1 && (yyvsp[(2) - (2)].data).type == TYPE_FLOAT) {
                    	(yyval.data).value.floatval = -(yyvsp[(2) - (2)].data).value.floatval;
                    }
              }
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 562 "lalg.y"
    { (yyval.data).type = TYPE_INTEGER; (yyval.data).value.intval =  1; }
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 563 "lalg.y"
    { (yyval.data).type = TYPE_INTEGER; (yyval.data).value.intval = -1; }
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 566 "lalg.y"
    {
                    if ((yyvsp[(2) - (2)].data).type == TYPE_NONE) {
                        (yyval.data) = (yyvsp[(1) - (2)].data);

                    } else {

	                    (yyval.data).type = (yyvsp[(2) - (2)].data).type;

	                    if ((yyvsp[(1) - (2)].data).type == TYPE_INTEGER && (yyvsp[(2) - (2)].data).type == TYPE_INTEGER) {
	                        (yyval.data).value.intval = (yyvsp[(1) - (2)].data).value.intval * (yyvsp[(2) - (2)].data).value.intval;

	                    } else if ((yyvsp[(1) - (2)].data).type == TYPE_FLOAT && (yyvsp[(2) - (2)].data).type == TYPE_FLOAT) {
	                        (yyval.data).value.floatval = (yyvsp[(1) - (2)].data).value.floatval * (yyvsp[(2) - (2)].data).value.floatval;
	                        if (div_flag == 1) {
	                        	yyerror("division with non-integer numerator");
	                        	div_flag = 0;
	                        }

	                    } else {
	                        if ((yyvsp[(1) - (2)].data).type == TYPE_INTEGER) {
	                            (yyvsp[(1) - (2)].data).value.floatval = (double)(yyvsp[(1) - (2)].data).value.intval;
	                        }
	                        if ((yyvsp[(2) - (2)].data).type == TYPE_INTEGER) {
	                            (yyvsp[(2) - (2)].data).value.floatval = (double)(yyvsp[(2) - (2)].data).value.intval;
	                        }

	                        (yyval.data).type = TYPE_FLOAT;
	                        (yyval.data).value.floatval = (yyvsp[(1) - (2)].data).value.floatval * (yyvsp[(2) - (2)].data).value.floatval;
	                        if (div_flag == 1) {
	                        	yyerror("division with non-integer numerator");
	                        	div_flag = 0;
	                        }
	                    }
	                }
              }
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 603 "lalg.y"
    {

                    if ((yyvsp[(1) - (2)].data).value.intval == -1 && (yyvsp[(2) - (2)].data).type == TYPE_INTEGER) {
                    	(yyvsp[(2) - (2)].data).value.intval = -(yyvsp[(2) - (2)].data).value.intval;
                    }
                    if ((yyvsp[(1) - (2)].data).value.intval == -1 && (yyvsp[(2) - (2)].data).type == TYPE_FLOAT) {
                        (yyvsp[(2) - (2)].data).value.floatval = -(yyvsp[(2) - (2)].data).value.floatval;
                    }
                    
                    (yyval.data) = (yyvsp[(2) - (2)].data);
              }
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 617 "lalg.y"
    {
                    if ((yyvsp[(2) - (2)].data).type == TYPE_NONE) {
                        (yyval.data) = (yyvsp[(1) - (2)].data);

                    } else {
	                    (yyval.data).type = (yyvsp[(2) - (2)].data).type;

	                    if ((yyvsp[(1) - (2)].data).type == TYPE_INTEGER && (yyvsp[(2) - (2)].data).type == TYPE_INTEGER) {
	                        (yyval.data).value.intval = (yyvsp[(1) - (2)].data).value.intval * (yyvsp[(2) - (2)].data).value.intval;

	                    } else if ((yyvsp[(1) - (2)].data).type == TYPE_FLOAT && (yyvsp[(2) - (2)].data).type == TYPE_FLOAT) {
	                        (yyval.data).value.floatval = (yyvsp[(1) - (2)].data).value.floatval * (yyvsp[(2) - (2)].data).value.floatval;
	                        if (div_flag == 1) {
	                        	yyerror("division with non-integer numerator");
	                        	div_flag = 0;
	                        }

	                    } else {
	                        if ((yyvsp[(1) - (2)].data).type == TYPE_INTEGER) {
	                            (yyvsp[(1) - (2)].data).value.floatval = (double)(yyvsp[(1) - (2)].data).value.intval;
	                        }
	                        if ((yyvsp[(2) - (2)].data).type == TYPE_INTEGER) {
	                            (yyvsp[(2) - (2)].data).value.floatval = (double)(yyvsp[(2) - (2)].data).value.intval;
	                        }

	                        (yyval.data).type = TYPE_FLOAT;
	                        (yyval.data).value.floatval = (yyvsp[(1) - (2)].data).value.floatval * (yyvsp[(2) - (2)].data).value.floatval;
	                        if (div_flag == 1) {
	                        	yyerror("division with non-integer numerator");
	                        	div_flag = 0;
	                        }
	                    }
	                }
              }
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 651 "lalg.y"
    { (yyval.data).type = TYPE_NONE; (yyval.data).value.floatval = 1.0; }
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 654 "lalg.y"
    {
                    (yyval.data) = (yyvsp[(2) - (2)].data);
                    if ((yyvsp[(1) - (2)].data).value.intval == -1 && (yyvsp[(2) - (2)].data).type == TYPE_INTEGER) {
                        (yyval.data).value.intval = 1/(yyvsp[(2) - (2)].data).value.intval;
                    } else if ((yyvsp[(1) - (2)].data).value.intval == -1 && (yyvsp[(2) - (2)].data).type == TYPE_FLOAT) {
                    	(yyval.data).value.floatval = 1.0/(yyvsp[(2) - (2)].data).value.floatval;
                    	yyerror("division with non-integer denominator");
                    }
              }
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 664 "lalg.y"
    { yyerrok; }
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 668 "lalg.y"
    { (yyval.data).type = TYPE_INTEGER; (yyval.data).value.intval =  1; div_flag = 0; }
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 669 "lalg.y"
    { (yyval.data).type = TYPE_INTEGER; (yyval.data).value.intval = -1; div_flag = 1; }
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 673 "lalg.y"
    { (yyval.data) = get_sym_data((yyvsp[(1) - (1)].data).value.strval); free((yyvsp[(1) - (1)].data).value.strval); }
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 674 "lalg.y"
    { (yyval.data) = (yyvsp[(1) - (1)].data); }
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 675 "lalg.y"
    { (yyval.data) = (yyvsp[(2) - (3)].data);}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 677 "lalg.y"
    { yyerrok; }
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 681 "lalg.y"
    { (yyval.data) = (yyvsp[(1) - (1)].data); }
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 682 "lalg.y"
    { (yyval.data) = (yyvsp[(1) - (1)].data); }
    break;



/* Line 1455 of yacc.c  */
#line 2992 "y.tab.c"
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
#line 684 "lalg.y"

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

