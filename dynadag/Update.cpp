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

using namespace std;

namespace Dynagraph {
namespace DynaDAG {

// once a translation of dag/order.c

void Config::makeRankList(DDChangeQueue &changeQ) {
	Ranks::IndexV &newRanks = ranking.newRanks,
		&extraRanks = gd<ExtraRanks>(changeQ.whole).extraRanks;
	newRanks.clear();
	for(DynaDAGLayout::node_iter ni = changeQ.current->nodes().begin(); ni!=changeQ.current->nodes().end(); ++ni)
		if(!changeQ.delN.find(*ni)) {
			newRanks.push_back(gd<NSRankerNode>(*ni).newTopRank);
			newRanks.push_back(gd<NSRankerNode>(*ni).newBottomRank);
		}
	newRanks.insert(newRanks.end(),extraRanks.begin(),extraRanks.end());
	sort(newRanks.begin(),newRanks.end());
	Ranks::IndexV::iterator uniquend = unique(newRanks.begin(),newRanks.end());
	newRanks.resize(uniquend-newRanks.begin());
}
double xAvgOfNeighbors(DynaDAGLayout::Node *vn) {
	double sum = 0.0;
	int count = 0;
	for(DynaDAGLayout::nodeedge_iter ei=vn->alledges().begin(); ei!=vn->alledges().end(); ++ei) {
		DynaDAGLayout::Node *oth = ei.target();
		NodeGeom &ng = gd<NodeGeom>(oth);
		if(ng.pos.valid) { // rely on layout's geom (e.g. hang off moved node)
			sum += ng.pos.x;
			count++;
		} // or maybe it was already inserted but pos hasn't made it back yet
		else if(DDMultiNode *mn = DDp(oth)) {
			Position pos = mn->pos();
			if(pos.valid) {
				sum += pos.x;
				count++;
			}
		}
	}
	if(count)
		return sum/count;
	else
		return 0.0;
}
struct constX : XGenerator {
	const double x;
	constX(double x) : x(x) {}
	double xval(double y) { return x; }
};
void Config::insertNode(DynaDAGLayout::Node *n) {
	NodeGeom &ng = gd<NodeGeom>(n);
	DDMultiNode *mn = dynaDAG->OpenModelNode(n).first;
	double x=0.0; // init for gcc
	bool haveX;
	if(ng.pos.valid) {
		x = ng.pos.x,haveX = true;
		//n->coordFixed = true;
	}
	else {
		if(haveX = n->degree()!=0)
			x = xAvgOfNeighbors(current->find(n));
		//n->coordFixed = false;
	}
	if(gd<NSRankerNode>(n).newTopRank!=gd<NSRankerNode>(n).newBottomRank) {
		DDModel::Node *top = mn->node,
			*bottom = dynaDAG->OpenModelNode(n).second;
		if(haveX)
			InstallAtPos(top,gd<NSRankerNode>(n).newTopRank,x);
		else
			InstallAtRight(top,gd<NSRankerNode>(n).newTopRank);
		InstallAtPos(bottom,gd<NSRankerNode>(n).newBottomRank,gd<DDNode>(top).cur.x);
		constX cx(gd<DDNode>(top).cur.x);
		buildChain(mn,top,bottom,&cx,n,0);
		mn->node = 0;
	}
	else
		if(haveX)
			InstallAtPos(mn->node,gd<NSRankerNode>(n).newTopRank,x);
		else
			InstallAtRight(mn->node,gd<NSRankerNode>(n).newTopRank);
}
void Config::insertNewNodes(DDChangeQueue &changeQ) {
	for(DynaDAGLayout::node_iter ni = changeQ.insN.nodes().begin(); ni!=changeQ.insN.nodes().end(); ++ni)
		insertNode(whole->find(*ni));
}
Coord interpolate(Coord p0, Coord p1, double t) {
	return p0 + (p1-p0)*t;
}

/* returns model nodes of layout edge, ordered with tail=low rank */
bool getLayoutEndpoints(DynaDAGLayout::Edge *ve, DDModel::Node **p_tl, DDModel::Node **p_hd) {
	DDMultiNode *tail = DDp(ve->tail),
		*head = DDp(ve->head);
	switch(getEdgeDirection(ve)) {
	case forward:
		*p_tl = tail->bottom(),
		*p_hd = head->top();
		return true;
	case reversed:
		*p_tl = head->bottom();
		*p_hd = tail->top();
		return true;
	default:
		dgassert(false);
	case flat:
		*p_tl = *p_hd = 0;
		return false;
	}
	/*
	bool ret = true;
	DDModel::Node *t = tail->bottom(),
		*h = head->top();
	if(gd<DDNode>(h).rank < gd<DDNode>(t).rank) {
		DDModel::Node *rt = head->bottom(),
			*rh = tail->top();
		if(gd<DDNode>(rh).rank < gd<DDNode>(rt).rank) {
			dgassert(gd<DDNode>(t).rank==gd<DDNode>(rt).rank && gd<DDNode>(h).rank==gd<DDNode>(rh).rank); // make sure it's flat
			ret = false;
		}
		t = rt;
		h = rh;
	}
	*p_tl = t; *p_hd = h;
	return ret;
	*/
}
void Config::buildChain(DDChain *chain, DDModel::Node *t, DDModel::Node *h, XGenerator *xgen,DynaDAGLayout::Node *vn,DynaDAGLayout::Edge *ve) {
	dynaDAG->CloseChain(chain,false);
	if(t==h)
		return;
	int tr = gd<DDNode>(t).rank,
		hr = gd<DDNode>(h).rank;
	if(tr==hr)
		return;
	dgassert(Ranks::Xlator::Above(whole,tr,hr));
	Ranks::iterator ti = ranking.GetIter(tr),
		hi = ranking.GetIter(hr),
		ri = ti;
	chain->first = chain->last = 0;
	if(++ri!=hi) {
		DDModel::Node *prev = h;
		for((ri = hi)--; ri!=ti; ri--) {
			Ranks::index i = ranking.IndexOfIter(ri);
			DDModel::Node *mn = dynaDAG->OpenModelNode(vn).second;
			InstallAtPos(mn,i,xgen->xval((*ri)->yBase));
			DDModel::Edge *me = dynaDAG->OpenModelEdge(mn,prev,ve).second;
			if(!chain->last)
				chain->last = me;
			prev = mn;
		}
		chain->first = dynaDAG->OpenModelEdge(t,prev,ve).second;
		dgassert(chain->last);
	}
	else chain->first = chain->last = dynaDAG->OpenModelEdge(t,h,ve).second;
}
struct autoX : XGenerator {
	Coord tc,hc;
	autoX(Coord tc,Coord hc) : tc(tc),hc(hc) {
	  dgassert(tc.y!=hc.y);
	}
	double xval(double y) {
		return interpolate(tc,hc,(tc.y-y)/(tc.y-hc.y)).x;
	}
};
struct userX : autoX {
	Line &pos;
	userX(Coord tc,Coord hc,Line &pos) : autoX(tc,hc),pos(pos) {}
	double xval(double y) {
		Position yint = pos.YIntersection(y);
		if(yint.valid)
			return yint.x;
		else
			return autoX::xval(y);
	}
};
void Config::userRouteEdge(DDPath *path) {
	DDModel::Node *t, *h;
	if(!getLayoutEndpoints(path->layoutE,&t,&h)) { // flat
		dynaDAG->CloseChain(path,false); 
		return;
	}
	userX xgen(gd<DDNode>(t).multi->pos(),gd<DDNode>(h).multi->pos(),gd<EdgeGeom>(path->layoutE).pos);
	buildChain(path,t,h,&xgen,0,path->layoutE);
}

void Config::autoRouteEdge(DDPath *path) {
	DDModel::Node *t, *h;
	if(!getLayoutEndpoints(path->layoutE,&t,&h))
		dynaDAG->CloseChain(path,false); // flat
	else {
		Position tp = gd<DDNode>(t).multi->pos(),
		  hp = gd<DDNode>(h).multi->pos();
		dgassert(tp.valid && hp.valid);
		autoX xgen(tp,hp);
		buildChain(path,t,h,&xgen,0,path->layoutE);
	}
}
void Config::adjustChain(DDChain *chain, bool tail,Ranks::index dest,DynaDAGLayout::Node *vn,DynaDAGLayout::Edge *ve) {
	DDModel::Node *endpoint,*v;
	if(tail) {
		endpoint = chain->first->tail;
		v = chain->first->head;
	}
	else {
		endpoint = chain->last->head;
		v = chain->last->tail;
	}
	// how to get end rank from penultimate rank
	Ranks::index (Ranks::*Out)(Ranks::index) = tail?&Ranks::Up:&Ranks::Down;
	Ranks::index start = (ranking.*Out)(gd<DDNode>(v).rank);
	if(start==dest)
		return;
	//xconOwner->RemoveNodeConstraints(v); // so that no RemoveNodeConstraints happen while chain is broken
	DDModel::Edge *&endEdge = tail?chain->first:chain->last,
		*&beginEdge = tail?chain->last:chain->first;
	dynaDAG->CloseModelEdge(endEdge);
	if(beginEdge == endEdge)
		beginEdge = endEdge = 0;
	else
		endEdge = 0;
	bool (*Pred)(DynaDAGLayout*,Ranks::index,Ranks::index) = tail?&Ranks::Xlator::Below:&Ranks::Xlator::Above;
	if((*Pred)(whole,start,dest)) // stretch
		while((*Pred)(whole,(ranking.*Out)(gd<DDNode>(v).rank),dest)) {
			DDModel::Node *nv = dynaDAG->OpenModelNode(vn).second;
			if(vn) // multinodes have single X
				InstallAtPos(nv,(ranking.*Out)(gd<DDNode>(v).rank),gd<DDNode>(v).cur.x);
			else
				percolate(nv,v,(ranking.*Out)(gd<DDNode>(v).rank));
			DDModel::Edge *e = tail?
				dynaDAG->OpenModelEdge(nv,v,ve).second:
				dynaDAG->OpenModelEdge(v,nv,ve).second;
			if(!beginEdge)
				beginEdge = e;
			v = nv;
		}
	else // shrink
		while((*Pred)(whole,dest,(ranking.*Out)(gd<DDNode>(v).rank))) {
			DDModel::Node *nv = tail?
				(*v->outs().begin())->head:
				(*v->ins().begin())->tail;
			if(gd<DDNode>(v).multi)
				dgassert(gd<DDNode>(v).multi==chain);
			for(DDModel::nodeedge_iter ei=v->alledges().begin(); ei!=v->alledges().end();) {
				DDModel::Edge *del = *ei++;
				if(gd<DDEdge>(del).path)
					dgassert(gd<DDEdge>(del).path==chain);
				dynaDAG->CloseModelEdge(del);
				if(del==beginEdge)
					beginEdge = 0;
			}
			dynaDAG->CloseModelNode(v);
			v = nv;
		}

	endEdge = tail?
		dynaDAG->OpenModelEdge(endpoint,v,ve).second:
		dynaDAG->OpenModelEdge(v,endpoint,ve).second;
	if(!beginEdge)
		beginEdge = endEdge;
}
void Config::rerouteChain(DDChain *chain,int tailRank,int headRank,XGenerator *xgen) {
	int r = tailRank;
	for(DDPath::node_iter ni = chain->nBegin(); ni!=chain->nEnd(); ++ni, r = ranking.Down(r)) {
		RemoveNode(*ni);
		if(Ranks::Xlator::Below(whole,r,headRank))
			gd<DDNode>(*ni).rank = r; // don't install if it's not getting used (might be off bottom of config even)
		else
			InstallAtPos(*ni,r,xgen->xval(ranking.GetRank(r)->yBase));
	}
}
/* Adjust a virtual node chain, by shrinking, moving, and/or stretching
 * the path.  The gd<DDNode>().newRank values determine the new endpoints.
 * Shrinking means cutting off vnodes on the tail side.  Stretching
 * means iteratively copying the last virtual node down to the next rank.
 * The rationale is that the tail moves downward toward the head.
 */
void Config::autoAdjustChain(DDChain *chain,int otr,int ohr,int ntr,int nhr,DynaDAGLayout::Node *vn,DynaDAGLayout::Edge *ve) {
	if(nhr == ntr)
		dynaDAG->CloseChain(chain,false);	/* flat edge / single node */
	else {
		if(!chain->first) {
			cerr << "jeepers!  it's the bug!" << endl;
			cerr << "otr " << otr << " ohr " << ohr << " ntr " << ntr << " nhr " << nhr << endl;
			if(ve) {
				DDPath *path = DDp(ve);
				cerr << "it's an edge with" << endl;
				cerr << "tail " << gd<Name>(ve->tail) << " head " << gd<Name>(ve->head) << endl;
				cerr << "secondOfTwo " << gd<NSRankerEdge>(ve).secondOfTwo << endl;
				if(path) cerr << "first " << path->first << " last " << path->last << endl;
				else cerr << "no path" << endl;
				cerr << "direction " << getEdgeDirection(ve) << endl;
				cerr << "suppression " << gd<Suppression>(ve).suppression << endl;
				if(DynaDAGLayout::Edge *other = whole->find_edge(ve->head,ve->tail)) {
					DDPath *path = DDp(other);
					cerr << "it has a twin with" << endl;
					cerr << "tail " << gd<Name>(other->tail) << " head " << gd<Name>(other->head) << endl;
					cerr << "secondOfTwo " << gd<NSRankerEdge>(other).secondOfTwo << endl;
					if(path) cerr << "first " << path->first << " last " << path->last << endl;
					else cerr << "no path" << endl;
					cerr << "direction " << getEdgeDirection(other) << endl;
					cerr << "suppression " << gd<Suppression>(other).suppression << endl;
				}
			}
			else if(vn)
				cerr << "it's a node" << endl;
		}
		dgassert(chain->first);
		if(!(Ranks::Xlator::Above(whole,otr,nhr)&&Ranks::Xlator::Above(whole,ntr,ohr))
			|| ve && gd<EdgeGeom>(ve).pos.Empty()) {
			if(vn) {
				constX cx(gd<NodeGeom>(vn).pos.x);
				rerouteChain(chain,ntr,nhr,&cx);
			}
			else {
				autoX ax(DDp(ve->tail)->pos(),DDp(ve->head)->pos());
				rerouteChain(chain,ntr,nhr,&ax);
			}
		}
		// stretch/shrink ends
		adjustChain(chain,false,nhr,vn,ve);
		adjustChain(chain,true,ntr,vn,ve);
	}
}
void Config::autoAdjustEdge(DDPath *path) {
	DDModel::Node *t, *h;
	if(!getLayoutEndpoints(path->layoutE,&t,&h)) { // flat
		dynaDAG->CloseChain(path,false); 
		return;
	}
	dgassert(gd<DDNode>(t).amNodePart()&&gd<DDNode>(h).amNodePart());
	// not the same as ve->tail,ve->head if reversed
	DynaDAGLayout::Node *tn = gd<DDNode>(t).multi->layoutN,
		*hn = gd<DDNode>(h).multi->layoutN;
	int otr = gd<NSRankerNode>(tn).oldBottomRank, ntr = gd<NSRankerNode>(tn).newBottomRank,
		ohr = gd<NSRankerNode>(hn).oldTopRank, nhr = gd<NSRankerNode>(hn).newTopRank;
	if(otr >= ohr != ntr >= nhr) // moving into or out of backwardness
		autoRouteEdge(path);
	else autoAdjustChain(path,otr,ohr,ntr,nhr,0,path->layoutE);

	// invalidate edge cost constraints so they get reconnected right.
	// xconOwner->InvalidatePathConstraints(DDp(ve));
}
/*
void unbindEndpoints(DynaDAGLayout::Edge *ve) {
	DDp(ve->tail)->coordFixed = false;
	DDp(ve->head)->coordFixed = false;
}
*/
void Config::insertEdge(DynaDAGLayout::Edge *ve) {
	DDPath *path = dynaDAG->OpenModelEdge(0,0,ve).first;
	if(ve->head==ve->tail || gd<NSRankerEdge>(ve).secondOfTwo)
		dynaDAG->CloseChain(path,false); // do not model self-edges or 2-cycle second
	else if(userDefinedMove(ve))
		userRouteEdge(path);
	else
		autoRouteEdge(path);
	/*unbindEndpoints(ve); */ 	/* i don't know if this is good or bad */
}
void Config::unfixOldSingletons(DDChangeQueue &changeQ) {
	/* this heuristic unfixes any node on first edge insertion */
	for(DynaDAGLayout::node_iter ni = changeQ.insE.nodes().begin(); ni!=changeQ.insE.nodes().end(); ++ni) {
		/* only unstick nodes if they changed ranks */
		DDModel::Node *mn = DDp(*ni)->top();
		if(!gd<DDNode>(mn).prev.valid)
			continue;
		if(gd<DDNode>(mn).rank == gd<NSRankerNode>(*ni).oldTopRank)
			continue;
		DynaDAGLayout::Node *vn = current->find(*ni);
		bool anyOldEdges = false;
		for(DynaDAGLayout::nodeedge_iter ei=vn->alledges().begin(); ei!=vn->alledges().end(); ++ei)
			if(!changeQ.insE.find(*ei)) {
				anyOldEdges = true;
				break;
			}
        /*
		if(!anyOldEdges)
			DDp(*ni)->coordFixed = false;
        */
	}
}

void Config::insertNewEdges(DDChangeQueue &changeQ) {
	for(DynaDAGLayout::graphedge_iter ei = changeQ.insE.edges().begin(); ei!=changeQ.insE.edges().end(); ++ei)
		insertEdge(whole->find(*ei));
	unfixOldSingletons(changeQ);
}
/* push a node through adjacent ranks.  */

void Config::percolate(DDModel::Node *n,DDModel::Node *ref,Ranks::index destrank) {
	Ranks::index r = gd<DDNode>(ref).rank;
	bool down = Ranks::Xlator::Above(whole,r,destrank);
	double x = gd<DDNode>(ref).cur.x;
	if(down)
		for(r = ranking.Down(r); !Ranks::Xlator::Below(whole,r,destrank); r = ranking.Down(r))
			x = placeAndReopt(n,r,x);
	else
		for(r = ranking.Up(r); !Ranks::Xlator::Above(whole,r,destrank); r = ranking.Up(r))
			x = placeAndReopt(n,r,x);
}
double Config::placeAndReopt(DDModel::Node *n, Ranks::index r, double x) {
	int oldRank = gd<DDNode>(n).rank;
	if(gd<DDNode>(n).inConfig)
		RemoveNode(n);
	InstallAtPos(n,r,x);
	//dir = (oldRank < r)? UP : DOWN;
	UpDown dir = (oldRank < r)? DOWN : UP;
	return dynaDAG->GetOptimizer()->Reopt(n,dir);
}
void Config::reattachEdgeTails(DDModel::Node *source,DDModel::Node *dest) {
	for(DDModel::outedge_iter ei = source->outs().begin(); ei!=source->outs().end();) {
		DDModel::Edge *e = *ei++;
		DDPath *path = gd<DDEdge>(e).path;
		DDModel::Edge *newe = dynaDAG->OpenModelEdge(dest,e->head,path->layoutE).second;
		if(path->first==path->last)
			path->last = newe;
		path->first = newe;
		dynaDAG->CloseModelEdge(e);
	}
}
void Config::moveOldNode(DynaDAGLayout::Node *vn) {
	DynaDAGLayout::Node *mvn = whole->find(vn);
	NodeGeom &ng = gd<NodeGeom>(vn);
	DDMultiNode *n = DDp(vn);
	NSRankerNode &nsn = gd<NSRankerNode>(vn);
	if(nsn.newTopRank!=nsn.oldTopRank || nsn.newBottomRank!=nsn.oldBottomRank) {
		double x;
		DDMultiNode::node_iter ni;
		// move all nodes to either specified X or percolated X
		if(igd<Dynagraph::Update>(vn).flags & DG_UPD_MOVE && ng.pos.valid ||
				gd<NodeGeom>(vn).nail & DG_NAIL_X) {
			if(!ng.pos.valid)
				throw NailWithoutPos<DynaDAGLayout>(vn);
			x = ng.pos.x;
			//n->coordFixed = true;
			ni = n->nBegin();
		}
		else {
			percolate(n->top(),n->top(),nsn.newTopRank);
			x = gd<DDNode>(n->top()).cur.x;
			(ni = n->nBegin())++;
			//n->coordFixed = false;
		}
		for(; ni!=n->nEnd(); ++ni) {
			int r = gd<DDNode>(*ni).rank;
			RemoveNode(*ni);
			InstallAtPos(*ni,r,x);
		}
		if(!n->first) {
			if(nsn.newTopRank!=nsn.newBottomRank) { // 1-node is becoming a chain
				RemoveNode(n->node);
				InstallAtPos(n->node,nsn.newTopRank,x);
				DDModel::Node *bottom = dynaDAG->OpenModelNode(mvn).second;
				InstallAtPos(bottom,nsn.newBottomRank,x);
				reattachEdgeTails(n->node,bottom);
				constX cx(x);
				buildChain(n,n->node,bottom,&cx,mvn,0);
				n->node = 0;
			}
			else {
				RemoveNode(n->node);
				InstallAtPos(n->node,nsn.newTopRank,x);
			}
		}
		else { // already a chain
			DDModel::Node *top = n->top(),
				*bottom = n->bottom();
			RemoveNode(top);
			InstallAtPos(top,nsn.newTopRank,gd<DDNode>(top).cur.x);
			RemoveNode(bottom);
			if(nsn.newTopRank==nsn.newBottomRank) { // chain becoming 1-node
				reattachEdgeTails(bottom,top);
				dynaDAG->CloseChain(n,false);
				dynaDAG->CloseModelNode(bottom);
				n->node = top;
			}
			else {
				InstallAtPos(n->bottom(),nsn.newBottomRank,gd<DDNode>(n->bottom()).cur.x);
				// stretch/shrink chain
				autoAdjustChain(n,nsn.oldTopRank,nsn.oldBottomRank,nsn.newTopRank,nsn.newBottomRank,mvn,0);
			}
		}
	}
	else { // only x has changed
		if(ng.pos.valid) {
			for(DDMultiNode::node_iter ni = n->nBegin(); ni!=n->nEnd(); ++ni) {
				DDModel::Node *mn = *ni,
					*left = Left(mn),
					*right = Right(mn);
				if((left && (gd<DDNode>(left).cur.x > ng.pos.x)) ||
					(right && (gd<DDNode>(right).cur.x < ng.pos.x))) {
					int r = gd<DDNode>(mn).rank;
					RemoveNode(mn);
					InstallAtPos(mn,r,ng.pos.x);
				}
				else
					gd<DDNode>(mn).cur.x = ng.pos.x;
			}
			//n->coordFixed = true;
		}
		//else n->coordFixed = false;
	}
}
struct compOldRank {
	bool operator()(DynaDAGLayout::Node *n1,DynaDAGLayout::Node *n2) {
		return gd<NSRankerNode>(n1).oldTopRank < gd<NSRankerNode>(n2).oldTopRank;
	}
};
void Config::moveOldNodes(DDChangeQueue &changeQ) {
	LNodeV moveOrder;
	for(DynaDAGLayout::node_iter vni = changeQ.modN.nodes().begin(); vni!=changeQ.modN.nodes().end(); ++vni)
		if(igd<Dynagraph::Update>(*vni).flags&DG_UPD_MOVE)
			moveOrder.push_back(*vni);
	sort(moveOrder.begin(),moveOrder.end(),compOldRank());
	for(LNodeV::iterator ni = moveOrder.begin(); ni != moveOrder.end(); ++ni) 
		moveOldNode(*ni);
}
void Config::moveOldEdges(DDChangeQueue &changeQ) {
	for(DynaDAGLayout::graphedge_iter ei = changeQ.modE.edges().begin(); ei!=changeQ.modE.edges().end(); ++ei)
		if(igd<Dynagraph::Update>(*ei).flags&DG_UPD_MOVE) { 
			DynaDAGLayout::Edge *ve = whole->find(*ei);
			if((*ei)->head==(*ei)->tail)
				; // ignore self-edges
			else if(gd<NSRankerEdge>(*ei).secondOfTwo)
				; // ignore one edge of 2-cycle
			else if(userDefinedMove(*ei))
				userRouteEdge(DDp(ve));
			else
				autoAdjustEdge(DDp(ve));
		}
}
void Config::splitRank(DDChain *chain,DDModel::Edge *e,DynaDAGLayout::Node *vn, DynaDAGLayout::Edge *ve) {
	Ranks::index newR = ranking.Down(gd<DDNode>(e->tail).rank);
	if(newR==gd<DDNode>(e->head).rank)
		return; // already there
	dgassert(Ranks::Xlator::Above(whole,newR,gd<DDNode>(e->head).rank));
	reports[dgr::ranks] << 
		(vn?"multinode ":"path ") << chain << ": chain split at " <<
		gd<DDNode>(e->tail).rank << "->" << gd<DDNode>(e->head).rank << ':' << endl;
	DDModel::Node *v = dynaDAG->OpenModelNode(vn).second;
	double x = (gd<DDNode>(e->tail).cur.x+gd<DDNode>(e->head).cur.x)/2.0; // roughly interpolate so as not to introduce crossings
	InstallAtPos(v,newR,x);
	DDModel::Edge *newE1 = dynaDAG->OpenModelEdge(e->tail,v,ve).second,
		*newE2 = dynaDAG->OpenModelEdge(v,e->head,ve).second;
	if(chain->first==e)
		chain->first = newE1;
	if(chain->last==e)
		chain->last = newE2;
	dgassert(chain->first && chain->last);
	dynaDAG->CloseModelEdge(e);
	Ranks::index ur = gd<DDNode>(newE1->tail).rank,
		vr = gd<DDNode>(newE1->head).rank,
		wr = gd<DDNode>(newE2->head).rank;
	reports[dgr::ranks] << "now " << ur << "->" << vr << "->" << wr << endl;
}
void Config::joinRanks(DDChain *chain,DDModel::Node *n,DynaDAGLayout::Edge *ve) {
	dgassert(n->ins().size()==1);
	dgassert(n->outs().size()==1);
	DDModel::Edge *e1 = *n->ins().begin(),
		*e2 = *n->outs().begin(),
		*newE = dynaDAG->OpenModelEdge(e1->tail,e2->head,ve).second;
	reports[dgr::ranks] << (ve?"path":"multinode") << ' ' << chain << ": chain joined at " <<
		gd<DDNode>(e1->tail).rank << "->" << gd<DDNode>(n).rank << "->" << gd<DDNode>(e2->head).rank << endl;
	// make sure this is in the middle of the specified chain
	if(gd<DDNode>(n).amNodePart())
		dgassert(gd<DDEdge>(e1).amNodePart() && gd<DDEdge>(e2).amNodePart() && gd<DDNode>(n).multi==chain);
	else
		dgassert(gd<DDEdge>(e1).amEdgePart() && gd<DDEdge>(e1).path==chain && gd<DDEdge>(e1).path==gd<DDEdge>(e2).path);
	if(chain->first==e1)
		chain->first = newE;
	if(chain->last==e2)
		chain->last = newE;
	dgassert(chain->first && chain->last);
	dynaDAG->CloseModelEdge(e1);
	dynaDAG->CloseModelEdge(e2);
	dynaDAG->CloseModelNode(n);
	reports[dgr::ranks] << "now " << gd<DDNode>(newE->tail).rank << "->" << gd<DDNode>(newE->head).rank << endl;
}
void Config::updateRanks(DDChangeQueue &changeQ) {
	// everything old has already been moved around, so we don't have to
	// deal with ends of chains
	ranking.newRanks.push_back(INT_MAX); // simple way to deal with endgame
	if(ranking.oldRanks.empty())
		ranking.oldRanks.push_back(INT_MAX);
	Ranks::IndexV::iterator ni=ranking.newRanks.begin(),
		oi = ranking.oldRanks.begin();
	if(reports.enabled(dgr::ranks)) {
		int dr = ranking.newRanks.size()-ranking.oldRanks.size();
		reports[dgr::ranks] << "update config: " << ranking.newRanks.size() << " ranks (" << (dr<0?"closing ":"creating ") << abs(dr) << ')' << endl;
		/*
		reports[dgr::ranks] << "old ranks: ";
		for(Ranks::IndexV::iterator test = ranking.oldRanks.begin(); test!=ranking.oldRanks.end(); ++test)
			reports[dgr::ranks] << *test << ' ';
		reports[dgr::ranks] << endl << "new ranks: ");
		for(test = ranking.newRanks.begin(); test!=ranking.newRanks.end(); ++test)
			reports[dgr::ranks] << *test << ' ';
		reports[dgr::ranks] << endl;
		*/
	}
	while(*ni!=INT_MAX || *oi!=INT_MAX) {
		while(*ni==*oi && *ni!=INT_MAX) {
			++ni;
			++oi;
		}
		while(Ranks::Xlator::Above(whole,*ni,*oi)) { // additions
			reports[dgr::ranks] << "adding " << *ni << endl;
			Ranks::iterator ri = ranking.EnsureRank(*ni,gd<GraphGeom>(changeQ.current).separation.y);
			if(ri!=ranking.begin()) {
				Ranks::iterator ri2 = ri;
				ri2--;
				for(NodeV::iterator ni = (*ri2)->order.begin(); ni!=(*ri2)->order.end(); ++ni)
					for(DDModel::outedge_iter ei = (*ni)->outs().begin(); ei!=(*ni)->outs().end();) {
						DDModel::Edge *e = *ei++;
						if(gd<DDEdge>(e).amEdgePart())
							splitRank(gd<DDEdge>(e).path,e,0,gd<DDEdge>(e).path->layoutE);
						else if(gd<DDNode>(*ni).amNodePart() && gd<DDNode>(*ni).multi==gd<DDNode>(e->head).multi)
							splitRank(gd<DDNode>(*ni).multi,e,gd<DDNode>(*ni).multi->layoutN,0);
						else dgassert(false); // what's this edge doing?
					}
			}
			++ni;
		}
		while(Ranks::Xlator::Above(whole,*oi,*ni)) { // deletions
			reports[dgr::ranks] << "removing " << *oi << endl;
			Ranks::iterator ri = ranking.GetIter(*oi);
			dgassert(ri!=ranking.end());
			while((*ri)->order.size()) {
				DDModel::Node *n = (*ri)->order.back();
				if(gd<DDNode>(n).amEdgePart()) {
					DDPath *path = gd<DDEdge>(*n->ins().begin()).path;
					joinRanks(path,n,path->layoutE);
				}
				else
					joinRanks(gd<DDNode>(n).multi,n,0);
			}
			ranking.RemoveRank(ri);
			++oi;
		}
	}
	ranking.oldRanks = ranking.newRanks;
}
void Config::Update(DDChangeQueue &changeQ) {
	makeRankList(changeQ);

	moveOldNodes(changeQ);
	moveOldEdges(changeQ);
	checkEdges(false);
	checkNodeRanks(changeQ,false);

	updateRanks(changeQ);
	ranking.Check();
	checkEdges(true);

	insertNewNodes(changeQ);
	insertNewEdges(changeQ);
	checkEdges(true);
	checkNodeRanks(changeQ,true);
	checkX();
}

} // namespace DynaDAG
} // namespace Dynagraph
