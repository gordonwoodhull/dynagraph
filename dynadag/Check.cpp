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


// Sanity checks.

#include "DynaDAG.h"

using namespace std;

namespace Dynagraph {
namespace DynaDAG {

void FlexiRanks::Check() {
	iterator ri = begin(),
		next;
	if(ri==end())
		return;
	(next=ri)++;
	while(next!=end()) {
		index a = IndexOfIter(ri), b = IndexOfIter(next);
		dgassert(Xlator::Above(layout_,a,b));
		ri = next++;
	}
}
void Config::checkEdges(bool strict) {
	for(DDModel::graphedge_iter ei = model.edges().begin(); ei!=model.edges().end(); ++ei) {
		DDModel::Node *t = (*ei)->tail,
			*h = (*ei)->head;
		// edges must be path parts or node parts; edges must belong to one node only
		if(gd<DDEdge>(*ei).amEdgePart())
			;
		else {
			dgassert(gd<DDNode>(t).amNodePart());
			dgassert(gd<DDNode>(t).multi==gd<DDNode>(h).multi);
		}
		Ranks::index tr = gd<DDNode>(t).rank,
			hr = gd<DDNode>(h).rank;
		if(strict) 
			dgassert(ranking.Down(tr)==hr); // edges must span exactly one rank
		else
			dgassert(Ranks::Xlator::Above(whole,tr,hr)); // edges must point "downward"
	}
	// nodes in paths must belong to one path only
	for(DDModel::node_iter ni = model.nodes().begin(); ni!=model.nodes().end(); ++ni) {
		DDModel::Node *n = *ni;
		if(gd<DDNode>(n).amEdgePart()) {
			dgassert(n->ins().size()==1);
			dgassert(n->outs().size()==1);
			DDModel::Edge *e1 = *n->ins().begin(),
				*e2 = *n->outs().begin();
			dgassert(gd<DDEdge>(e1).path==gd<DDEdge>(e2).path);
		}
	}
	// view edges' paths connect the tops & bottoms of nodes
	for(DynaDAGLayout::graphedge_iter ei2 = current->edges().begin(); ei2!=current->edges().end(); ++ei2) {
		DynaDAGLayout::Edge *e = *ei2;
		DDPath *path = DDp(e);
		if(!path)
			continue;
		if(!path->first)
			continue;
		DDMultiNode *tn = DDp(e->tail),
			*hn = DDp(e->head);
		switch(getEdgeDirection(e)) {
			case forward:
				dgassert(path->first->tail==tn->bottom());
				dgassert(path->last->head==hn->top());
				break;
			case reversed:
				dgassert(path->first->tail==hn->bottom());
				dgassert(path->last->head==tn->top());
				break;
			case flat:
				dgassert(false); // first and last should be null
		}
	}
}
void Config::checkNodeRanks(DDChangeQueue &Q, bool news) {
	for(DynaDAGLayout::node_iter ni = current->nodes().begin(); ni!=current->nodes().end(); ++ni)
		if(news||!Q.insN.find(*ni)) {
			DynaDAGLayout::Node *n = *ni;
			dgassert(gd<DDNode>(DDp(n)->top()).rank==gd<NSRankerNode>(n).newTopRank);
			dgassert(gd<DDNode>(DDp(n)->bottom()).rank==gd<NSRankerNode>(n).newBottomRank);
		}
}
void Config::checkX() {
	for(Ranks::iterator ri = ranking.begin(); ri!=ranking.end(); ++ri) {
		Rank *r = *ri;
		for(NodeV::iterator ni = r->order.begin(); ni!=r->order.end(); ++ni)
			if(ni!=r->order.begin()) {
				double cx = gd<DDNode>(*ni).cur.x,
					px = gd<DDNode>(*(ni-1)).cur.x;
				dgassert(cx>=px);
			}
	}
}
void XSolver::checkLRConstraints() {
	bool missing = false;
	for(Config::Ranks::iterator ri = config.ranking.begin(); ri!=config.ranking.end(); ++ri) {
		Rank *r = *ri;
		for(NodeV::iterator ni = r->order.begin(); ni!=r->order.end(); ++ni)
			if(DDModel::Node *left = config.Left(*ni)) {
				UniqConstraintGraph::Node *l = gd<DDNode>(left).getXcon().n,
					*n = gd<DDNode>(*ni).getXcon().n;
				dgassert(l&&n);
				UniqConstraintGraph::Edge *e = cg.find_edge(l,n);
				if(!e) {
					reports[dgr::error] << 
						left << " (" << type(left) << ' ' << thing(left) << ") and " << *ni << " (" << type(*ni) << ' ' << thing(*ni) << ')' << endl;
					missing = true;
				}
				else
					dgassert(UniqNS::NSd(e).minlen >= ROUND(config.UVSep(left,*ni)/gd<GraphGeom>(config.current).resolution.x));
				/*
				// (hopeless)
				// don't allow extraneous constraints: only edge,stab, and L-R are good
				for(UniqConstraintGraph::edge_iter ei = n->ins().begin(); ei!=n->ins().end(); ++ei) {
					UniqConstraintGraph::Edge *e2 = *ei;
					dgassert(e2==e ||
						e2->tail == gd<DDNode>(*ni).getXcon().stab ||
						gd<ConstraintType>(e2->tail).why==ConstraintType::orderEdgeStraighten);
				}
				*/
			}
	}
	dgassert(!missing);
}
void XSolver::checkEdgeConstraints() {
	for(DDModel::graphedge_iter ei = config.model.edges().begin(); ei!=config.model.edges().end(); ++ei)
		if(gd<DDEdge>(*ei).amEdgePart()) {
			UniqConstraintGraph::Node *cn = gd<DDEdge>(*ei).cn;
			dgassert(cn);
			dgassert(cn->ins().size()==0);
			if(cn->outs().size()!=2) {
				reports[dgr::error] << "AARGH!  Why isn't node " << (*ei)->tail << " of " << 
					(gd<DDNode>((*ei)->tail).amEdgePart()?"path":"multinode") << ' ' << thing((*ei)->tail) << " constrained with" << endl;
				reports[dgr::error] << "node %p of %s %p????\n" << (*ei)->head << " of " <<
					(gd<DDNode>((*ei)->head).amEdgePart()?"path":"multinode") << ' ' << thing((*ei)->head) << "???" << endl;
				throw BadXConstraints();
			}
		}
}
/*
void DynaDAG::checkAll(ddview_t *view) {
	config.CheckRanks();
	int		r;

	for(r = view->config->low; r <= view->config->high; r++)
		dd_check_rank(view,r);
	dd_check_edges(view->layout);
}
void Config::CheckRanks() {
	for(int i = low; i<high; ++i) {
		Rank *r = GetRank(i);
		r->check(i);
	}
	for(RankV::iterator ri = ranking.begin(); ri!=ranking.end(); ++ri)
		(*ri)->check();
}
void Rank::check(int r) {
	Agnode_t	*ln,*rn,**list;
	int			i;
	rank_t		*rd;

	DDModel::Node *ln=0;
	for(NodeV::iterator ni = order.begin(); ni!=order.end(); ++ni) {
		dgassert(gd<DDNode>(*ni).inConfig);
		dgassert(gd<DDNode>(*ni).rank == r);
		dd_check_elts(*ni);
		if(ln) {
			dgassert(gd<DDNode>(ln).order + 1 == gd<DDNode>(*ni).order);
			dgassert(gd<DDNode>(ln).cur.x + BASE(view)->whole->separation.x <= dd_pos(rn).x);
		}
		ln = rn;
	}
	dgassert (i == rd->n);
}

void dd_check_containment(ddview_t *view, int r, Agnode_t *n, int must_be_in)
{
	Agnode_t	*rn;

	for(rn = dd_leftmost(view,r); rn; rn = dd_right(view,rn)) {
		if(must_be_in) { if(n == rn) break; }
		else dgassert (n != rn);
	}
	if(must_be_in) dgassert (n == rn);
}

ilbool dd_check_pathnode(ddview_t *view, Agnode_t *n)
{
	rank_t		*rd;
	int			i,r;

	i = dd_order(n);
	r = dd_rank(n);
	rd = dd_rankd(view,r);
	dgassert(rd->v[i] == n);
	return FALSE;
}

void dd_check_vnode_path(ddview_t *view, Agedge_t **vpath)
{
	int			i;
	Agedge_t	*e,*f;

	f = NILedge;
	for(i = 0; (e = vpath[i]); i++) {
		dd_check_pathnode(view,agtail(e));
		if(i > 0) dgassert(dd_is_a_vnode(agtail(e)));
		f = e;
	}
	dd_check_pathnode(view,aghead(f));
}

void dd_check_elts(ddview_t *view, Agnode_t *n)
{
	Agedge_t	*e,*f,*fst,*lst;

	if(dd_is_a_vnode(n)) return;
	for(e = agfstout(n); e; e = agnxtout(e)) {
		fst = dd_first_elt(e);
		lst = dd_last_elt(e);

		for(f = fst; f; f = agfstout(aghead(f))) {
			dd_check_pathnode(view,aghead(f));
			if(f == lst) break;
		}
	}
}

void dd_check_newranks(Agraph_t *g)
{
	Agnode_t	*n;
	Agedge_t	*e;

	for(n = agfstnode(g); n; n = agnxtnode(n)) {
		if(dd_is_a_vnode(n)) continue;
		for(e = agfstout(n); e; e = agnxtout(e)) {
			if(NOT(dd_constraint(e))) continue;
			dgassert (dd_newrank(dd_pathhead(e)) - dd_newrank(dd_pathtail(e)) >= 1);
		}
	}
}

static void check_mg(Agraph_t *g, Agraph_t *root)
{
	Agnode_t	*mn;
	Agedge_t	*me;

	for(mn = agfstnode(g); mn; mn = agnxtnode(mn)) {
		dgassert(mn->base.data);
		dgassert(agsubnode(root,mn,FALSE));
		for(me = agfstout(mn); me; me = agnxtout(me)) {
			dgassert(me->base.data);
			dgassert(agsubedge(root,me,FALSE));
		}
	}
}

void dd_check_model(ddview_t *view)
{
	Agraph_t	*root;

	root = BASE(view)->model.main;
	check_mg(root,root);
	check_mg(BASE(view)->model.v[IL_INS],root);
	check_mg(BASE(view)->model.e[IL_INS],root);
	check_mg(BASE(view)->model.v[IL_MOD],root);
	check_mg(BASE(view)->model.e[IL_MOD],root);
	check_mg(BASE(view)->model.v[IL_DEL],root);
	check_mg(BASE(view)->model.e[IL_DEL],root);
}

void dd_check_really_gone(Agraph_t *g, Agnode_t *n, ulong id)
{
	Agnode_t	*u;
	Agedge_t	*e;

	dgassert (agidnode(g,id,FALSE) == NILnode);

	for(u = agfstnode(g); u; u = agnxtnode(u)) {
		dgassert(u != n);
		for(e = agfstedge(u); e; e = agnxtedge(e,u))
			dgassert(e->node != n);
	}
}

void dd_check_vnodes(ddview_t *view)
{
	Agnode_t	*n;
	Agedge_t	*e;

	for(n = agfstnode(view->layout); n; n = agnxtnode(n)) {
		if(NOT(dd_is_a_vnode(n))) continue;
		e = agfstin(n);
		if(e == NILedge) abort();
		e = agfstout(n);
		if(e == NILedge) abort();
	}
}

static int CLcnt = 0;
void dd_check_links(ddview_t *view)
{
	Agraph_t	*model, *layout;
	Agnode_t	*mn, *ln;
	Agedge_t	*me, *le, *mme;
	ddpath_t	*path;

dd_check_model(view);
	model = BASE(view)->model.main;
	layout = view->layout;

	for(mn = agfstnode(model); mn; mn = agnxtnode(mn)) {
		ln = dd_rep(mn);
		if(ln == NILnode) continue;
		dgassert(dd_node(ln)->model == mn);

		for(me = agfstedge(mn); me; me = agnxtedge(me,mn)) {
			path = dd_pathrep(me);
			mme = path->model;
			if(mme == NILedge) continue;
			dgassert((mme == me) || (mme == AGOPP(me)));
		}
	}

	for(ln = agfstnode(layout); ln; ln = agnxtnode(ln)) {
		if(dd_is_a_vnode(ln) == FALSE) {
			mn = dd_node(ln)->model;
			dgassert(mn);
			dgassert(agsubnode(model,mn,FALSE) == mn);
			dgassert(ln == dd_rep(mn));
			for(le = agfstedge(ln); le; le = agnxtedge(le,ln)) {
				path = dd_edge(le)->path;
				me = path->model;
				dgassert(agsubedge(model,me,FALSE) == me);
			}
		}
		else {
			dgassert(agfstin(ln) != NILedge);
			dgassert(agfstout(ln) != NILedge);
		}
	}
CLcnt++;
}
*/

} // namespace DynaDAG
} // namespace Dynagraph

