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

#ifndef GeneralLayout_h
#define GeneralLayout_h

#include "common/LayoutAttrs.h"

namespace Dynagraph {

typedef NamedGraph<ADTisCDT,GeneralGraphAttrs,GeneralNodeAttrs,GeneralEdgeAttrs,LayoutUpdates,LayoutUpdates,LayoutUpdates> GeneralLayout;

} // namespace Dynagraph

#endif // GeneralLayout_h
