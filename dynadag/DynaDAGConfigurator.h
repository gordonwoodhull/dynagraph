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
	template<typename Configurators,typename Layout> 
	static void config(DString name,const StrAttrs &attrs,ChangingGraph<Layout> *,EnginePair<Layout>) {
		BOOST_MPL_ASSERT((boost::is_same<Layout,void>)); // this is a starterator (prob called by LayoutChooserConfigurator)
		ChangingGraph<DynaDAG::DynaDAGLayout> *world = new ChangingGraph<DynaDAG::DynaDAGLayout>;
		EnginePair<DynaDAG::DynaDAGLayout> engines;
		engines.Append(new DynaDAG::DynaDAGServer(world));
		configureLayout<Configurators>(name,attrs,world,engines);
	}
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif //DynaDAGConfigurator_h
