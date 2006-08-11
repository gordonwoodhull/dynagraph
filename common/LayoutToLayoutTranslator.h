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

template<typename Layout1,typename Layout2,bool CopySecondOfTwo=true>
struct LayoutToLayoutTranslator {
	void copyNodeAttrs(typename Layout1::Node *ln1,typename Layout2::Node *ln2) {
		gd<Drawn>(ln2) = gd<Drawn>(ln1);
		gd<NodeGeom>(ln2) = gd<NodeGeom>(ln1);
		gd<NodeLabels>(ln2) = gd<NodeLabels>(ln1);
		gd<IfPolyDef>(ln2) = gd<IfPolyDef>(ln1);
		gd<DynaDAG::NSRankerNode>(ln2) = gd<DynaDAG::NSRankerNode>(ln1);

		igd<Update>(ln2) = igd<Update>(ln1);
	}
	void copyEdgeAttrs(typename Layout1::Edge *le1,typename Layout2::Edge *le2) {
		gd<Drawn>(le2) = gd<Drawn>(le1);
		gd<EdgeGeom>(le2) = gd<EdgeGeom>(le1);
		gd<EdgeLabels>(le2) = gd<EdgeLabels>(le1);
		if(CopySecondOfTwo)
			gd<DynaDAG::NSRankerEdge>(le2).secondOfTwo = gd<DynaDAG::NSRankerEdge>(le1).secondOfTwo;
		gd<DynaDAG::Suppression>(le2) = gd<DynaDAG::Suppression>(le1);
	
		igd<Update>(le2) = igd<Update>(le1);
	}
	void ModifyGraph(Layout1 *l1,Layout2 *l2) {
		gd<Drawn>(l2) = gd<Drawn>(l1);
		gd<GraphGeom>(l2) = gd<GraphGeom>(l1);
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
