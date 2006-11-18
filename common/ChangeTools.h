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

namespace Dynagraph {

template<typename Layout>
void ClearInsDel(ChangeQueue<Layout> &Q) {
	Q.ExecuteDeletions();
	// turn inserts into modifies (this belongs within ChangeQueue, except the DG::Update specific part)
	for(typename Layout::node_iter ni = Q.insN.nodes().begin(); ni!=Q.insN.nodes().end();) {
		typename Layout::Node *n = *ni++,
			*wn = Q.whole->find(n);
		Q.insN.erase(n);
		ModifyNode(Q,wn,DG_UPD_MOVE);
	}
	for(typename Layout::graphedge_iter ei = Q.insE.edges().begin(); ei!=Q.insE.edges().end();) {
		typename Layout::Edge *e = *ei++,
			*we = Q.whole->find(e);
		Q.insE.erase(e);
		ModifyEdge(Q,we,DG_UPD_MOVE);
	}
	Q.insE.clear(); // erase the nodes
}
template<typename Layout>
void ClearStrAttrChanges(ChangeQueue<Layout> &Q) {
	igd<StrAttrChanges>(Q.ModGraph()).clear();
	for(typename Layout::node_iter ni = Q.modN.nodes().begin(); ni!=Q.modN.nodes().end(); ++ni) 
		igd<StrAttrChanges>(*ni).clear();
	for(typename Layout::graphedge_iter ei = Q.modE.edges().begin(); ei!=Q.modE.edges().end(); ++ei) 
		igd<StrAttrChanges>(*ei).clear();
}
// again this seems like something any layout engine might want
template<typename Layout>
bool ChangesAreRelevant(ChangeQueue<Layout> &Q) {
	if(Q.Empty())
		return igd<Update>(Q.ModGraph()).flags;
	if(!Q.insN.empty() || !Q.insE.empty() || !Q.delN.empty() || !Q.delE.empty())
		return true;
	for(typename Layout::node_iter ni = Q.modN.nodes().begin(); ni!=Q.modN.nodes().end(); ++ni)
		if(igd<Update>(*ni).flags)
			return true;
	for(typename Layout::graphedge_iter ei = Q.modE.edges().begin(); ei!=Q.modE.edges().end(); ++ei)
		if(igd<Update>(*ei).flags)
			return true;
	return false;
}

} // namespace Dynagraph
