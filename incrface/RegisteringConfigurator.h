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

#ifndef RegisteringConfigurator_h
#define RegisteringConfigurator_h

#include "common/Configurator.h"
#include "IncrStrGraphHandler.h"
#include "TextWatcherOutput.h"
#include "OutputIncrface.h"
#include "OutputDot.h"

namespace Dynagraph {

struct RegisteringConfigurator {
    template<typename Configurators,typename Layout>
    static void config(DString name,const StrAttrs &attrs,ChangingGraph<Layout> *world,EnginePair<Layout> engines) {
        BOOST_MPL_ASSERT((boost::mpl::empty<Configurators>)); // must be end of line
        // handler for parser
        IncrStrGraphHandler<Layout> *handler = new IncrStrGraphHandler<Layout>(world);
        // its handler (request command deferred)
        handler->watcher_ =  new TextWatcherOutput<Layout>;
        // output engines at end of chain
        engines.Append(new OutputIncrface<Layout>(world,dgr::incrface));
        engines.Append(new OutputDot<Layout>(world));
        // start chain
        handler->engine_ = engines.first;
        // and we're go!
        incr_set_handler(name,handler);
    }
};

} // namespace Dynagraph

#endif //RegisteringConfigurator_h
