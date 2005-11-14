
/*  A Bison parser, made from incrgram.ypp
    by GNU Bison version 1.28  */

#define YYBISON 1  /* Identify Bison output.  */

#define	T_graph	257
#define	T_node	258
#define	T_edge	259
#define	T_open	260
#define	T_close	261
#define	T_insert	262
#define	T_delete	263
#define	T_modify	264
#define	T_lock	265
#define	T_unlock	266
#define	T_segue	267
#define	T_message	268
#define	T_request	269
#define	T_fulfil	270
#define	T_id	271
#define	T_edgeop	272
#define	T_subgraph	273

#line 1 "incrgram.ypp"

#pragma prototyped
#include "incrface/incrcmds.h"

#line 13 "incrgram.ypp"
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



#define	YYFINAL		110
#define	YYFLAG		-32768
#define	YYNTBASE	26

#define YYTRANSLATE(x) ((unsigned)(x) <= 273 ? incr_yytranslate[x] : 57)

static const char incr_yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    20,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,    25,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,    24,     2,
    23,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    21,     2,    22,     2,     2,     2,     2,     2,     2,     2,
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
    17,    18,    19
};

#if YYDEBUG != 0
static const short incr_yyprhs[] = {     0,
     0,     2,     6,     7,     9,    11,    13,    15,    17,    19,
    21,    23,    25,    27,    29,    31,    33,    35,    37,    39,
    41,    43,    45,    47,    49,    53,    56,    60,    65,    69,
    73,    77,    83,    91,    97,   103,   108,   113,   117,   122,
   127,   131,   137,   143,   146,   147,   148,   153,   154,   156,
   157,   159,   163,   167,   169,   171,   172
};

static const short incr_yyrhs[] = {    27,
     0,    27,    28,    20,     0,     0,    29,     0,    31,     0,
    32,     0,    33,     0,    34,     0,    35,     0,    36,     0,
    37,     0,    38,     0,    39,     0,    40,     0,    41,     0,
    42,     0,    43,     0,    44,     0,    45,     0,    46,     0,
    47,     0,    48,     0,    49,     0,     1,     0,     6,     3,
    30,     0,    56,    50,     0,     7,     3,    56,     0,    10,
     3,    56,    50,     0,    11,     3,    56,     0,    12,     3,
    56,     0,    13,     3,    56,     0,     8,     4,    56,    17,
    50,     0,     8,     5,    56,    17,    17,    17,    50,     0,
    10,     4,    56,    17,    50,     0,    10,     5,    56,    17,
    50,     0,     9,     4,    56,    17,     0,     9,     5,    56,
    17,     0,    15,     3,    56,     0,    15,     4,    56,    17,
     0,    15,     5,    56,    17,     0,    16,     3,    17,     0,
    16,     4,    56,    17,    50,     0,    16,     5,    56,    17,
    50,     0,    14,    17,     0,     0,     0,    21,    51,    52,
    22,     0,     0,    53,     0,     0,    54,     0,    53,    55,
    54,     0,    17,    23,    17,     0,    24,     0,    25,     0,
     0,    17,     0
};

#endif

#if YYDEBUG != 0
static const short incr_yyrline[] = { 0,
    20,    24,    25,    28,    29,    29,    30,    30,    31,    32,
    32,    33,    33,    34,    34,    35,    35,    35,    36,    36,
    36,    37,    38,    39,    42,    45,    48,    51,    54,    57,
    60,    63,    66,    69,    72,    75,    78,    81,    83,    85,
    87,    89,    91,    93,    96,   100,   100,   101,   104,   105,
   108,   109,   112,   115,   115,   115,   117
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const incr_yytname[] = {   "$","error","$undefined.","T_graph",
"T_node","T_edge","T_open","T_close","T_insert","T_delete","T_modify","T_lock",
"T_unlock","T_segue","T_message","T_request","T_fulfil","T_id","T_edgeop","T_subgraph",
"'\\n'","'['","']'","'='","';'","','","session","commands","command","open_graph",
"open_graph2","close_graph","mod_graph","lock_graph","unlock_graph","segue",
"ins_node","ins_edge","mod_node","mod_edge","del_node","del_edge","req_graph",
"req_node","req_edge","ful_graph","ful_node","ful_edge","message","nop","attrlist",
"@1","optattrdefs","attrdefs","attritem","optsep","graphid", NULL
};
#endif

static const short incr_yyr1[] = {     0,
    26,    27,    27,    28,    28,    28,    28,    28,    28,    28,
    28,    28,    28,    28,    28,    28,    28,    28,    28,    28,
    28,    28,    28,    28,    29,    30,    31,    32,    33,    34,
    35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
    45,    46,    47,    48,    49,    51,    50,    50,    52,    52,
    53,    53,    54,    55,    55,    55,    56
};

static const short incr_yyr2[] = {     0,
     1,     3,     0,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     3,     2,     3,     4,     3,     3,
     3,     5,     7,     5,     5,     4,     4,     3,     4,     4,
     3,     5,     5,     2,     0,     0,     4,     0,     1,     0,
     1,     3,     3,     1,     1,     0,     1
};

static const short incr_yydefact[] = {     3,
     0,    24,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     4,     5,     6,     7,     8,     9,
    10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
    20,    21,    22,    23,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,    44,     0,     0,     0,
     0,     0,     0,     2,    57,    25,    48,    27,     0,     0,
     0,     0,    48,     0,     0,    29,    30,    31,    38,     0,
     0,    41,     0,     0,    46,    26,    48,     0,    36,    37,
    28,    48,    48,    39,    40,    48,    48,    50,    32,     0,
    34,    35,    42,    43,     0,     0,    49,    51,    48,     0,
    47,    54,    55,     0,    33,    53,    52,     0,     0,     0
};

static const short incr_yydefgoto[] = {   108,
     1,    14,    15,    56,    16,    17,    18,    19,    20,    21,
    22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
    32,    33,    34,    76,    88,    96,    97,    98,   104,    57
};

static const short incr_yypact[] = {-32768,
    31,-32768,    10,    18,    11,    13,    -1,    19,    22,    23,
    12,     2,     7,     8,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,    17,    17,    17,    17,    17,    17,
    17,    17,    17,    17,    17,    17,-32768,    17,    17,    17,
    32,    17,    17,-32768,-32768,-32768,     6,-32768,    33,    46,
    50,    53,     6,    54,    55,-32768,-32768,-32768,-32768,    56,
    57,-32768,    58,    59,-32768,-32768,     6,    60,-32768,-32768,
-32768,     6,     6,-32768,-32768,     6,     6,    61,-32768,    62,
-32768,-32768,-32768,-32768,    25,    63,   -16,-32768,     6,    64,
-32768,-32768,-32768,    61,-32768,-32768,-32768,    30,    35,-32768
};

static const short incr_yypgoto[] = {-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,   -63,-32768,-32768,-32768,   -71,-32768,    16
};


#define	YYLAST		85


static const short incr_yytable[] = {    81,
   -56,    41,    42,    43,    48,    49,    50,   102,   103,    51,
    52,    53,    35,    89,    37,    38,    39,    40,    91,    92,
    36,    44,    93,    94,    45,    46,    75,    54,    47,   109,
    -1,     2,   107,    55,   110,   105,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,   100,    72,    77,
   -45,    58,    59,    60,    61,    62,    63,    64,    65,    66,
    67,    68,    78,    69,    70,    71,    79,    73,    74,    80,
    82,    83,    84,    85,    86,    87,    90,    95,    99,     0,
   106,     0,     0,     0,   101
};

static const short incr_yycheck[] = {    63,
    17,     3,     4,     5,     3,     4,     5,    24,    25,     3,
     4,     5,     3,    77,     4,     5,     4,     5,    82,    83,
     3,     3,    86,    87,     3,     3,    21,    20,    17,     0,
     0,     1,   104,    17,     0,    99,     6,     7,     8,     9,
    10,    11,    12,    13,    14,    15,    16,    23,    17,    17,
    20,    36,    37,    38,    39,    40,    41,    42,    43,    44,
    45,    46,    17,    48,    49,    50,    17,    52,    53,    17,
    17,    17,    17,    17,    17,    17,    17,    17,    17,    -1,
    17,    -1,    -1,    -1,    22
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

#define incr_yyerrok		(incr_yyerrstatus = 0)
#define incr_yyclearin	(incr_yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto incr_yyacceptlab
#define YYABORT 	goto incr_yyabortlab
#define YYERROR		goto incr_yyerrlab1
/* Like YYERROR except do call incr_yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto incr_yyerrlab
#define YYRECOVERING()  (!!incr_yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (incr_yychar == YYEMPTY && incr_yylen == 1)				\
    { incr_yychar = (token), incr_yylval = (value);			\
      incr_yychar1 = YYTRANSLATE (incr_yychar);				\
      YYPOPSTACK;						\
      goto incr_yybackup;						\
    }								\
  else								\
    { incr_yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		incr_yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		incr_yylex(&incr_yylval, &incr_yylloc, YYLEX_PARAM)
#else
#define YYLEX		incr_yylex(&incr_yylval, &incr_yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		incr_yylex(&incr_yylval, YYLEX_PARAM)
#else
#define YYLEX		incr_yylex(&incr_yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	incr_yychar;			/*  the lookahead symbol		*/
YYSTYPE	incr_yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE incr_yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int incr_yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int incr_yydebug;			/*  nonzero means print parse trace	*/
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

/* Define __incr_yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __incr_yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__incr_yy_memcpy (to, from, count)
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
__incr_yy_memcpy (char *to, char *from, unsigned int count)
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
   into incr_yyparse.  The argument should have type void *.
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
int incr_yyparse (void *);
#else
int incr_yyparse (void);
#endif
#endif

int
incr_yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int incr_yystate;
  register int incr_yyn;
  register short *incr_yyssp;
  register YYSTYPE *incr_yyvsp;
  int incr_yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int incr_yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	incr_yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE incr_yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *incr_yyss = incr_yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *incr_yyvs = incr_yyvsa;	/*  to allow incr_yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE incr_yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *incr_yyls = incr_yylsa;
  YYLTYPE *incr_yylsp;

#define YYPOPSTACK   (incr_yyvsp--, incr_yyssp--, incr_yylsp--)
#else
#define YYPOPSTACK   (incr_yyvsp--, incr_yyssp--)
#endif

  int incr_yystacksize = YYINITDEPTH;
  int incr_yyfree_stacks = 0;

#ifdef YYPURE
  int incr_yychar;
  YYSTYPE incr_yylval;
  int incr_yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE incr_yylloc;
#endif
#endif

  YYSTYPE incr_yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int incr_yylen;

#if YYDEBUG != 0
  if (incr_yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  incr_yystate = 0;
  incr_yyerrstatus = 0;
  incr_yynerrs = 0;
  incr_yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  incr_yyssp = incr_yyss - 1;
  incr_yyvsp = incr_yyvs;
#ifdef YYLSP_NEEDED
  incr_yylsp = incr_yyls;
#endif

/* Push a new state, which is found in  incr_yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
incr_yynewstate:

  *++incr_yyssp = incr_yystate;

  if (incr_yyssp >= incr_yyss + incr_yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *incr_yyvs1 = incr_yyvs;
      short *incr_yyss1 = incr_yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *incr_yyls1 = incr_yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = incr_yyssp - incr_yyss + 1;

#ifdef incr_yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if incr_yyoverflow is a macro.  */
      incr_yyoverflow("parser stack overflow",
		 &incr_yyss1, size * sizeof (*incr_yyssp),
		 &incr_yyvs1, size * sizeof (*incr_yyvsp),
		 &incr_yyls1, size * sizeof (*incr_yylsp),
		 &incr_yystacksize);
#else
      incr_yyoverflow("parser stack overflow",
		 &incr_yyss1, size * sizeof (*incr_yyssp),
		 &incr_yyvs1, size * sizeof (*incr_yyvsp),
		 &incr_yystacksize);
#endif

      incr_yyss = incr_yyss1; incr_yyvs = incr_yyvs1;
#ifdef YYLSP_NEEDED
      incr_yyls = incr_yyls1;
#endif
#else /* no incr_yyoverflow */
      /* Extend the stack our own way.  */
      if (incr_yystacksize >= YYMAXDEPTH)
	{
	  incr_yyerror("parser stack overflow");
	  if (incr_yyfree_stacks)
	    {
	      free (incr_yyss);
	      free (incr_yyvs);
#ifdef YYLSP_NEEDED
	      free (incr_yyls);
#endif
	    }
	  return 2;
	}
      incr_yystacksize *= 2;
      if (incr_yystacksize > YYMAXDEPTH)
	incr_yystacksize = YYMAXDEPTH;
#ifndef YYSTACK_USE_ALLOCA
      incr_yyfree_stacks = 1;
#endif
      incr_yyss = (short *) YYSTACK_ALLOC (incr_yystacksize * sizeof (*incr_yyssp));
      __incr_yy_memcpy ((char *)incr_yyss, (char *)incr_yyss1,
		   size * (unsigned int) sizeof (*incr_yyssp));
      incr_yyvs = (YYSTYPE *) YYSTACK_ALLOC (incr_yystacksize * sizeof (*incr_yyvsp));
      __incr_yy_memcpy ((char *)incr_yyvs, (char *)incr_yyvs1,
		   size * (unsigned int) sizeof (*incr_yyvsp));
#ifdef YYLSP_NEEDED
      incr_yyls = (YYLTYPE *) YYSTACK_ALLOC (incr_yystacksize * sizeof (*incr_yylsp));
      __incr_yy_memcpy ((char *)incr_yyls, (char *)incr_yyls1,
		   size * (unsigned int) sizeof (*incr_yylsp));
#endif
#endif /* no incr_yyoverflow */

      incr_yyssp = incr_yyss + size - 1;
      incr_yyvsp = incr_yyvs + size - 1;
#ifdef YYLSP_NEEDED
      incr_yylsp = incr_yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (incr_yydebug)
	fprintf(stderr, "Stack size increased to %d\n", incr_yystacksize);
#endif

      if (incr_yyssp >= incr_yyss + incr_yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (incr_yydebug)
    fprintf(stderr, "Entering state %d\n", incr_yystate);
#endif

  goto incr_yybackup;
 incr_yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* incr_yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  incr_yyn = incr_yypact[incr_yystate];
  if (incr_yyn == YYFLAG)
    goto incr_yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* incr_yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (incr_yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (incr_yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      incr_yychar = YYLEX;
    }

  /* Convert token to internal form (in incr_yychar1) for indexing tables with */

  if (incr_yychar <= 0)		/* This means end of input. */
    {
      incr_yychar1 = 0;
      incr_yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (incr_yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      incr_yychar1 = YYTRANSLATE(incr_yychar);

#if YYDEBUG != 0
      if (incr_yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", incr_yychar, incr_yytname[incr_yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, incr_yychar, incr_yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  incr_yyn += incr_yychar1;
  if (incr_yyn < 0 || incr_yyn > YYLAST || incr_yycheck[incr_yyn] != incr_yychar1)
    goto incr_yydefault;

  incr_yyn = incr_yytable[incr_yyn];

  /* incr_yyn is what to do for this token type in this state.
     Negative => reduce, -incr_yyn is rule number.
     Positive => shift, incr_yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (incr_yyn < 0)
    {
      if (incr_yyn == YYFLAG)
	goto incr_yyerrlab;
      incr_yyn = -incr_yyn;
      goto incr_yyreduce;
    }
  else if (incr_yyn == 0)
    goto incr_yyerrlab;

  if (incr_yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (incr_yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", incr_yychar, incr_yytname[incr_yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (incr_yychar != YYEOF)
    incr_yychar = YYEMPTY;

  *++incr_yyvsp = incr_yylval;
#ifdef YYLSP_NEEDED
  *++incr_yylsp = incr_yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (incr_yyerrstatus) incr_yyerrstatus--;

  incr_yystate = incr_yyn;
  goto incr_yynewstate;

/* Do the default action for the current state.  */
incr_yydefault:

  incr_yyn = incr_yydefact[incr_yystate];
  if (incr_yyn == 0)
    goto incr_yyerrlab;

/* Do a reduction.  incr_yyn is the number of a rule to reduce with.  */
incr_yyreduce:
  incr_yylen = incr_yyr2[incr_yyn];
  if (incr_yylen > 0)
    incr_yyval = incr_yyvsp[1-incr_yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (incr_yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       incr_yyn, incr_yyrline[incr_yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = incr_yyprhs[incr_yyn]; incr_yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", incr_yytname[incr_yyrhs[i]]);
      fprintf (stderr, " -> %s\n", incr_yytname[incr_yyr1[incr_yyn]]);
    }
#endif


  switch (incr_yyn) {

case 24:
#line 39 "incrgram.ypp"
{incr_abort(IF_ERR_SYNTAX);;
    break;}
case 25:
#line 42 "incrgram.ypp"
{;
    break;}
case 26:
#line 45 "incrgram.ypp"
{incr_open_graph(incr_yyvsp[-1].str);;
    break;}
case 27:
#line 48 "incrgram.ypp"
{incr_close_graph(incr_yyvsp[0].str);;
    break;}
case 28:
#line 51 "incrgram.ypp"
{incr_mod_graph(incr_yyvsp[-1].str);;
    break;}
case 29:
#line 54 "incrgram.ypp"
{incr_lock(incr_yyvsp[0].str);;
    break;}
case 30:
#line 57 "incrgram.ypp"
{incr_unlock(incr_yyvsp[0].str);;
    break;}
case 31:
#line 60 "incrgram.ypp"
{incr_segue(incr_yyvsp[0].str);;
    break;}
case 32:
#line 63 "incrgram.ypp"
{incr_ins_node(incr_yyvsp[-2].str,incr_yyvsp[-1].str);;
    break;}
case 33:
#line 66 "incrgram.ypp"
{incr_ins_edge(incr_yyvsp[-4].str,incr_yyvsp[-3].str,incr_yyvsp[-2].str,incr_yyvsp[-1].str);;
    break;}
case 34:
#line 69 "incrgram.ypp"
{incr_mod_node(incr_yyvsp[-2].str,incr_yyvsp[-1].str);;
    break;}
case 35:
#line 72 "incrgram.ypp"
{incr_mod_edge(incr_yyvsp[-2].str,incr_yyvsp[-1].str);;
    break;}
case 36:
#line 75 "incrgram.ypp"
{incr_del_node(incr_yyvsp[-1].str,incr_yyvsp[0].str);;
    break;}
case 37:
#line 78 "incrgram.ypp"
{incr_del_edge(incr_yyvsp[-1].str,incr_yyvsp[0].str);;
    break;}
case 38:
#line 81 "incrgram.ypp"
{incr_req_graph(incr_yyvsp[0].str);;
    break;}
case 39:
#line 83 "incrgram.ypp"
{incr_req_node(incr_yyvsp[-1].str,incr_yyvsp[0].str);;
    break;}
case 40:
#line 85 "incrgram.ypp"
{incr_req_edge(incr_yyvsp[-1].str,incr_yyvsp[0].str);;
    break;}
case 41:
#line 87 "incrgram.ypp"
{incr_ful_graph(incr_yyvsp[0].str);;
    break;}
case 42:
#line 89 "incrgram.ypp"
{incr_ful_node(incr_yyvsp[-2].str,incr_yyvsp[-1].str);;
    break;}
case 43:
#line 91 "incrgram.ypp"
{incr_ful_edge(incr_yyvsp[-2].str,incr_yyvsp[-1].str);;
    break;}
case 44:
#line 93 "incrgram.ypp"
{incr_message(incr_yyvsp[0].str);;
    break;}
case 46:
#line 100 "incrgram.ypp"
{incr_reset_attrs();;
    break;}
case 48:
#line 101 "incrgram.ypp"
{incr_reset_attrs();;
    break;}
case 53:
#line 112 "incrgram.ypp"
{incr_append_attr(incr_yyvsp[-2].str,incr_yyvsp[0].str);;
    break;}
case 57:
#line 117 "incrgram.ypp"
{incr_yyval.str = incr_yyvsp[0].str; ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 543 "/usr/share/bison.simple"

  incr_yyvsp -= incr_yylen;
  incr_yyssp -= incr_yylen;
#ifdef YYLSP_NEEDED
  incr_yylsp -= incr_yylen;
#endif

#if YYDEBUG != 0
  if (incr_yydebug)
    {
      short *ssp1 = incr_yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != incr_yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++incr_yyvsp = incr_yyval;

#ifdef YYLSP_NEEDED
  incr_yylsp++;
  if (incr_yylen == 0)
    {
      incr_yylsp->first_line = incr_yylloc.first_line;
      incr_yylsp->first_column = incr_yylloc.first_column;
      incr_yylsp->last_line = (incr_yylsp-1)->last_line;
      incr_yylsp->last_column = (incr_yylsp-1)->last_column;
      incr_yylsp->text = 0;
    }
  else
    {
      incr_yylsp->last_line = (incr_yylsp+incr_yylen-1)->last_line;
      incr_yylsp->last_column = (incr_yylsp+incr_yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  incr_yyn = incr_yyr1[incr_yyn];

  incr_yystate = incr_yypgoto[incr_yyn - YYNTBASE] + *incr_yyssp;
  if (incr_yystate >= 0 && incr_yystate <= YYLAST && incr_yycheck[incr_yystate] == *incr_yyssp)
    incr_yystate = incr_yytable[incr_yystate];
  else
    incr_yystate = incr_yydefgoto[incr_yyn - YYNTBASE];

  goto incr_yynewstate;

incr_yyerrlab:   /* here on detecting error */

  if (! incr_yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++incr_yynerrs;

#ifdef YYERROR_VERBOSE
      incr_yyn = incr_yypact[incr_yystate];

      if (incr_yyn > YYFLAG && incr_yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -incr_yyn if nec to avoid negative indexes in incr_yycheck.  */
	  for (x = (incr_yyn < 0 ? -incr_yyn : 0);
	       x < (sizeof(incr_yytname) / sizeof(char *)); x++)
	    if (incr_yycheck[x + incr_yyn] == x)
	      size += strlen(incr_yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (incr_yyn < 0 ? -incr_yyn : 0);
		       x < (sizeof(incr_yytname) / sizeof(char *)); x++)
		    if (incr_yycheck[x + incr_yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, incr_yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      incr_yyerror(msg);
	      free(msg);
	    }
	  else
	    incr_yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	incr_yyerror("parse error");
    }

  goto incr_yyerrlab1;
incr_yyerrlab1:   /* here on error raised explicitly by an action */

  if (incr_yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (incr_yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (incr_yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", incr_yychar, incr_yytname[incr_yychar1]);
#endif

      incr_yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  incr_yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto incr_yyerrhandle;

incr_yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  incr_yyn = incr_yydefact[incr_yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (incr_yyn) goto incr_yydefault;
#endif

incr_yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (incr_yyssp == incr_yyss) YYABORT;
  incr_yyvsp--;
  incr_yystate = *--incr_yyssp;
#ifdef YYLSP_NEEDED
  incr_yylsp--;
#endif

#if YYDEBUG != 0
  if (incr_yydebug)
    {
      short *ssp1 = incr_yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != incr_yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

incr_yyerrhandle:

  incr_yyn = incr_yypact[incr_yystate];
  if (incr_yyn == YYFLAG)
    goto incr_yyerrdefault;

  incr_yyn += YYTERROR;
  if (incr_yyn < 0 || incr_yyn > YYLAST || incr_yycheck[incr_yyn] != YYTERROR)
    goto incr_yyerrdefault;

  incr_yyn = incr_yytable[incr_yyn];
  if (incr_yyn < 0)
    {
      if (incr_yyn == YYFLAG)
	goto incr_yyerrpop;
      incr_yyn = -incr_yyn;
      goto incr_yyreduce;
    }
  else if (incr_yyn == 0)
    goto incr_yyerrpop;

  if (incr_yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (incr_yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++incr_yyvsp = incr_yylval;
#ifdef YYLSP_NEEDED
  *++incr_yylsp = incr_yylloc;
#endif

  incr_yystate = incr_yyn;
  goto incr_yynewstate;

 incr_yyacceptlab:
  /* YYACCEPT comes here.  */
  if (incr_yyfree_stacks)
    {
      free (incr_yyss);
      free (incr_yyvs);
#ifdef YYLSP_NEEDED
      free (incr_yyls);
#endif
    }
  return 0;

 incr_yyabortlab:
  /* YYABORT comes here.  */
  if (incr_yyfree_stacks)
    {
      free (incr_yyss);
      free (incr_yyvs);
#ifdef YYLSP_NEEDED
      free (incr_yyls);
#endif
    }
  return 1;
}
#line 118 "incrgram.ypp"
