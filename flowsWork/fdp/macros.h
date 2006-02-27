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


#define BETWEEN(a,b,c)	(((a) <= (b)) && ((b) <= (c)))
#define RADIANS(deg)	((deg)/180.0 * PI)
#define DEGREES(rad)	((rad)/PI * 180.0)
#define DIST(x1,y1,x2,y2) (sqrt(((x1) - (x2))*((x1) - (x2)) + ((y1) - (y2))*((y1) - (y2))))

#ifndef streq
#define streq(s,t)		(!strcmp((s),(t)))
#endif
#ifndef NOTUSED
#define NOTUSED(var)      (void) var
#endif

#ifndef NIL
#define NIL(T) reinterpret_cast<T>(0)
#endif
