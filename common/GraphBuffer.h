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


struct GraphBufferXep : DGException {
  DString description;
  EndNodesDontMatch(DString description) :
    DGException("GraphBuffer exception",true),
    description(description)
  {}
};

struct GraphBuffer {
	StrGraph *basis; // reference needed for modify_edge and delete_edge endnodes
	StrGraph insN,insE,modN,modE,delN,delE;
	RecordingGraph(StrGraph *basis) : basis(basis) {}

	StrGraph::Node *insert_node(DString name) {
		if(modN.ndict[name])
			throw GraphBufferXep("insert after modify");
		if(insN.ndict[name])
			throw GraphBufferXep("insert after insert");
		StrGraph::Node *n = insN.create_node(name);
		if(StrGraph::Node *d = delN.dict[name])
			delN.erase(d);
		return n;
	}
	StrGraph::Edge *insert_edge(DString tail, DString head,DString name) {
		if(modN.edict[name])
			throw GraphBufferXep("insert after modify");
		if(insN.edict[name])
			throw GraphBufferXep("insert after insert");
		StrGraph::Edge *e = insE.get_edge(tail,head,name);
		if(StrGraph::Edge *d = delE.edict[name])
			delE.erase(d);
		return e;
	}
	StrGraph::Node *modify_node(DString name) {
		if(delN.ndict[name])
			throw GraphBufferXep("modify after delete");
		if(StrGraph::Node *i = insN.ndict[name])
			return i;
		if(!basis->ndict[name])
			throw GraphBufferXep("modify not found");
		return modN.get_node(name);
	}
	StrGraph::Edge *modify_edge(DString name) {
		if(delE.edict[name])
			throw GraphBufferXep("modify after delete");
		if(StrGraph::Edge *i = insE.edict[name])
			return i;
		StrGraph::Edge *e = basis->edict[name];
		if(!e)
			throw GraphBufferXep("modify not found");
		return modN.get_edge(gd<Name>(e->tail),gd<Name>(e->head),name);
	}
	void delete_node(DString name) {
		if(delN.ndict[name])
			throw GraphBufferXep("delete after delete");
		if(StrGraph::Node *i = insN.ndict[name]) {
			insN.erase(i);
			return;
		}
		StrGraph::Node *d = delN.create_node(name);
		if(StrGraph::Node *m = modN.ndict[name])
			modN.erase(m);
	}
	void delete_edge(DString name) {
		if(delE.edict[name])
			throw GraphBufferXep("delete after delete");
		StrGraph::Edge *e = basis->edict[name];
		if(!e)
			throw GraphBufferXep("modify not found");
		StrGraph::Edge *d = delE.get_edge(gd<Name>(e->tail),gd<Name>(e->head),name);
		if(StrGraph::Edge *i = insE.edict[name])
			insE.erase(i);
		if(StrGraph::Node *m = modN.ndict[name])
			modE.erase(m);
	}
	void apply() {
		for(StrGraph::node_iter ni = insN.nodes().begin(); ni!=insN.nodes().end(); ++ni) {
			StrGraph::Node *n = basis->create_node(gd<Name>(*ni));
			gd<StrAttrs>(n) = gd<StrAttrs>(*ni);
		}
		for(StrGraph::graphedge_iter ei = insE.edges().begin(); ei!=insE.edges().end(); ++ei) {
			StrGraph::Node *t = basis->ndict[gd<Name>((*ei)->tail)],
				*h = basis->ndict[gd<Name>((*ei)->head)];
			if(!t || !h)
				throw GraphBufferXep("insert edge before nodes");
			StrGraph::Edge *e = basis->create_edge(t,h,gd<Name>(*ei));
			gd<StrAttrs>(e) = gd<StrAttrs>(*ei);
		}
		for(StrGraph::node_iter ni = modN.nodes().begin(); ni!=modN.nodes().end(); ++ni) {
			StrGraph::Node *n = basis->ndict[gd<Name>(*ni)];
			if(!n)
				throw GraphBufferXep("modify not found");
			gd<StrAttrs>(n) = gd<StrAttrs>(*ni);
		}
		for(StrGraph::graphedge_iter ei = insE.edges().begin(); ei!=insE.edges().end(); ++ei)
		...

};
