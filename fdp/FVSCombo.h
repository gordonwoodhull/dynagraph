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


#include "fdp.h"
#include "voronoi/voronoi.h"
#include "shortspline/ObAvSplinerEngine.h"
struct FVSCombo : CompoundServer {
	FVSCombo(Layout *client,Layout *current) : CompoundServer(client,current) {
		actors.push_back(new FDP::FDPServer(client,current));
		actors.push_back(new Voronoi::VoronoiServer(client,current));
		actors.push_back(new ObAvSplinerEngine(client,current));
	}
};
