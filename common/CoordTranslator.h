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

#ifndef CoordTranslator_h
#define CoordTranslator_h

#include <algorithm>
#include <boost/bind.hpp>
#include "reorient.h"
#include "LayoutToLayoutTranslator.h"

namespace Dynagraph {

// intention is that more sophisticated e.g. matrix-based translation could be plugged in someday
// canvas coords could get translated differently from the relative coords used in node shapes
struct OrientationTranslator {
	template<typename Layout>
	static Coord xlateIn(Layout *l,bool isCanvas,const Coord &c) {
		return reorient(c,true,gd<Translation>(l).orientation);
	}
	template<typename Layout>
	static Coord xlateOut(Layout *l,bool isCanvas,const Coord &c) {
		return reorient(c,false,gd<Translation>(l).orientation);
	}
	// give me convenience or give me death
	template<typename Layout>
	static void xlatePos(Coord (*fn)(Layout *,bool,const Coord&),Layout *l,bool isCanvas,const Position &src,Position &dest) {
		if(src.valid)
			dest = fn(l,isCanvas,src);
		else
			dest.valid = false;
	}
	template<typename Layout>
	static void xlateIn(Layout *l,bool isCanvas,const Position &src,Position &dest) {
		xlatePos(xlateIn<Layout>,l,isCanvas,src,dest);
	}
	template<typename Layout>
	static void xlateOut(Layout *l,bool isCanvas,const Position &src,Position &dest) {
		xlatePos(xlateOut<Layout>,l,isCanvas,src,dest);
	}
	template<typename Layout>
	static void xlateLine(Coord (*fn)(Layout *,bool,const Coord&),Layout *l,bool isCanvas,const Line &src,Line &dest) {
		dest.resize(src.size());
		dest.degree = src.degree;
		std::transform(src.begin(),src.end(),dest.begin(),boost::bind(fn,l,isCanvas,_1));
	}
	template<typename Layout>
	static void xlateIn(Layout *l,bool isCanvas,const Line &src,Line &dest) {
		xlateLine(xlateIn<Layout>,l,isCanvas,src,dest);
	}
	template<typename Layout>
	static void xlateOut(Layout *l,bool isCanvas,const Line &src,Line &dest) {
		xlateLine(xlateOut<Layout>,l,isCanvas,src,dest);
	}

};
// coord translators take responsibility for some attributes away from LayoutToLayoutTranslator
struct CoordTranslatorCopyPolicy : LayoutToLayoutCopyAllPolicy {
	enum {
		CopyDrawn = false,
		CopyNodePos = false,
		CopyEdgePos = false,
		CopyNodeRegion = false
	};
};
// the implementation is based on the NamedToNamedChangeTranslator, with LayoutToLayoutTranslator actions
// for the actions, in the interest of clarity, we define LayoutToLayoutTranslator as Base class 
// and include them as members rather than using inheritance
template<typename OuterLayout,typename InnerLayout>
struct CoordTranslatorInActions {
	typedef LayoutToLayoutTranslator<OuterLayout,InnerLayout,CoordTranslatorCopyPolicy> Base;
	Base base_;
	typedef OrientationTranslator Trans;
	void OpenGraph(OuterLayout *ol,InnerLayout *il) {
		base_.OpenGraph(ol,il);
	}
	void ModifyGraph(OuterLayout *ol,InnerLayout *il) {
		base_.ModifyGraph(ol,il);
	}
	void InsertNode(typename OuterLayout::Node *on,typename InnerLayout::Node *in) {
		base_.InsertNode(on,in);
		Trans::xlateIn(on->g,true,gd<NodeGeom>(on).pos,gd<NodeGeom>(in).pos);
		Trans::xlateIn(on->g,false,gd<NodeGeom>(on).region.shape,gd<NodeGeom>(in).region.shape);
		gd<NodeGeom>(in).region.updateBounds();
	}
	void InsertEdge(typename OuterLayout::Edge *oe,typename InnerLayout::Edge *ie) {
		base_.InsertEdge(oe,ie);
		Trans::xlateIn(oe->g,true,gd<EdgeGeom>(oe).pos,gd<EdgeGeom>(ie).pos);
	}
	void ModifyNode(typename OuterLayout::Node *on,typename InnerLayout::Node *in) {
		base_.ModifyNode(on,in);
		if(igd<Update>(on).flags&DG_UPD_MOVE)
			Trans::xlateIn(on->g,true,gd<NodeGeom>(on).pos,gd<NodeGeom>(in).pos);
		if(igd<Update>(on).flags&DG_UPD_REGION) {
			Trans::xlateIn(on->g,false,gd<NodeGeom>(on).region.shape,gd<NodeGeom>(in).region.shape);
			gd<NodeGeom>(in).region.updateBounds();
		}
	}
	void ModifyEdge(typename OuterLayout::Edge *oe,typename InnerLayout::Edge *ie) {
		base_.ModifyEdge(oe,ie);
		if(igd<Update>(oe).flags&DG_UPD_MOVE)
			Trans::xlateIn(oe->g,true,gd<EdgeGeom>(oe).pos,gd<EdgeGeom>(ie).pos);			
	}
	void DeleteNode(typename OuterLayout::Node *on,typename InnerLayout::Node *in) {
		base_.DeleteNode(on,in);
	}
	void DeleteEdge(typename OuterLayout::Edge *oe,typename InnerLayout::Edge *ie) {
		base_.DeleteEdge(oe,ie);
	}
};
template<typename InnerLayout,typename OuterLayout>
struct CoordTranslatorOutActions {
	typedef LayoutToLayoutTranslator<InnerLayout,OuterLayout,CoordTranslatorCopyPolicy> Base;
	Base base_;
	typedef OrientationTranslator Trans;
	void OpenGraph(InnerLayout *il,OuterLayout *ol) {
		base_.OpenGraph(il,ol);
	}
	void ModifyGraph(InnerLayout *il,OuterLayout *ol) {
		base_.ModifyGraph(il,ol);
	}
	void InsertNode(typename InnerLayout::Node *in,typename OuterLayout::Node *on) {
		base_.InsertNode(in,on);
		Trans::xlateOut(on->g,true,gd<NodeGeom>(in).pos,gd<NodeGeom>(on).pos);
	}
	void InsertEdge(typename InnerLayout::Edge *ie,typename OuterLayout::Edge *oe) {
		base_.InsertEdge(ie,oe);
		Trans::xlateOut(oe->g,true,gd<EdgeGeom>(ie).pos,gd<EdgeGeom>(oe).pos);
	}
	void ModifyNode(typename InnerLayout::Node *in,typename OuterLayout::Node *on) {
		base_.ModifyNode(in,on);
		if(igd<Update>(in).flags&DG_UPD_MOVE)
			Trans::xlateOut(on->g,true,gd<NodeGeom>(in).pos,gd<NodeGeom>(on).pos);
	}
	void ModifyEdge(typename InnerLayout::Edge *ie,typename OuterLayout::Edge *oe) {
		base_.ModifyEdge(ie,oe);
		if(igd<Update>(ie).flags&DG_UPD_MOVE)
			Trans::xlateOut(oe->g,true,gd<EdgeGeom>(ie).pos,gd<EdgeGeom>(oe).pos);			
	}
	void DeleteNode(typename InnerLayout::Node *in,typename OuterLayout::Node *on) {
		base_.DeleteNode(in,on);
	}
	void DeleteEdge(typename InnerLayout::Edge *ie,typename OuterLayout::Edge *oe) {
		base_.DeleteEdge(ie,oe);
	}
};
template<typename OuterLayout,typename InnerLayout>
struct CoordTranslatorIn : ChangeTranslator<OuterLayout,InnerLayout> {
	typedef typename CoordTranslatorInActions<OuterLayout,InnerLayout>::Trans Trans;
	CoordTranslatorIn(ChangingGraph<OuterLayout> *world1,ChangingGraph<InnerLayout> *world2) 
		: ChangeTranslator<OuterLayout,InnerLayout>(world1,world2) {}
	// regular translation of ins,mod,del with above actions usually applies,
	// but when the translation itself has changed, ALL inner nodes' shapes need to be rerotated
	// we intentionally do not change positions here, because it would be nice if internal layout can keep its worldview mosty intact
	// instead, CoordTranslatorOut will catch DG_UPD_TRANSLATION too, and translate everything anew
	void Process() {
		ChangeQueue<OuterLayout> &srcQ = this->sourceWorld_->Q_;
		ChangeQueue<InnerLayout> &destQ = this->destWorld_->Q_;
		if(igd<Update>(srcQ.ModGraph()).flags&DG_UPD_TRANSLATION)
			for(typename OuterLayout::node_iter ni = srcQ.current->nodes().begin(); ni!=srcQ.current->nodes().end(); ++ni)
				//if(!srcQ.insN.find(*ni)&&!srcQ.modN.find(*ni)) // possible optimization?
				if(typename InnerLayout::Node *in = destQ.whole->fetch_node(gd<Name>(*ni))) {
					Trans::xlateIn(srcQ.whole,false,gd<NodeGeom>(*ni).region.shape,gd<NodeGeom>(in).region.shape);
					gd<NodeGeom>(in).region.updateBounds();
					ModifyNode(destQ,in,DG_UPD_REGION);
				}
	}
};
template<typename InnerLayout,typename OuterLayout>
struct CoordTranslatorOut : ChangeTranslator<InnerLayout,OuterLayout> {
	typedef typename CoordTranslatorOutActions<InnerLayout,OuterLayout>::Trans Trans;
	CoordTranslatorOut(ChangingGraph<InnerLayout> *world1,ChangingGraph<OuterLayout> *world2)
		: ChangeTranslator<InnerLayout,OuterLayout>(world1,world2) {}
	// again, translation is mostly mundane, but if translation itself changes,
	// we've now got to update ALL outer positions
	void Process() {
		ChangeQueue<InnerLayout> &srcQ = this->sourceWorld_->Q_;
		ChangeQueue<OuterLayout> &destQ = this->destWorld_->Q_;
		if(igd<Update>(srcQ.ModGraph()).flags&DG_UPD_TRANSLATION) {
			for(typename InnerLayout::node_iter ni = srcQ.current->nodes().begin(); ni!=srcQ.current->nodes().end(); ++ni)
				//if(!srcQ.insN.find(*ni)&&!srcQ.modN.find(*ni)) // possible optimization?
				if(typename OuterLayout::Node *on = destQ.whole->fetch_node(gd<Name>(*ni))) {
					Trans::xlateOut(destQ.whole,true,gd<NodeGeom>(*ni).pos,gd<NodeGeom>(on).pos);
					ModifyNode(destQ,on,DG_UPD_MOVE);
				}
			for(typename InnerLayout::graphedge_iter ei = srcQ.current->edges().begin(); ei!=srcQ.current->edges().end(); ++ei)
				//if(!srcQ.insE.find(*ei)&&!srcQ.modE.find(*ei)) // possible optimization?
				if(typename OuterLayout::Edge *oe = destQ.whole->fetch_edge(gd<Name>(*ei))) {
					Trans::xlateOut(destQ.whole,true,gd<EdgeGeom>(*ei).pos,gd<EdgeGeom>(oe).pos);
					ModifyEdge(destQ,oe,DG_UPD_MOVE);
				}
		}
	}
};

} // namespace Dynagraph
#endif // CoordTranslator_h
