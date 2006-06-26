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

#include "ChangeProcessor.h"

namespace Dynagraph {

template<typename Layout>
struct FCRData {
	Layout changeN,changeE; // subgraph union of ins,mod,del
	ChangingGraph<Layout> *world;
	FCRData(ChangingGraph<Layout> *world) : changeN(&world->whole_),changeE(&world->whole_),world(world) {}
};
template<typename Layout>
struct FCRBefore : LinkedChangeProcessor<Layout> {
    FCRData<Layout> *data;
	FCRBefore(FCRData<Layout> *data) : LinkedChangeProcessor<Layout>(data->world),data(data) {}
	void Process() {
		ChangeQueue<Layout> &Q = this->world_->Q_;
		data->changeN = Q.insN|Q.modN|Q.delN;
		data->changeE = Q.insE|Q.modE|Q.delE;
		this->NextProcess();
	}
};
template<typename Layout>
struct FCRAfter : LinkedChangeProcessor<Layout> {
    FCRData<Layout> *data;
	FCRAfter(FCRData<Layout> *data) : LinkedChangeProcessor<Layout>(data->world),data(data) {}
    ~FCRAfter() {
        delete data;
    }
	void Process() {
		Bounds changerect;
		for(typename Layout::node_iter ni = data->changeN.nodes().begin(); ni!=data->changeN.nodes().end(); ++ni)
			changerect |= gd<NodeGeom>(*ni).BoundingBox();
		for(typename Layout::graphedge_iter ei = data->changeE.edges().begin(); ei!=data->changeE.edges().end(); ++ei)
			changerect |= gd<EdgeGeom>(*ei).pos.BoundingBox();
		if(assign(gd<GraphGeom>(this->world_->Q_.ModGraph()).changerect,changerect))
			ModifyFlags(this->world_->Q_) |= DG_UPD_CHANGERECT;
		this->NextProcess();
	}
};

} // namespace Dynagraph

#endif //FindChangeRect_h
