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
*                   *        *        *                   *
*            Current source code available from           *
*                http://gordon.woodhull.com               *
**********************************************************/

struct IncrError : DGException {
  DString descrip;
  IncrError(DString descrip) :
    DGException("incrface error"),descrip(descrip) {}
};
struct IncrGraphNotOpen : DGException2 {
	IncrGraphNotOpen(const char *graph) : DGException2("incr: graph not open",graph) {}
};

