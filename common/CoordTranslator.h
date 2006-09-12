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

#include "reorient.h"
#include <algorithm>
#include <boost/bind.hpp>

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
};
// the implementation is based on the NamedToNamedChangeTranslator, with LayoutToLayoutTranslator actions
// for the actions, in the interest of clarity, we define LayoutToLayoutTranslator as Base class 
// and include them as members rather than using inheritance
template<typename OuterLayout,typename InnerLayout>
struct CoordTranslatorInActions {
	typedef LayoutToLayoutTranslator<OuterLayout,InnerLayout> Base;
	Base base_;
	typedef OrientationTranslator Trans;
	void translate_shape(OuterLayout *l,const Line &source,Line &dest) {
		dest.degree = source.degree;
		std::transform(source.begin(),source.end(),dest.begin(),boost::bind(Trans::xlateIn,l,false));
	}
	void translate_pos(OuterLayout *l,const Position &source,Position &dest) {
		if(source.valid)
			dest = Trans::xlateIn(l,true,source);
		else
			dest.valid = false;
	}
	void translate_line(OuterLayout *l,const Line &source,Line &dest) {
		dest.degree = source.degree;
		std::transform(source.begin(),source.end(),dest.begin(),boost::bind(Trans::xlateIn,l,true));
	}
	void ModifyGraph(OuterLayout *ol,InnerLayout *il) {
		base_.ModifyGraph(ol,il);
	}
	void InsertNode(typename OuterLayout::Node *on,typename InnerLayout::Node *in) {
		base_.InsertNode(on,in);
		translate_pos(gd<NodeGeom>(on).pos,gd<NodeGeom>(in).pos);
		translate_shape(on->g,gd<NodeGeom>(on).region.shape,gd<NodeGeom>(in).region.shape);
		gd<NodeGeom>(in).region.updateBounds();
	}
	void InsertEdge(typename OuterLayout::Edge *oe,typename InnerLayout::Edge *ie) {
		base_.InsertEdge(oe,ie);
		translate_line(oe->g,gd<EdgeGeom>(oe).pos,gd<EdgeGeom>(ie).pos);
	}
	void ModifyNode(typename OuterLayout::Node *on,typename InnerLayout::Node *in) {
		base_.ModifyNode(on,in);
		if(igd<Update>(on)&DG_UPD_MOVE)
			translate_pos(gd<NodeGeom>(on).pos,gd<NodeGeom>(in).pos);
		if(igd<Update>(on)&DG_UPD_REGION) {
			translate_shape(on->g,gd<NodeGeom>(on).region.shape,gd<NodeGeom>(in).region.shape);
			gd<NodeGeom>(in).region.updateBounds();
		}
	}
	void ModifyEdge(typename OuterLayout::Edge *oe,typename InnerLayout::Edge *ie) {
		base_.ModifyEdge(oe,ie);
		if(igd<Update>(oe)&DG_UPD_MOVE)
			translate_line(oe->g,gd<EdgeGeom>(oe).pos,gd<EdgeGeom>(ie).pos);			
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
	typedef LayoutToLayoutTranslator<InnerLayout,OuterLayout> Base;
	Base base_;
	typedef OrientationTranslator Trans;
	void translate_pos(OuterLayout *l,const Position &source,Position &dest) {
		if(source.valid)
			dest = Trans::xlateOut(l,true,source);
		else
			dest.valid = false;
	}
	void translate_line(OuterLayout *l,const Line &source,Line &dest) {
		dest.degree = source.degree;
		std::transform(source.begin(),source.end(),dest.begin(),boost::bind(Trans::xlateOut,l,true));
	}
	void ModifyGraph(InnerLayout *il,OuterLayout *ol) {
		base_.ModifyGraph(il,ol);
	}
	void InsertNode(typename InnerLayout::Node *in,typename OuterLayout::Node *on) {
		base_.InsertNode(in,on);
		translate_pos(in->g,gd<NodeGeom>(in).pos,gd<NodeGeom>(on).pos);
	}
	void InsertEdge(typename InnerLayout::Edge *ie,typename OuterLayout::Edge *oe) {
		base_.InsertEdge(ie,oe);
		translate_line(ei->g,gd<EdgeGeom>(ie).pos,gd<EdgeGeom>(oe).pos);
	}
	void ModifyNode(typename InnerLayout::Node *in,typename OuterLayout::Node *on) {
		base_.ModifyNode(in,on);
		if(igd<Update>(in)&DG_UPD_MOVE)
			translate_pos(gd<NodeGeom>(in).pos,gd<NodeGeom>(on).pos);
	}
	void ModifyEdge(typename InnerLayout::Edge *ie,typename OuterLayout::Edge *oe) {
		base_.ModifyEdge(ie,oe);
		if(igd<Update>(ie)&DG_UPD_MOVE)
			translate_line(oe->g,gd<EdgeGeom>(ie).pos,gd<EdgeGeom>(oe).pos);			
	}
	void DeleteNode(typename InnerLayout::Node *in,typename OuterLayout::Node *on) {
		base_.DeleteNode(in,on);
	}
	void DeleteEdge(typename InnerLayout::Edge *ie,typename OuterLayout::Edge *oe) {
		base_.DeleteEdge(ie,oe);
	}
};
template<typename OuterLayout,typename InnerLayout>
struct CoordTranslatorIn : NamedToNamedChangeTranslator<OuterLayout,InnerLayout,CoordTranslatorInActions<OuterLayout,InnerLayout>,true> {
	typedef NamedToNamedChangeTranslator<OuterLayout,InnerLayout,CoordTranslatorInActions<OuterLayout,InnerLayout>,true > Base;
	CoordTranslatorIn(ChangingGraph<OuterLayout> *world1,ChangingGraph<InnerLayout> *world2) 
		: Base(world1,world2) {}
	// regular translation of ins,mod,del with above actions usually applies,
	// but when the translation itself has changed, ALL inner nodes' shapes need to be rerotated
	// we intentionally do not change positions here, because it would be nice if internal layout can keep its worldview mosty intact
	// instead, CoordTranslatorOut will catch DG_UPD_TRANSLATION too, and translate everything anew
	void Process() {
		Base::Process();
		ChangeQueue<OuterLayout> &Q1 = LinkedChangeProcessor<OuterLayout>::world_->Q_;
		ChangeQueue<InnerLayout> &Q2 = LinkedChangeProcessor<InnerLayout>::world_->Q_;
		if(igd<Update>(Q1.ModGraph())&DG_UPD_TRANSLATION)
			for(typename OuterLayout::node_iter ni = Q1.current->nodes().begin(); ni!=Q1.current->nodes().end(); ++ni)
				//if(!Q1.insN.find(*ni)&&!Q1.modN.find(*ni)) // possible optimization?
				if(typename InnerLayout::Node *in = Q2.whole->fetch_node(gd<Name>(*ni))) {
					Base::actions_.translate_shape(gd<NodeGeom>(*ni).region.shape,gd<NodeGeom>(in).region.shape);
					ModifyNode(Q2,in,DG_UPD_REGION);
				}
	}
};
template<typename InnerLayout,typename OuterLayout>
struct CoordTranslatorOut : NamedToNamedChangeTranslator<InnerLayout,OuterLayout,CoordTranslatorOutActions<InnerLayout,OuterLayout>,false> {
	typename NamedToNamedChangeTranslator<InnerLayout,OuterLayout,CoordTranslatorOutActions<InnerLayout,OuterLayout>,false> Base;
	CoordTranslatorOut(ChangingGraph<InnerLayout> *world1,ChangingGraph<OuterLayout> *world2)
		: Base(world1,world2) {}
	// again, translation is mostly mundane, but if translation itself changes,
	// we've now got to update ALL outer positions
	void Process() {
		Base::Process();
		ChangeQueue<InnerLayout> &Q1 = LinkedChangeProcessor<InnerLayout>::world_->Q_;
		ChangeQueue<OuterLayout> &Q2 = LinkedChangeProcessor<OuterLayout>::world_->Q_;
		if(igd<Update>(Q1.ModGraph())&DG_UPD_TRANSLATION) {
			for(typename InnerLayout::node_iter ni = Q1.current->nodes().begin(); ni!=Q1.current->nodes().end(); ++ni)
				//if(!Q1.insN.find(*ni)&&!Q1.modN.find(*ni)) // possible optimization?
				if(typename OuterLayout::Node *on = Q2.whole->fetch_node(gd<Name>(*ni))) {
					Base::actions_.translate_pos(gd<NodeGeom>(*ni).pos,gd<NodeGeom>(on).pos);
					ModifyNode(Q2,on,DG_UPD_MOVE);
				}
			for(typename InnerLayout::graphedge_iter ei = Q1.current->edges().begin(); ei!=Q1.current->edges().end(); ++ei)
				//if(!Q1.insE.find(*ei)&&!Q1.modE.find(*ei)) // possible optimization?
				if(typename OuterLayout::Edge *oe = Q2.whole->fetch_edge(gd<Name>(*ei))) {
					Base::actions_.translate_line(gd<EdgeGeom>(*ei).pos,gd<EdgeGeom>(oe).pos);
					ModifyEdge(Q2,oe,DG_UPD_MOVE);
				}
		}
	}
};

} // namespace Dynagraph
#endif CoordTranslator_h
