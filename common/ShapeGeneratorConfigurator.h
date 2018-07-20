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

#ifndef ShapeGeneratorConfigurator_h
#define ShapeGeneratorConfigurator_h

#include "common/Configurator.h"
#include "ShapeGenerator.h"

namespace Dynagraph {

struct ShapeGeneratorConfigurator {
    template<typename Configurators,typename Layout>
    static void config(DString name,const StrAttrs &attrs,ChangingGraph<Layout> *world,EnginePair<Layout> engines) {
        if(attrs.look("shapegen","true")=="true")
            engines.Prepend(new ShapeGenerator<Layout>(world));
        configureLayout<Configurators>(name,attrs,world,engines);
    }
};

} // namespace Dynagraph

#endif //ShapeGeneratorConfigurator_h
