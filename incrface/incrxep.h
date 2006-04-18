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

#ifndef INCRXEP_H
#define INCRXEP_H

#include "common/dgxep.h"

namespace Dynagraph {

// all these exceptions are recoverable: report error to client and ignore
struct IncrException : DGException2 {
	IncrException(DString xep,DString parm) : DGException2(xep,parm,false) {}
};
struct IncrError : IncrException {
  IncrError(DString descrip) : IncrException("Incrface syntax error",descrip) {}
};
struct IncrGraphNotOpen : IncrException {
	IncrGraphNotOpen(DString graph) : IncrException("Graph not open",graph) {}
};
struct IncrReopenXep : IncrException {
    IncrReopenXep(DString name) : IncrException("Tried to reopen graph",name) {}
};

} // namespace Dynagraph

#endif // INCRXEP_H
