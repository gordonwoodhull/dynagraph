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


#include "Dynagraph.h"
#include "parsestr.h"

template<typename G>
void emitGraph(std::ostream &os,G *g) {
  os << "digraph " << mquote(gd<Name>(g)) << " {" << std::endl;
  StrAttrs gattrs = gd<StrAttrs>(g);
  if(!gattrs.empty())
      os << "\tgraph " << gattrs << std::endl;
  for(typename G::node_iter ni = g->nodes().begin(); ni!=g->nodes().end(); ++ni)
      os << '\t' << mquote(gd<Name>(*ni)) << ' ' << gd<StrAttrs>(*ni) << std::endl;
  for(typename G::graphedge_iter ei = g->edges().begin(); ei!=g->edges().end(); ++ei) {
    os << '\t' << mquote(gd<Name>((*ei)->tail)) << " -> " << mquote(gd<Name>((*ei)->head)) << ' ';
    emitAttrs(os,gd<StrAttrs>(*ei),gd<Name>(*ei));
    os << std::endl;
  }
  os << "}\n";
}

template<class GD,class ND,class ED,class GID,class NID,class EID>
inline std::ostream &operator <<(std::ostream &os,LGraph<ADTisCDT,GD,ND,ED,GID,NID,EID> &g) {
    emitGraph(os,&g);
    return os;
}

// try to substitute labels for names to make dotfile more pleasant
template<typename G>
void emitGraph2(std::ostream &os,G *g) {
	typedef std::map<DString,typename G::Node*> node_dict;
	typedef std::map<typename G::Node*,DString> node_reverse_dict;
	typedef std::map<DString,typename G::Edge*> edge_dict;
	node_dict ndict;
	node_reverse_dict nameOf;
	StrAttrs::iterator ati = gd<StrAttrs>(g).find("label");
	Name &gname = (ati!=gd<StrAttrs>(g).end())?ati->second:gd<Name>(g);

	os << "digraph " << mquote(gname);
    os << " {" << std::endl;
    StrAttrs gattrs = gd<StrAttrs>(g);
    if(!gattrs.empty())
        os << "\tgraph " << gattrs << std::endl;
	for(typename G::node_iter ni = g->nodes().begin(); ni!=g->nodes().end(); ++ni) {
		StrAttrs::iterator ati = gd<StrAttrs>(*ni).find("label");
		Name nname;
		if(ati!=gd<StrAttrs>(*ni).end()&&!ndict[ati->second]) {
			ndict[ati->second] = *ni;
			nameOf[*ni] = ati->second;
			nname = ati->second;
		}
		else
			ndict[nameOf[*ni] = nname = gd<Name>(*ni)] = *ni;
        os << '\t' << mquote(nname) << ' ' << gd<StrAttrs>(*ni) << std::endl;
	}
	edge_dict edict;
	for(typename G::graphedge_iter ei = g->edges().begin(); ei!=g->edges().end(); ++ei) {
		os << '\t' << mquote(nameOf[(*ei)->tail]) << " -> " << mquote(nameOf[(*ei)->head]);
		os << ' ';
		StrAttrs::iterator ati = gd<StrAttrs>(*ei).find("label");
		Name ename;
		if(ati!=gd<StrAttrs>(*ei).end()&&!ndict[ati->second]) {
			edict[ati->second] = *ei;
			ename = ati->second;
		}
		else
			edict[ename = gd<Name>(*ei)] = *ei;
		emitAttrs(os,gd<StrAttrs>(*ei),ename);
        os << std::endl;
	}
	os << "}\n";
}
