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


#ifndef VORONOI_H
#define VORONOI_H

#include "site.h"
#include "info.h"
#include "edges.h"
#include "hedges.h"

namespace Dynagraph {
namespace Voronoi {

// this could actually be templated on FDP::FDPLayout, which would turn all of voronoi
// into a template library.  i'm trying to focus on more useful things at the moment.
struct VoronoiServer : LinkedChangeProcessor<FDP::FDPLayout> {
	double margin;     /* Create initial bounding box by adding
                        * margin * dimension around box enclosing
                        * nodes.
                        */
	double incr;       /* Increase bounding box by adding
                        * incr * dimension around box.
                        */
	double pmargin;  /* Margin around polygons, in inches */
	int iterations;  /* Number of iterations */
	bool useIter;   /* Use specified number of iterations */

	bool doAll;  /* Move all nodes, regardless of overlap */
	Bounds bounds;

	const int N;

	VoronoiServer(ChangingGraph<FDP::FDPLayout> *world) 
		: LinkedChangeProcessor<FDP::FDPLayout>(world),
		margin(0.05),incr(0.025),pmargin(5.0/72),iterations(-1),useIter(false),
		N(400), infos(N),sites(N),edges(sites,infos,bounds,N),hedges(sites,N) {}


	// ChangeProcessor
	void Process();

private:
	Infos infos;
	Sites sites;
	Edges edges;
	Halfedges hedges;

	void chkBoundBox();
	void makeInfo();
	void sortSites(std::vector<Site*> &sort);
	void geomUpdate(std::vector<Site*> &sort);
	int countOverlap(int iter);
	void increaseBoundBox();
	bool isInterior (Info* ip);
	void newpos(Info* ip);
	void addCorners();
	void newPos();
	bool vAdjust();
	void updateLayout(ChangeQueue<FDP::FDPLayout> &Q);

	void voronoi(const std::vector<Site*> &order);
};

} // namespace Voronoi
} // namespace Dynagraph

#endif // VORONOI_H
