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


//#include "voronoi/voronoi.h"
//#include "fdp/fdp.h"
//#include "shortspline/ObAvSplinerEngine.h"
#include "dynadag/DynaDAG.h"
#include "common/ColorByAge.h"
#include "createLayoutServer.h"
#include "common/LabelPlacer.h"
#include "common/ShapeGenerator.h"

using namespace std;

creators<DynaDAG::DynaDAGLayout>::creators creators<DynaDAG::DynaDAGLayout>::the_creators;

creators<DynaDAG::DynaDAGLayout>::creators()  {
	using DynaDAG::DynaDAGLayout;
	creators &me = *this;
	me["dynadag"] = ServerCreatorInstance<DynaDAG::DynaDAGServer>::create;
	me["labels"] = ServerCreatorInstance<LabelPlacer<DynaDAGLayout> >::create;
	me["shapegen"] = ServerCreatorInstance<ShapeGenerator<DynaDAGLayout> >::create;
	me["colorbyage"] = ServerCreatorInstance<ColorByAge<DynaDAGLayout> >::create;
}
/*
creators<FDPLayout>::creators creators<FDPLayout>::the_creators;
creators<FDPLayout>::creators() {
	creators &me = *this;
	me["fdp"] = ServerCreatorInstance<FDP::FDPServer>::create;
	me["voronoi"] = ServerCreatorInstance<Voronoi::VoronoiServer>::create;
	me["visspline"] = ServerCreatorInstance<ObAvSplinerEngine>::create;
}
*/
