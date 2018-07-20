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

#ifndef Configurator_h
#define Configurator_h

#include "common/StrAttr.h"
#include "common/ChangingGraph.h"
#include "ChangeProcessor.h"
#include "EnginePair.h"
#include <boost/mpl/assert.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/if.hpp>

namespace Dynagraph {

/*
    Configurators are semi-autonomous builders of Engines and Worlds
    The configuration gets built from the inside out.
    Each Configurator (so far) either pre/appends Engines to the chain,
    or boxes up a chain and world into an InternalWorld, 
    and creates a new world in which it's an Engine.
    All of this sounds almost philosophical, no?  
    Unforch it craves better language; for now it's a mixed-up metaphor.

    Each configurator implements a method config(), which is templated both on
    * the vector typelist of the rest of the Configurators
    * the layout type
    In this way it can conceivably act on any type of layout, and also it can change the type
    by wrapping it in another layer of layout.  When it's done it recursively calls configureLayout
    so that the new type can be passed on to the next Configurator.  (Since the types are "passed" at
    compile time, this means that a tree of layout-specialized Configurators gets generated by the compiler.)
*/

struct PopConfigurator {
    template<typename Configurators,typename Layout>
    static void config(DString name,const StrAttrs &attrs,ChangingGraph<Layout> *world,EnginePair<Layout> engines) {
        typedef typename boost::mpl::front<Configurators>::type FirstConfigurator;
        typedef typename boost::mpl::pop_front<Configurators>::type Rest;
        FirstConfigurator::template config<Rest>(name,attrs,world,engines);
    }
};
struct DoNothingConfigurator {
    template<typename Configurators,typename Layout>
    static void config(DString name,const StrAttrs &attrs,ChangingGraph<Layout> *world,EnginePair<Layout> engines) {}
};
template<typename Configurators,typename Layout>
void configureLayout(DString name,const StrAttrs &attrs,ChangingGraph<Layout> *world,EnginePair<Layout> engines) {
    boost::mpl::if_<boost::mpl::empty<Configurators>,DoNothingConfigurator,PopConfigurator>::type
        ::template config<Configurators>(name,attrs,world,engines);
}
template<typename Configurators>
void configureLayout(DString name,const StrAttrs &attrs) {
    configureLayout<Configurators,void>(name,attrs,0,EnginePair<void>());
}
struct PassConfigurator {
    template<typename Configurators,typename Layout>
    static void config(DString name,const StrAttrs &attrs,ChangingGraph<Layout> *world,EnginePair<Layout> engines) {
        configureLayout<Configurators>(name,attrs,world,engines);
    }
};

} // namespace Dynagraph

#endif // Configurator_h
