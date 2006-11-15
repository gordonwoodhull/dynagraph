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

#ifndef createConfiguration_h
#define createConfiguration_h

#include "LayoutChooserConfigurator.h"
#include "dynadag/EdgeSuppressorConfigurator.h"
#include "dynadag/ClearExtraRanksConfigurator.h"
#include "dynadag/EmphasizeFlowConfigurator.h"
#include "dynadag/RankerConfigurator.h"
#include "common/CoordTranslatorConfigurator.h"
#include "common/ShapeGeneratorConfigurator.h"
#include "common/ColorByAgeConfigurator.h"
#include "common/FindChangeRectsConfigurator.h"
#include "common/UpdateCurrentConfigurator.h"
#include "common/StringizerConfigurator.h"
#include "incrface/RegisteringConfigurator.h"

#include <boost/mpl/vector.hpp>
#include <boost/mpl/push_back.hpp>


namespace Dynagraph {

// the order of TheConfigurators is "magic" 
// a more clever (perhaps too clever) way to do this might be to specify 
// dependencies and order constraints on TheConfigurators(?)
typedef boost::mpl::vector<
	LayoutChooserConfigurator,
	DynaDAG::EdgeSuppressorConfigurator,
	DynaDAG::ClearExtraRanksConfigurator,
	DynaDAG::EmphasizeFlowConfigurator,
	DynaDAG::RankerConfigurator,
	CoordTranslatorConfigurator,
	ShapeGeneratorConfigurator,
	ColorByAgeConfigurator,
	FindChangeRectsConfigurator,
	UpdateCurrentConfigurator,
	StringizerConfigurator,
	RegisteringConfigurator
> TheConfigurators;

template<typename FinalConfigurator,typename SourceLayout>
bool createConfiguration(Name name,StrAttrs &attrs,SourceLayout *source) {
	BOOST_MPL_ASSERT((boost::mpl::has_push_back<TheConfigurators>));
	typedef typename boost::mpl::push_back<TheConfigurators,FinalConfigurator>::type PlusFinalizer;
	return createConfiguration<PlusFinalizer>(name,attrs,source);
}
bool createConfiguration(Name name,StrAttrs &attrs) {
	return createConfiguration<PassConfigurator>(name,attrs,(void*)0);
}

} // namespace Dynagraph

#endif // createConfiguration_h
