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

#ifndef LayoutToLayoutTranslator_h
#define LayoutToLayoutTranslator_h

namespace Dynagraph {

struct LayoutToLayoutCopyAllPolicy {
	enum {
		CopySecondOfTwo = true,
		CopyDrawn = true,
		CopyNodePos = true,
		CopyNodeRegion = true,
		CopyEdgePos = true,
		CopyBounds = true,
	};
};
template<typename Layout1,typename Layout2,typename CopyPolicy = LayoutToLayoutCopyAllPolicy>
struct LayoutToLayoutTranslator {
	void copyNodeAttrs(typename Layout1::Node *ln1,typename Layout2::Node *ln2) {
		if(CopyPolicy::CopyDrawn)
			gd<Drawn>(ln2) = gd<Drawn>(ln1);
		if(CopyPolicy::CopyNodePos)
			gd<NodeGeom>(ln2).pos = gd<NodeGeom>(ln1).pos;
		if(CopyPolicy::CopyNodeRegion)
			gd<NodeGeom>(ln2).region = gd<NodeGeom>(ln1).region;
		gd<NodeGeom>(ln2).nail = gd<NodeGeom>(ln1).nail;
		gd<NodeGeom>(ln2).flow = gd<NodeGeom>(ln1).flow;
		gd<NodeGeom>(ln2).suppressed = gd<NodeGeom>(ln1).suppressed;
		gd<NodeGeom>(ln2).freezeTailFanning = gd<NodeGeom>(ln1).freezeTailFanning;
		gd<NodeGeom>(ln2).freezeHeadFanning = gd<NodeGeom>(ln1).freezeHeadFanning;
		gd<NodeLabels>(ln2) = gd<NodeLabels>(ln1);
		gd<IfPolyDef>(ln2) = gd<IfPolyDef>(ln1);
		gd<DynaDAG::NSRankerNode>(ln2) = gd<DynaDAG::NSRankerNode>(ln1);

		igd<Update>(ln2) = igd<Update>(ln1);
	}
	void copyEdgeAttrs(typename Layout1::Edge *le1,typename Layout2::Edge *le2) {
		if(CopyPolicy::CopyDrawn)
			gd<Drawn>(le2) = gd<Drawn>(le1);
		if(CopyPolicy::CopyEdgePos)
			gd<EdgeGeom>(le2).pos = gd<EdgeGeom>(le1).pos;
		gd<EdgeGeom>(le2).headPort = gd<EdgeGeom>(le1).headPort;
		gd<EdgeGeom>(le2).tailPort = gd<EdgeGeom>(le1).tailPort;
		gd<EdgeGeom>(le2).headClipped = gd<EdgeGeom>(le1).headClipped;
		gd<EdgeGeom>(le2).tailClipped = gd<EdgeGeom>(le1).tailClipped;
		gd<EdgeLabels>(le2) = gd<EdgeLabels>(le1);
		if(CopyPolicy::CopySecondOfTwo)
			gd<DynaDAG::NSRankerEdge>(le2).secondOfTwo = gd<DynaDAG::NSRankerEdge>(le1).secondOfTwo;
		gd<DynaDAG::Suppression>(le2) = gd<DynaDAG::Suppression>(le1);
	
		igd<Update>(le2) = igd<Update>(le1);
	}
	void OpenGraph(Layout1 *l1,Layout2 *l2) {
		ModifyGraph(l1,l2);
	}
	void ModifyGraph(Layout1 *l1,Layout2 *l2) {
		gd<Drawn>(l2) = gd<Drawn>(l1);
		// this is starting to suck.
		if(CopyPolicy::CopyBounds) {
			gd<GraphGeom>(l2).bounds = gd<GraphGeom>(l1).bounds;
			gd<GraphGeom>(l2).changerect = gd<GraphGeom>(l1).changerect;
		}
		gd<GraphGeom>(l2).splineLevel,
		gd<GraphGeom>(l2).labelGap = gd<GraphGeom>(l1).labelGap;
		gd<GraphGeom>(l2).resolution = gd<GraphGeom>(l1).resolution;
		gd<GraphGeom>(l2).separation = gd<GraphGeom>(l1).separation;
		gd<GraphGeom>(l2).defaultSize = gd<GraphGeom>(l1).defaultSize;
		gd<GraphGeom>(l2).edgeSeparation = gd<GraphGeom>(l1).edgeSeparation;
		gd<GraphGeom>(l2).ticks = gd<GraphGeom>(l1).ticks;
		gd<GraphGeom>(l2).reportIntermediate = gd<GraphGeom>(l1).reportIntermediate;
		gd<GraphGeom>(l2).s_edges = gd<GraphGeom>(l1).s_edges;
		gd<Translation>(l2) = gd<Translation>(l1);
		gd<StaticLabels>(l2) = gd<StaticLabels>(l1);
		gd<Interruptible>(l2) = gd<Interruptible>(l1);
		igd<Update>(l2) = igd<Update>(l1);
	}
	void InsertNode(typename Layout1::Node *ln1,typename Layout2::Node *ln2) {
		copyNodeAttrs(ln1,ln2);
	}
	void InsertEdge(typename Layout1::Edge *le1,typename Layout2::Edge *le2) {
		copyEdgeAttrs(le1,le2);
	}
	void ModifyNode(typename Layout1::Node *ln1,typename Layout2::Node *ln2) {
		copyNodeAttrs(ln1,ln2);
	}
	void ModifyEdge(typename Layout1::Edge *le1,typename Layout2::Edge *le2) {
		copyEdgeAttrs(le1,le2);
	}
	void DeleteNode(typename Layout1::Node *ln1,typename Layout2::Node *ln2) {}
	void DeleteEdge(typename Layout1::Edge *le1,typename Layout2::Edge *le2) {}
};

} // namespace Dynagraph

#endif // LayoutToLayoutTranslator_h
