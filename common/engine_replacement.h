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

namespace Dynagraph {

// To replace a layout engine, re-insert everything into a new engine and delete the old
// These functions manipulate the queue to make it look like everything just moved
// instead of being re-inserted
inline void pre_engine_replacement(ChangeProcessor *oldEngine,ChangeQueue &Q,Layout &oldCurrent) {
	// remember the current - will be used in post_engine_replacement
    oldCurrent = *Q.current;
	// erase old layout
	ChangeQueue delQ(Q.whole,Q.current);
	delQ.delN = delQ.delE = *Q.current;
	oldEngine->Process(delQ);
	assert(Q.current->empty());
    // feed the current graph into insertion queues & remove dead links to old engine
    for(Layout::node_iter ni = oldCurrent.nodes().begin(); ni!=oldCurrent.nodes().end(); ++ni) {
        gd<ModelPointer>(*ni).model = 0;
        Q.InsNode(*ni);
    }
    for(Layout::graphedge_iter ei = oldCurrent.edges().begin(); ei!=oldCurrent.edges().end(); ++ei) {
        gd<ModelPointer>(*ei).model = 0;
        Q.InsEdge(*ei);
    }
}
inline void post_engine_replacement(ChangeQueue &Q,Layout &oldCurrent) {
    // this is kinda weird: mark all that were re-inserted into new graph "moved"
    // (although there may be other changes)
    for(Layout::node_iter ni = oldCurrent.nodes().begin(); ni!=oldCurrent.nodes().end(); ++ni)
        if(Q.insN.erase(*ni)) // else it got deleted after the engine was replaced
			ModifyNode(Q,*ni,DG_UPD_MOVE);
    for(Layout::graphedge_iter ei = oldCurrent.edges().begin(); ei!=oldCurrent.edges().end(); ++ei) {
        if(Q.insE.erase(*ei))
			ModifyEdge(Q,*ei,DG_UPD_MOVE);
    }
	oldCurrent.clear();
}

} // namespace Dynagraph
