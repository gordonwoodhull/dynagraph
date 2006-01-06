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


#ifndef DGXEP_H
#define DGXEP_H
// a base for all exceptions so we can report the basics
struct DGException {
    const char *exceptype;
    bool fatal;
    DGException(const char *exceptype,bool fatal = false) : exceptype(exceptype),fatal(fatal) {}
};
// pretty dopey - eliminate a few more chars in xep defs
struct DGException2 : DGException {
    const char *param;
    DGException2(const char *exceptype,const char *param,bool fatal = false) : DGException(exceptype,fatal),param(param) {}
};
#endif
