/*   Copyright (c) AT&T Corp.  All rights reserved.
   
This software may only be used by you under license from 
AT&T Corp. ("AT&T").  A copy of AT&T's Source Code Agreement 
is available at AT&T's Internet website having the URL 

http://www.research.att.com/sw/tools/graphviz/license/

If you received this software without first entering into a license 
with AT&T, you have an infringing copy of this software and cannot 
use it without violating AT&T's intellectual property rights. */

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
