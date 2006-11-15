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

struct UnknownLayoutType : DGException2 {
	UnknownLayoutType(DString name) : DGException2("Unknown layout type",name) {}
};

struct LayoutChooserConfigurator {
	template<typename Configurators,typename Source,typename Dest> 
	static bool Create(DString name,const StrAttrs &attrs,typename Data<Source>::type &source,typename Data<Dest>::type dest) {
		BOOST_MPL_ASSERT((boost::is_same<Layout,void>)); // this Configurator must go first
		DString layout = attrs.look("layout","dynadag");
		if(layout=="dynadag") 
			return DynaDAG::DynaDAGConfigurator::config<Configurators>(name,attrs,source,dest);
		else if(layout=="fdp")
			return FDP::FDPConfigurator::config<Configurators>(name,attrs,source,dest);
		else {
			throw UnknownLayoutType(layout);
			return false;
		}
	}
};

} // namespace Dynagraph

#endif //LayoutChooserConfigurator_h
