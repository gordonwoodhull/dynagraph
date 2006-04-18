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


#include "PathPlot.h"

namespace Dynagraph {
namespace PathPlot {

void PolyBarriers(const LineV &polygons, SegmentV &out) {
	size_t n = 0,i;
	for(i = 0; i < polygons.size(); i++)
		n += polygons[i].size();

	out.reserve(n);
	for(i = 0; i < polygons.size(); i++) {
		const Line &pp = polygons[i];
		for(unsigned j = 0; j < pp.size(); j++) {
			int k = (j + 1)%pp.size();
			out.push_back(Segment(pp[j],pp[k]));
		}
	}
	assert(out.size() == n);
}

} // namespace PathPlot
} // namespace Dynagraph
