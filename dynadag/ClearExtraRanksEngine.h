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

#ifndef ClearExtraRanksEngine_h
#define ClearExtraRanksEngine_h

namespace Dynagraph {
namespace DynaDAG {

template<typename Layout>
struct ClearExtraRanksEngine : LinkedChangeProcessor<Layout> {
    ClearExtraRanksEngine(ChangingGraph<Layout> *world) : LinkedChangeProcessor<Layout>(world) {}
    void Process() {
        gd<ExtraRanks>(&this->world_->whole_).extraRanks.clear();
        this->NextProcess();
    }
};


} // namespace DynaDAG
} // namespace Dynagraph

#endif // ClearExtraRanksEngine_h
