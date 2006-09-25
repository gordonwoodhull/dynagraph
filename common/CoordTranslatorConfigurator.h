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

#ifndef CoordTranslatorConfigurator_h
#define CoordTranslatorConfigurator_h

#include "Configurator.h"
#include "GeneralLayout.h"
#include "InternalWorld.h" 
#include "NamedToNamedChangeTranslator.h"
#include "CoordTranslator.h"

namespace Dynagraph {

struct CoordTranslatorConfigurator {
	template<typename Configurators,typename Layout> 
	static void config(DString name,const StrAttrs &attrs,ChangingGraph<Layout> *innerWorld,EnginePair<Layout> innerEngines) {
		if(attrs.look("coordtranslation","false")=="true"||attrs.look("rankdir")) {
			typedef NamedToNamedChangeTranslator<GeneralLayout,Layout,CoordTranslatorInActions<GeneralLayout,Layout>,true> InNaTranslator;
			typedef CoordTranslatorIn<GeneralLayout,Layout> InCoTranslator;
			typedef NamedToNamedChangeTranslator<Layout,GeneralLayout,CoordTranslatorOutActions<Layout,GeneralLayout>,false> OutNaTranslator;
			typedef CoordTranslatorOut<Layout,GeneralLayout> OutCoTranslator;
			typedef InternalWorld<GeneralLayout,Layout> InWorld;
			ChangingGraph<GeneralLayout> *outerWorld = new ChangingGraph<GeneralLayout>;
			gd<Name>(&innerWorld->whole_) = name+"_xlated";
			InWorld *inWorld = new InWorld(outerWorld,innerWorld);
			innerEngines.Prepend(new UpdateCurrentProcessor<Layout>(innerWorld)); // this...
			inWorld->innerEngines_ = innerEngines;
			inWorld->inTranslators_.push_back(new InNaTranslator(outerWorld,innerWorld));
			inWorld->inTranslators_.push_back(new InCoTranslator(outerWorld,innerWorld));
			inWorld->outTranslators_.push_back(new OutNaTranslator(innerWorld,outerWorld));
			inWorld->outTranslators_.push_back(new OutCoTranslator(innerWorld,outerWorld));
			inWorld->CompleteConfiguration();
			EnginePair<GeneralLayout> outerEngines;
			outerEngines.Append(inWorld);
			outerEngines.Append(new OkayEngine<GeneralLayout>(outerWorld)); // and this, belong in some general internalizer configurator, no? but how?
			configureLayout<Configurators>(name,attrs,outerWorld,outerEngines);
		}
		else
			configureLayout<Configurators>(name,attrs,innerWorld,innerEngines);
	}
};

} // namespace Dynagraph

#endif //CoordTranslatorConfigurator_h
