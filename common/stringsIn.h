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


#ifndef stringsIn_h
#define stringsIn_h

Update stringsIn(Transform *trans,bool useDotDefaults,Layout *l,const StrAttrs &attrs,bool clearOld);
Update stringsIn(Transform *trans,Layout::Node *n,const StrAttrs &attrs,bool clearOld);
Update stringsIn(Transform *trans,Layout::Edge *e,const StrAttrs &attrs,bool clearOld);

void applyStrGraph(Transform *trans,bool useDotDefaults,StrGraph *g,Layout *out, Layout *subg);

struct NonSizeableShape {};
struct UnknownShape {};

#endif // stringsIn_h
