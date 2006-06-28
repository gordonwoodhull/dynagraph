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

#ifndef LabelPlacer_h
#define LabelPlacer_h

namespace Dynagraph {

template<typename Layout>
struct LabelPlacer : LinkedChangeProcessor<Layout> {
	LabelPlacer(ChangingGraph<Layout> *world) : LinkedChangeProcessor<Layout>(world) {}
	void Process();
};
template<typename Layout>
void placeLabels(typename Layout::Edge *e) {
	EdgeLabels &el = gd<EdgeLabels>(e);
	if(el.empty())
		return;

	EdgeGeom &eg = gd<EdgeGeom>(e);
	Drawn &drawn = gd<Drawn>(e);
	drawn.clear();
	drawn.push_back(eg.pos);
	Line &l = eg.pos;
	if(l.empty())
		return;
	segsizes sizes(&*l.begin(),(int)l.size(),l.degree);
	for(EdgeLabels::iterator il = el.begin(); il!=el.end(); ++il)
		if(l.size()) {
			std::pair<Coord,Coord> pc = secant(&*l.begin(),(int)l.size(),l.degree,sizes,il->where,il->length);
			if(il->shape) {
				Coord dq = pc.second-pc.first,
					dp = il->pos2 - il->pos1;
				double thq = atan2(dq.y,dq.x),
					thp = atan2(dp.y,dp.x),
					dth = thq-thp,
					mult = dq.Len()/dp.Len();
				drawn.push_back(Line());
				Line &dest = drawn.back();
				dest.reserve(il->shape->size());
				for(Line::iterator pi = il->shape->begin(); pi!=il->shape->end(); ++pi) {
					Coord r = *pi - il->pos1,
						r2(r.x*cos(dth)-r.y*sin(dth),r.y*cos(dth)+r.x*sin(dth));
					r2 *= mult;
					r2 += pc.first;
					dest.push_back(r2);
				}
			}
			else {
				il->pos1 = pc.first;
				il->pos2 = pc.second;
			}
		}
		else
			il->pos1 = il->pos2 = Position();
}
template<typename Layout>
void placeLabels(typename Layout::Node *n) {
	NodeLabels &nl = gd<NodeLabels>(n);
	if(nl.empty())
		return;

	NodeGeom &ng = gd<NodeGeom>(n);
	Coord start = ng.pos;
	Coord gap = gd<GraphGeom>(n->g).labelGap;
	for(NodeLabels::iterator il = nl.begin(); il!=nl.end(); ++il) {
		// simulate old align with new
		switch(il->align) {
		case DG_NODELABEL_CENTER:
			il->nodeAlign = il->labelAlign = Coord(0.5,0.5);
			goto old_align;
		case DG_NODELABEL_RIGHT:
			il->nodeAlign = Coord(1.0,0.5);
			il->labelAlign = Coord(0,0.5);
			goto old_align;
		case DG_NODELABEL_TOP:
			il->nodeAlign = Coord(0.5,0);
			il->labelAlign = Coord(0.5,1);
			goto old_align;
		case DG_NODELABEL_LEFT:
			il->nodeAlign = Coord(0,0.5);
			il->labelAlign = Coord(1,0.5);
			goto old_align;
		case DG_NODELABEL_BOTTOM:
			il->nodeAlign = Coord(0.5,1);
			il->labelAlign = Coord(0.5,0);
		old_align:
			il->nodeOffset = il->labelOffset = Coord(0,0);
			il->scaleX = il->scaleY = false;
			break;
		case DG_NODELABEL_BETTER:
			break;
		default:
			continue;
		}
		Coord sizevec = il->size
#ifndef DOWN_GREATER
			* Coord(1,-1)
#endif
		;
		const Bounds &bb = ng.BoundingBox();
		Coord rpn = bb.NW() + il->nodeAlign*(bb.SE()-bb.NW()) + il->nodeOffset,
			rpl = il->labelAlign*sizevec + il->labelOffset;
		Coord rpn2 = bb.NW() + il->nodeAlign2*(bb.SE()-bb.NW()) + il->nodeOffset2,
			rpl2 = il->labelAlign2*sizevec + il->labelOffset2;
		Coord scale(1,1);
		if(il->scaleX)
			scale.x = (rpn2.x-rpn.x)/(rpl2.x-rpl.x);
		if(il->scaleY)
			scale.y = (rpn2.y-rpn.y)/(rpl2.y-rpl.y);
		Coord ul = rpn - scale*rpl;
		il->bounds.valid = true;
		il->bounds.l = ul.x;
		il->bounds.t = ul.y;
		il->bounds.r = ul.x + scale.x*sizevec.x;
		il->bounds.b = ul.y + scale.y*sizevec.y;
	}
}

template<typename Layout>
void LabelPlacer<Layout>::Process() {
	ChangeQueue<Layout> &Q = this->world_->Q_;
	typename Layout::graphedge_iter ei = Q.insE.edges().begin();
	for(; ei!=Q.insE.edges().end(); ++ei)
		placeLabels<Layout>(*ei);
	for(ei = Q.modE.edges().begin(); ei!=Q.modE.edges().end(); ++ei)
		if(igd<Update>(*ei).flags&(DG_UPD_MOVE|DG_UPD_LABEL))
			placeLabels<Layout>(*ei);
	typename Layout::node_iter ni;
	for(ni = Q.insN.nodes().begin(); ni!=Q.insN.nodes().end(); ++ni)
		placeLabels<Layout>(*ni);
	for(ni = Q.modN.nodes().begin(); ni!=Q.modN.nodes().end(); ++ni)
		if(igd<Update>(*ni).flags&(DG_UPD_MOVE|DG_UPD_LABEL|DG_UPD_REGION|DG_UPD_POLYDEF|DG_UPD_DRAWN))
			placeLabels<Layout>(*ni);
	this->NextProcess();
}

} // namespace Dynagraph

#endif //LabelPlacer_h
