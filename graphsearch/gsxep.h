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


#include "common/dgxep.h"
struct ParseError : DGException {
	ParseError() : DGException("graphsearch parse error") {}
};
struct GSError : DGException {
	GSError() : DGException("graphsearch error") {}
};
// not yet implemented
struct NYI : DGException {
	NYI() : DGException("graphsearch ins/mod/del commands not yet implemented") {}
};
