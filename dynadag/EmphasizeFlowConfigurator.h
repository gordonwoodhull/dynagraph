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
#include "common/InternalWorld.h"
#include "SEdger.h"
#include "common/EdgeSplicer.h"

namespace Dynagraph {
namespace DynaDAG {

struct EmphasizeFlowConfigurator {
    struct EmphasizeFlowConfiguratorImpl {
        template<typename Configurators>
        static void config(DString name,const StrAttrs &attrs,ChangingGraph<DynaDAGLayout> *innerWorld,EnginePair<DynaDAGLayout> innerEngines) {
            if(attrs.look("emphasizeflow","false")=="true"||attrs.look("flowemphasizable","false")=="true") {
                typedef SEdger<GeneralLayout,DynaDAGLayout> InTranslator;
                typedef EdgeSplicer<DynaDAGLayout,GeneralLayout> OutTranslator;
                typedef InternalWorld<GeneralLayout,DynaDAGLayout> InWorld;
                gd<Name>(&innerWorld->whole_) = std::string(name)+"_flowed";
                ChangingGraph<GeneralLayout> *outerWorld = new ChangingGraph<GeneralLayout>;
                InWorld *inWorld = new InWorld(outerWorld,innerWorld);
                inWorld->inTranslators_.push_back(new InTranslator(outerWorld,innerWorld));
                inWorld->outTranslators_.push_back(new OutTranslator(innerWorld,outerWorld));
                innerEngines.Prepend(new UpdateCurrentProcessor<DynaDAGLayout>(innerWorld));
                inWorld->innerEngines_ = innerEngines;
                inWorld->CompleteConfiguration();
                EnginePair<GeneralLayout> outerEngines;
                outerEngines.Append(inWorld);
                outerEngines.Append(new OkayEngine<GeneralLayout>(outerWorld));
                configureLayout<Configurators>(name,attrs,outerWorld,outerEngines);
            }
            else
                configureLayout<Configurators>(name,attrs,innerWorld,innerEngines);
        }
    };
    template<typename Configurators,typename Layout>
    static void config(DString name,const StrAttrs &attrs,ChangingGraph<Layout> *world,EnginePair<Layout> engines) {
        boost::mpl::if_<boost::is_same<Layout,DynaDAGLayout>,EmphasizeFlowConfiguratorImpl,PassConfigurator>::type
            ::template config<Configurators>(name,attrs,world,engines);
    }
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif //EmphasizeFlowConfigurator_h
