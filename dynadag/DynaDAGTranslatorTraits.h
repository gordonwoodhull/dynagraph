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


#ifndef DynaDAGTranslatorTraits_h
#define DynaDAGTranslatorTraits_h

#include "SEdger.h"
#include "common/EdgeSplicer.h"

namespace Dynagraph {

template<>
struct translator_traitses<GeneralLayout,DynaDAG::DynaDAGLayout> {
	typedef DynaDAG::SEdger<GeneralLayout,DynaDAG::DynaDAGLayout> in_translator;
	typedef EdgeSplicer<DynaDAG::DynaDAGLayout,GeneralLayout> out_translator;
};

} // namespace Dynagraph

#endif // DynaDAGTranslatorTraits_h
