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
#include "dynadag/ClearExtraRanksConfigurator.h"
#include "dynadag/RankerConfigurator.h"
#include "dynadag/EmphasizeFlowConfigurator.h"
#include "common/ShapeGeneratorConfigurator.h"
#include "common/FindChangeRectsConfigurator.h"
#include "common/UpdateCurrentConfigurator.h"
#include "common/StringizerConfigurator.h"
#include "incrface/RegisteringConfigurator.h"

namespace mpl = boost::mpl;

namespace Dynagraph {

// the order of configurators is "magic" 
// a more clever (perhaps too clever) way to do this might be to specify 
// dependencies and order constraints on configurators(?)
typedef mpl::vector<
	LayoutChooserConfigurator,
	DynaDAG::EdgeSuppressorConfigurator,
	DynaDAG::ClearExtraRanksConfigurator,
	DynaDAG::EmphasizeFlowConfigurator,
	DynaDAG::RankerConfigurator,
	ShapeGeneratorConfigurator,
	FindChangeRectsConfigurator,
	UpdateCurrentConfigurator,
	StringizerConfigurator,
	RegisteringConfigurator
> ConfiguratorVec;

void createConfiguration(Name name,StrAttrs &attrs) {
	configureLayout<ConfiguratorVec>(name,attrs);
}

} // namespace Dynagraph
