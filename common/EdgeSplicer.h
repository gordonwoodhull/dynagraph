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


#ifndef EdgeSplicer_h
#define EdgeSplicer_h

#include "LayoutToLayoutTranslator.h"
#include "SpliceParts.h"

namespace Dynagraph {

struct EdgeSplicerEndsDontMatch : DGException {
	EdgeSplicerEndsDontMatch() : DGException("attempt to splice together two edge splines that don't share an endpoint",true) {}
};
// do not copy secondOfTwo back out of inner layout
struct EdgeSplicerCopyPolicy : LayoutToLayoutCopyAllPolicy {
	enum {
		CopySecondOfTwo = false
	};
};
template<typename Layout1,typename Layout2>
struct EdgeSplicer : ChangeTranslator<Layout1,Layout2> {
	LayoutToLayoutTranslator<Layout1,Layout2,EdgeSplicerCopyPolicy> actions_;
	EdgeSplicer(ChangingGraph<Layout1> *world1,ChangingGraph<Layout2> *world2)
		: ChangeTranslator<Layout1,Layout2>(world1,world2) {}
	void Open() {
		ChangeQueue<Layout1> &srcQ = this->sourceWorld_->Q_;
		ChangeQueue<Layout2> &destQ = this->destWorld_->Q_;
		actions_.OpenGraph(srcQ.current,destQ.current);
	}
	void Process() {
		ChangeQueue<Layout1> &srcQ = this->sourceWorld_->Q_;
		ChangeQueue<Layout2> &destQ = this->destWorld_->Q_;
		actions_.ModifyGraph(srcQ.ModGraph(),destQ.ModGraph());

		// consider: maybe it would make more sense if all edges were checked "backward"
		// that is, use the loop at bottom to check for sources from targets
		// then an insert is when already inserted in target AND there exist either single or spliced source(s)
		// a modify is when any sources are ins/mod/del
		// a delete is when all sources are deleted
		// this strategy only works when target is the "outer world" i.e. when inserts must be initiated there
		// any truly new edges created in inner world will not be seen by either strategy

		// any node or edge that matches by name, we just copy layout
		for(typename Layout1::node_iter ni = srcQ.insN.nodes().begin(); ni!=srcQ.insN.nodes().end(); ++ni)
			if(typename Layout2::Node *n2 = destQ.whole->fetch_node(gd<Name>(*ni))) {
				typename Layout2::Node *n2i = destQ.InsNode(n2,false).object;
				actions_.InsertNode(*ni,n2i);
			}
		for(typename Layout1::graphedge_iter ei = srcQ.insE.edges().begin(); ei!=srcQ.insE.edges().end(); ++ei)
			if(typename Layout2::Edge *e2 = destQ.whole->fetch_edge(gd<Name>(*ei))) {
				typename Layout2::Edge *e2i = destQ.InsEdge(e2,false).object;
				actions_.InsertEdge(*ei,e2i);
			}
		for(typename Layout1::node_iter ni = srcQ.modN.nodes().begin(); ni!=srcQ.modN.nodes().end(); ++ni)
			if(typename Layout2::Node *n2 = destQ.whole->fetch_node(gd<Name>(*ni))) {
				typename Layout2::Node *n2m = destQ.ModNode(n2).object;
				actions_.ModifyNode(*ni,n2m);
			}
		for(typename Layout1::graphedge_iter ei = srcQ.modE.edges().begin(); ei!=srcQ.modE.edges().end(); ++ei)
			if(typename Layout2::Edge *e2 = destQ.whole->fetch_edge(gd<Name>(*ei))) {
				typename Layout2::Edge *e2m = destQ.ModEdge(e2).object;
				actions_.ModifyEdge(*ei,e2m);
			}
		for(typename Layout1::graphedge_iter ei = srcQ.delE.edges().begin(); ei!=srcQ.delE.edges().end(); ++ei) {
			if(*SpliceEdgePartsIterator<Layout1>(srcQ.current,gd<Name>(*ei),'e'))
				continue; // this is now a spliced edge - not really a delete
			if(typename Layout2::Edge *e2 = destQ.whole->fetch_edge(gd<Name>(*ei))) {
				typename Layout2::Edge *e2d = destQ.DelEdge(e2,false).object;
				actions_.DeleteEdge(*ei,e2d);
			}
		}
		for(typename Layout1::node_iter ni = srcQ.delN.nodes().begin(); ni!=srcQ.delN.nodes().end(); ++ni)
			if(typename Layout2::Node *n2 = destQ.whole->fetch_node(gd<Name>(*ni))) {
				typename Layout2::Node *n2d = destQ.DelNode(n2,false).object;
				actions_.DeleteNode(*ni,n2d);
			}

		// next look at all current edges in the target,
		// generate names of the parts that would be used
		// and look those up in the source to see if they've changed
		// if anything has changed, redraw that edge
		for(typename Layout2::graphedge_iter ei2 = destQ.current->edges().begin(); ei2!=destQ.current->edges().end(); ++ei2) {
			if(srcQ.current->fetch_edge(gd<Name>(*ei2)))
				continue; // this is an unspliced edge - ignore splice parts if any
			bool redraw = false;
			Name ename = gd<Name>(*ei2);
			typename Layout1::Edge *e;
			for(SpliceEdgePartsIterator<Layout1> ei1(srcQ.whole,ename,'e'); (e=*ei1); ++ei1)
				if(srcQ.insE.find(e) || srcQ.modE.find(e) || srcQ.delE.find(e)) {
					redraw = true;
					break;
				}
			if(!redraw) {
				typename Layout1::Node *n;
				for(SpliceNodePartsIterator<Layout1> ni1(srcQ.whole,ename,'e'); (n=*ni1); ++ni1)
					if(srcQ.insN.find(n) || srcQ.modN.find(n) || srcQ.delN.find(n)) {
						redraw = true;
						break;
					}
			}
			if(redraw) {
				redrawCompoundEdge(*ei2);
				ModifyEdge(destQ,*ei2,DG_UPD_MOVE);
			}
		}
	}
	void redrawCompoundEdge(typename Layout2::Edge *e) {
		Name ename = gd<Name>(e);
		EdgeGeom &eg2 = gd<EdgeGeom>(e);
		eg2.pos.Clear();
		eg2.pos.degree = 3;
		typename Layout1::Edge *e1;
		for(SpliceEdgePartsIterator<Layout1> ei1(&this->sourceWorld_->current_,ename,'e'); (e1=*ei1);++ei1) {
			EdgeGeom &eg1 = gd<EdgeGeom>(e1);
			if(eg1.pos.size()) {
				Line::iterator begin = eg1.pos.begin();
				if(eg2.pos.size()) {
					if(eg2.pos.back()!=*begin)
						throw EdgeSplicerEndsDontMatch();
					++begin;
				}
				eg2.pos.insert(eg2.pos.end(),begin,eg1.pos.end());
			}
		}
	}
};

} // namespace Dynagraph

#endif // EdgeSplicer_h
