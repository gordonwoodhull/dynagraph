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




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 13 "incrgram.ypp"
typedef union YYSTYPE {
			int				i;
			char			*str;
} YYSTYPE;
/* Line 1252 of yacc.c.  */
#line 80 "incrgram.tab.hpp"
# define incr_yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE incr_yylval;



