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


struct IncrError : DGException {
  DString descrip;
  IncrError(DString descrip) :
    DGException("incrface error"),descrip(descrip) {}
};
struct IncrGraphNotOpen : DGException {
	IncrGraphNotOpen(const char *graph) : DGException("incr: graph not open",graph) {}
};

