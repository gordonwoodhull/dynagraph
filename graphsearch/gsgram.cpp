/* A Bison parser, made by GNU Bison 1.875.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with gs_yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum gs_yytokentype {
     T_graph = 258,
     T_node = 259,
     T_edge = 260,
     T_pattern = 261,
     T_search = 262,
     T_input = 263,
     T_open = 264,
     T_close = 265,
     T_insert = 266,
     T_delete = 267,
     T_modify = 268,
     T_lock = 269,
     T_unlock = 270,
     T_segue = 271,
     T_define = 272,
     T_id = 273,
     T_edgeop = 274,
     T_subgraph = 275
   };
#endif
#define T_graph 258
#define T_node 259
#define T_edge 260
#define T_pattern 261
#define T_search 262
#define T_input 263
#define T_open 264
#define T_close 265
#define T_insert 266
#define T_delete 267
#define T_modify 268
#define T_lock 269
#define T_unlock 270
#define T_segue 271
#define T_define 272
#define T_id 273
#define T_edgeop 274
#define T_subgraph 275




/* Copy the first part of user declarations.  */
#line 1 "gsgram.ypp"

#pragma prototyped
#include "graphsearch/gscmds.h"


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 12 "gsgram.ypp"
typedef union YYSTYPE {
			int				i;
			char			*str;
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 125 "gsgram.tab.c"
# define gs_yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 137 "gsgram.tab.c"

#if ! defined (gs_yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (gs_yyoverflow) || YYERROR_VERBOSE */


#if (! defined (gs_yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union gs_yyalloc
{
  short gs_yyss;
  YYSTYPE gs_yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union gs_yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T gs_yyi;		\
	  for (gs_yyi = 0; gs_yyi < (Count); gs_yyi++)	\
	    (To)[gs_yyi] = (From)[gs_yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T gs_yynewbytes;						\
	YYCOPY (&gs_yyptr->Stack, Stack, gs_yysize);				\
	Stack = &gs_yyptr->Stack;						\
	gs_yynewbytes = gs_yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	gs_yyptr += gs_yynewbytes / sizeof (*gs_yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char gs_yysigned_char;
#else
   typedef short gs_yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   71

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  27
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  28
/* YYNRULES -- Number of rules. */
#define YYNRULES  50
/* YYNRULES -- Number of states. */
#define YYNSTATES  88

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   275

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? gs_yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char gs_yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      21,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    26,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    25,
       2,    24,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    22,     2,    23,     2,     2,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char gs_yyprhs[] =
{
       0,     0,     3,     5,     9,    10,    12,    14,    16,    18,
      20,    22,    24,    26,    28,    30,    32,    34,    36,    38,
      40,    42,    44,    45,    49,    52,    56,    61,    65,    69,
      73,    76,    79,    82,    88,    94,    99,   107,   113,   118,
     119,   124,   125,   127,   128,   130,   134,   138,   140,   142,
     143
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const gs_yysigned_char gs_yyrhs[] =
{
      28,     0,    -1,    29,    -1,    29,    30,    21,    -1,    -1,
      42,    -1,    45,    -1,    43,    -1,    46,    -1,    44,    -1,
      47,    -1,    32,    -1,    34,    -1,    35,    -1,    36,    -1,
      37,    -1,    38,    -1,    39,    -1,    40,    -1,    41,    -1,
      31,    -1,     1,    -1,    -1,     9,     3,    33,    -1,    54,
      48,    -1,    10,     3,    54,    -1,    13,     3,    54,    48,
      -1,    14,     3,    54,    -1,    15,     3,    54,    -1,    16,
       3,    54,    -1,    17,     6,    -1,    17,     7,    -1,    17,
       8,    -1,    11,     4,    54,    18,    48,    -1,    13,     4,
      54,    18,    48,    -1,    12,     4,    54,    18,    -1,    11,
       5,    54,    18,    18,    18,    48,    -1,    13,     5,    54,
      18,    48,    -1,    12,     5,    54,    18,    -1,    -1,    22,
      49,    50,    23,    -1,    -1,    51,    -1,    -1,    52,    -1,
      51,    53,    52,    -1,    18,    24,    18,    -1,    25,    -1,
      26,    -1,    -1,    18,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned char gs_yyrline[] =
{
       0,    19,    19,    23,    24,    28,    28,    29,    29,    30,
      30,    31,    31,    31,    32,    32,    33,    34,    34,    34,
      35,    36,    39,    42,    45,    48,    51,    54,    57,    60,
      63,    66,    69,    72,    75,    78,    81,    84,    87,    90,
      90,    91,    94,    95,    98,    99,   102,   105,   105,   105,
     107
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const gs_yytname[] =
{
  "$end", "error", "$undefined", "T_graph", "T_node", "T_edge", "T_pattern", 
  "T_search", "T_input", "T_open", "T_close", "T_insert", "T_delete", 
  "T_modify", "T_lock", "T_unlock", "T_segue", "T_define", "T_id", 
  "T_edgeop", "T_subgraph", "'\\n'", "'['", "']'", "'='", "';'", "','", 
  "$accept", "session", "commands", "command", "nop", "open_graph", 
  "open_graph2", "close_graph", "mod_graph", "lock_graph", "unlock_graph", 
  "segue", "define_pattern", "define_search", "define_input", "ins_node", 
  "mod_node", "del_node", "ins_edge", "mod_edge", "del_edge", "attrlist", 
  "@1", "optattrdefs", "attrdefs", "attritem", "optsep", "viewid", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short gs_yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,    10,    91,    93,    61,    59,    44
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char gs_yyr1[] =
{
       0,    27,    28,    29,    29,    30,    30,    30,    30,    30,
      30,    30,    30,    30,    30,    30,    30,    30,    30,    30,
      30,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    49,
      48,    48,    50,    50,    51,    51,    52,    53,    53,    53,
      54
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char gs_yyr2[] =
{
       0,     2,     1,     3,     0,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     3,     2,     3,     4,     3,     3,     3,
       2,     2,     2,     5,     5,     4,     7,     5,     4,     0,
       4,     0,     1,     0,     1,     3,     3,     1,     1,     0,
       1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char gs_yydefact[] =
{
       4,     0,     0,     1,    21,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    20,    11,    12,    13,    14,
      15,    16,    17,    18,    19,     5,     7,     9,     6,     8,
      10,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    30,    31,    32,     3,    50,    23,    41,
      25,     0,     0,     0,     0,    41,     0,     0,    27,    28,
      29,    39,    24,    41,     0,    35,    38,    26,    41,    41,
      43,    33,     0,    34,    37,     0,     0,    42,    44,    41,
       0,    40,    47,    48,     0,    36,    46,    45
};

/* YYDEFGOTO[NTERM-NUM]. */
static const gs_yysigned_char gs_yydefgoto[] =
{
      -1,     1,     2,    14,    15,    16,    48,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    62,    70,    76,    77,    78,    84,    49
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -56
static const gs_yysigned_char gs_yypact[] =
{
     -56,     1,     6,   -56,   -56,    39,    40,     7,    21,     0,
      41,    42,    43,    33,    26,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,    30,    30,    30,    30,    30,    30,    30,    30,    30,
      30,    30,    30,   -56,   -56,   -56,   -56,   -56,   -56,    27,
     -56,    32,    34,    35,    36,    27,    37,    38,   -56,   -56,
     -56,   -56,   -56,    27,    44,   -56,   -56,   -56,    27,    27,
      45,   -56,    46,   -56,   -56,    47,    28,   -16,   -56,    27,
      48,   -56,   -56,   -56,    45,   -56,   -56,   -56
};

/* YYPGOTO[NTERM-NUM].  */
static const gs_yysigned_char gs_yypgoto[] =
{
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -55,   -56,   -56,   -56,   -27,   -56,    -4
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -50
static const gs_yysigned_char gs_yytable[] =
{
      67,     3,   -49,    37,    38,    39,    -2,     4,    71,    82,
      83,    33,    34,    73,    74,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    85,    35,    36,   -22,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    43,
      44,    45,    31,    32,    40,    41,    42,    46,    47,    61,
      63,    81,    64,    65,    66,    68,    69,    87,     0,     0,
       0,     0,    72,    75,    79,     0,    86,     0,     0,     0,
       0,    80
};

static const gs_yysigned_char gs_yycheck[] =
{
      55,     0,    18,     3,     4,     5,     0,     1,    63,    25,
      26,     4,     5,    68,    69,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    79,     4,     5,    21,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,     6,
       7,     8,     3,     3,     3,     3,     3,    21,    18,    22,
      18,    23,    18,    18,    18,    18,    18,    84,    -1,    -1,
      -1,    -1,    18,    18,    18,    -1,    18,    -1,    -1,    -1,
      -1,    24
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char gs_yystos[] =
{
       0,    28,    29,     0,     1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    30,    31,    32,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,     3,     3,     4,     5,     4,     5,     3,     4,     5,
       3,     3,     3,     6,     7,     8,    21,    18,    33,    54,
      54,    54,    54,    54,    54,    54,    54,    54,    54,    54,
      54,    22,    48,    18,    18,    18,    18,    48,    18,    18,
      49,    48,    18,    48,    48,    18,    50,    51,    52,    18,
      24,    23,    25,    26,    53,    48,    18,    52
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define gs_yyerrok		(gs_yyerrstatus = 0)
#define gs_yyclearin	(gs_yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto gs_yyacceptlab
#define YYABORT		goto gs_yyabortlab
#define YYERROR		goto gs_yyerrlab1

/* Like YYERROR except do call gs_yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto gs_yyerrlab

#define YYRECOVERING()  (!!gs_yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (gs_yychar == YYEMPTY && gs_yylen == 1)				\
    {								\
      gs_yychar = (Token);						\
      gs_yylval = (Value);						\
      gs_yytoken = YYTRANSLATE (gs_yychar);				\
      YYPOPSTACK;						\
      goto gs_yybackup;						\
    }								\
  else								\
    { 								\
      gs_yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* YYLEX -- calling `gs_yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX gs_yylex (YYLEX_PARAM)
#else
# define YYLEX gs_yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (gs_yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (gs_yydebug)					\
    gs_yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (gs_yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      gs_yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| gs_yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
gs_yy_stack_print (short *bottom, short *top)
#else
static void
gs_yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (gs_yydebug)							\
    gs_yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
gs_yy_reduce_print (int gs_yyrule)
#else
static void
gs_yy_reduce_print (gs_yyrule)
    int gs_yyrule;
#endif
{
  int gs_yyi;
  unsigned int gs_yylineno = gs_yyrline[gs_yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             gs_yyrule - 1, gs_yylineno);
  /* Print the symbols being reduced, and their result.  */
  for (gs_yyi = gs_yyprhs[gs_yyrule]; 0 <= gs_yyrhs[gs_yyi]; gs_yyi++)
    YYFPRINTF (stderr, "%s ", gs_yytname [gs_yyrhs[gs_yyi]]);
  YYFPRINTF (stderr, "-> %s\n", gs_yytname [gs_yyr1[gs_yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (gs_yydebug)				\
    gs_yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int gs_yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
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
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef gs_yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define gs_yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
gs_yystrlen (const char *gs_yystr)
#   else
gs_yystrlen (gs_yystr)
     const char *gs_yystr;
#   endif
{
  register const char *gs_yys = gs_yystr;

  while (*gs_yys++ != '\0')
    continue;

  return gs_yys - gs_yystr - 1;
}
#  endif
# endif

# ifndef gs_yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define gs_yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
gs_yystpcpy (char *gs_yydest, const char *gs_yysrc)
#   else
gs_yystpcpy (gs_yydest, gs_yysrc)
     char *gs_yydest;
     const char *gs_yysrc;
#   endif
{
  register char *gs_yyd = gs_yydest;
  register const char *gs_yys = gs_yysrc;

  while ((*gs_yyd++ = *gs_yys++) != '\0')
    continue;

  return gs_yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
gs_yysymprint (FILE *gs_yyoutput, int gs_yytype, YYSTYPE *gs_yyvaluep)
#else
static void
gs_yysymprint (gs_yyoutput, gs_yytype, gs_yyvaluep)
    FILE *gs_yyoutput;
    int gs_yytype;
    YYSTYPE *gs_yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) gs_yyvaluep;

  if (gs_yytype < YYNTOKENS)
    {
      YYFPRINTF (gs_yyoutput, "token %s (", gs_yytname[gs_yytype]);
# ifdef YYPRINT
      YYPRINT (gs_yyoutput, gs_yytoknum[gs_yytype], *gs_yyvaluep);
# endif
    }
  else
    YYFPRINTF (gs_yyoutput, "nterm %s (", gs_yytname[gs_yytype]);

  switch (gs_yytype)
    {
      default:
        break;
    }
  YYFPRINTF (gs_yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
gs_yydestruct (int gs_yytype, YYSTYPE *gs_yyvaluep)
#else
static void
gs_yydestruct (gs_yytype, gs_yyvaluep)
    int gs_yytype;
    YYSTYPE *gs_yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) gs_yyvaluep;

  switch (gs_yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int gs_yyparse (void *YYPARSE_PARAM);
# else
int gs_yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int gs_yyparse (void);
#else
int gs_yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The lookahead symbol.  */
int gs_yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE gs_yylval;

/* Number of syntax errors so far.  */
int gs_yynerrs;



/*----------.
| gs_yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int gs_yyparse (void *YYPARSE_PARAM)
# else
int gs_yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
gs_yyparse (void)
#else
int
gs_yyparse ()

#endif
#endif
{
  
  register int gs_yystate;
  register int gs_yyn;
  int gs_yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int gs_yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int gs_yytoken = 0;

  /* Three stacks and their tools:
     `gs_yyss': related to states,
     `gs_yyvs': related to semantic values,
     `gs_yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow gs_yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	gs_yyssa[YYINITDEPTH];
  short *gs_yyss = gs_yyssa;
  register short *gs_yyssp;

  /* The semantic value stack.  */
  YYSTYPE gs_yyvsa[YYINITDEPTH];
  YYSTYPE *gs_yyvs = gs_yyvsa;
  register YYSTYPE *gs_yyvsp;



#define YYPOPSTACK   (gs_yyvsp--, gs_yyssp--)

  YYSIZE_T gs_yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE gs_yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int gs_yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  gs_yystate = 0;
  gs_yyerrstatus = 0;
  gs_yynerrs = 0;
  gs_yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  gs_yyssp = gs_yyss;
  gs_yyvsp = gs_yyvs;

  goto gs_yysetstate;

/*------------------------------------------------------------.
| gs_yynewstate -- Push a new state, which is found in gs_yystate.  |
`------------------------------------------------------------*/
 gs_yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  gs_yyssp++;

 gs_yysetstate:
  *gs_yyssp = gs_yystate;

  if (gs_yyss + gs_yystacksize - 1 <= gs_yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T gs_yysize = gs_yyssp - gs_yyss + 1;

#ifdef gs_yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *gs_yyvs1 = gs_yyvs;
	short *gs_yyss1 = gs_yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if gs_yyoverflow is a macro.  */
	gs_yyoverflow ("parser stack overflow",
		    &gs_yyss1, gs_yysize * sizeof (*gs_yyssp),
		    &gs_yyvs1, gs_yysize * sizeof (*gs_yyvsp),

		    &gs_yystacksize);

	gs_yyss = gs_yyss1;
	gs_yyvs = gs_yyvs1;
      }
#else /* no gs_yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto gs_yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= gs_yystacksize)
	goto gs_yyoverflowlab;
      gs_yystacksize *= 2;
      if (YYMAXDEPTH < gs_yystacksize)
	gs_yystacksize = YYMAXDEPTH;

      {
	short *gs_yyss1 = gs_yyss;
	union gs_yyalloc *gs_yyptr =
	  (union gs_yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (gs_yystacksize));
	if (! gs_yyptr)
	  goto gs_yyoverflowlab;
	YYSTACK_RELOCATE (gs_yyss);
	YYSTACK_RELOCATE (gs_yyvs);

#  undef YYSTACK_RELOCATE
	if (gs_yyss1 != gs_yyssa)
	  YYSTACK_FREE (gs_yyss1);
      }
# endif
#endif /* no gs_yyoverflow */

      gs_yyssp = gs_yyss + gs_yysize - 1;
      gs_yyvsp = gs_yyvs + gs_yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) gs_yystacksize));

      if (gs_yyss + gs_yystacksize - 1 <= gs_yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", gs_yystate));

  goto gs_yybackup;

/*-----------.
| gs_yybackup.  |
`-----------*/
gs_yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* gs_yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  gs_yyn = gs_yypact[gs_yystate];
  if (gs_yyn == YYPACT_NINF)
    goto gs_yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (gs_yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      gs_yychar = YYLEX;
    }

  if (gs_yychar <= YYEOF)
    {
      gs_yychar = gs_yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      gs_yytoken = YYTRANSLATE (gs_yychar);
      YYDSYMPRINTF ("Next token is", gs_yytoken, &gs_yylval, &gs_yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  gs_yyn += gs_yytoken;
  if (gs_yyn < 0 || YYLAST < gs_yyn || gs_yycheck[gs_yyn] != gs_yytoken)
    goto gs_yydefault;
  gs_yyn = gs_yytable[gs_yyn];
  if (gs_yyn <= 0)
    {
      if (gs_yyn == 0 || gs_yyn == YYTABLE_NINF)
	goto gs_yyerrlab;
      gs_yyn = -gs_yyn;
      goto gs_yyreduce;
    }

  if (gs_yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", gs_yytname[gs_yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (gs_yychar != YYEOF)
    gs_yychar = YYEMPTY;

  *++gs_yyvsp = gs_yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (gs_yyerrstatus)
    gs_yyerrstatus--;

  gs_yystate = gs_yyn;
  goto gs_yynewstate;


/*-----------------------------------------------------------.
| gs_yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
gs_yydefault:
  gs_yyn = gs_yydefact[gs_yystate];
  if (gs_yyn == 0)
    goto gs_yyerrlab;
  goto gs_yyreduce;


/*-----------------------------.
| gs_yyreduce -- Do a reduction.  |
`-----------------------------*/
gs_yyreduce:
  /* gs_yyn is the number of a rule to reduce with.  */
  gs_yylen = gs_yyr2[gs_yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  gs_yyval = gs_yyvsp[1-gs_yylen];


  YY_REDUCE_PRINT (gs_yyn);
  switch (gs_yyn)
    {
        case 21:
#line 36 "gsgram.ypp"
    {gs_abort(IF_ERR_SYNTAX);;}
    break;

  case 23:
#line 42 "gsgram.ypp"
    {;}
    break;

  case 24:
#line 45 "gsgram.ypp"
    {gs_open_graph(gs_yyvsp[-1].str);;}
    break;

  case 25:
#line 48 "gsgram.ypp"
    {gs_close_graph(gs_yyvsp[0].str);;}
    break;

  case 26:
#line 51 "gsgram.ypp"
    {gs_mod_graph(gs_yyvsp[-1].str);;}
    break;

  case 27:
#line 54 "gsgram.ypp"
    {gs_lock(gs_yyvsp[0].str);;}
    break;

  case 28:
#line 57 "gsgram.ypp"
    {gs_unlock(gs_yyvsp[0].str);;}
    break;

  case 29:
#line 60 "gsgram.ypp"
    {gs_segue(gs_yyvsp[0].str);;}
    break;

  case 30:
#line 63 "gsgram.ypp"
    { gs_define_pattern(); ;}
    break;

  case 31:
#line 66 "gsgram.ypp"
    { gs_define_search(); ;}
    break;

  case 32:
#line 69 "gsgram.ypp"
    { gs_define_input(); ;}
    break;

  case 33:
#line 72 "gsgram.ypp"
    {gs_ins_node(gs_yyvsp[-2].str,gs_yyvsp[-1].str);;}
    break;

  case 34:
#line 75 "gsgram.ypp"
    {gs_mod_node(gs_yyvsp[-2].str,gs_yyvsp[-1].str);;}
    break;

  case 35:
#line 78 "gsgram.ypp"
    {gs_del_node(gs_yyvsp[-1].str,gs_yyvsp[0].str);;}
    break;

  case 36:
#line 81 "gsgram.ypp"
    {gs_ins_edge(gs_yyvsp[-4].str,gs_yyvsp[-3].str,gs_yyvsp[-2].str,gs_yyvsp[-1].str);;}
    break;

  case 37:
#line 84 "gsgram.ypp"
    {gs_mod_edge(gs_yyvsp[-2].str,gs_yyvsp[-1].str);;}
    break;

  case 38:
#line 87 "gsgram.ypp"
    {gs_del_edge(gs_yyvsp[-1].str,gs_yyvsp[0].str);;}
    break;

  case 39:
#line 90 "gsgram.ypp"
    {gs_reset_attrs();;}
    break;

  case 41:
#line 91 "gsgram.ypp"
    {gs_reset_attrs();;}
    break;

  case 46:
#line 102 "gsgram.ypp"
    {gs_append_attr(gs_yyvsp[-2].str,gs_yyvsp[0].str);;}
    break;

  case 50:
#line 107 "gsgram.ypp"
    {gs_yyval.str = gs_yyvsp[0].str; ;}
    break;


    }

/* Line 991 of yacc.c.  */
#line 1189 "gsgram.tab.c"

  gs_yyvsp -= gs_yylen;
  gs_yyssp -= gs_yylen;


  YY_STACK_PRINT (gs_yyss, gs_yyssp);

  *++gs_yyvsp = gs_yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  gs_yyn = gs_yyr1[gs_yyn];

  gs_yystate = gs_yypgoto[gs_yyn - YYNTOKENS] + *gs_yyssp;
  if (0 <= gs_yystate && gs_yystate <= YYLAST && gs_yycheck[gs_yystate] == *gs_yyssp)
    gs_yystate = gs_yytable[gs_yystate];
  else
    gs_yystate = gs_yydefgoto[gs_yyn - YYNTOKENS];

  goto gs_yynewstate;


/*------------------------------------.
| gs_yyerrlab -- here on detecting error |
`------------------------------------*/
gs_yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!gs_yyerrstatus)
    {
      ++gs_yynerrs;
#if YYERROR_VERBOSE
      gs_yyn = gs_yypact[gs_yystate];

      if (YYPACT_NINF < gs_yyn && gs_yyn < YYLAST)
	{
	  YYSIZE_T gs_yysize = 0;
	  int gs_yytype = YYTRANSLATE (gs_yychar);
	  char *gs_yymsg;
	  int gs_yyx, gs_yycount;

	  gs_yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (gs_yyx = gs_yyn < 0 ? -gs_yyn : 0;
	       gs_yyx < (int) (sizeof (gs_yytname) / sizeof (char *)); gs_yyx++)
	    if (gs_yycheck[gs_yyx + gs_yyn] == gs_yyx && gs_yyx != YYTERROR)
	      gs_yysize += gs_yystrlen (gs_yytname[gs_yyx]) + 15, gs_yycount++;
	  gs_yysize += gs_yystrlen ("syntax error, unexpected ") + 1;
	  gs_yysize += gs_yystrlen (gs_yytname[gs_yytype]);
	  gs_yymsg = (char *) YYSTACK_ALLOC (gs_yysize);
	  if (gs_yymsg != 0)
	    {
	      char *gs_yyp = gs_yystpcpy (gs_yymsg, "syntax error, unexpected ");
	      gs_yyp = gs_yystpcpy (gs_yyp, gs_yytname[gs_yytype]);

	      if (gs_yycount < 5)
		{
		  gs_yycount = 0;
		  for (gs_yyx = gs_yyn < 0 ? -gs_yyn : 0;
		       gs_yyx < (int) (sizeof (gs_yytname) / sizeof (char *));
		       gs_yyx++)
		    if (gs_yycheck[gs_yyx + gs_yyn] == gs_yyx && gs_yyx != YYTERROR)
		      {
			const char *gs_yyq = ! gs_yycount ? ", expecting " : " or ";
			gs_yyp = gs_yystpcpy (gs_yyp, gs_yyq);
			gs_yyp = gs_yystpcpy (gs_yyp, gs_yytname[gs_yyx]);
			gs_yycount++;
		      }
		}
	      gs_yyerror (gs_yymsg);
	      YYSTACK_FREE (gs_yymsg);
	    }
	  else
	    gs_yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	gs_yyerror ("syntax error");
    }



  if (gs_yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (gs_yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (gs_yyss < gs_yyssp)
	    {
	      YYDSYMPRINTF ("Error: popping", gs_yystos[*gs_yyssp], gs_yyvsp, gs_yylsp);
	      gs_yydestruct (gs_yystos[*gs_yyssp], gs_yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDSYMPRINTF ("Error: discarding", gs_yytoken, &gs_yylval, &gs_yylloc);
      gs_yydestruct (gs_yytoken, &gs_yylval);
      gs_yychar = YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto gs_yyerrlab2;


/*----------------------------------------------------.
| gs_yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
gs_yyerrlab1:

  /* Suppress GCC warning that gs_yyerrlab1 is unused when no action
     invokes YYERROR.  */
#if defined (__GNUC_MINOR__) && 2093 <= (__GNUC__ * 1000 + __GNUC_MINOR__)
  __attribute__ ((__unused__))
#endif


  goto gs_yyerrlab2;


/*---------------------------------------------------------------.
| gs_yyerrlab2 -- pop states until the error token can be shifted.  |
`---------------------------------------------------------------*/
gs_yyerrlab2:
  gs_yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      gs_yyn = gs_yypact[gs_yystate];
      if (gs_yyn != YYPACT_NINF)
	{
	  gs_yyn += YYTERROR;
	  if (0 <= gs_yyn && gs_yyn <= YYLAST && gs_yycheck[gs_yyn] == YYTERROR)
	    {
	      gs_yyn = gs_yytable[gs_yyn];
	      if (0 < gs_yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (gs_yyssp == gs_yyss)
	YYABORT;

      YYDSYMPRINTF ("Error: popping", gs_yystos[*gs_yyssp], gs_yyvsp, gs_yylsp);
      gs_yydestruct (gs_yystos[gs_yystate], gs_yyvsp);
      gs_yyvsp--;
      gs_yystate = *--gs_yyssp;

      YY_STACK_PRINT (gs_yyss, gs_yyssp);
    }

  if (gs_yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++gs_yyvsp = gs_yylval;


  gs_yystate = gs_yyn;
  goto gs_yynewstate;


/*-------------------------------------.
| gs_yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
gs_yyacceptlab:
  gs_yyresult = 0;
  goto gs_yyreturn;

/*-----------------------------------.
| gs_yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
gs_yyabortlab:
  gs_yyresult = 1;
  goto gs_yyreturn;

#ifndef gs_yyoverflow
/*----------------------------------------------.
| gs_yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
gs_yyoverflowlab:
  gs_yyerror ("parser stack overflow");
  gs_yyresult = 2;
  /* Fall through.  */
#endif

gs_yyreturn:
#ifndef gs_yyoverflow
  if (gs_yyss != gs_yyssa)
    YYSTACK_FREE (gs_yyss);
#endif
  return gs_yyresult;
}



