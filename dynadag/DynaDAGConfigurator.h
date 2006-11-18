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

#ifndef DynaDAGConfigurator_h
#define DynaDAGConfigurator_h

#include "common/Configurator.h"
#include "dynadag/DynaDAGLayout.h"
#include "dynadag/DynaDAG.h"

namespace Dynagraph {
namespace DynaDAG {

struct DynaDAGConfigurator {
	template<typename Configurators,typename Source,typename Dest> 
	static bool Create(DString name,const StrAttrs &attrs,Source &source,Dest dest) {
		BOOST_MPL_ASSERT((boost::mpl::empty<Configurator::DataList<typename Dest::Configuration>::type >)); // this is a starterator (prob called by LayoutChooserConfigurator)
		typedef Configurator::Configuration<Configurator::LayoutLevel,
			boost::mpl::vector<Configurator::Level<DynaDAGLayout,Configurator::LayoutLevel> > > NewDest;
		Configurator::Data<NewDest> newDest;
		newDest.world.reset(new ChangingGraph<DynaDAG::DynaDAGLayout>);
		newDest.engines.Append(new DynaDAG::DynaDAGServer(newDest.world.get()));
		return Configurator::Create<Configurators>(name,attrs,source,newDest);
	}
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif //DynaDAGConfigurator_h
