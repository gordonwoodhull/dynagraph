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

#include "StringDict.h"

namespace Dynagraph {

// a base for all exceptions so we can report the basics
struct DGException {
    DString exceptype;
    bool fatal;
    DGException(DString exceptype,bool fatal = false) : exceptype(exceptype),fatal(fatal) {}
};
struct DGException2 : DGException {
    DString param;
    DGException2(DString exceptype,DString param,bool fatal = false) : DGException(exceptype,fatal),param(param) {}
};

// STANDARD GRAPH EXCEPTIONS
struct DGNodeNameUsed : DGException2 {
    DGNodeNameUsed(DString name) : DGException2("The node name has already been used",name) {}
};
struct DGEdgeNameUsed : DGException2 {
    DGEdgeNameUsed(DString name) : DGException2("The edge name has already been used",name) {}
};
struct DGEdgeTailDoesNotExist : DGException2 {
	DGEdgeTailDoesNotExist(DString name) : DGException2("Tail node does not exist",name) {}
};
struct DGEdgeHeadDoesNotExist : DGException2 {
	DGEdgeHeadDoesNotExist(DString name) : DGException2("Head node does not exist",name) {}
};
struct DGNodeDoesNotExist : DGException2 {
    DGNodeDoesNotExist(DString name) : DGException2("Node does not exist",name) {}
};
struct DGEdgeDoesNotExist : DGException2 {
    DGEdgeDoesNotExist(DString name) : DGException2("Edge does not exist",name) {}
};
struct DGParallelEdgesNotSupported : DGException2 {
    DGParallelEdgesNotSupported(DString name) : DGException2("Parallel edges between the same end-nodes are not yet supported",name) {}
};


} // namespace Dynagraph

#endif //dgxep_h
