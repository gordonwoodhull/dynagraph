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
		dgassert(bounds.valid);
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
			dgassert(side==RIGHT);
			return right;
		}
	}
	void appendPoint(LeftRight side,Coord c) {
		Line &l = getSide(side);
		if(l.size()) {
			Coord c0 = getSide(side).back();
#ifndef DOWN_GREATER
			dgassert(c0.y>=c.y);
#else
			dgassert(c0.y<=c.y);
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
	return gd<DDNode>(n).cur.x + s*((s==LEFT)?(config.LeftExtent(n)+config.LeftSep(n)):(config.RightExtent(n)+config.RightSep(n)));
}
bool RouteBounds::localCrossing(DDModel::Edge *e,UpDown ud, DDModel::Node *n) {
	const int dist=2;
	dgassert(gd<DDNode>(n).amEdgePart());
	DDModel::Node *v = ud==UP?e->tail:e->head,
		*w = n;
	dgassert(gd<DDNode>(v).rank==gd<DDNode>(w).rank);
	bool vw = gd<DDNode>(v).order<gd<DDNode>(w).order;
	int i;
	for(i = 0;i<dist;++i) {
		DDModel::Node *vv,*ww;
		if(gd<DDNode>(w).amNodePart())
			break;
		ww = (*w->outs().begin())->head;
		if(v==e->tail)
			vv = e->head;
		else if(gd<DDNode>(v).amNodePart())
			break;
		else
			vv = (*v->outs().begin())->head;
		if(vv==ww) // common end node
			return false;
		dgassert(gd<DDNode>(vv).rank==gd<DDNode>(ww).rank);
		if((gd<DDNode>(vv).order<gd<DDNode>(ww).order) != vw)
			return true;
		v = vv;
		w = ww;
	}
	v = ud==UP?e->tail:e->head;
	w = n;
	for(i = 0;i<dist;++i) {
		DDModel::Node *vv,*ww;
		if(gd<DDNode>(w).amNodePart())
			break;
		ww = (*w->ins().begin())->tail;
		if(v==e->head)
			vv = e->tail;
		else if(gd<DDNode>(v).amNodePart())
			break;
		else
			vv = (*v->ins().begin())->tail;
		if(vv==ww) // common end node
			return false;
		dgassert(gd<DDNode>(vv).rank==gd<DDNode>(ww).rank);
		if((gd<DDNode>(vv).order<gd<DDNode>(ww).order) != vw)
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
		if(gd<DDNode>(q).amNodePart()) {
			if(skipNodeEnds) {
				if(q==gd<DDNode>(q).multi->top() && ud==DOWN && q->ins().empty())
					continue;
				if(q==gd<DDNode>(q).multi->bottom() && ud==UP && q->outs().empty())
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
	double l = gd<DDNode>(n).cur.x - config.LeftExtent(n),
		r = gd<DDNode>(n).cur.x + config.RightExtent(n),
		el = bounding(e,LEFT,start?UP:DOWN,false),
		er = bounding(e,RIGHT,start?UP:DOWN,false);
	double edge = config.ranking.GetRank(gd<DDNode>(n).rank)->yBase;
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
		ty = config.ranking.GetRank(gd<DDNode>(e->tail).rank)->yBase,
		hy = config.ranking.GetRank(gd<DDNode>(e->head).rank)->yBase;
	// cerr << "foo tl " << tl << " tr " << tr << " hl " << hl << " hr " << hr << " ty " << ty << " hy " << hy << endl;
	appendQuad(tl,tr,hl,hr,ty,hy);
}
bool FlexiSpliner::MakeEdgeSpline(DDPath *path,SpliningLevel level) { //,ObstacleAvoiderSpliner<DynaDAGLayout> &obav) {
	Line &unclipped = path->unclippedPath;
	unclipped.Clear();
	DynaDAGLayout::Edge *e = path->layoutE;
	EdgeGeom &eg = gd<EdgeGeom>(e);
	EdgeDirection direction = getEdgeDirection(e);
	if(gd<Suppression>(e).suppression!=Suppression::suppressed) {
		DDModel::Node *tl,*hd;
		if(direction==flat) {
			tl = DDp(e->tail)->bottom();
			hd = DDp(e->head)->top();
		}
		else {
			tl = path->first->tail;
			hd = path->last->head;
		}
		Coord tailpt,
			headpt;
		if(gd<Suppression>(e).suppression==Suppression::tailSuppressed&&direction==forward
			|| gd<Suppression>(e).suppression==Suppression::headSuppressed&&direction==reversed) {
			/*
			DDPath::edge_iter ei;
			for(ei = path->eBegin(); ei!=path->eEnd(); ++ei)
				if(!config.IsSuppressed(*ei)) {
					tailpt = gd<DDNode>((*ei)->tail).cur;
					break;
				}
			dgassert(ei!=path->eEnd());
			*/
			tailpt = checkPos(cutPos(path));
			/*
			double ehei = fabs(gd<DDNode>(cutNode(path)).cur.y - gd<DDNode>(path->last->head).cur.y),
				limhei = gd<GraphGeom>(config.current).separation.y/3;
			dgassert(ehei <= limhei);
			*/
		}
		else
			tailpt = (direction==reversed?eg.tailPort:eg.headPort).pos + gd<DDNode>(tl).multi->pos();
		if(gd<Suppression>(e).suppression==Suppression::headSuppressed&&direction==forward
			|| gd<Suppression>(e).suppression==Suppression::tailSuppressed&&direction==reversed) {
			/*
			DDPath::edge_iter ei;
			for(ei = path->eBegin(); ei!=path->eEnd(); ++ei)
				if(config.IsSuppressed(*ei)) {
					headpt = gd<DDNode>((*ei)->tail).cur;
					break;
				}
			dgassert(ei!=path->eEnd());
			*/
			headpt = checkPos(cutPos(path));
			/*
			double ehei = fabs(gd<DDNode>(cutNode(path)).cur.y - gd<DDNode>(path->first->tail).cur.y),
				limhei = gd<GraphGeom>(config.current).separation.y/3;
			dgassert(ehei <= limhei);
			*/
		}
		else
			headpt = (direction==reversed?eg.headPort:eg.tailPort).pos + gd<DDNode>(hd).multi->pos();
		Line region;
		dgassert(e->tail!=e->head); // DynaDAGServer should draw self-edges
		if(direction==flat) { // flat edge
			/*
			// disabled because of header dependencies (needs more work)
			DDModel::Node *left = tl,
				*right = hd;
			if(gd<DDNode>(left).order>gd<DDNode>(right).order)
				swap(left,right);
			obav.FindSpline(tailpt,headpt,unclipped);
			*/

			// hope that no nodes are in the way (calculating path to avoid 'em will be difficult)
			unclipped.degree = 1;
			unclipped.push_back(tailpt);
			unclipped.push_back(headpt);
		}
		else { // normal edge
			dgassert(path->first); // no flat or self edges!
			RouteBounds rb(config,gd<GraphGeom>(e->g).bounds);
			if(!config.IsSuppressed(tl))
				for(DDMultiNode::edge_iter ei0 = gd<DDNode>(tl).multi->eBegin(); ei0!=gd<DDNode>(tl).multi->eEnd(); ++ei0)
					rb.path(*ei0);
			for(DDPath::edge_iter ei = path->eBegin(); ei!=path->eEnd(); ++ei)
				if(!config.IsSuppressed(*ei))
					rb.path(*ei);
			if(!config.IsSuppressed(hd))
				for(DDMultiNode::edge_iter ei2 = gd<DDNode>(hd).multi->eBegin(); ei2!=gd<DDNode>(hd).multi->eEnd(); ++ei2)
					rb.path(*ei2);
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

						//reports[dgr::error] << "message \"endslopes " << gd<NodeGeom>(e->tail).flowTan << " " << gd<NodeGeom>(e->head).flowTan << '"' << endl;
						Segment endSlopes(
							gd<Suppression>(e).suppression==Suppression::tailSuppressed ? (tailpt - gd<NodeGeom>(e->tail).pos) : gd<NodeGeom>(e->tail).flowTan,
							gd<Suppression>(e).suppression==Suppression::headSuppressed ? (gd<NodeGeom>(e->head).pos - headpt) : gd<NodeGeom>(e->head).flowTan
						);
						if(direction==reversed) {
							Coord t = endSlopes.b;
							endSlopes.b = -endSlopes.a;
							endSlopes.a = -t;
						}
						/*
						if(direction==reversed) {
							endSlopes.a = -endSlopes.a;
							endSlopes.b = -endSlopes.b;
						}
						if(gd<EdgeGeom>(e).backward) {
							endSlopes.a = -endSlopes.a;
							endSlopes.b = -endSlopes.b;
						}
						*/
						dgcheck(PathPlot::Route(barriers,polylineRoute,endSlopes,unclipped));
					}
					else
						unclipped = polylineRoute;
				}
				catch(...) {
					//return false;
				}
				for(DDPath::node_iter ni = path->nBegin(); ni!=path->nEnd(); ++ni) 
					if(config.IsSuppressed(*ni)) 
						gd<DDNode>(*ni).actualXValid = false;
					else {
						double y = gd<DDNode>(*ni).cur.y,
							x = checkPos(unclipped.YIntersection(y)).x;
						gd<DDNode>(*ni).actualX = x;
						gd<DDNode>(*ni).actualXValid = true;
					}
				break;
			}
			default:
				dgassert(0);
			}
		}
	}
	NodeGeom &tg = gd<NodeGeom>(e->tail),
		&hg = gd<NodeGeom>(e->head);
	if(direction==reversed) 
		eg.pos.ClipEndpoints(unclipped,hg.pos,eg.headClipped?&hg.region:0,
			tg.pos,eg.tailClipped?&tg.region:0);
	else
		eg.pos.ClipEndpoints(unclipped,tg.pos,eg.tailClipped?&tg.region:0,
			hg.pos,eg.headClipped?&hg.region:0);
	if((direction==reversed))
		reverse(eg.pos.begin(),eg.pos.end());
	return true;
}

} // namespace DynaDAG
} // namespace Dynagraph


