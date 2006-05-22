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


#ifndef NSRankerModel_h
#define NSRankerModel_h

namespace Dynagraph {
namespace DynaDAG {

// Network Simplex is not an incremental algorithm -- yet.
// But it's good to use the framework.  NSRankerModel changes are applied incrementally, 
// but the return is handled exclusively by NSRankerModelToLayoutTranslator

struct NSRankerModelToConstraintTranslator : ChangeTranslator<NSRankerModel,ConstraintGraph> {
	GoingQueueTransition<NSRankerModel,ConstraintGraph> transition_;
	NSRankerModelToConstraintTranslator(ConstraintGraph *whole,ConstraintGraph *current) 
		: transition_(whole,current) {}
	void Process(ChangeQueue<NSRankerModel> &Q) {
		ChangeQueue<ConstraintGraph> &Q2 = transition_.NextQ();

		transition_.EndLastQ(Q);
		LinkedChangeProcessor<Graph2>::NextProcess(Q2);
	}
};

// currently this is just to keep the processor chain alive
struct NSRankerModelToConstraintTranslator : ChangeTranslator<ConstraintGraph,NSRankerModel> {
	ReturningQueueTransition<ConstraintGraph,NSRankerModel> transition_;
	void Process(ChangeQueue<ConstraintGraph> &Q) {
		ChangeQueue<NSRankerModel> &Q2 = transition_.NextQ();
		transition_.EndLastQ(Q); // (but it's empty)
		LinkedChangeProcessor<Graph2>::NextProcess(Q2);
	}

} // namespace DynaDAG
} // namespace Dynagraph

#endif // NSRankerModel_h
