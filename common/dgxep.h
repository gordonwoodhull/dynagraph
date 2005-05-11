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


#ifndef dgxep_h
#define dgxep_h

// a base for all exceptions so we can report the basics
struct DGException {
    const char *exceptype;
    DGException(const char *exceptype) : exceptype(exceptype) {}
};
// pretty dopey - eliminate a few more chars in xep defs
struct DGException2 : DGException {
    const char *param;
    DGException2(const char *exceptype,const char *param) : DGException(exceptype),param(param) {}
};
// so that main knows when to continue and when to bail out
struct DGNonFatalException : DGException2 {
	DGNonFatalException(const char *exceptype,const char *param) : DGException2(exceptype,param) {}
};

#endif //dgxep_h
