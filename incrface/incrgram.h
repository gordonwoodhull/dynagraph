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




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 12 "incrgram.ypp"
typedef union YYSTYPE {
			int				i;
			char			*str;
} YYSTYPE;
/* Line 1275 of yacc.c.  */
#line 78 "y.tab.h"
# define incr_yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE incr_yylval;



