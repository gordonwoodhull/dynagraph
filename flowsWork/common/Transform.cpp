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


#include "Geometry.h"
#include "Transform.h"

namespace Dynagraph {

Transform g_dotRatios(Coord(72,72), // node sizes in inches
						Coord(1,1)); // position in up-positive points

bool transformShape(Transform *trans,Line &shape) {
	bool nonzero = false;
	for(Line::iterator pi = shape.begin(); pi!=shape.end(); ++pi) {
		if(pi->x!=0 || pi->y!=0)
			nonzero = true;
		*pi = trans->in(*pi);
	}
	return nonzero;
}

} // namespace Dynagraph
