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

#ifndef EdgeSuppressorConfigurator_h
#define EdgeSuppressorConfigurator_h

#include "common/Configurator.h"
#include "EdgeSuppressor.h"

namespace Dynagraph {
namespace DynaDAG {

struct EdgeSuppressorConfigurator {
	struct EdgeSuppressorConfiguratorImpl {
		template<typename Configurators,typename Source,typename Dest> 
		static bool config(DString name,const StrAttrs &attrs,ChangingGraph<DynaDAGLayout> *world,EnginePair<DynaDAGLayout> engines, SourceLayout *source) {
			if(attrs.look("nodesuppression","true")=="true")
				engines.Prepend(new EdgeSuppressor<DynaDAGLayout>(world));
			return createConfiguration<Configurators>(name,attrs,source,dest);
		}
	};
	template<typename Configurators,typename Source,typename Dest> 
	static bool Create(DString name,const StrAttrs &attrs,typename Data<Source>::type &source,typename Data<Dest>::type dest) {
		typedef boost::mpl::if_<boost::is_same<Layout,DynaDAGLayout>,EdgeSuppressorConfiguratorImpl,PassConfigurator>::type Choice;
		return Choice::template config<Configurators>(name,attrs,source,dest);
	}
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif //EdgeSuppressorConfigurator_h
