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


#include "DynaDAG.h"
#include "pathplot/PathPlot.h"

using namespace std;

namespace Dynagraph {
namespace DynaDAG {

struct RouteBounds {
	RouteBounds(Config &config,Bounds &bounds) : config(config),bounds(bounds) {
		assert(bounds.valid);
	}
	void poly(Line &out);
	void term(DDModel::Edge *e,Coord t, bool start);
	void path(DDModel::Edge *e);
private:
	Config &config;
	Bounds &bounds;
	Line left,right; /* left and right sides of region */
	Line &getSide(LeftRight side) {
		if(side==LEFT)
			return left;
		else {
			assert(side==RIGHT);
			return right;
		}
	}
	void appendPoint(LeftRight side,Coord c) {
		Line &l = getSide(side);
		if(l.size()) {
			Coord c0 = getSide(side).back();
#ifndef DOWN_GREATER
			assert(c0.y>=c.y);
#else
			assert(c0.y<=c.y);
#endif
			if(c==c0)
				return;
		}
		getSide(side).push_back(c);
	}
	void appendQuad(double l1,double r1,double l2,double r2,double y1,double y2) {
		appendPoint(LEFT,Coord(l1,y1));
		appendPoint(LEFT,Coord(l2,y2));
		appendPoint(RIGHT,Coord(r1,y1));
		appendPoint(RIGHT,Coord(r2,y2));
	}
	double side(DDModel::Node *n,int s);
	bool localCrossing(DDModel::Edge *e,UpDown ud, DDModel::Node *n);
	double bounding(DDModel::Edge *e,LeftRight lr,UpDown ud,bool skipNodeEnds);
};
void RouteBounds::poly(Line &out) {
	out.Clear();
	out.degree = 1;
	for(Line::iterator pi = left.begin(); pi!=left.end(); ++pi)
		if(out.empty() || out.back()!=*pi)
			out.push_back(*pi);
	for(Line::reverse_iterator pri = right.rbegin(); pri!=right.rend(); ++pri)
		if(out.empty() || out.back()!=*pri)
			out.push_back(*pri);
}
double RouteBounds::side(DDModel::Node *n,int s) {
	return DDd(n).cur.x + s*(((s==LEFT)?config.LeftExtent(n):config.RightExtent(n)) + gd<GraphGeom>(config.whole).separation.x/2.0);
}
bool RouteBounds::localCrossing(DDModel::Edge *e,UpDown ud, DDModel::Node *n) {
	const int dist=2;
	assert(DDd(n).amEdgePart());
	DDModel::Node *v = ud==UP?e->tail:e->head,
		*w = n;
	assert(DDd(v).rank==DDd(w).rank);
	bool vw = DDd(v).order<DDd(w).order;
	int i;
	for(i = 0;i<dist;++i) {
		DDModel::Node *vv,*ww;
		if(DDd(w).amNodePart())
			break;
		ww = (*w->outs().begin())->head;
		if(v==e->tail)
			vv = e->head;
		else if(DDd(v).amNodePart())
			break;
		else
			vv = (*v->outs().begin())->head;
		if(vv==ww) // common end node
			return false;
		assert(DDd(vv).rank==DDd(ww).rank);
		if((DDd(vv).order<DDd(ww).order) != vw)
			return true;
		v = vv;
		w = ww;
	}
	v = ud==UP?e->tail:e->head;
	w = n;
	for(i = 0;i<dist;++i) {
		DDModel::Node *vv,*ww;
		if(DDd(w).amNodePart())
			break;
		ww = (*w->ins().begin())->tail;
		if(v==e->head)
			vv = e->tail;
		else if(DDd(v).amNodePart())
			break;
		else
			vv = (*v->ins().begin())->tail;
		if(vv==ww) // common end node
			return false;
		assert(DDd(vv).rank==DDd(ww).rank);
		if((DDd(vv).order<DDd(ww).order) != vw)
			return true;
		v = vv;
		w =ww;
	}
	return false;
}
double RouteBounds::bounding(DDModel::Edge *e,LeftRight lr,UpDown ud, bool skipNodeEnds) {
	DDModel::Node *n = ud==DOWN?e->head:e->tail;
	DDModel::Node *q = n;
	while((q = config.RelNode(q,lr))) {
		if(DDd(q).amNodePart()) {
			if(skipNodeEnds) {
				if(q==DDd(q).multi->top() && ud==DOWN && q->ins().empty())
					continue;
				if(q==DDd(q).multi->bottom() && ud==UP && q->outs().empty())
					continue;
			}
			break;
		}
		else if(!localCrossing(e,ud,q))
			break;
	}
	return q?side(q,-lr):(lr==LEFT?bounds.l:bounds.r);
}
void RouteBounds::term(DDModel::Edge *e,Coord t, bool start) {
	DDModel::Node *n = start?e->tail:e->head;
	double l = DDd(n).cur.x - config.LeftExtent(n),
		r = DDd(n).cur.x + config.RightExtent(n),
		el = bounding(e,LEFT,start?UP:DOWN,false),
		er = bounding(e,RIGHT,start?UP:DOWN,false);
	double edge = config.ranking.GetRank(DDd(n).rank)->yBase;
	if(start)
		appendQuad(l,r,el,er,t.y,edge);
	else
		appendQuad(el,er,l,r,edge,t.y);
}
void RouteBounds::path(DDModel::Edge *e) {
	double tl = bounding(e,LEFT,UP,true),
		tr = bounding(e,RIGHT,UP,true),
		hl = bounding(e,LEFT,DOWN,true),
		hr = bounding(e,RIGHT,DOWN,true),
		ty = config.ranking.GetRank(DDd(e->tail).rank)->yBase,
		hy = config.ranking.GetRank(DDd(e->head).rank)->yBase;
	appendQuad(tl,tr,hl,hr,ty,hy);
}
bool FlexiSpliner::MakeEdgeSpline(DDPath *path,SpliningLevel level) { //,ObstacleAvoiderSpliner<DynaDAGLayout> &obav) {
	assert(path->unclippedPath.Empty());

	DynaDAGLayout::Edge *e = path->layoutE;
	DDModel::Node *tl,*hd;
	if(path->direction==DDPath::flat) {
		tl = DDp(e->tail)->bottom();
		hd = DDp(e->head)->top();
	}
	else {
		tl = path->first->tail;
		hd = path->last->head;
	}
	EdgeGeom &eg = gd<EdgeGeom>(e);
	Coord tailpt = (path->direction==DDPath::reversed?eg.tailPort:eg.headPort).pos + DDd(tl).multi->pos(),
		headpt = (path->direction==DDPath::reversed?eg.headPort:eg.tailPort).pos + DDd(hd).multi->pos();

	Line &unclipped = path->unclippedPath;
	Line region;
	assert(e->tail!=e->head); // DynaDAGServer should draw self-edges
	if(path->direction==DDPath::flat) { // flat edge
		/*
		// disabled because of header dependencies (needs more work)
		DDModel::Node *left = tl,
			*right = hd;
		if(DDd(left).order>DDd(right).order)
			swap(left,right);
		obav.FindSpline(tailpt,headpt,unclipped);
		*/

		// hope that no nodes are in the way (calculating path to avoid 'em will be difficult)
		unclipped.degree = 1;
		unclipped.push_back(tailpt);
		unclipped.push_back(headpt);
	}
	else { // normal edge
		assert(path->first); // no flat or self edges!
		RouteBounds rb(config,gd<GraphGeom>(e->g).bounds);
		//rb.term(path->first,tailpt,true);
		for(DDMultiNode::edge_iter ei0 = DDd(tl).multi->eBegin(); ei0!=DDd(tl).multi->eEnd(); ++ei0)
			rb.path(*ei0);
		for(DDPath::edge_iter ei = path->eBegin(); ei!=path->eEnd(); ++ei)
			rb.path(*ei);
		for(DDMultiNode::edge_iter ei2 = DDd(hd).multi->eBegin(); ei2!=DDd(hd).multi->eEnd(); ++ei2)
			rb.path(*ei2);
		//rb.term(path->last,headpt,false);
		rb.poly(region);
	}
	if(!region.empty()) {
		switch(level) {
		case DG_SPLINELEVEL_BOUNDS:
			eg.pos = region;
			return true;
		case DG_SPLINELEVEL_SHORTEST:
		case DG_SPLINELEVEL_SPLINE: {
			try {
				Line polylineRoute;
				PathPlot::Shortest(region,Segment(tailpt,headpt),polylineRoute);
				if(level==DG_SPLINELEVEL_SPLINE) {
					PathPlot::SegmentV barriers;
					PathPlot::PolyBarriers(PathPlot::LineV(1,region),barriers);

					//cout << "message \"endslopes " << DDp(e->tail)->flowSlope << " " << DDp(e->head)->flowSlope << '"' << endl;
					Segment endSlopes(DDp(e->tail)->flowSlope,DDp(e->head)->flowSlope);
					if(path->direction==DDPath::reversed) {
						Coord t = endSlopes.b;
						endSlopes.b = -endSlopes.a;
						endSlopes.a = -t;
					}
					check(PathPlot::Route(barriers,polylineRoute,endSlopes,unclipped));
				}
				else
					unclipped = polylineRoute;
			}
			catch(...) {
				//return false;
			}
			for(DDPath::node_iter ni = path->nBegin(); ni!=path->nEnd(); ++ni) {
				double y = DDd(*ni).cur.y,
					x = checkPos(unclipped.YIntersection(y)).x;
				DDd(*ni).actualX = x;
				DDd(*ni).actualXValid = true;
			}
			break;
		}
		default:
			assert(0);
		}
	}
	NodeGeom &tg = gd<NodeGeom>(e->tail),
		&hg = gd<NodeGeom>(e->head);
	if(path->direction==DDPath::reversed) {
		eg.pos.ClipEndpoints(path->unclippedPath,hg.pos,eg.headClipped?&hg.region:0,
			tg.pos,eg.tailClipped?&tg.region:0);
		reverse(eg.pos.begin(),eg.pos.end());
	}
	else
		eg.pos.ClipEndpoints(path->unclippedPath,tg.pos,eg.tailClipped?&tg.region:0,
			hg.pos,eg.headClipped?&hg.region:0);
	return true;
}

} // namespace DynaDAG
} // namespace Dynagraph

