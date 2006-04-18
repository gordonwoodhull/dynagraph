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


#include "fdp/FDPLayout.h"
#include "common/ChangeQueue.h"
#include "common/ChangeProcessor.h"
#include "common/CalculateBounds.h"

#include "ObstacleAvoiderSpliner.h"

namespace Dynagraph {

template<typename Layout>
struct ObAvSplinerEngine : LinkedChangeProcessor<Layout> {
	Layout *current_;
	ObAvSplinerEngine(Layout *whole,Layout *current) : current_(current) {}
	// ChangeProcessor
	void Process(ChangeQueue<Layout> &changeQ) {
		if(CalculateBounds(changeQ.current))
			ModifyFlags(changeQ) |= Update(DG_UPD_BOUNDS);
		double		SEP = gd<GraphGeom>(changeQ.current).separation.Len();

		ObstacleAvoiderSpliner<Layout> obav(current_);

		/* route edges  */
		for(typename Layout::graphedge_iter ei = current_->edges().begin(); ei!=current_->edges().end(); ++ei) {
			typename Layout::Edge *e = *ei;
			NodeGeom &tg = gd<NodeGeom>(e->tail),
				&hg = gd<NodeGeom>(e->head);
			EdgeGeom &eg = gd<EdgeGeom>(e);
			Line unclipped;
			if(e->tail != e->head)
				obav.FindSpline(tg.pos,hg.pos,unclipped);
			if(unclipped.empty()) {
				unclipped.degree = 1;
				unclipped.resize(4);
				unclipped[0] = tg.pos;
				unclipped[3] = hg.pos;
				if (e->tail != e->head) {
					Coord d = (unclipped[3] - unclipped[0])/3.0;
					unclipped[1] = unclipped[0]+d;
					unclipped[2] = unclipped[3]-d;
				}
				else {	/* self arc */
					Coord d(tg.region.boundary.Width()/2.0+.66 * SEP,0.0);
					unclipped[1] = unclipped[2] = unclipped[0]+d;
					unclipped[1].y += d.x;
					unclipped[2].y -= d.x;
				}
			}
			gd<EdgeGeom>(e).pos.ClipEndpoints(unclipped,tg.pos,eg.tailClipped?&tg.region:0,
				hg.pos,eg.headClipped?&hg.region:0);
			ModifyEdge(changeQ,e,DG_UPD_MOVE);
		}
		NextProcess(changeQ);
	}

};

} // namespace Dynagraph
