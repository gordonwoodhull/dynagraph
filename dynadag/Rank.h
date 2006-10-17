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

#ifndef Rank_h
#define Rank_h

#include <algorithm>

namespace Dynagraph {
namespace DynaDAG {

struct Rank {
	NodeV order;
	std::vector<double> x_backup; // ugly figure out better way to do this
	double yBase,	/* absolute */
		deltaAbove, deltaBelow, spaceBelow;
	Rank(double sep) : yBase(-17),deltaAbove(sep/20),deltaBelow(sep/20), spaceBelow(sep) {}
	Rank(Rank &o) {
		*this = o;
	}
	Rank &operator=(const Rank&o) {
		order = o.order;
		x_backup = o.x_backup;
		yBase = o.yBase;
		deltaAbove = o.deltaAbove;
		deltaBelow = o.deltaBelow;
		spaceBelow = o.spaceBelow;
		return *this;
	}
	double yBelow(double fract) {
		return yBase;
	}
	double yAbove(double fract) {
		return yBase + 2*deltaAbove;
	}
	double Height() {
		return deltaAbove+deltaBelow+spaceBelow;
	}
	void backup_x() {
		// ugly figure out better way to do this
		x_backup.resize(order.size());
		for(NodeV::iterator ni = order.begin(); ni!=order.end(); ++ni)
			x_backup[ni-order.begin()] = gd<DDNode>(*ni).cur.x;
		check_backdup_x();
	}
	void check_backdup_x() {
		dgassert(std::adjacent_find(x_backup.begin(),x_backup.end(),std::greater<double>())==x_backup.end());
	}
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // Rank_h
