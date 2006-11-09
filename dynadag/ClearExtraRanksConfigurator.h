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

#ifndef ClearExtraRanksConfigurator_h
#define ClearExtraRanksConfigurator_h

#include "common/Configurator.h"
#include "ClearExtraRanksEngine.h"

namespace Dynagraph {
namespace DynaDAG {

struct ClearExtraRanksConfigurator {
	struct ClearExtraRanksConfiguratorImpl {
		template<typename Configurators,typename SourceLayout> 
		static bool config(DString name,const StrAttrs &attrs,ChangingGraph<DynaDAGLayout> *world,EnginePair<DynaDAGLayout> engines, SourceLayout *source) {
			engines.Prepend(new ClearExtraRanksEngine<DynaDAGLayout>(world));
			return configureLayout<Configurators>(name,attrs,world,engines,source);
		}
	};
	template<typename Configurators,typename Layout,typename SourceLayout> 
	static bool config(DString name,const StrAttrs &attrs,ChangingGraph<Layout> *world,EnginePair<Layout> engines, SourceLayout *source) {
		typedef boost::mpl::if_<boost::is_same<Layout,DynaDAGLayout>,ClearExtraRanksConfiguratorImpl,PassConfigurator>::type Choice;
		return Choice::template config<Configurators>(name,attrs,world,engines,source);
	}
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif //ClearExtraRanksConfigurator_h
