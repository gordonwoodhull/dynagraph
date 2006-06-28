/**********************************************************
*      This software is part of the graphviz toolset      *
*                http://www.graphviz.org/                 *
*                                                         *
*            Copyright (c) 1994-2005 AT&T Corp.           *
*                and is licensed under the                *
*            Common Public License, Version 1.0           *
*                      by AT&T Corp.                      *
*                                                         *
*        Information and Software Systems Research        *
*              AT&T Research, Florham Park NJ             *
*                                                         *
*               This version available from               *
*                   http://dynagraph.org                  *
**********************************************************/

#ifndef InternalTranslator_h
#define InternalTranslator_h

#include "ChangeProcessor.h"

namespace Dynagraph {
template<typename Graph,typename ChangeActions>
struct InternalTranslator : LinkedChangeProcessor<Graph> {
	ChangeActions actions_;
	InternalTranslator(ChangingGraph<Graph> *world,const ChangeActions &action) 
		: LinkedChangeProcessor<Graph>(world),actions_(action) {}
	virtual void Process() {
		ChangeQueue<Graph> &Q = this->world_->Q_;
		actions_.ModifyGraph(Q.ModGraph());
		for(typename Graph::node_iter ni = Q.insN.nodes().begin(); ni!=Q.insN.nodes().end(); ++ni)
			actions_.InsertNode(*ni);
		for(typename Graph::graphedge_iter ei = Q.insE.edges().begin(); ei!=Q.insE.edges().end(); ++ei)
			actions_.InsertEdge(*ei);
		for(typename Graph::node_iter ni = Q.modN.nodes().begin(); ni!=Q.modN.nodes().end(); ++ni)
			actions_.ModifyNode(*ni);
		for(typename Graph::graphedge_iter ei = Q.modE.edges().begin(); ei!=Q.modE.edges().end(); ++ei)
			actions_.ModifyEdge(*ei);
		for(typename Graph::node_iter ni = Q.delN.nodes().begin(); ni!=Q.delN.nodes().end(); ++ni)
			actions_.DeleteNode(*ni);
		for(typename Graph::graphedge_iter ei = Q.delE.edges().begin(); ei!=Q.delE.edges().end(); ++ei)
			actions_.DeleteEdge(*ei);
		this->NextProcess();
	}
};
template<typename Graph,typename ChangeActions>
struct InternalTranslator2 : LinkedChangeProcessor<Graph> {
	ChangeActions actions_;
	InternalTranslator2(ChangingGraph<Graph> *world,const ChangeActions &action) 
		: LinkedChangeProcessor<Graph>(world),actions_(action) {}
	virtual void Process() {
		ChangeQueue<Graph> &Q = this->world_->Q_;
		actions_.ModifyGraph(Q.ModGraph(),Q.ModGraph());
		for(typename Graph::node_iter ni = Q.insN.nodes().begin(); ni!=Q.insN.nodes().end(); ++ni)
			actions_.InsertNode(*ni,*ni);
		for(typename Graph::graphedge_iter ei = Q.insE.edges().begin(); ei!=Q.insE.edges().end(); ++ei)
			actions_.InsertEdge(*ei,*ei);
		for(typename Graph::node_iter ni = Q.modN.nodes().begin(); ni!=Q.modN.nodes().end(); ++ni)
			actions_.ModifyNode(*ni,*ni);
		for(typename Graph::graphedge_iter ei = Q.modE.edges().begin(); ei!=Q.modE.edges().end(); ++ei)
			actions_.ModifyEdge(*ei,*ei);
		for(typename Graph::node_iter ni = Q.delN.nodes().begin(); ni!=Q.delN.nodes().end(); ++ni)
			actions_.DeleteNode(*ni,*ni);
		for(typename Graph::graphedge_iter ei = Q.delE.edges().begin(); ei!=Q.delE.edges().end(); ++ei)
			actions_.DeleteEdge(*ei,*ei);
		this->NextProcess();
	}
};

} // namespace Dynagraph

#endif // InternalTranslator_h
