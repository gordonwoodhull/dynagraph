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

#include "dgxep.h"
#include "useful.h"

namespace Dynagraph {

/*
	CHANGE QUEUE
*/
template<typename Graph>
struct ChangeQueue {
	// the whole edits this supergraph of server's current layout
	// then calls the methods below to signal the changes in the subgraphs
	Graph * const whole, * const current;
	Graph insN,modN,delN,
		insE,modE,delE,
		unbornN,unbornE; // inserted and then deleted before ever realized

	ChangeQueue(Graph *whole,Graph *current) : whole(whole),current(current),
	insN(whole),modN(whole),delN(whole),insE(whole),modE(whole),delE(whole),unbornN(whole),unbornE(whole) {}
	ChangeQueue(ChangeQueue &copy) : whole(copy.whole),current(copy.current),
	insN(whole),modN(whole),delN(whole),insE(whole),modE(whole),delE(whole),unbornN(whole),unbornE(whole) {
		insN = copy.insN;
		modN = copy.modN;
		delN = copy.delN;
		insE = copy.insE;
		modE = copy.modE;
		delE = copy.delE;
	}
	typedef enum {inserted,modified,deleted,nothing} Action;
	template<typename GO>
	struct Result {
		Action action;
		// this will turn into a union if subgraphs get different type
		GO *object;
	};
	typedef Result<typename Graph::Node> NodeResult;
	typedef Result<typename Graph::Edge> EdgeResult;
	NodeResult InsNode(typename Graph::Node *n,bool checkRedundancy=true);
	EdgeResult InsEdge(typename Graph::Edge *e,bool checkRedundancy=true);
	NodeResult ModNode(typename Graph::Node *n);
	EdgeResult ModEdge(typename Graph::Edge *e);
	Graph *ModGraph() {
		return &modN;
	}
	NodeResult DelNode(typename Graph::Node *n,bool checkRedundancy=true);
	EdgeResult DelEdge(typename Graph::Edge *e,bool checkRedundancy=true);
	// called by server to update current subgraph based on current changes
	void UpdateCurrent();

	// called by client after server processing; clear subgraphs and maybe do deletions
	void Execute(bool doDelete);

	bool Empty() { return insN.empty()&&modN.empty()&&delN.empty()&&
		insE.empty()&&modE.empty()&&delE.empty()&&unbornN.empty()&&unbornE.empty(); }

	// copy
	ChangeQueue &operator=(const ChangeQueue &Q);
	// accumulate
	ChangeQueue &operator+=(const ChangeQueue &Q);

	// Exceptions
	struct InsertInserted : DGException {
		InsertInserted(bool fatal = false) : DGException("insertion of an already inserted object",fatal) {}
	};
	struct ModifyDeleted : DGException {
		ModifyDeleted(bool fatal = false) : DGException("modify of a deleted object",fatal) {}
	};
	struct ModifyUninserted : DGException {
		ModifyUninserted(bool fatal = false) : DGException("modify of an uninserted object",fatal) {}
	};
	struct DeleteDeleted : DGException {
		DeleteDeleted(bool fatal = false) : DGException("deletion of an already deleted object",fatal) {}
	};
	struct DeleteUninserted : DGException {
		DeleteUninserted(bool fatal = false) : DGException("deletion of an uninserted object",fatal) {}
	};
	struct EndnodesNotInserted : DGException {
		EndnodesNotInserted(bool fatal = false) : DGException("insertion of edge without nodes",fatal) {}
	};
};
template<typename Graph>
typename ChangeQueue<Graph>::NodeResult ChangeQueue<Graph>::InsNode(typename Graph::Node *n,bool checkRedundancy) {
	Result<typename Graph::Node> result;
	if(delN.erase(n)) { // del+ins = mod!
		assert(current->find(n));
		result.action = modified;
		result.object = modN.insert(n).first;
	}
	else if(unbornN.erase(n)) { // ins+del+ins = ins
		result.action = inserted;
		result.object = insN.insert(n).first;
	}
	else if(modN.find(n) || checkRedundancy&&(insN.find(n)||current->find(n)))
		throw InsertInserted();
	else {
		result.action = inserted;
		result.object = insN.insert(n).first;
	}
	return result;
}
template<typename Graph>
typename ChangeQueue<Graph>::EdgeResult ChangeQueue<Graph>::InsEdge(typename Graph::Edge *e,bool checkRedundancy) {
	Result<typename Graph::Edge> result;
	if(delE.inducing_erase_edge(e)) { // del+ins = mod!
		assert(current->find(e));
		result.action = modified;
		result.object = modE.insert(e).first;
	}
	else if(unbornE.inducing_erase_edge(e)) { // ins+del+ins = ins
		result.action = inserted;
		result.object = insE.insert(e).first;
	}
	else if(modE.find(e) || checkRedundancy&&(insE.find(e)||current->find(e)))
		throw InsertInserted();
	else {
		result.action = inserted;
		result.object = insE.insert(e).first;
	}
	return result;
}
template<typename Graph>
typename ChangeQueue<Graph>::NodeResult ChangeQueue<Graph>::ModNode(typename Graph::Node *n) {
	Result<typename Graph::Node> result;
	if(typename Graph::Node *n2 = insN.find(n)) {
		result.action = inserted;
		result.object = n2;
	}
	else if(delN.find(n))
		throw ModifyDeleted();
	else if(!current->find(n))
		throw ModifyUninserted();
	else {
		result.action = modified;
		result.object = modN.insert(n).first;
	}
	return result;
}
template<typename Graph>
typename ChangeQueue<Graph>::EdgeResult ChangeQueue<Graph>::ModEdge(typename Graph::Edge *e) {
	Result<typename Graph::Edge> result;
	if(typename Graph::Edge *e2 = insE.find(e)) {
		result.action = inserted;
		result.object = e2;
	}
	else if(delE.find(e))
		throw ModifyDeleted();
	else if(!current->find(e))
		throw ModifyUninserted();
	else {
		result.action = modified;
		result.object = modE.insert(e).first;
	}
	return result;
}
template<typename Graph>
typename ChangeQueue<Graph>::NodeResult ChangeQueue<Graph>::DelNode(typename Graph::Node *n,bool checkRedundancy) {
	Result<typename Graph::Node> result;
	if(checkRedundancy&&delN.find(n))
		throw DeleteDeleted();
	if(insN.find(n)) { //ins+del = do nothing
		// delete edges that haven't yet been inserted
		if(typename Graph::Node *in = insE.find(n))
			for(typename Graph::nodeedge_iter i = in->alledges().begin(); i!=in->alledges().end();) {
				typename Graph::Edge *ie = *i++;
				DelEdge(ie);
			}
		result.action = nothing;
		result.object = 0;
		unbornN.insert(n);
		insN.erase(n);
	}
	else {
		modN.erase(n);
		// remove edges that are currently inserted
		if(typename Graph::Node *cn = current->find(n))
			for(typename Graph::nodeedge_iter i = cn->alledges().begin(); i!=cn->alledges().end();) {
				typename Graph::Edge *ce = *i++;
				if(!delE.find(ce))
					DelEdge(ce);
			}
		// and edges that haven't yet been inserted
		if(typename Graph::Node *in = insE.find(n))
			for(typename Graph::nodeedge_iter i = in->alledges().begin(); i!=in->alledges().end();) {
				typename Graph::Edge *ie = *i++;
				DelEdge(ie);
			}
		result.action = deleted;
		result.object = delN.insert(n).first;
	}
	return result;
}
template<typename Graph>
typename ChangeQueue<Graph>::EdgeResult ChangeQueue<Graph>::DelEdge(typename Graph::Edge *e,bool checkRedundancy) {
	Result<typename Graph::Edge> result;
	if(checkRedundancy&&delE.find(e))
		throw DeleteDeleted();
	if(typename Graph::Edge *ie = insE.find(e)) { //ins+del = do nothing
		result.action = nothing;
		result.object = 0;
		unbornE.insert(e);
		insE.inducing_erase_edge(ie);
	}
	else {
		modE.inducing_erase_edge(e);
		result.action = deleted;
		result.object = delE.insert(e).first;
	}
	return result;
}
template<typename Graph>
void ChangeQueue<Graph>::UpdateCurrent() {
	// hopefully the other methods will catch any errors before this
	// so all these throws are just double-checks
	typename Graph::node_iter ni;
	typename Graph::graphedge_iter ei;
	for(ni = insN.nodes().begin(); ni!=insN.nodes().end(); ++ni)
		if(!current->insert(*ni).second)
			throw InsertInserted(true);
	for(ei = insE.edges().begin(); ei!=insE.edges().end(); ++ei) {
		typename Graph::Node *t =(*ei)->tail,*h = (*ei)->head;
		if(!current->find(t) && !insN.find(t))
			throw EndnodesNotInserted(true);
		if(!current->find(h) && !insN.find(h))
			throw EndnodesNotInserted(true);
		if(!current->insert(*ei).second)
			throw InsertInserted(true);
	}
	for(ei = delE.edges().begin(); ei!=delE.edges().end(); ++ei)
		if(!current->erase(*ei))
			throw DeleteUninserted(true);
	for(ni = delN.nodes().begin(); ni!=delN.nodes().end(); ++ni)
		if(!current->erase(*ni))
			throw DeleteUninserted(true);
	for(ni = modN.nodes().begin(); ni!=modN.nodes().end(); ++ni)
		if(!current->find(*ni))
			throw ModifyUninserted(true);
	for(ei = modE.edges().begin(); ei!=modE.edges().end(); ++ei)
		if(!current->find(*ei))
			throw ModifyUninserted(true);
}
template<typename Graph>
void ChangeQueue<Graph>::Execute(bool doDelete) {
	insN.clear();
	insE.clear();
	modN.clear();
	modE.clear();
	if(doDelete) {
		for(typename Graph::graphedge_iter j = delE.edges().begin(); j!=delE.edges().end();) {
			typename Graph::Edge *e = *j++;
			check(whole->erase_edge(e));
		}
        delE.clear(); // (clear nodes)
		for(typename Graph::node_iter i = delN.nodes().begin(); i!=delN.nodes().end();) {
			typename Graph::Node *n = *i++;
			check(whole->erase_node(n));
		}
		for(typename Graph::graphedge_iter j = unbornE.edges().begin(); j!=unbornE.edges().end();) {
			typename Graph::Edge *e = *j++;
			check(whole->erase_edge(e));
		}
        unbornE.clear(); // (clear nodes)
		for(typename Graph::node_iter i = unbornN.nodes().begin(); i!=unbornN.nodes().end();) {
			typename Graph::Node *n = *i++;
			check(whole->erase_node(n));
		}
	}
	else {
		delE.clear();
		delN.clear();
		unbornE.clear();
		unbornN.clear();
	}
    assert(Empty());
}
template<typename Graph>
ChangeQueue<Graph> &ChangeQueue<Graph>::operator=(const ChangeQueue<Graph> &Q) {
	assert(whole==Q.whole);
	insN = Q.insN;
	modN = Q.modN;
	delN = Q.delN;
	insE = Q.insE;
	modE = Q.modE;
	delE = Q.delE;
	whole->idat = Q.whole->idat; 
	return *this;
}
template<typename Graph>
ChangeQueue<Graph> &ChangeQueue<Graph>::operator+=(const ChangeQueue<Graph> &Q) {
	assert(whole==Q.whole);
	typename Graph::node_iter ni;
	typename Graph::graphedge_iter ei;
	for(ni = Q.insN.nodes().begin(); ni!=Q.insN.nodes().end(); ++ni)
		InsNode(*ni);
	for(ei = Q.insE.edges().begin(); ei!=Q.insE.edges().end(); ++ei)
		InsEdge(*ei);
	for(ni = Q.modN.nodes().begin(); ni!=Q.modN.nodes().end(); ++ni) 
		ModNode(*ni)->idat = (*ni)->idat;
	for(ei = Q.modE.edges().begin(); ei!=Q.modE.edges().end(); ++ei)
		ModEdge(*ei)->idat = (*ei)->idat;
	for(ni = Q.delN.nodes().begin(); ni!=Q.delN.nodes().end(); ++ni)
		DelNode(*ni);
	for(ei = Q.delE.edges().begin(); ei!=Q.delE.edges().end(); ++ei)
		DelEdge(*ei);
	return *this;
}

} // namespace Dynagraph

#endif // ChangeQueue_h
