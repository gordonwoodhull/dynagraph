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

#ifndef ChangeQueue_h
#define ChangeQueue_h

/*
	CHANGE QUEUE
*/
template<typename Layout>
struct ChangeQueue {
	// the client edits this supergraph of server's current layout
	// then calls the methods below to signal the changes in the subgraphs
	Layout * const client, * const current;
	Layout insN,modN,delN,
		insE,modE,delE;

	ChangeQueue(Layout *client,Layout *current) : client(client),current(current),
	insN(client),modN(client),delN(client),insE(client),modE(client),delE(client) {}
	ChangeQueue(ChangeQueue &copy) : client(copy.client),current(copy.current),
	insN(client),modN(client),delN(client),insE(client),modE(client),delE(client) {
		insN = copy.insN;
		modN = copy.modN;
		delN = copy.delN;
		insE = copy.insE;
		modE = copy.modE;
		delE = copy.delE;
	}
	void InsNode(typename Layout::Node *n) {
		insN.insert(n);
	}
	void InsEdge(typename Layout::Edge *e) {
		insE.insert(e);
	}
	void ModNode(typename Layout::Node *n,Update u) {
		if(!insN.find(n) && !delN.find(n)) { // u.flags &&
			typename Layout::Node *n2 = modN.insert(n).first;
			igd<Update>(n2).flags |= u.flags;
		}
	}
	void ModEdge(typename Layout::Edge *e,Update u) {
		if(!insE.find(e) && !delE.find(e)) { // u.flags &&
			typename Layout::Edge *e2 = modE.insert(e).first;
			igd<Update>(e2).flags |= u.flags;
		}
	}
	void DelNode(typename Layout::Node *n) {
		insN.erase(n);
		modN.erase(n);
		delN.insert(n);
		n = current->find(n); // remove edges that are currently inserted
		for(typename Layout::nodeedge_iter i = n->alledges().begin(); i!=n->alledges().end(); ++i)
			DelEdge(*i);
	}

	void DelEdge(typename Layout::Edge *e) {
		insE.erase(e);
		modE.erase(e);
		delE.insert(e);
	}

	unsigned &GraphUpdateFlags() { return igd<Update>(&modN).flags; }

	// called by server to update current subgraph based on current changes
	void UpdateCurrent();
	void CalcBounds();

	// called by client after server processing clear subgraphs and maybe do deletions
	void Okay(bool doDelete = false);

	bool Empty() { return insN.nodes().empty()&&modN.nodes().empty()&&delN.nodes().empty()&&
		insE.nodes().empty()&&modE.nodes().empty()&&delE.nodes().empty()&&GraphUpdateFlags()==0; }

	// copy
	ChangeQueue &operator=(ChangeQueue &Q);
	// accumulate
	ChangeQueue &operator+=(ChangeQueue &Q);

	// Exceptions

	// insertions must not already be inserted; modifications & deletions must already be inserted
	struct InsertInserted : DGException {
	  InsertInserted() : DGException("insertion of an already inserted object") {}
	};
	struct ModifyUninserted : DGException {
	  ModifyUninserted() : DGException("modify of an uninserted object") {}
	};
	struct DeleteUninserted : DGException {
	  DeleteUninserted() : DGException("deletion of an uninserted object") {}
	};
	struct EndnodesNotInserted : DGException {
	  EndnodesNotInserted() : DGException("insertion of edge without nodes") {}
	};
};
template<typename Layout>
void ChangeQueue<Layout>::UpdateCurrent() {
	typename Layout::node_iter ni;
	typename Layout::graphedge_iter ei;
	for(ni = insN.nodes().begin(); ni!=insN.nodes().end(); ++ni)
		if(!current->insert(*ni).second)
			throw InsertInserted();
	for(ei = insE.edges().begin(); ei!=insE.edges().end(); ++ei) {
		typename Layout::Node *t =(*ei)->tail,*h = (*ei)->head;
		if(!current->find(t) && !insN.find(t))
			throw EndnodesNotInserted();
		if(!current->find(h) && !insN.find(h))
			throw EndnodesNotInserted();
		if(!current->insert(*ei).second)
			throw InsertInserted();
	}
	for(ei = delE.edges().begin(); ei!=delE.edges().end(); ++ei)
		if(!current->erase(*ei))
			throw DeleteUninserted();
	for(ni = delN.nodes().begin(); ni!=delN.nodes().end(); ++ni)
		if(!current->erase(*ni))
			throw DeleteUninserted();
	for(ni = modN.nodes().begin(); ni!=modN.nodes().end(); ++ni)
		if(!current->find(*ni))
			throw ModifyUninserted();
	for(ei = modE.edges().begin(); ei!=modE.edges().end(); ++ei)
		if(!current->find(*ei))
			throw ModifyUninserted();
}
template<typename Layout>
void ChangeQueue<Layout>::CalcBounds() {
	Bounds &b = gd<GraphGeom>(current).bounds,
		b2;
	for(typename Layout::node_iter ni = current->nodes().begin(); ni!=current->nodes().end(); ++ni)
		b2 |= gd<NodeGeom>(*ni).BoundingBox();
	for(typename Layout::graphedge_iter ei = current->edges().begin(); ei!=current->edges().end(); ++ei)
		b2 |= gd<EdgeGeom>(*ei).pos.BoundingBox();
	if(b!=b2) {
		b = b2;
		GraphUpdateFlags() |= DG_UPD_BOUNDS;
	}
}
// clear update flags and maybe do deletions
template<typename Layout>
void ChangeQueue<Layout>::Okay(bool doDelete) {
	insN.clear();
	insE.clear();
	modN.clear();
	modE.clear();
	if(doDelete) {
		for(typename Layout::graphedge_iter j = delE.edges().begin(); j!=delE.edges().end();) {
			typename Layout::Edge *e = *j++;
			check(client->erase_edge(e));
		}
        delE.clear(); // the nodes may still exist
		for(typename Layout::node_iter i = delN.nodes().begin(); i!=delN.nodes().end();) {
			typename Layout::Node *n = *i++;
			check(client->erase_node(n));
		}
	}
	else {
		delE.clear();
		delN.clear();
	}
	GraphUpdateFlags() = 0;
    assert(Empty());
}
template<typename Layout>
ChangeQueue<Layout> &ChangeQueue<Layout>::operator=(ChangeQueue<Layout> &Q) {
	assert(client==Q.client);
	insN = Q.insN;
	modN = Q.modN;
	delN = Q.delN;
	insE = Q.insE;
	modE = Q.modE;
	delE = Q.delE;
	GraphUpdateFlags() = Q.GraphUpdateFlags();
	return *this;
}
template<typename Layout>
ChangeQueue<Layout> &ChangeQueue<Layout>::operator+=(ChangeQueue<Layout> &Q) {
	assert(client==Q.client);
	typename Layout::node_iter ni;
	typename Layout::graphedge_iter ei;
	for(ni = Q.insN.nodes().begin(); ni!=Q.insN.nodes().end(); ++ni)
		InsNode(*ni);
	for(ei = Q.insE.edges().begin(); ei!=Q.insE.edges().end(); ++ei)
		InsEdge(*ei);
	for(ni = Q.modN.nodes().begin(); ni!=Q.modN.nodes().end(); ++ni)
		ModNode(*ni,igd<Update>(*ni));
	for(ei = Q.modE.edges().begin(); ei!=Q.modE.edges().end(); ++ei)
		ModEdge(*ei,igd<Update>(*ei));
	for(ni = Q.delN.nodes().begin(); ni!=Q.delN.nodes().end(); ++ni)
		DelNode(*ni);
	for(ei = Q.delE.edges().begin(); ei!=Q.delE.edges().end(); ++ei)
		DelEdge(*ei);
	GraphUpdateFlags() |= Q.GraphUpdateFlags();
	return *this;
}

#endif // ChangeQueue_h
