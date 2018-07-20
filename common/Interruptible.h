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

#ifndef Interruptible_h
#define Interruptible_h

#include "StrAttr.h"

namespace Dynagraph {

struct Interruptible {
    bool interrupt;
    StrAttrs attrs; // specific instructions about when to stop (e.g. from incrface pulse command)
    Interruptible() : interrupt(false) {}
};

} // namespace Dynagraph

#endif // Interruptible_h
