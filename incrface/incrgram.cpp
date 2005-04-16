/* A Bison parser, made by GNU Bison 1.875b.  */

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
     T_open = 261,
     T_close = 262,
     T_insert = 263,
     T_delete = 264,
     T_modify = 265,
     T_lock = 266,
     T_unlock = 267,
     T_segue = 268,
     T_message = 269,
     T_request = 270,
     T_fulfil = 271,
     T_id = 272,
     T_edgeop = 273,
     T_subgraph = 274
   };
#endif
#define T_graph 258
#define T_node 259
#define T_edge 260
#define T_open 261
#define T_close 262
#define T_insert 263
#define T_delete 264
#define T_modify 265
#define T_lock 266
#define T_unlock 267
#define T_segue 268
#define T_message 269
#define T_request 270
#define T_fulfil 271
#define T_id 272
#define T_edgeop 273
#define T_subgraph 274




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
#line 13 "incrgram.ypp"
typedef union YYSTYPE {
			int				i;
			char			*str;
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 124 "incrgram.tab.c"
# define incr_yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 136 "incrgram.tab.c"

#if ! defined (incr_yyoverflow) || YYERROR_VERBOSE

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
#endif /* ! defined (incr_yyoverflow) || YYERROR_VERBOSE */


#if (! defined (incr_yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYSTYPE_IS_TRIVIAL)))

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
#  if 1 < __GNUC__
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
#define YYLAST   83

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  26
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  32
/* YYNRULES -- Number of rules. */
#define YYNRULES  58
/* YYNRULES -- Number of states. */
#define YYNSTATES  110

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   274

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? incr_yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char incr_yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      20,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    25,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    24,
       2,    23,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    21,     2,    22,     2,     2,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char incr_yyprhs[] =
{
       0,     0,     3,     5,     9,    10,    12,    14,    16,    18,
      20,    22,    24,    26,    28,    30,    32,    34,    36,    38,
      40,    42,    44,    46,    48,    50,    52,    56,    59,    63,
      68,    72,    76,    80,    86,    94,   100,   106,   111,   116,
     120,   125,   130,   134,   140,   146,   149,   150,   151,   156,
     157,   159,   160,   162,   166,   170,   172,   174,   175
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const incr_yysigned_char incr_yyrhs[] =
{
      27,     0,    -1,    28,    -1,    28,    29,    20,    -1,    -1,
      30,    -1,    32,    -1,    33,    -1,    34,    -1,    35,    -1,
      36,    -1,    37,    -1,    38,    -1,    39,    -1,    40,    -1,
      41,    -1,    42,    -1,    43,    -1,    44,    -1,    45,    -1,
      46,    -1,    47,    -1,    48,    -1,    49,    -1,    50,    -1,
       1,    -1,     6,     3,    31,    -1,    57,    51,    -1,     7,
       3,    57,    -1,    10,     3,    57,    51,    -1,    11,     3,
      57,    -1,    12,     3,    57,    -1,    13,     3,    57,    -1,
       8,     4,    57,    17,    51,    -1,     8,     5,    57,    17,
      17,    17,    51,    -1,    10,     4,    57,    17,    51,    -1,
      10,     5,    57,    17,    51,    -1,     9,     4,    57,    17,
      -1,     9,     5,    57,    17,    -1,    15,     3,    57,    -1,
      15,     4,    57,    17,    -1,    15,     5,    57,    17,    -1,
      16,     3,    17,    -1,    16,     4,    57,    17,    51,    -1,
      16,     5,    57,    17,    51,    -1,    14,    17,    -1,    -1,
      -1,    21,    52,    53,    22,    -1,    -1,    54,    -1,    -1,
      55,    -1,    54,    56,    55,    -1,    17,    23,    17,    -1,
      24,    -1,    25,    -1,    -1,    17,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned char incr_yyrline[] =
{
       0,    20,    20,    24,    25,    29,    29,    29,    30,    30,
      31,    32,    32,    33,    33,    34,    34,    35,    35,    35,
      36,    36,    36,    37,    38,    39,    42,    45,    48,    51,
      54,    57,    60,    63,    66,    69,    72,    75,    78,    81,
      83,    85,    87,    89,    91,    93,    96,   100,   100,   101,
     104,   105,   108,   109,   112,   115,   115,   115,   117
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const incr_yytname[] =
{
  "$end", "error", "$undefined", "T_graph", "T_node", "T_edge", "T_open", 
  "T_close", "T_insert", "T_delete", "T_modify", "T_lock", "T_unlock", 
  "T_segue", "T_message", "T_request", "T_fulfil", "T_id", "T_edgeop", 
  "T_subgraph", "'\\n'", "'['", "']'", "'='", "';'", "','", "$accept", 
  "session", "commands", "command", "open_graph", "open_graph2", 
  "close_graph", "mod_graph", "lock_graph", "unlock_graph", "segue", 
  "ins_node", "ins_edge", "mod_node", "mod_edge", "del_node", "del_edge", 
  "req_graph", "req_node", "req_edge", "ful_graph", "ful_node", 
  "ful_edge", "message", "nop", "attrlist", "@1", "optattrdefs", 
  "attrdefs", "attritem", "optsep", "graphid", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short incr_yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
      10,    91,    93,    61,    59,    44
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char incr_yyr1[] =
{
       0,    26,    27,    28,    28,    29,    29,    29,    29,    29,
      29,    29,    29,    29,    29,    29,    29,    29,    29,    29,
      29,    29,    29,    29,    29,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    52,    51,    51,
      53,    53,    54,    54,    55,    56,    56,    56,    57
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char incr_yyr2[] =
{
       0,     2,     1,     3,     0,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     2,     3,     4,
       3,     3,     3,     5,     7,     5,     5,     4,     4,     3,
       4,     4,     3,     5,     5,     2,     0,     0,     4,     0,
       1,     0,     1,     3,     3,     1,     1,     0,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char incr_yydefact[] =
{
       4,     0,     0,     1,    25,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    45,
       0,     0,     0,     0,     0,     0,     3,    58,    26,    49,
      28,     0,     0,     0,     0,    49,     0,     0,    30,    31,
      32,    39,     0,     0,    42,     0,     0,    47,    27,    49,
       0,    37,    38,    29,    49,    49,    40,    41,    49,    49,
      51,    33,     0,    35,    36,    43,    44,     0,     0,    50,
      52,    49,     0,    48,    55,    56,     0,    34,    54,    53
};

/* YYDEFGOTO[NTERM-NUM]. */
static const incr_yysigned_char incr_yydefgoto[] =
{
      -1,     1,     2,    16,    17,    58,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    78,    90,    98,    99,   100,
     106,    59
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -66
static const incr_yysigned_char incr_yypact[] =
{
     -66,    21,    31,   -66,   -66,    10,    19,    11,    13,    -1,
      22,    23,    24,    12,     2,     7,     8,   -66,   -66,   -66,
     -66,   -66,   -66,   -66,   -66,   -66,   -66,   -66,   -66,   -66,
     -66,   -66,   -66,   -66,   -66,   -66,   -66,    16,    16,    16,
      16,    16,    16,    16,    16,    16,    16,    16,    16,   -66,
      16,    16,    16,    17,    16,    16,   -66,   -66,   -66,     9,
     -66,    18,    32,    33,    46,     9,    50,    53,   -66,   -66,
     -66,   -66,    54,    55,   -66,    56,    57,   -66,   -66,     9,
      58,   -66,   -66,   -66,     9,     9,   -66,   -66,     9,     9,
      59,   -66,    60,   -66,   -66,   -66,   -66,    25,    61,   -16,
     -66,     9,    62,   -66,   -66,   -66,    59,   -66,   -66,   -66
};

/* YYPGOTO[NTERM-NUM].  */
static const incr_yysigned_char incr_yypgoto[] =
{
     -66,   -66,   -66,   -66,   -66,   -66,   -66,   -66,   -66,   -66,
     -66,   -66,   -66,   -66,   -66,   -66,   -66,   -66,   -66,   -66,
     -66,   -66,   -66,   -66,   -66,   -65,   -66,   -66,   -66,   -28,
     -66,    14
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -58
static const incr_yysigned_char incr_yytable[] =
{
      83,   -57,    43,    44,    45,    50,    51,    52,   104,   105,
      53,    54,    55,    37,    91,    39,    40,    41,    42,    93,
      94,     3,    38,    95,    96,    46,    47,    48,    56,    49,
      77,    -2,     4,    57,    74,    79,   107,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,   102,    80,
      81,   -46,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    82,    71,    72,    73,    84,    75,    76,
      85,    86,    87,    88,    89,    92,    97,   101,   109,   108,
       0,     0,     0,   103
};

static const incr_yysigned_char incr_yycheck[] =
{
      65,    17,     3,     4,     5,     3,     4,     5,    24,    25,
       3,     4,     5,     3,    79,     4,     5,     4,     5,    84,
      85,     0,     3,    88,    89,     3,     3,     3,    20,    17,
      21,     0,     1,    17,    17,    17,   101,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    23,    17,
      17,    20,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    17,    50,    51,    52,    17,    54,    55,
      17,    17,    17,    17,    17,    17,    17,    17,   106,    17,
      -1,    -1,    -1,    22
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char incr_yystos[] =
{
       0,    27,    28,     0,     1,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    29,    30,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,     3,     3,     4,
       5,     4,     5,     3,     4,     5,     3,     3,     3,    17,
       3,     4,     5,     3,     4,     5,    20,    17,    31,    57,
      57,    57,    57,    57,    57,    57,    57,    57,    57,    57,
      57,    57,    57,    57,    17,    57,    57,    21,    51,    17,
      17,    17,    17,    51,    17,    17,    17,    17,    17,    17,
      52,    51,    17,    51,    51,    51,    51,    17,    53,    54,
      55,    17,    23,    22,    24,    25,    56,    51,    17,    55
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
#define YYERROR		goto incr_yyerrlab1


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
# define YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
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
| TOP (cinluded).                                                   |
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

#if YYMAXDEPTH == 0
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
        case 25:
#line 39 "incrgram.ypp"
    {incr_abort(IF_ERR_SYNTAX);;}
    break;

  case 26:
#line 42 "incrgram.ypp"
    {;}
    break;

  case 27:
#line 45 "incrgram.ypp"
    {incr_open_graph(incr_yyvsp[-1].str);;}
    break;

  case 28:
#line 48 "incrgram.ypp"
    {incr_close_graph(incr_yyvsp[0].str);;}
    break;

  case 29:
#line 51 "incrgram.ypp"
    {incr_mod_graph(incr_yyvsp[-1].str);;}
    break;

  case 30:
#line 54 "incrgram.ypp"
    {incr_lock(incr_yyvsp[0].str);;}
    break;

  case 31:
#line 57 "incrgram.ypp"
    {incr_unlock(incr_yyvsp[0].str);;}
    break;

  case 32:
#line 60 "incrgram.ypp"
    {incr_segue(incr_yyvsp[0].str);;}
    break;

  case 33:
#line 63 "incrgram.ypp"
    {incr_ins_node(incr_yyvsp[-2].str,incr_yyvsp[-1].str);;}
    break;

  case 34:
#line 66 "incrgram.ypp"
    {incr_ins_edge(incr_yyvsp[-4].str,incr_yyvsp[-3].str,incr_yyvsp[-2].str,incr_yyvsp[-1].str);;}
    break;

  case 35:
#line 69 "incrgram.ypp"
    {incr_mod_node(incr_yyvsp[-2].str,incr_yyvsp[-1].str);;}
    break;

  case 36:
#line 72 "incrgram.ypp"
    {incr_mod_edge(incr_yyvsp[-2].str,incr_yyvsp[-1].str);;}
    break;

  case 37:
#line 75 "incrgram.ypp"
    {incr_del_node(incr_yyvsp[-1].str,incr_yyvsp[0].str);;}
    break;

  case 38:
#line 78 "incrgram.ypp"
    {incr_del_edge(incr_yyvsp[-1].str,incr_yyvsp[0].str);;}
    break;

  case 39:
#line 81 "incrgram.ypp"
    {incr_req_graph(incr_yyvsp[0].str);;}
    break;

  case 40:
#line 83 "incrgram.ypp"
    {incr_req_node(incr_yyvsp[-1].str,incr_yyvsp[0].str);;}
    break;

  case 41:
#line 85 "incrgram.ypp"
    {incr_req_edge(incr_yyvsp[-1].str,incr_yyvsp[0].str);;}
    break;

  case 42:
#line 87 "incrgram.ypp"
    {incr_ful_graph(incr_yyvsp[0].str);;}
    break;

  case 43:
#line 89 "incrgram.ypp"
    {incr_ful_node(incr_yyvsp[-2].str,incr_yyvsp[-1].str);;}
    break;

  case 44:
#line 91 "incrgram.ypp"
    {incr_ful_edge(incr_yyvsp[-2].str,incr_yyvsp[-1].str);;}
    break;

  case 45:
#line 93 "incrgram.ypp"
    {incr_message(incr_yyvsp[0].str);;}
    break;

  case 47:
#line 100 "incrgram.ypp"
    {incr_reset_attrs();;}
    break;

  case 49:
#line 101 "incrgram.ypp"
    {incr_reset_attrs();;}
    break;

  case 54:
#line 112 "incrgram.ypp"
    {incr_append_attr(incr_yyvsp[-2].str,incr_yyvsp[0].str);;}
    break;

  case 58:
#line 117 "incrgram.ypp"
    {incr_yyval.str = incr_yyvsp[0].str; ;}
    break;


    }

/* Line 999 of yacc.c.  */
#line 1223 "incrgram.tab.c"

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

      /* Return failure if at end of input.  */
      if (incr_yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (incr_yyss < incr_yyssp)
	    {
	      YYDSYMPRINTF ("Error: popping", incr_yystos[*incr_yyssp], incr_yyvsp, incr_yylsp);
	      incr_yydestruct (incr_yystos[*incr_yyssp], incr_yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDSYMPRINTF ("Error: discarding", incr_yytoken, &incr_yylval, &incr_yylloc);
      incr_yydestruct (incr_yytoken, &incr_yylval);
      incr_yychar = YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto incr_yyerrlab1;


/*----------------------------------------------------.
| incr_yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
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
      incr_yyvsp--;
      incr_yystate = *--incr_yyssp;

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



