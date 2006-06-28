/**********************************************************
*            This software is part of Dynagraph           *
*                http://www.dynagraph.org/                *
*                                                         *
*            Copyright (c) 2006 Gordon Woodhull           *
*        Portions Copyright (c) 1994-2005 AT&T Corp.      *
*  Licensed under the Common Public License, Version 1.0  *
*                                                         *
*      Dynagraph originates in the Graphviz toolset       *
*                   http://graphviz.org                   *
*        Information and Software Systems Research        *
*              AT&T Research, Florham Park NJ             *
*                       Many thanks.                      *
**********************************************************/


#ifndef NSRankerSimpleCycleBreaker_h
#define NSRankerSimpleCycleBreaker_h

#include "NSRankerModel.h"

namespace Dynagraph {
namespace DynaDAG {

struct NSRankerSimpleCycleBreaker : ChangeProcessor<NSRankerModel> {
	void Process() {

		NextProcess();
	}
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // NSRankerSimpleCycleBreaker_h
