/* A Bison parser, made by GNU Bison 1.875c.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003 Free Software Foundation, Inc.

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

/* All symbols defined below should begin with incr_yy or YY, to avoid
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
   enum incr_yytokentype {
     T_graph = 258,
     T_node = 259,
     T_edge = 260,
     T_view = 261,
     T_open = 262,
     T_close = 263,
     T_insert = 264,
     T_delete = 265,
     T_modify = 266,
     T_lock = 267,
     T_unlock = 268,
     T_segue = 269,
     T_message = 270,
     T_id = 271,
     T_edgeop = 272,
     T_subgraph = 273
   };
#endif
#define T_graph 258
#define T_node 259
#define T_edge 260
#define T_view 261
#define T_open 262
#define T_close 263
#define T_insert 264
#define T_delete 265
#define T_modify 266
#define T_lock 267
#define T_unlock 268
#define T_segue 269
#define T_message 270
#define T_id 271
#define T_edgeop 272
#define T_subgraph 273




/* Copy the first part of user declarations.  */
#line 1 "incrgram.ypp"

#pragma prototyped
#include "incrface/incrcmds.h"


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
#line 12 "incrgram.ypp"
typedef union YYSTYPE {
			int				i;
			char			*str;
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 122 "y.tab.c"
# define incr_yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 134 "y.tab.c"

#if ! defined (incr_yyoverflow) || YYERROR_VERBOSE

# ifndef YYFREE
#  define YYFREE free
# endif
# ifndef YYMALLOC
#  define YYMALLOC malloc
# endif

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   define YYSTACK_ALLOC alloca
#  endif
# else
#  if defined (alloca) || defined (_ALLOCA_H)
#   define YYSTACK_ALLOC alloca
#  else
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
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
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
# endif
#endif /* ! defined (incr_yyoverflow) || YYERROR_VERBOSE */


#if (! defined (incr_yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union incr_yyalloc
{
  short incr_yyss;
  YYSTYPE incr_yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union incr_yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T incr_yyi;		\
	  for (incr_yyi = 0; incr_yyi < (Count); incr_yyi++)	\
	    (To)[incr_yyi] = (From)[incr_yyi];		\
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
	YYSIZE_T incr_yynewbytes;						\
	YYCOPY (&incr_yyptr->Stack, Stack, incr_yysize);				\
	Stack = &incr_yyptr->Stack;						\
	incr_yynewbytes = incr_yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	incr_yyptr += incr_yynewbytes / sizeof (*incr_yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char incr_yysigned_char;
#else
   typedef short incr_yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   62

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  25
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  26
/* YYNRULES -- Number of rules. */
#define YYNRULES  46
/* YYNRULES -- Number of states. */
#define YYNSTATES  81

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   273

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? incr_yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char incr_yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      19,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    24,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    23,
       2,    22,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    20,     2,    21,     2,     2,     2,     2,     2,     2,
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
      15,    16,    17,    18
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char incr_yyprhs[] =
{
       0,     0,     3,     5,     9,    10,    12,    14,    16,    18,
      20,    22,    24,    26,    28,    30,    32,    34,    36,    38,
      40,    41,    44,    48,    51,    55,    60,    64,    68,    72,
      78,    84,    89,    97,   103,   108,   109,   114,   115,   117,
     118,   120,   124,   128,   130,   132,   133
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const incr_yysigned_char incr_yyrhs[] =
{
      26,     0,    -1,    27,    -1,    27,    28,    19,    -1,    -1,
      38,    -1,    41,    -1,    39,    -1,    42,    -1,    40,    -1,
      43,    -1,    31,    -1,    33,    -1,    34,    -1,    35,    -1,
      36,    -1,    37,    -1,    30,    -1,    29,    -1,     1,    -1,
      -1,    15,    16,    -1,     7,     6,    32,    -1,    50,    44,
      -1,     8,     6,    50,    -1,    11,     6,    50,    44,    -1,
      12,     6,    50,    -1,    13,     6,    50,    -1,    14,     6,
      50,    -1,     9,    50,     4,    16,    44,    -1,    11,    50,
       4,    16,    44,    -1,    10,    50,     4,    16,    -1,     9,
      50,     5,    16,    16,    16,    44,    -1,    11,    50,     5,
      16,    44,    -1,    10,    50,     5,    16,    -1,    -1,    20,
      45,    46,    21,    -1,    -1,    47,    -1,    -1,    48,    -1,
      47,    49,    48,    -1,    16,    22,    16,    -1,    23,    -1,
      24,    -1,    -1,    16,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned char incr_yyrline[] =
{
       0,    19,    19,    23,    24,    28,    28,    29,    29,    30,
      30,    31,    31,    31,    32,    32,    33,    34,    35,    36,
      39,    42,    45,    48,    51,    54,    57,    60,    63,    66,
      69,    72,    75,    78,    81,    84,    84,    85,    88,    89,
      92,    93,    96,    99,    99,    99,   101
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const incr_yytname[] =
{
  "$end", "error", "$undefined", "T_graph", "T_node", "T_edge", "T_view",
  "T_open", "T_close", "T_insert", "T_delete", "T_modify", "T_lock",
  "T_unlock", "T_segue", "T_message", "T_id", "T_edgeop", "T_subgraph",
  "'\\n'", "'['", "']'", "'='", "';'", "','", "$accept", "session",
  "commands", "command", "nop", "message", "open_view", "open_view2",
  "close_view", "mod_view", "lock_view", "unlock_view", "segue",
  "ins_node", "mod_node", "del_node", "ins_edge", "mod_edge", "del_edge",
  "attrlist", "@1", "optattrdefs", "attrdefs", "attritem", "optsep",
  "viewid", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short incr_yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,    10,
      91,    93,    61,    59,    44
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char incr_yyr1[] =
{
       0,    25,    26,    27,    27,    28,    28,    28,    28,    28,
      28,    28,    28,    28,    28,    28,    28,    28,    28,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    45,    44,    44,    46,    46,
      47,    47,    48,    49,    49,    49,    50
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char incr_yyr2[] =
{
       0,     2,     1,     3,     0,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     2,     3,     2,     3,     4,     3,     3,     3,     5,
       5,     4,     7,     5,     4,     0,     4,     0,     1,     0,
       1,     3,     3,     1,     1,     0,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char incr_yydefact[] =
{
       4,     0,     0,     1,    19,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    18,    17,    11,    12,    13,
      14,    15,    16,     5,     7,     9,     6,     8,    10,     0,
       0,    46,     0,     0,     0,     0,     0,     0,     0,    21,
       3,    22,    37,    24,     0,     0,     0,     0,    37,     0,
       0,    26,    27,    28,    35,    23,    37,     0,    31,    34,
      25,    37,    37,    39,    29,     0,    30,    33,     0,     0,
      38,    40,    37,     0,    36,    43,    44,     0,    32,    42,
      41
};

/* YYDEFGOTO[NTERM-NUM]. */
static const incr_yysigned_char incr_yydefgoto[] =
{
      -1,     1,     2,    14,    15,    16,    17,    41,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    55,
      63,    69,    70,    71,    77,    32
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -46
static const incr_yysigned_char incr_yypact[] =
{
     -46,    15,    24,   -46,   -46,    12,    13,     4,     4,    -4,
      17,    34,    35,    26,    25,   -46,   -46,   -46,   -46,   -46,
     -46,   -46,   -46,   -46,   -46,   -46,   -46,   -46,   -46,     4,
       4,   -46,     0,     3,     4,     5,     4,     4,     4,   -46,
     -46,   -46,    27,   -46,    29,    30,    32,    33,    27,    36,
      37,   -46,   -46,   -46,   -46,   -46,    27,    38,   -46,   -46,
     -46,    27,    27,    39,   -46,    40,   -46,   -46,    28,    41,
     -10,   -46,    27,    42,   -46,   -46,   -46,    39,   -46,   -46,
     -46
};

/* YYPGOTO[NTERM-NUM].  */
static const incr_yysigned_char incr_yypgoto[] =
{
     -46,   -46,   -46,   -46,   -46,   -46,   -46,   -46,   -46,   -46,
     -46,   -46,   -46,   -46,   -46,   -46,   -46,   -46,   -46,   -45,
     -46,   -46,   -46,   -26,   -46,    -8
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -46
static const incr_yysigned_char incr_yytable[] =
{
      33,    35,    34,    60,    44,    45,   -45,    46,    47,    49,
      50,    64,    31,    75,    76,     3,    66,    67,    29,    30,
      31,    42,    43,    36,    -2,     4,    48,    78,    51,    52,
      53,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      37,    38,    39,   -20,    40,    56,    57,    54,    58,    59,
      73,    80,    61,    62,    65,    68,    72,     0,    79,     0,
       0,     0,    74
};

static const incr_yysigned_char incr_yycheck[] =
{
       8,     9,     6,    48,     4,     5,    16,     4,     5,     4,
       5,    56,    16,    23,    24,     0,    61,    62,     6,     6,
      16,    29,    30,     6,     0,     1,    34,    72,    36,    37,
      38,     7,     8,     9,    10,    11,    12,    13,    14,    15,
       6,     6,    16,    19,    19,    16,    16,    20,    16,    16,
      22,    77,    16,    16,    16,    16,    16,    -1,    16,    -1,
      -1,    -1,    21
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char incr_yystos[] =
{
       0,    26,    27,     0,     1,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    28,    29,    30,    31,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,     6,
       6,    16,    50,    50,     6,    50,     6,     6,     6,    16,
      19,    32,    50,    50,     4,     5,     4,     5,    50,     4,
       5,    50,    50,    50,    20,    44,    16,    16,    16,    16,
      44,    16,    16,    45,    44,    16,    44,    44,    16,    46,
      47,    48,    16,    22,    21,    23,    24,    49,    44,    16,
      48
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

#define incr_yyerrok		(incr_yyerrstatus = 0)
#define incr_yyclearin	(incr_yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto incr_yyacceptlab
#define YYABORT		goto incr_yyabortlab
#define YYERROR		goto incr_yyerrorlab


/* Like YYERROR except do call incr_yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto incr_yyerrlab

#define YYRECOVERING()  (!!incr_yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (incr_yychar == YYEMPTY && incr_yylen == 1)				\
    {								\
      incr_yychar = (Token);						\
      incr_yylval = (Value);						\
      incr_yytoken = YYTRANSLATE (incr_yychar);				\
      YYPOPSTACK;						\
      goto incr_yybackup;						\
    }								\
  else								\
    { 								\
      incr_yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)		\
   ((Current).first_line   = (Rhs)[1].first_line,	\
    (Current).first_column = (Rhs)[1].first_column,	\
    (Current).last_line    = (Rhs)[N].last_line,	\
    (Current).last_column  = (Rhs)[N].last_column)
#endif

/* YYLEX -- calling `incr_yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX incr_yylex (YYLEX_PARAM)
#else
# define YYLEX incr_yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (incr_yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (incr_yydebug)					\
    incr_yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (incr_yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      incr_yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| incr_yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
incr_yy_stack_print (short *bottom, short *top)
#else
static void
incr_yy_stack_print (bottom, top)
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
  if (incr_yydebug)							\
    incr_yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
incr_yy_reduce_print (int incr_yyrule)
#else
static void
incr_yy_reduce_print (incr_yyrule)
    int incr_yyrule;
#endif
{
  int incr_yyi;
  unsigned int incr_yylno = incr_yyrline[incr_yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             incr_yyrule - 1, incr_yylno);
  /* Print the symbols being reduced, and their result.  */
  for (incr_yyi = incr_yyprhs[incr_yyrule]; 0 <= incr_yyrhs[incr_yyi]; incr_yyi++)
    YYFPRINTF (stderr, "%s ", incr_yytname [incr_yyrhs[incr_yyi]]);
  YYFPRINTF (stderr, "-> %s\n", incr_yytname [incr_yyr1[incr_yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (incr_yydebug)				\
    incr_yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int incr_yydebug;
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

#if defined (YYMAXDEPTH) && YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef incr_yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define incr_yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
incr_yystrlen (const char *incr_yystr)
#   else
incr_yystrlen (incr_yystr)
     const char *incr_yystr;
#   endif
{
  register const char *incr_yys = incr_yystr;

  while (*incr_yys++ != '\0')
    continue;

  return incr_yys - incr_yystr - 1;
}
#  endif
# endif

# ifndef incr_yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define incr_yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
incr_yystpcpy (char *incr_yydest, const char *incr_yysrc)
#   else
incr_yystpcpy (incr_yydest, incr_yysrc)
     char *incr_yydest;
     const char *incr_yysrc;
#   endif
{
  register char *incr_yyd = incr_yydest;
  register const char *incr_yys = incr_yysrc;

  while ((*incr_yyd++ = *incr_yys++) != '\0')
    continue;

  return incr_yyd - 1;
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
incr_yysymprint (FILE *incr_yyoutput, int incr_yytype, YYSTYPE *incr_yyvaluep)
#else
static void
incr_yysymprint (incr_yyoutput, incr_yytype, incr_yyvaluep)
    FILE *incr_yyoutput;
    int incr_yytype;
    YYSTYPE *incr_yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) incr_yyvaluep;

  if (incr_yytype < YYNTOKENS)
    {
      YYFPRINTF (incr_yyoutput, "token %s (", incr_yytname[incr_yytype]);
# ifdef YYPRINT
      YYPRINT (incr_yyoutput, incr_yytoknum[incr_yytype], *incr_yyvaluep);
# endif
    }
  else
    YYFPRINTF (incr_yyoutput, "nterm %s (", incr_yytname[incr_yytype]);

  switch (incr_yytype)
    {
      default:
        break;
    }
  YYFPRINTF (incr_yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
incr_yydestruct (int incr_yytype, YYSTYPE *incr_yyvaluep)
#else
static void
incr_yydestruct (incr_yytype, incr_yyvaluep)
    int incr_yytype;
    YYSTYPE *incr_yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) incr_yyvaluep;

  switch (incr_yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int incr_yyparse (void *YYPARSE_PARAM);
# else
int incr_yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int incr_yyparse (void);
#else
int incr_yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The lookahead symbol.  */
int incr_yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE incr_yylval;

/* Number of syntax errors so far.  */
int incr_yynerrs;



/*----------.
| incr_yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int incr_yyparse (void *YYPARSE_PARAM)
# else
int incr_yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
incr_yyparse (void)
#else
int
incr_yyparse ()

#endif
#endif
{
  
  register int incr_yystate;
  register int incr_yyn;
  int incr_yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int incr_yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int incr_yytoken = 0;

  /* Three stacks and their tools:
     `incr_yyss': related to states,
     `incr_yyvs': related to semantic values,
     `incr_yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow incr_yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	incr_yyssa[YYINITDEPTH];
  short *incr_yyss = incr_yyssa;
  register short *incr_yyssp;

  /* The semantic value stack.  */
  YYSTYPE incr_yyvsa[YYINITDEPTH];
  YYSTYPE *incr_yyvs = incr_yyvsa;
  register YYSTYPE *incr_yyvsp;



#define YYPOPSTACK   (incr_yyvsp--, incr_yyssp--)

  YYSIZE_T incr_yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE incr_yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int incr_yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  incr_yystate = 0;
  incr_yyerrstatus = 0;
  incr_yynerrs = 0;
  incr_yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  incr_yyssp = incr_yyss;
  incr_yyvsp = incr_yyvs;

  goto incr_yysetstate;

/*------------------------------------------------------------.
| incr_yynewstate -- Push a new state, which is found in incr_yystate.  |
`------------------------------------------------------------*/
 incr_yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  incr_yyssp++;

 incr_yysetstate:
  *incr_yyssp = incr_yystate;

  if (incr_yyss + incr_yystacksize - 1 <= incr_yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T incr_yysize = incr_yyssp - incr_yyss + 1;

#ifdef incr_yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *incr_yyvs1 = incr_yyvs;
	short *incr_yyss1 = incr_yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if incr_yyoverflow is a macro.  */
	incr_yyoverflow ("parser stack overflow",
		    &incr_yyss1, incr_yysize * sizeof (*incr_yyssp),
		    &incr_yyvs1, incr_yysize * sizeof (*incr_yyvsp),

		    &incr_yystacksize);

	incr_yyss = incr_yyss1;
	incr_yyvs = incr_yyvs1;
      }
#else /* no incr_yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto incr_yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= incr_yystacksize)
	goto incr_yyoverflowlab;
      incr_yystacksize *= 2;
      if (YYMAXDEPTH < incr_yystacksize)
	incr_yystacksize = YYMAXDEPTH;

      {
	short *incr_yyss1 = incr_yyss;
	union incr_yyalloc *incr_yyptr =
	  (union incr_yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (incr_yystacksize));
	if (! incr_yyptr)
	  goto incr_yyoverflowlab;
	YYSTACK_RELOCATE (incr_yyss);
	YYSTACK_RELOCATE (incr_yyvs);

#  undef YYSTACK_RELOCATE
	if (incr_yyss1 != incr_yyssa)
	  YYSTACK_FREE (incr_yyss1);
      }
# endif
#endif /* no incr_yyoverflow */

      incr_yyssp = incr_yyss + incr_yysize - 1;
      incr_yyvsp = incr_yyvs + incr_yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) incr_yystacksize));

      if (incr_yyss + incr_yystacksize - 1 <= incr_yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", incr_yystate));

  goto incr_yybackup;

/*-----------.
| incr_yybackup.  |
`-----------*/
incr_yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* incr_yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  incr_yyn = incr_yypact[incr_yystate];
  if (incr_yyn == YYPACT_NINF)
    goto incr_yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (incr_yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      incr_yychar = YYLEX;
    }

  if (incr_yychar <= YYEOF)
    {
      incr_yychar = incr_yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      incr_yytoken = YYTRANSLATE (incr_yychar);
      YYDSYMPRINTF ("Next token is", incr_yytoken, &incr_yylval, &incr_yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  incr_yyn += incr_yytoken;
  if (incr_yyn < 0 || YYLAST < incr_yyn || incr_yycheck[incr_yyn] != incr_yytoken)
    goto incr_yydefault;
  incr_yyn = incr_yytable[incr_yyn];
  if (incr_yyn <= 0)
    {
      if (incr_yyn == 0 || incr_yyn == YYTABLE_NINF)
	goto incr_yyerrlab;
      incr_yyn = -incr_yyn;
      goto incr_yyreduce;
    }

  if (incr_yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", incr_yytname[incr_yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (incr_yychar != YYEOF)
    incr_yychar = YYEMPTY;

  *++incr_yyvsp = incr_yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (incr_yyerrstatus)
    incr_yyerrstatus--;

  incr_yystate = incr_yyn;
  goto incr_yynewstate;


/*-----------------------------------------------------------.
| incr_yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
incr_yydefault:
  incr_yyn = incr_yydefact[incr_yystate];
  if (incr_yyn == 0)
    goto incr_yyerrlab;
  goto incr_yyreduce;


/*-----------------------------.
| incr_yyreduce -- Do a reduction.  |
`-----------------------------*/
incr_yyreduce:
  /* incr_yyn is the number of a rule to reduce with.  */
  incr_yylen = incr_yyr2[incr_yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  incr_yyval = incr_yyvsp[1-incr_yylen];


  YY_REDUCE_PRINT (incr_yyn);
  switch (incr_yyn)
    {
        case 19:
#line 36 "incrgram.ypp"
    {incr_abort(IF_ERR_SYNTAX);}
    break;

  case 21:
#line 42 "incrgram.ypp"
    {incr_message(incr_yyvsp[0].str);}
    break;

  case 22:
#line 45 "incrgram.ypp"
    {}
    break;

  case 23:
#line 48 "incrgram.ypp"
    {incr_open_view(incr_yyvsp[-1].str);}
    break;

  case 24:
#line 51 "incrgram.ypp"
    {incr_close_view(incr_yyvsp[0].str);}
    break;

  case 25:
#line 54 "incrgram.ypp"
    {incr_mod_view(incr_yyvsp[-1].str);}
    break;

  case 26:
#line 57 "incrgram.ypp"
    {incr_lock(incr_yyvsp[0].str);}
    break;

  case 27:
#line 60 "incrgram.ypp"
    {incr_unlock(incr_yyvsp[0].str);}
    break;

  case 28:
#line 63 "incrgram.ypp"
    {incr_segue(incr_yyvsp[0].str);}
    break;

  case 29:
#line 66 "incrgram.ypp"
    {incr_ins_node(incr_yyvsp[-3].str,incr_yyvsp[-1].str);}
    break;

  case 30:
#line 69 "incrgram.ypp"
    {incr_mod_node(incr_yyvsp[-3].str,incr_yyvsp[-1].str);}
    break;

  case 31:
#line 72 "incrgram.ypp"
    {incr_del_node(incr_yyvsp[-2].str,incr_yyvsp[0].str);}
    break;

  case 32:
#line 75 "incrgram.ypp"
    {incr_ins_edge(incr_yyvsp[-5].str,incr_yyvsp[-3].str,incr_yyvsp[-2].str,incr_yyvsp[-1].str);}
    break;

  case 33:
#line 78 "incrgram.ypp"
    {incr_mod_edge(incr_yyvsp[-3].str,incr_yyvsp[-1].str);}
    break;

  case 34:
#line 81 "incrgram.ypp"
    {incr_del_edge(incr_yyvsp[-2].str,incr_yyvsp[0].str);}
    break;

  case 35:
#line 84 "incrgram.ypp"
    {incr_reset_attrs();}
    break;

  case 37:
#line 85 "incrgram.ypp"
    {incr_reset_attrs();}
    break;

  case 42:
#line 96 "incrgram.ypp"
    {incr_append_attr(incr_yyvsp[-2].str,incr_yyvsp[0].str);}
    break;

  case 46:
#line 101 "incrgram.ypp"
    {incr_yyval.str = incr_yyvsp[0].str; }
    break;


    }

/* Line 1000 of yacc.c.  */
#line 1177 "y.tab.c"

  incr_yyvsp -= incr_yylen;
  incr_yyssp -= incr_yylen;


  YY_STACK_PRINT (incr_yyss, incr_yyssp);

  *++incr_yyvsp = incr_yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  incr_yyn = incr_yyr1[incr_yyn];

  incr_yystate = incr_yypgoto[incr_yyn - YYNTOKENS] + *incr_yyssp;
  if (0 <= incr_yystate && incr_yystate <= YYLAST && incr_yycheck[incr_yystate] == *incr_yyssp)
    incr_yystate = incr_yytable[incr_yystate];
  else
    incr_yystate = incr_yydefgoto[incr_yyn - YYNTOKENS];

  goto incr_yynewstate;


/*------------------------------------.
| incr_yyerrlab -- here on detecting error |
`------------------------------------*/
incr_yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!incr_yyerrstatus)
    {
      ++incr_yynerrs;
#if YYERROR_VERBOSE
      incr_yyn = incr_yypact[incr_yystate];

      if (YYPACT_NINF < incr_yyn && incr_yyn < YYLAST)
	{
	  YYSIZE_T incr_yysize = 0;
	  int incr_yytype = YYTRANSLATE (incr_yychar);
	  const char* incr_yyprefix;
	  char *incr_yymsg;
	  int incr_yyx;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int incr_yyxbegin = incr_yyn < 0 ? -incr_yyn : 0;

	  /* Stay within bounds of both incr_yycheck and incr_yytname.  */
	  int incr_yychecklim = YYLAST - incr_yyn;
	  int incr_yyxend = incr_yychecklim < YYNTOKENS ? incr_yychecklim : YYNTOKENS;
	  int incr_yycount = 0;

	  incr_yyprefix = ", expecting ";
	  for (incr_yyx = incr_yyxbegin; incr_yyx < incr_yyxend; ++incr_yyx)
	    if (incr_yycheck[incr_yyx + incr_yyn] == incr_yyx && incr_yyx != YYTERROR)
	      {
		incr_yysize += incr_yystrlen (incr_yyprefix) + incr_yystrlen (incr_yytname [incr_yyx]);
		incr_yycount += 1;
		if (incr_yycount == 5)
		  {
		    incr_yysize = 0;
		    break;
		  }
	      }
	  incr_yysize += (sizeof ("syntax error, unexpected ")
		     + incr_yystrlen (incr_yytname[incr_yytype]));
	  incr_yymsg = (char *) YYSTACK_ALLOC (incr_yysize);
	  if (incr_yymsg != 0)
	    {
	      char *incr_yyp = incr_yystpcpy (incr_yymsg, "syntax error, unexpected ");
	      incr_yyp = incr_yystpcpy (incr_yyp, incr_yytname[incr_yytype]);

	      if (incr_yycount < 5)
		{
		  incr_yyprefix = ", expecting ";
		  for (incr_yyx = incr_yyxbegin; incr_yyx < incr_yyxend; ++incr_yyx)
		    if (incr_yycheck[incr_yyx + incr_yyn] == incr_yyx && incr_yyx != YYTERROR)
		      {
			incr_yyp = incr_yystpcpy (incr_yyp, incr_yyprefix);
			incr_yyp = incr_yystpcpy (incr_yyp, incr_yytname[incr_yyx]);
			incr_yyprefix = " or ";
		      }
		}
	      incr_yyerror (incr_yymsg);
	      YYSTACK_FREE (incr_yymsg);
	    }
	  else
	    incr_yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	incr_yyerror ("syntax error");
    }



  if (incr_yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (incr_yychar <= YYEOF)
        {
          /* If at end of input, pop the error token,
	     then the rest of the stack, then return failure.  */
	  if (incr_yychar == YYEOF)
	     for (;;)
	       {
		 YYPOPSTACK;
		 if (incr_yyssp == incr_yyss)
		   YYABORT;
		 YYDSYMPRINTF ("Error: popping", incr_yystos[*incr_yyssp], incr_yyvsp, incr_yylsp);
		 incr_yydestruct (incr_yystos[*incr_yyssp], incr_yyvsp);
	       }
        }
      else
	{
	  YYDSYMPRINTF ("Error: discarding", incr_yytoken, &incr_yylval, &incr_yylloc);
	  incr_yydestruct (incr_yytoken, &incr_yylval);
	  incr_yychar = YYEMPTY;

	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto incr_yyerrlab1;


/*---------------------------------------------------.
| incr_yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
incr_yyerrorlab:

#ifdef __GNUC__
  /* Pacify GCC when the user code never invokes YYERROR and the label
     incr_yyerrorlab therefore never appears in user code.  */
  if (0)
     goto incr_yyerrorlab;
#endif

  incr_yyvsp -= incr_yylen;
  incr_yyssp -= incr_yylen;
  incr_yystate = *incr_yyssp;
  goto incr_yyerrlab1;


/*-------------------------------------------------------------.
| incr_yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
incr_yyerrlab1:
  incr_yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      incr_yyn = incr_yypact[incr_yystate];
      if (incr_yyn != YYPACT_NINF)
	{
	  incr_yyn += YYTERROR;
	  if (0 <= incr_yyn && incr_yyn <= YYLAST && incr_yycheck[incr_yyn] == YYTERROR)
	    {
	      incr_yyn = incr_yytable[incr_yyn];
	      if (0 < incr_yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (incr_yyssp == incr_yyss)
	YYABORT;

      YYDSYMPRINTF ("Error: popping", incr_yystos[*incr_yyssp], incr_yyvsp, incr_yylsp);
      incr_yydestruct (incr_yystos[incr_yystate], incr_yyvsp);
      YYPOPSTACK;
      incr_yystate = *incr_yyssp;
      YY_STACK_PRINT (incr_yyss, incr_yyssp);
    }

  if (incr_yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++incr_yyvsp = incr_yylval;


  incr_yystate = incr_yyn;
  goto incr_yynewstate;


/*-------------------------------------.
| incr_yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
incr_yyacceptlab:
  incr_yyresult = 0;
  goto incr_yyreturn;

/*-----------------------------------.
| incr_yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
incr_yyabortlab:
  incr_yyresult = 1;
  goto incr_yyreturn;

#ifndef incr_yyoverflow
/*----------------------------------------------.
| incr_yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
incr_yyoverflowlab:
  incr_yyerror ("parser stack overflow");
  incr_yyresult = 2;
  /* Fall through.  */
#endif

incr_yyreturn:
#ifndef incr_yyoverflow
  if (incr_yyss != incr_yyssa)
    YYSTACK_FREE (incr_yyss);
#endif
  return incr_yyresult;
}



