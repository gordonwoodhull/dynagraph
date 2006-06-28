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

#ifndef StringLayoutTranslation_h
#define StringLayoutTranslation_h

namespace Dynagraph {

template<typename Layout1,typename Layout2>
struct LayoutToLayoutTranslator {
	void ModifyGraph(Layout1 *l1,Layout2 *l2) {
		gd<GraphAttrs>(l2) = gd<GraphAttrs>(l1);
		igd<Update>(l2) = igd<Update>(l1);
	}
	void InsertNode(typename Layout1::Node *ln1,typename Layout2::Node *ln2) {
		gd<NodeAttrs>(ln2) = gd<NodeAttrs>(ln1);
	}
	void InsertEdge(typename Layout1::Edge *le1,typename Layout2::Edge *le2) {
		gd<EdgeAttrs>(le2) = gd<EdgeAttrs>(le1);
	}
	void ModifyNode(typename Layout1::Node *ln1,typename Layout2::Node *ln2) {
		gd<NodeAttrs>(ln2) = gd<NodeAttrs>(ln1);
		igd<Update>(ln2) = igd<Update>(ln1);
	}
	void ModifyEdge(typename Layout1::Edge *le1,typename Layout2::Edge *le2) {
		gd<EdgeAttrs>(le2) = gd<EdgeAttrs>(le1);
		igd<Update>(le2) = igd<Update>(le1);
	}
	void DeleteNode(typename Layout1::Node *ln1,typename Layout2::Node *ln2) {}
	void DeleteEdge(typename Layout1::Edge *le1,typename Layout2::Edge *le2) {}
};

} // namespace Dynagraph

#endif // StringLayoutTranslation_h
