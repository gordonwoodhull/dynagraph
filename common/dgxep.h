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

#ifndef DGXEP_H
#define DGXEP_H
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
#endif
