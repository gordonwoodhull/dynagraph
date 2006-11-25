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
	template<typename Configurators,typename Source,typename Dest,typename CurrLayout>
	static bool Create2(DString name,const StrAttrs &attrs,Source &source,Dest dest,
            Configurator::Level<CurrLayout,typename Dest::CurrLevel> &level) {
        typedef typename Dest::CurrLevel CurrLevel;
        typedef Configurator::Engine<CurrLayout,FindChangeRectsConfigurator> ThisEngine;
		typedef Configurator::Configuration<CurrLevel,typename boost::mpl::push_back<typename Dest::DataList,ThisEngine>::type> NewDest;
		Configurator::Data<NewDest> newDest = dest;
		FCRData<CurrLayout> *fcrdata = new FCRData<CurrLayout>(level.world.get());
		FCRBefore<CurrLayout> *fcrbefore = new FCRBefore<CurrLayout>(fcrdata);
		FCRAfter<CurrLayout> *fcrafter = new FCRAfter<CurrLayout>(fcrdata);
		level.engines.Prepend(fcrbefore);
		level.engines.Append(fcrafter);
        ((ThisEngine&)newDest).engine.reset(fcrbefore);
		return Configurator::Create<Configurators>(name,attrs,source,newDest);
	}
	template<typename Configurators,typename Source,typename Dest> 
	static bool Create(DString name,const StrAttrs &attrs,Source &source,Dest dest) {
		if(attrs.look("findchangerects","false")=="true") 
            return Create2<Configurators>(name,attrs,source,dest,dest);
		return Configurator::Create<Configurators>(name,attrs,source,dest);
	}
};

} // namespace Dynagraph

#endif //FindChangeRectsConfigurator_h
