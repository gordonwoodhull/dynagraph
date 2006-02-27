/**********************************************************
*      This software is part of the graphviz toolset      *
*                http://www.graphviz.org/                 *
*                                                         *
*            Copyright (c) 1994-2005 AT&T Corp.           *
*                and is licensed under the                *
*            Common Public License, Version 1.0           *
*                      by AT&T Corp.                      *
*                                                         *
*        Information and Software Systems Research        *
*              AT&T Research, Florham Park NJ             *
*                                                         *
*               This version available from               *
*                   http://dynagraph.org                  *
**********************************************************/


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "gsxep.h"
#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif

void gs_begin_session(char *id);
void gs_end_session(char *id);
void gs_open_graph(char *id);
void gs_mod_graph(char *id);
void gs_close_graph(char *id);
void gs_lock(char *id);
void gs_unlock(char *id);
void gs_segue(char *id);
void gs_define_pattern();
void gs_define_search();
void gs_define_input();

void gs_ins_node(char *view,char *id);
void gs_mod_node(char *view,char *id);
void gs_del_node(char *view,char *id);
void gs_ins_edge(char *view,char *id, char *tail, char *head);
void gs_mod_edge(char *view,char *id);
void gs_del_edge(char *view,char *id);

void gs_abort(int code);
void gs_reset_attrs();
void gs_append_attr(char *name, char *value);
char *gs_get_attr(char *name);
void gs_error(int code, char *msg);
void gs_lexeof();

// in gsscan
int gs_yylex();
void gs_yyerror(char *str);


//#define IF_ERR_UNKNOWN			0
#define IF_ERR_ALREADY_OPEN		1
#define IF_ERR_NOT_OPEN			2
//#define IF_ERR_NAME_MISMATCH	3
#define IF_ERR_SYNTAX			4
//#define IF_ERR_DUPLICATE_ID		5
//#define IF_ERR_NOT_IMPLEMENTED	6
//#define IF_ERR_OBJECT_DOESNT_EXIST 7
#define IF_MAX_ERR				8

#define IF_MAXATTR				128

#ifdef offsetof
#undef offsetof
#endif
#define offsetof(typ,fld)  ((int)(&(((typ*)0)->fld)))
#ifndef streq
#define streq(s,t) (!strcmp(s,t))
#endif
