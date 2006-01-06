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


extern "C" {
#include "pathplan.h"
}

struct ObstacleAvoiderSpliner {
	std::vector<Ppoly_t*> obs;
	std::vector<Ppoly_t> polys;
	std::vector<Line> polydat;
	vconfig_t	*vconfig;

	ObstacleAvoiderSpliner(Layout *layout);
	void FindSpline(Coord a,Coord b,Line &ret);
	void make_barriers(int pp, int qp, Pedge_t **barriers, int *n_barriers);
};
struct ClockwiseShapes : DGException {
	ClockwiseShapes() : DGException("node shapes must be counter-clockwise",true) {}
};
