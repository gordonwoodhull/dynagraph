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
struct ClearExtraRanksEngine : ChangeProcessor<Layout> {
	ClearExtraRanksEngine(ChangingGraph<Layout> *world) : ChangeProcessor<Layout>(world) {}
	void Process(ChangeProcessing *next) {
		gd<ExtraRanks>(&this->world_->whole_).extraRanks.clear();
		next->Process(0);
	}
};


} // namespace DynaDAG
} // namespace Dynagraph

#endif // ClearExtraRanksEngine_h
