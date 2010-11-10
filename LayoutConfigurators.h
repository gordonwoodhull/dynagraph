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

#include "LayoutChooserConfigurator.h"
#include "dynadag/EdgeSuppressorConfigurator.h"
#include "dynadag/EnsureParallelEdgeRanksConfigurator.h"
#include "dynadag/ClearExtraRanksConfigurator.h"
#include "dynadag/EmphasizeFlowConfigurator.h"
#include "dynadag/RankerConfigurator.h"
#include "common/CoordTranslatorConfigurator.h"
#include "common/ShapeGeneratorConfigurator.h"
#include "common/ColorByAgeConfigurator.h"
#include "common/FindChangeRectsConfigurator.h"
#include "common/UpdateCurrentConfigurator.h"

#include <boost/mpl/list.hpp>

namespace Dynagraph {

// the order of configurators is "magic" 
// a more clever (perhaps too clever) way to do this might be to specify 
// dependencies and order constraints on configurators(?)
typedef boost::mpl::list<
	LayoutChooserConfigurator,
	DynaDAG::EdgeSuppressorConfigurator,
	DynaDAG::EnsureParallelEdgeRanksConfigurator,
	DynaDAG::ClearExtraRanksConfigurator,
	DynaDAG::EmphasizeFlowConfigurator,
	DynaDAG::RankerConfigurator,
	CoordTranslatorConfigurator,
	ShapeGeneratorConfigurator,
	ColorByAgeConfigurator,
	FindChangeRectsConfigurator,
    UpdateCurrentConfigurator> LayoutConfigurators;
    
}

