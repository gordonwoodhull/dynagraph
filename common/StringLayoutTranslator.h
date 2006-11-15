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

#ifndef StringLayoutTranslator_h
#define StringLayoutTranslator_h

#include "Transform.h"
#include "stringsIn.h"
#include "stringsOut.h"

namespace Dynagraph {

template<typename StringGraph,typename Layout>
struct StringToLayoutTranslator {
	Transform *transform_;
	bool useDotDefaults_;
	StringToLayoutTranslator(Transform *transform,bool useDotDefaults) : transform_(transform),useDotDefaults_(useDotDefaults) {}
	void OpenGraph(StringGraph *sg,Layout *lg) {
		stringsIn<Layout>(transform_,useDotDefaults_,lg,gd<StrAttrs>(sg),false);
	}
	void ModifyGraph(StringGraph *sg,Layout *lg) {
		igd<Update>(lg) |= stringsIn<Layout>(transform_,useDotDefaults_,lg,getChanges(sg),false);
	}
	void InsertNode(typename StringGraph::Node *sn,typename Layout::Node *ln) {
		stringsIn<Layout>(transform_,ln,gd<StrAttrs>(sn),true);
	}
	void InsertEdge(typename StringGraph::Edge *se,typename Layout::Edge *le) {
		stringsIn<Layout>(transform_,le,gd<StrAttrs>(se),true);
	}
	void ModifyNode(typename StringGraph::Node *sn,typename Layout::Node *ln) {
		igd<Update>(ln) |= stringsIn<Layout>(transform_,ln,getChanges(sn),false);
	}
	void ModifyEdge(typename StringGraph::Edge *se,typename Layout::Edge *le) {
		igd<Update>(le) |= stringsIn<Layout>(transform_,le,getChanges(se),false);
	}
	void DeleteNode(typename StringGraph::Node *sn,typename Layout::Node *ln) {}
	void DeleteEdge(typename StringGraph::Edge *se,typename Layout::Edge *le) {}
};
template<typename Layout,typename StringGraph>
struct LayoutToStringTranslator {
	Transform *transform_;
	LayoutToStringTranslator(Transform *transform) : transform_(transform) {}
	void OpenGraph(Layout *lg,StringGraph *sg) {
		stringsOut<Layout>(transform_,lg,AllFlags);
		SetNoMark(sg,getChanges(lg));
		igd<StrAttrChanges>(lg).clear(); //?
	}
	void ModifyGraph(Layout *lg,StringGraph *sg) {
		stringsOut<Layout>(transform_,lg,igd<Update>(lg));
		SetAndMark(sg,getChanges(lg));
	}
	void InsertNode(typename Layout::Node *ln,typename StringGraph::Node *sn) {
		stringsOut<Layout>(transform_,ln,AllFlags);
		gd<StrAttrs>(sn) = gd<StrAttrs>(ln);
	}
	void InsertEdge(typename Layout::Edge *le,typename StringGraph::Edge *se) {
		stringsOut<Layout>(transform_,le,AllFlags);
		gd<StrAttrs>(se) = gd<StrAttrs>(le);
	}
	void ModifyNode(typename Layout::Node *ln,typename StringGraph::Node *sn) {
		stringsOut<Layout>(transform_,ln,igd<Update>(ln));
		SetAndMark(sn,getChanges(ln));
	}
	void ModifyEdge(typename Layout::Edge *le,typename StringGraph::Edge *se) {
		stringsOut<Layout>(transform_,le,igd<Update>(le));
		SetAndMark(se,getChanges(le));
	}
	void DeleteNode(typename Layout::Node *ln,typename StringGraph::Node *sn) {}
	void DeleteEdge(typename Layout::Edge *le,typename StringGraph::Edge *se) {}
};

} // namespace Dynagraph

#endif // StringLayoutTranslator_h
