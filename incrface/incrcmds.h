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
#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif

namespace Dynagraph {

void incr_open_graph(const char *id);
void incr_close_graph(const char *id);
void incr_mod_graph(const char *id);
void incr_pulse(const char *id);
void incr_lock(const char *id);
void incr_unlock(const char *id);
void incr_segue(const char *id);
void incr_ins_node(const char *graph,const char *id);
void incr_ins_edge(const char *graph,const char *id, const char *tail, const char *head);
void incr_mod_node(const char *graph,const char *id);
void incr_mod_edge(const char *graph,const char *id);
void incr_del_node(const char *graph,const char *id);
void incr_del_edge(const char *graph,const char *id);
void incr_req_graph(const char *graph);
void incr_req_node(const char *graph,const char *id);
void incr_req_edge(const char *graph,const char *id);
void incr_ful_graph(const char *graph);
void incr_ful_node(const char *graph,const char *id);
void incr_ful_edge(const char *graph,const char *id);
void incr_message(const char *msg);

void incr_abort(int code);
void incr_reset_attrs();
void incr_append_attr(const char *name, const char *value);
const char *incr_get_attr(const char *name);
void incr_error(int code, const char *msg);
void incr_lexeof();

} // namespace Dynagraph

// in incrscan
void incr_yyerror(const char *str);
int incr_yylex(void);

//#define IF_ERR_UNKNOWN			0
//#define IF_ERR_ALREADY_OPEN		1
//#define IF_ERR_NOT_OPEN			2
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

