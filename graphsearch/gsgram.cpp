
/*  A Bison parser, made from gsgram.ypp
    by GNU Bison version 1.28  */

#define YYBISON 1  /* Identify Bison output.  */

#define	T_graph	257
#define	T_node	258
#define	T_edge	259
#define	T_pattern	260
#define	T_search	261
#define	T_input	262
#define	T_open	263
#define	T_close	264
#define	T_insert	265
#define	T_delete	266
#define	T_modify	267
#define	T_lock	268
#define	T_unlock	269
#define	T_segue	270
#define	T_define	271
#define	T_id	272
#define	T_edgeop	273
#define	T_subgraph	274

#line 1 "gsgram.ypp"

#pragma prototyped
#include "graphsearch/gscmds.h"

#line 12 "gsgram.ypp"
typedef union	{
			int				i;
			char			*str;
} YYSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		88
#define	YYFLAG		-32768
#define	YYNTBASE	27

#define YYTRANSLATE(x) ((unsigned)(x) <= 274 ? gs_yytranslate[x] : 54)

static const char gs_yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    21,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,    26,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,    25,     2,
    24,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    22,     2,    23,     2,     2,     2,     2,     2,     2,     2,
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
     2,     2,     2,     2,     2,     1,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20
};

#if YYDEBUG != 0
static const short gs_yyprhs[] = {     0,
     0,     2,     6,     7,     9,    11,    13,    15,    17,    19,
    21,    23,    25,    27,    29,    31,    33,    35,    37,    39,
    41,    42,    46,    49,    53,    58,    62,    66,    70,    73,
    76,    79,    85,    91,    96,   104,   110,   115,   116,   121,
   122,   124,   125,   127,   131,   135,   137,   139,   140
};

static const short gs_yyrhs[] = {    28,
     0,    28,    29,    21,     0,     0,    41,     0,    44,     0,
    42,     0,    45,     0,    43,     0,    46,     0,    31,     0,
    33,     0,    34,     0,    35,     0,    36,     0,    37,     0,
    38,     0,    39,     0,    40,     0,    30,     0,     1,     0,
     0,     9,     3,    32,     0,    53,    47,     0,    10,     3,
    53,     0,    13,     3,    53,    47,     0,    14,     3,    53,
     0,    15,     3,    53,     0,    16,     3,    53,     0,    17,
     6,     0,    17,     7,     0,    17,     8,     0,    11,     4,
    53,    18,    47,     0,    13,     4,    53,    18,    47,     0,
    12,     4,    53,    18,     0,    11,     5,    53,    18,    18,
    18,    47,     0,    13,     5,    53,    18,    47,     0,    12,
     5,    53,    18,     0,     0,    22,    48,    49,    23,     0,
     0,    50,     0,     0,    51,     0,    50,    52,    51,     0,
    18,    24,    18,     0,    25,     0,    26,     0,     0,    18,
     0
};

#endif

#if YYDEBUG != 0
static const short gs_yyrline[] = { 0,
    19,    23,    24,    27,    28,    29,    29,    30,    30,    31,
    31,    31,    32,    32,    33,    34,    34,    34,    35,    36,
    39,    42,    45,    48,    51,    54,    57,    60,    63,    66,
    69,    72,    75,    78,    81,    84,    87,    90,    90,    91,
    94,    95,    98,    99,   102,   105,   105,   105,   107
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const gs_yytname[] = {   "$","error","$undefined.","T_graph",
"T_node","T_edge","T_pattern","T_search","T_input","T_open","T_close","T_insert",
"T_delete","T_modify","T_lock","T_unlock","T_segue","T_define","T_id","T_edgeop",
"T_subgraph","'\\n'","'['","']'","'='","';'","','","session","commands","command",
"nop","open_graph","open_graph2","close_graph","mod_graph","lock_graph","unlock_graph",
"segue","define_pattern","define_search","define_input","ins_node","mod_node",
"del_node","ins_edge","mod_edge","del_edge","attrlist","@1","optattrdefs","attrdefs",
"attritem","optsep","viewid", NULL
};
#endif

static const short gs_yyr1[] = {     0,
    27,    28,    28,    29,    29,    29,    29,    29,    29,    29,
    29,    29,    29,    29,    29,    29,    29,    29,    29,    29,
    30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
    40,    41,    42,    43,    44,    45,    46,    48,    47,    47,
    49,    49,    50,    50,    51,    52,    52,    52,    53
};

static const short gs_yyr2[] = {     0,
     1,     3,     0,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     0,     3,     2,     3,     4,     3,     3,     3,     2,     2,
     2,     5,     5,     4,     7,     5,     4,     0,     4,     0,
     1,     0,     1,     3,     3,     1,     1,     0,     1
};

static const short gs_yydefact[] = {     3,
     0,    20,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    19,    10,    11,    12,    13,    14,    15,    16,
    17,    18,     4,     6,     8,     5,     7,     9,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    29,    30,    31,     2,    49,    22,    40,    24,     0,     0,
     0,     0,    40,     0,     0,    26,    27,    28,    38,    23,
    40,     0,    34,    37,    25,    40,    40,    42,    32,     0,
    33,    36,     0,     0,    41,    43,    40,     0,    39,    46,
    47,     0,    35,    45,    44,     0,     0,     0
};

static const short gs_yydefgoto[] = {    86,
     1,    12,    13,    14,    46,    15,    16,    17,    18,    19,
    20,    21,    22,    23,    24,    25,    26,    27,    28,    60,
    68,    74,    75,    76,    82,    47
};

static const short gs_yypact[] = {-32768,
     6,-32768,    39,    40,     7,    21,     0,    41,    42,    43,
    33,   -20,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    29,    29,
    29,    29,    29,    29,    29,    29,    29,    29,    29,    29,
-32768,-32768,-32768,-32768,-32768,-32768,    26,-32768,    31,    32,
    34,    35,    26,    36,    37,-32768,-32768,-32768,-32768,-32768,
    26,    38,-32768,-32768,-32768,    26,    26,    44,-32768,    45,
-32768,-32768,    27,    46,   -16,-32768,    26,    47,-32768,-32768,
-32768,    44,-32768,-32768,-32768,    57,    58,-32768
};

static const short gs_yypgoto[] = {-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   -53,
-32768,-32768,-32768,   -23,-32768,    -2
};


#define	YYLAST		69


static const short gs_yytable[] = {    65,
    44,   -48,    35,    36,    37,    -1,     2,    69,    80,    81,
    31,    32,    71,    72,     3,     4,     5,     6,     7,     8,
     9,    10,    11,    83,    33,    34,   -21,    48,    49,    50,
    51,    52,    53,    54,    55,    56,    57,    58,    41,    42,
    43,    29,    30,    38,    39,    40,    45,    59,    61,    62,
    78,    63,    64,    66,    67,    70,    87,    88,    85,     0,
     0,    73,    77,     0,    84,     0,     0,     0,    79
};

static const short gs_yycheck[] = {    53,
    21,    18,     3,     4,     5,     0,     1,    61,    25,    26,
     4,     5,    66,    67,     9,    10,    11,    12,    13,    14,
    15,    16,    17,    77,     4,     5,    21,    30,    31,    32,
    33,    34,    35,    36,    37,    38,    39,    40,     6,     7,
     8,     3,     3,     3,     3,     3,    18,    22,    18,    18,
    24,    18,    18,    18,    18,    18,     0,     0,    82,    -1,
    -1,    18,    18,    -1,    18,    -1,    -1,    -1,    23
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/share/bison.simple"
/* This file comes from bison-1.28.  */

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

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

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

#ifndef YYSTACK_USE_ALLOCA
#ifdef alloca
#define YYSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#define YYSTACK_USE_ALLOCA
#include <alloca.h>
#else /* not sparc */
/* We think this test detects Watcom and Microsoft C.  */
/* This used to test MSDOS, but that is a bad idea
   since that symbol is in the user namespace.  */
#if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#if 0 /* No need for malloc.h, which pollutes the namespace;
	 instead, just don't use alloca.  */
#include <malloc.h>
#endif
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
/* I don't know what this was needed for, but it pollutes the namespace.
   So I turned it off.   rms, 2 May 1997.  */
/* #include <malloc.h>  */
 #pragma alloca
#define YYSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
		 and on HPUX 10.  Eventually we can turn this on.  */
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* YYSTACK_USE_ALLOCA not defined */

#ifdef YYSTACK_USE_ALLOCA
#define YYSTACK_ALLOC alloca
#else
#define YYSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define gs_yyerrok		(gs_yyerrstatus = 0)
#define gs_yyclearin	(gs_yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto gs_yyacceptlab
#define YYABORT 	goto gs_yyabortlab
#define YYERROR		goto gs_yyerrlab1
/* Like YYERROR except do call gs_yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto gs_yyerrlab
#define YYRECOVERING()  (!!gs_yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (gs_yychar == YYEMPTY && gs_yylen == 1)				\
    { gs_yychar = (token), gs_yylval = (value);			\
      gs_yychar1 = YYTRANSLATE (gs_yychar);				\
      YYPOPSTACK;						\
      goto gs_yybackup;						\
    }								\
  else								\
    { gs_yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		gs_yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		gs_yylex(&gs_yylval, &gs_yylloc, YYLEX_PARAM)
#else
#define YYLEX		gs_yylex(&gs_yylval, &gs_yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		gs_yylex(&gs_yylval, YYLEX_PARAM)
#else
#define YYLEX		gs_yylex(&gs_yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	gs_yychar;			/*  the lookahead symbol		*/
YYSTYPE	gs_yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE gs_yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int gs_yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int gs_yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Define __gs_yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __gs_yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__gs_yy_memcpy (to, from, count)
     char *to;
     char *from;
     unsigned int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__gs_yy_memcpy (char *to, char *from, unsigned int count)
{
  register char *t = to;
  register char *f = from;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 217 "/usr/share/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into gs_yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef YYPARSE_PARAM
int gs_yyparse (void *);
#else
int gs_yyparse (void);
#endif
#endif

int
gs_yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int gs_yystate;
  register int gs_yyn;
  register short *gs_yyssp;
  register YYSTYPE *gs_yyvsp;
  int gs_yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int gs_yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	gs_yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE gs_yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *gs_yyss = gs_yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *gs_yyvs = gs_yyvsa;	/*  to allow gs_yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE gs_yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *gs_yyls = gs_yylsa;
  YYLTYPE *gs_yylsp;

#define YYPOPSTACK   (gs_yyvsp--, gs_yyssp--, gs_yylsp--)
#else
#define YYPOPSTACK   (gs_yyvsp--, gs_yyssp--)
#endif

  int gs_yystacksize = YYINITDEPTH;
  int gs_yyfree_stacks = 0;

#ifdef YYPURE
  int gs_yychar;
  YYSTYPE gs_yylval;
  int gs_yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE gs_yylloc;
#endif
#endif

  YYSTYPE gs_yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int gs_yylen;

#if YYDEBUG != 0
  if (gs_yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  gs_yystate = 0;
  gs_yyerrstatus = 0;
  gs_yynerrs = 0;
  gs_yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  gs_yyssp = gs_yyss - 1;
  gs_yyvsp = gs_yyvs;
#ifdef YYLSP_NEEDED
  gs_yylsp = gs_yyls;
#endif

/* Push a new state, which is found in  gs_yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
gs_yynewstate:

  *++gs_yyssp = gs_yystate;

  if (gs_yyssp >= gs_yyss + gs_yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *gs_yyvs1 = gs_yyvs;
      short *gs_yyss1 = gs_yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *gs_yyls1 = gs_yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = gs_yyssp - gs_yyss + 1;

#ifdef gs_yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if gs_yyoverflow is a macro.  */
      gs_yyoverflow("parser stack overflow",
		 &gs_yyss1, size * sizeof (*gs_yyssp),
		 &gs_yyvs1, size * sizeof (*gs_yyvsp),
		 &gs_yyls1, size * sizeof (*gs_yylsp),
		 &gs_yystacksize);
#else
      gs_yyoverflow("parser stack overflow",
		 &gs_yyss1, size * sizeof (*gs_yyssp),
		 &gs_yyvs1, size * sizeof (*gs_yyvsp),
		 &gs_yystacksize);
#endif

      gs_yyss = gs_yyss1; gs_yyvs = gs_yyvs1;
#ifdef YYLSP_NEEDED
      gs_yyls = gs_yyls1;
#endif
#else /* no gs_yyoverflow */
      /* Extend the stack our own way.  */
      if (gs_yystacksize >= YYMAXDEPTH)
	{
	  gs_yyerror("parser stack overflow");
	  if (gs_yyfree_stacks)
	    {
	      free (gs_yyss);
	      free (gs_yyvs);
#ifdef YYLSP_NEEDED
	      free (gs_yyls);
#endif
	    }
	  return 2;
	}
      gs_yystacksize *= 2;
      if (gs_yystacksize > YYMAXDEPTH)
	gs_yystacksize = YYMAXDEPTH;
#ifndef YYSTACK_USE_ALLOCA
      gs_yyfree_stacks = 1;
#endif
      gs_yyss = (short *) YYSTACK_ALLOC (gs_yystacksize * sizeof (*gs_yyssp));
      __gs_yy_memcpy ((char *)gs_yyss, (char *)gs_yyss1,
		   size * (unsigned int) sizeof (*gs_yyssp));
      gs_yyvs = (YYSTYPE *) YYSTACK_ALLOC (gs_yystacksize * sizeof (*gs_yyvsp));
      __gs_yy_memcpy ((char *)gs_yyvs, (char *)gs_yyvs1,
		   size * (unsigned int) sizeof (*gs_yyvsp));
#ifdef YYLSP_NEEDED
      gs_yyls = (YYLTYPE *) YYSTACK_ALLOC (gs_yystacksize * sizeof (*gs_yylsp));
      __gs_yy_memcpy ((char *)gs_yyls, (char *)gs_yyls1,
		   size * (unsigned int) sizeof (*gs_yylsp));
#endif
#endif /* no gs_yyoverflow */

      gs_yyssp = gs_yyss + size - 1;
      gs_yyvsp = gs_yyvs + size - 1;
#ifdef YYLSP_NEEDED
      gs_yylsp = gs_yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (gs_yydebug)
	fprintf(stderr, "Stack size increased to %d\n", gs_yystacksize);
#endif

      if (gs_yyssp >= gs_yyss + gs_yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (gs_yydebug)
    fprintf(stderr, "Entering state %d\n", gs_yystate);
#endif

  goto gs_yybackup;
 gs_yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* gs_yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  gs_yyn = gs_yypact[gs_yystate];
  if (gs_yyn == YYFLAG)
    goto gs_yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* gs_yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (gs_yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (gs_yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      gs_yychar = YYLEX;
    }

  /* Convert token to internal form (in gs_yychar1) for indexing tables with */

  if (gs_yychar <= 0)		/* This means end of input. */
    {
      gs_yychar1 = 0;
      gs_yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (gs_yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      gs_yychar1 = YYTRANSLATE(gs_yychar);

#if YYDEBUG != 0
      if (gs_yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", gs_yychar, gs_yytname[gs_yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, gs_yychar, gs_yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  gs_yyn += gs_yychar1;
  if (gs_yyn < 0 || gs_yyn > YYLAST || gs_yycheck[gs_yyn] != gs_yychar1)
    goto gs_yydefault;

  gs_yyn = gs_yytable[gs_yyn];

  /* gs_yyn is what to do for this token type in this state.
     Negative => reduce, -gs_yyn is rule number.
     Positive => shift, gs_yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (gs_yyn < 0)
    {
      if (gs_yyn == YYFLAG)
	goto gs_yyerrlab;
      gs_yyn = -gs_yyn;
      goto gs_yyreduce;
    }
  else if (gs_yyn == 0)
    goto gs_yyerrlab;

  if (gs_yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (gs_yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", gs_yychar, gs_yytname[gs_yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (gs_yychar != YYEOF)
    gs_yychar = YYEMPTY;

  *++gs_yyvsp = gs_yylval;
#ifdef YYLSP_NEEDED
  *++gs_yylsp = gs_yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (gs_yyerrstatus) gs_yyerrstatus--;

  gs_yystate = gs_yyn;
  goto gs_yynewstate;

/* Do the default action for the current state.  */
gs_yydefault:

  gs_yyn = gs_yydefact[gs_yystate];
  if (gs_yyn == 0)
    goto gs_yyerrlab;

/* Do a reduction.  gs_yyn is the number of a rule to reduce with.  */
gs_yyreduce:
  gs_yylen = gs_yyr2[gs_yyn];
  if (gs_yylen > 0)
    gs_yyval = gs_yyvsp[1-gs_yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (gs_yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       gs_yyn, gs_yyrline[gs_yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = gs_yyprhs[gs_yyn]; gs_yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", gs_yytname[gs_yyrhs[i]]);
      fprintf (stderr, " -> %s\n", gs_yytname[gs_yyr1[gs_yyn]]);
    }
#endif


  switch (gs_yyn) {

case 20:
#line 36 "gsgram.ypp"
{gs_abort(IF_ERR_SYNTAX);;
    break;}
case 22:
#line 42 "gsgram.ypp"
{;
    break;}
case 23:
#line 45 "gsgram.ypp"
{gs_open_graph(gs_yyvsp[-1].str);;
    break;}
case 24:
#line 48 "gsgram.ypp"
{gs_close_graph(gs_yyvsp[0].str);;
    break;}
case 25:
#line 51 "gsgram.ypp"
{gs_mod_graph(gs_yyvsp[-1].str);;
    break;}
case 26:
#line 54 "gsgram.ypp"
{gs_lock(gs_yyvsp[0].str);;
    break;}
case 27:
#line 57 "gsgram.ypp"
{gs_unlock(gs_yyvsp[0].str);;
    break;}
case 28:
#line 60 "gsgram.ypp"
{gs_segue(gs_yyvsp[0].str);;
    break;}
case 29:
#line 63 "gsgram.ypp"
{ gs_define_pattern(); ;
    break;}
case 30:
#line 66 "gsgram.ypp"
{ gs_define_search(); ;
    break;}
case 31:
#line 69 "gsgram.ypp"
{ gs_define_input(); ;
    break;}
case 32:
#line 72 "gsgram.ypp"
{gs_ins_node(gs_yyvsp[-2].str,gs_yyvsp[-1].str);;
    break;}
case 33:
#line 75 "gsgram.ypp"
{gs_mod_node(gs_yyvsp[-2].str,gs_yyvsp[-1].str);;
    break;}
case 34:
#line 78 "gsgram.ypp"
{gs_del_node(gs_yyvsp[-1].str,gs_yyvsp[0].str);;
    break;}
case 35:
#line 81 "gsgram.ypp"
{gs_ins_edge(gs_yyvsp[-4].str,gs_yyvsp[-3].str,gs_yyvsp[-2].str,gs_yyvsp[-1].str);;
    break;}
case 36:
#line 84 "gsgram.ypp"
{gs_mod_edge(gs_yyvsp[-2].str,gs_yyvsp[-1].str);;
    break;}
case 37:
#line 87 "gsgram.ypp"
{gs_del_edge(gs_yyvsp[-1].str,gs_yyvsp[0].str);;
    break;}
case 38:
#line 90 "gsgram.ypp"
{gs_reset_attrs();;
    break;}
case 40:
#line 91 "gsgram.ypp"
{gs_reset_attrs();;
    break;}
case 45:
#line 102 "gsgram.ypp"
{gs_append_attr(gs_yyvsp[-2].str,gs_yyvsp[0].str);;
    break;}
case 49:
#line 107 "gsgram.ypp"
{gs_yyval.str = gs_yyvsp[0].str; ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 543 "/usr/share/bison.simple"

  gs_yyvsp -= gs_yylen;
  gs_yyssp -= gs_yylen;
#ifdef YYLSP_NEEDED
  gs_yylsp -= gs_yylen;
#endif

#if YYDEBUG != 0
  if (gs_yydebug)
    {
      short *ssp1 = gs_yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != gs_yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++gs_yyvsp = gs_yyval;

#ifdef YYLSP_NEEDED
  gs_yylsp++;
  if (gs_yylen == 0)
    {
      gs_yylsp->first_line = gs_yylloc.first_line;
      gs_yylsp->first_column = gs_yylloc.first_column;
      gs_yylsp->last_line = (gs_yylsp-1)->last_line;
      gs_yylsp->last_column = (gs_yylsp-1)->last_column;
      gs_yylsp->text = 0;
    }
  else
    {
      gs_yylsp->last_line = (gs_yylsp+gs_yylen-1)->last_line;
      gs_yylsp->last_column = (gs_yylsp+gs_yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  gs_yyn = gs_yyr1[gs_yyn];

  gs_yystate = gs_yypgoto[gs_yyn - YYNTBASE] + *gs_yyssp;
  if (gs_yystate >= 0 && gs_yystate <= YYLAST && gs_yycheck[gs_yystate] == *gs_yyssp)
    gs_yystate = gs_yytable[gs_yystate];
  else
    gs_yystate = gs_yydefgoto[gs_yyn - YYNTBASE];

  goto gs_yynewstate;

gs_yyerrlab:   /* here on detecting error */

  if (! gs_yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++gs_yynerrs;

#ifdef YYERROR_VERBOSE
      gs_yyn = gs_yypact[gs_yystate];

      if (gs_yyn > YYFLAG && gs_yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -gs_yyn if nec to avoid negative indexes in gs_yycheck.  */
	  for (x = (gs_yyn < 0 ? -gs_yyn : 0);
	       x < (sizeof(gs_yytname) / sizeof(char *)); x++)
	    if (gs_yycheck[x + gs_yyn] == x)
	      size += strlen(gs_yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (gs_yyn < 0 ? -gs_yyn : 0);
		       x < (sizeof(gs_yytname) / sizeof(char *)); x++)
		    if (gs_yycheck[x + gs_yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, gs_yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      gs_yyerror(msg);
	      free(msg);
	    }
	  else
	    gs_yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	gs_yyerror("parse error");
    }

  goto gs_yyerrlab1;
gs_yyerrlab1:   /* here on error raised explicitly by an action */

  if (gs_yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (gs_yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (gs_yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", gs_yychar, gs_yytname[gs_yychar1]);
#endif

      gs_yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  gs_yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto gs_yyerrhandle;

gs_yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  gs_yyn = gs_yydefact[gs_yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (gs_yyn) goto gs_yydefault;
#endif

gs_yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (gs_yyssp == gs_yyss) YYABORT;
  gs_yyvsp--;
  gs_yystate = *--gs_yyssp;
#ifdef YYLSP_NEEDED
  gs_yylsp--;
#endif

#if YYDEBUG != 0
  if (gs_yydebug)
    {
      short *ssp1 = gs_yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != gs_yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

gs_yyerrhandle:

  gs_yyn = gs_yypact[gs_yystate];
  if (gs_yyn == YYFLAG)
    goto gs_yyerrdefault;

  gs_yyn += YYTERROR;
  if (gs_yyn < 0 || gs_yyn > YYLAST || gs_yycheck[gs_yyn] != YYTERROR)
    goto gs_yyerrdefault;

  gs_yyn = gs_yytable[gs_yyn];
  if (gs_yyn < 0)
    {
      if (gs_yyn == YYFLAG)
	goto gs_yyerrpop;
      gs_yyn = -gs_yyn;
      goto gs_yyreduce;
    }
  else if (gs_yyn == 0)
    goto gs_yyerrpop;

  if (gs_yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (gs_yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++gs_yyvsp = gs_yylval;
#ifdef YYLSP_NEEDED
  *++gs_yylsp = gs_yylloc;
#endif

  gs_yystate = gs_yyn;
  goto gs_yynewstate;

 gs_yyacceptlab:
  /* YYACCEPT comes here.  */
  if (gs_yyfree_stacks)
    {
      free (gs_yyss);
      free (gs_yyvs);
#ifdef YYLSP_NEEDED
      free (gs_yyls);
#endif
    }
  return 0;

 gs_yyabortlab:
  /* YYABORT comes here.  */
  if (gs_yyfree_stacks)
    {
      free (gs_yyss);
      free (gs_yyvs);
#ifdef YYLSP_NEEDED
      free (gs_yyls);
#endif
    }
  return 1;
}
#line 109 "gsgram.ypp"
