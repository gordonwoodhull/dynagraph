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


#include "common/Dynagraph.h"
#include "ObstacleAvoiderSpliner.h"

extern "C" {
#include "pathplan.h"
#include "vispath.h"
#include "pathutil.h"
#include "legal_arrangement.h"
}


using namespace std;

ObstacleAvoiderSpliner::ObstacleAvoiderSpliner(Layout *layout) {
	int N = 0;
	for(Layout::node_iter ni = layout->nodes().begin(); ni!=layout->nodes().end(); ++ni)
		if(gd<NodeGeom>(*ni).region.shape.size())
			++N;
	obs.resize(N,0);
	polys.resize(N);
	polydat.resize(N);
	int i = 0;
	for(Layout::node_iter ni = layout->nodes().begin(); ni!=layout->nodes().end(); ++ni) {
		if(gd<NodeGeom>(*ni).region.shape.empty())
			continue;
		obs[i] = &polys[i];
		polydat[i] = gd<NodeGeom>(*ni).region.shape + gd<NodeGeom>(*ni).pos;
		reverse(polydat[i].begin(),polydat[i].end()); // pathplan wants CW
		polys[i].ps = reinterpret_cast<Ppoint_t*>(&*polydat[i].begin());
		polys[i].pn = polydat[i].size()-1;
		++i;
	}
	assert(i==N);
	if (!Plegal_arrangement(&*obs.begin(),N)) {
		//if (Verbose) fprintf(stderr,"nodes touch - falling back to straight line edges\n");
		vconfig = 0;
	}
	else
		vconfig = Pobsopen(&*obs.begin(),N);
}
void ObstacleAvoiderSpliner::FindSpline(Coord a,Coord b,Line &ret) {
	if(!vconfig)
		return;
	Ppoint_t p = *reinterpret_cast<Ppoint_t*>(&a),
		q = *reinterpret_cast<Ppoint_t*>(&b);
	Ppolyline_t line;
	int			pp, qp;

	/* determine the polygons (if any) that contain the endpoints */
	pp = qp = POLYID_NONE;
	for(unsigned i = 0; i < obs.size(); i++) {
		if ((pp == POLYID_NONE) && in_poly(*obs[i], p)) pp = i;
		if ((qp == POLYID_NONE) && in_poly(*obs[i], q)) qp = i;
	}
	if(!Pobspath(vconfig, p, pp, q, qp, &line))
		throw ClockwiseShapes();
	Ppolyline_t spline;
	Pvector_t	slopes[2];
	int			n_barriers;

	Pedge_t     *barriers;
	make_barriers(pp, qp, &barriers, &n_barriers);
	slopes[0].x = slopes[0].y = 0.0;
	slopes[1].x = slopes[1].y = 0.0;
	Coord *begin,*end;
	spline.ps = 0;
	if(!Proutespline (barriers, n_barriers, line, slopes, &spline)) {
		begin = reinterpret_cast<Coord*>(spline.ps);
		end = begin+spline.pn;
		ret.degree = 3;
	}
	else {
		begin = reinterpret_cast<Coord*>(line.ps);
		end = begin+line.pn;
		ret.degree = 1;
	}
	ret.assign(begin,end);
	// free(spline.ps); // oh right, globals, good.
	free(barriers);
	free(line.ps);
}
void ObstacleAvoiderSpliner::make_barriers(int pp, int qp, Pedge_t **barriers, int *n_barriers){
	int     i, j, k, n, b;
	Pedge_t *bar;
	Ppoly_t **poly = &*obs.begin();
	int npoly = obs.size();
	n = 0;
	for (i = 0; i < npoly; i++) {
		if (i == pp) continue;
		if (i == qp) continue;
		n = n + poly[i]->pn;
	}
	bar = reinterpret_cast<Pedge_t*>(malloc(n * sizeof(Pedge_t)));
	b = 0;
	for (i = 0; i < npoly; i++) {
		if (i == pp) continue;
		if (i == qp) continue;
		for (j = 0; j < poly[i]->pn; j++) {
			k = j + 1;
			if (k >= poly[i]->pn) k = 0;
			bar[b].a = poly[i]->ps[j];
			bar[b].b = poly[i]->ps[k];
			b++;
		}
	}
	assert(b == n);
	*barriers = bar;
	*n_barriers = n;
}
