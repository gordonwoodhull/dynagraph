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

#ifndef FindChangeRectsConfigurator_h
#define FindChangeRectsConfigurator_h

#include "Configurator.h"
#include "FindChangeRect.h"

namespace Dynagraph {

struct FindChangeRectsConfigurator {
	template<typename LevelTag,typename Layout>
	static void make_it(Configurator::Level<Layout,LevelTag> &level) {
		FCRData<Layout> *fcrdata = new FCRData<Layout>(level.world.get());
		FCRBefore<Layout> *fcrbefore = new FCRBefore<Layout>(fcrdata);
		FCRAfter<Layout> *fcrafter = new FCRAfter<Layout>(fcrdata);
		level.engines.Prepend(fcrbefore);
		level.engines.Append(fcrafter);
	}
	template<typename Configurators,typename Source,typename Dest> 
	static bool Create(DString name,const StrAttrs &attrs,Source &source,Dest dest) {
		if(attrs.look("findchangerects","false")=="true") {
			typedef Configurator::Data<Configurator::Configuration<typename Dest::CurrLevel,boost::mpl::push_back<typename Dest::DataList>::type> > NewDest;
			NewDest newDest = dest;
			make_it<typename Dest::CurrLevel>(newDest,newDest);
			return Configurator::Create<Configurators>(name,attrs,source,newDest);
		}
		return Configurator::Create<Configurators>(name,attrs,source,dest);
	}
};

} // namespace Dynagraph

#endif //FindChangeRectsConfigurator_h
