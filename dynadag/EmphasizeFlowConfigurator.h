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

#ifndef EmphasizeFlowConfigurator_h
#define EmphasizeFlowConfigurator_h

#include "common/Configurator.h"
#include "common/GeneralLayout.h"
#include "common/Transform.h"
#include "incrface/WorldInABox.h"
#include "SEdger.h"
#include "common/EdgeSplicer.h"

namespace Dynagraph {

// these globals should get folded into graph attributes, eliminating much useless code
extern Transform *g_transform;

namespace DynaDAG {

struct EmphasizeFlowConfigurator {
	struct EmphasizeFlowConfiguratorImpl {
		template<typename ConfiguratorVec> 
		static void config(DString name,const StrAttrs &attrs,ChangingGraph<DynaDAGLayout> *innerWorld,EnginePair<DynaDAGLayout> innerEngines) {
			if(attrs.look("emphasizeflow","false")=="true"||attrs.look("flowemphasizable","false")=="true") {
				typedef SEdger<GeneralLayout,DynaDAGLayout> InTranslator;
				typedef EdgeSplicer<DynaDAGLayout,GeneralLayout> OutTranslator;
				typedef WorldInABox<GeneralLayout,DynaDAGLayout,InTranslator,OutTranslator> Box;
				ChangingGraph<GeneralLayout> *outerWorld = new ChangingGraph<GeneralLayout>;
				gd<Name>(&innerWorld->whole_) = name+"_flowed";
				Box *box = new Box(outerWorld,innerWorld);
				box->assignEngine(innerEngines,g_transform);
				EnginePair<GeneralLayout> outerEngines;
				outerEngines.Append(box);
				configureLayout<ConfiguratorVec>(name,attrs,outerWorld,outerEngines);
			}
			else
				configureLayout<ConfiguratorVec>(name,attrs,innerWorld,innerEngines);
		}
	};
	template<typename ConfiguratorVec,typename Layout> 
	static void config(DString name,const StrAttrs &attrs,ChangingGraph<Layout> *world,EnginePair<Layout> engines) {
		boost::mpl::if_<boost::is_same<Layout,DynaDAGLayout>,EmphasizeFlowConfiguratorImpl,PassConfigurator>::type
			::template config<ConfiguratorVec>(name,attrs,world,engines);
	}
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif //EmphasizeFlowConfigurator_h
