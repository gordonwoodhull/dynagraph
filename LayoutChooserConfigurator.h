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

#ifndef LayoutChooserConfigurator_h
#define LayoutChooserConfigurator_h

#include "common/Configurator.h"

#include "dynadag/DynaDAGConfigurator.h"
#include "fdp/FDPConfigurator.h"

namespace Dynagraph {

struct LayoutChooserConfigurator {
	template<typename Configurators,typename Layout> 
	static void config(DString name,const StrAttrs &attrs,ChangingGraph<Layout> *world,EnginePair<Layout> engines) {
		BOOST_MPL_ASSERT((boost::is_same<Layout,void>)); // this Configurator must go first
		DString layout = attrs.look("layout","dynadag");
		if(layout=="dynadag") 
			DynaDAG::DynaDAGConfigurator::config<Configurators>(name,attrs,world,engines);
		else if(layout=="fdp")
			FDP::FDPConfigurator::config<Configurators>(name,attrs,world,engines);
	}
};

} // namespace Dynagraph

#endif //LayoutChooserConfigurator_h
