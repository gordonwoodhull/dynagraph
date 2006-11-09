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

#ifndef FDPConfigurator_h
#define FDPConfigurator_h

#include "common/Configurator.h"
#include "fdp/FDPLayout.h"
#include "fdp/fdp.h"
#include "common/SimpleEngines.h"

namespace Dynagraph {
namespace FDP {

struct FDPConfigurator {
	template<typename Configurators,typename Layout,typename SourceLayout> 
	static bool config(DString name,const StrAttrs &attrs,ChangingGraph<Layout> *,EnginePair<Layout>, SourceLayout *source) {
		BOOST_MPL_ASSERT((boost::is_same<Layout,void>)); // this is a starterator (prob called by LayoutChooserConfigurator)
		ChangingGraph<FDPLayout> *world = new ChangingGraph<FDPLayout>;
		EnginePair<FDPLayout> engines;
		engines.Prepend(new FDPServer(world));
		engines.Prepend(new OkayEngine<FDPLayout>(world));
		return configureLayout<Configurators>(name,attrs,world,engines,source);
	}
};

} // namespace FDP
} // namespace Dynagraph

#endif //FDPConfigurator_h
