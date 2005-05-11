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

#ifndef FindChangeRect_h
#define FindChangeRect_h

template<typename Layout>
struct FCRData {
	Layout changeN,changeE; // subgraph union of ins,mod,del
    FCRData(Layout *clientL) : changeN(clientL),changeE(clientL) {}
};
template<typename Layout>
struct FCRBefore : Server<Layout> {
    FCRData<Layout> *data;
	FCRBefore(Layout *clientL,Layout *currentLayout,FCRData<Layout> *data)  :
        Server<Layout>(clientL,currentLayout), data(data) {}
	void Process(ChangeQueue<Layout> &Q);
};
template<typename Layout>
void FCRBefore<Layout>::Process(ChangeQueue<Layout> &Q) {
	data->changeN = Q.insN|Q.modN|Q.delN;
	data->changeE = Q.insE|Q.modE|Q.delE;
}
template<typename Layout>
struct FCRAfter : Server<Layout> {
    FCRData<Layout> *data;
	FCRAfter(Layout *clientL,Layout *currentLayout,FCRData<Layout> *data)  :
        Server<Layout>(clientL,currentLayout),data(data) {}
    ~FCRAfter() {
        delete data;
    }
	void Process(ChangeQueue<Layout> &Q);
};
template<typename Layout>
void FCRAfter<Layout>::Process(ChangeQueue<Layout> &Q) {
	Bounds changerect;
	for(typename Layout::node_iter ni = data->changeN.nodes().begin(); ni!=data->changeN.nodes().end(); ++ni)
		changerect |= gd<NodeGeom>(*ni).BoundingBox();
	for(typename Layout::graphedge_iter ei = data->changeE.edges().begin(); ei!=data->changeE.edges().end(); ++ei)
		changerect |= gd<EdgeGeom>(*ei).pos.BoundingBox();
	if(assign(gd<GraphGeom>(Q.current).changerect,changerect))
		Q.GraphUpdateFlags() |= DG_UPD_CHANGERECT;
}
#endif //FindChangeRect_h