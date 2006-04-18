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

#ifndef Modify_h
#define Modify_h

#include "LayoutAttrs.h"
#include "ChangeQueue.h"

namespace Dynagraph {

template<typename Layout>
inline void ModifyNode(ChangeQueue<Layout> &Q,typename Layout::Node *n,Update upd) {
	typename ChangeQueue<Layout>::NodeResult result = Q.ModNode(n);
	if(result.action==ChangeQueue<Layout>::modified) 
		igd<Update>(result.object) |= upd;
}
template<typename Layout>
inline void ModifyEdge(ChangeQueue<Layout> &Q,typename Layout::Edge *e,Update upd) {
	typename ChangeQueue<Layout>::EdgeResult result = Q.ModEdge(e);
	if(result.action==ChangeQueue<Layout>::modified)
		igd<Update>(result.object) |= upd;
}
template<typename Layout>
inline Update &ModifyFlags(ChangeQueue<Layout> &Q) {
	return igd<Update>(Q.ModGraph());
}

} // namespace Dynagraph

#endif // Modify_h
