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


#include "common/Dynagraph.h"
#include "common/emitGraph.h"
using namespace std;

const unsigned int AllFlags = 0xffffffff;

void emitChanges(ostream &os,ChangeQueue &Q,const char *view) {
	os << "lock graph " << view << endl;
	if(!gd<StrAttrChanges>(Q.client).empty()) {
		os << "modify graph " << view << " " << gd<StrAttrs2>(Q.client) << endl;
		gd<StrAttrChanges>(Q.client).clear();
	}
	Layout::node_iter ni;
	Layout::graphedge_iter ei;
	for(ei = Q.delE.edges().begin(); ei!=Q.delE.edges().end(); ++ei) {
		os << "delete edge " << view << " " << mquote(gd<Name>(*ei).c_str()) << endl;
		gd<StrAttrChanges>(*ei).clear();
	}
	for(ni = Q.delN.nodes().begin(); ni!=Q.delN.nodes().end(); ++ni) {
		os << "delete node " << view << " " << mquote(gd<Name>(*ni).c_str()) << endl;
		gd<StrAttrChanges>(*ni).clear();
	}
	for(ni = Q.insN.nodes().begin(); ni!=Q.insN.nodes().end(); ++ni) {
		os << "insert node " << view << " " << mquote(gd<Name>(*ni).c_str())
			<< " " << gd<StrAttrs2>(*ni) << endl;
		gd<StrAttrChanges>(*ni).clear();
	}
	for(ei = Q.insE.edges().begin(); ei!=Q.insE.edges().end(); ++ei) {
		os << "insert edge " << view << " " << mquote(gd<Name>(*ei))
			<< " " << mquote(gd<Name>((*ei)->tail))
			<< " " << mquote(gd<Name>((*ei)->head)) << " " << gd<StrAttrs2>(*ei) << endl;
		gd<StrAttrChanges>(*ei).clear();
	}
	// all things that still have StrAttrChanges are either layout-modified or had an irrelevant string changed
	for(ni = Q.client->nodes().begin(); ni!=Q.client->nodes().end(); ++ni)
		if(!gd<StrAttrChanges>(*ni).empty()) {
			os << "modify node " << view << " " << mquote(gd<Name>(*ni).c_str())
				<< " " << gd<StrAttrs2>(*ni) << endl;
			gd<StrAttrChanges>(*ni).clear();
		}
	for(ei = Q.client->edges().begin(); ei!=Q.client->edges().end(); ++ei)
		if(!gd<StrAttrChanges>(*ei).empty()) {
			os << "modify edge " << view << " " << mquote(gd<Name>(*ei))
				<< " " << gd<StrAttrs2>(*ei) << endl;
			gd<StrAttrChanges>(*ei).clear();
		}
	os << "unlock graph " << view << endl;
}
