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


#ifndef strattr_h
#define strattr_h
// string attributes & names are (mostly) not used directly by the dynagraph layout engines;
// they are used by the command-line tool to name & tag things the way dot does.

#include "LGraph.h"
#include "LGraph-cdt.h"
#include "StringDict.h"
#include "dgxep.h"
#include "traversal.h"
#include <map>
#include "parsestr.h"

namespace Dynagraph {

typedef DString Name;
struct StrAttrs : std::map<DString,DString> {
	DString look(DString attr,DString defval=DString()) const {
		const_iterator ai = find(attr);
		if(ai==end()||ai->second.empty())
			return defval;
		else
			return ai->second;
	}
	StrAttrs &operator +=(const StrAttrs &attrs) {
		for(StrAttrs::const_iterator i = attrs.begin(); i!=attrs.end(); ++i)
			(*this)[i->first] = i->second;
		return *this;
	}
};
inline void emitAttrs(std::ostream &os,const StrAttrs &attrs,const DString &id = DString()) {
	bool comma=false;
	os << "[";
	if(!id.empty()) {
        // for edges when writing to dotfile, as dot edges don't have names
		os << "id=" << id;
		comma = true;
	}
	for(StrAttrs::const_iterator ai = attrs.begin(); ai!=attrs.end(); ++ai) {
		if(comma)
			os << ", ";
		os << mquote(ai->first) << "=" << mquote(ai->second);
		comma = true;
	}
	os << "]";
}
inline std::ostream &operator <<(std::ostream &os,const StrAttrs &attrs) {
	emitAttrs(os,attrs);
	return os;
}
typedef std::set<DString> StrAttrChanges;
template<typename GO>
inline bool SetAndMark(GO *go,DString name,DString val) {
	DString &cur = gd<StrAttrs>(go)[name];
	if(cur!=val) {
		cur = val;
		igd<StrAttrChanges>(go).insert(name);
		return true;
	}
	return false;
}
template<typename GO>
inline bool SetAndMark(GO *go,const StrAttrs &attrs) {
	bool result = false;
	for(StrAttrs::const_iterator ai = attrs.begin(); ai!=attrs.end(); ++ai)
		result |= SetAndMark(go,ai->first,ai->second);
	return result;
}
template<typename GO>
inline StrAttrs getChanges(GO *go) {
	StrAttrs ret;
	for(StrAttrChanges::iterator ci = igd<StrAttrChanges>(go).begin(); ci!=igd<StrAttrChanges>(go).end(); ++ci)
		ret[*ci] = gd<StrAttrs>(go)[*ci];
	return ret;
}
template<typename GO>
struct Changes {
	GO *go;
	Changes(GO *go) : go(go) {}
};
template<typename GO>
Changes<GO> changes(GO *go) {
	return Changes<GO>(go);
}
template<typename GO>	
inline std::ostream &operator << (std::ostream &os,const Changes<GO> &cgo) {
	StrAttrChanges &cha = igd<StrAttrChanges>(cgo.go);
	StrAttrs &att = gd<StrAttrs>(cgo.go);
	os << "[";
	for(StrAttrChanges::const_iterator ci = cha.begin(); ci!=cha.end(); ++ci) {
	  if(ci!=cha.begin())
	    os << ", ";
	  os << mquote(*ci) << "=" << mquote(att[*ci]);
	}
	os << "]";
	return os;
}
struct NamedAttrs : StrAttrs,Name,Hit {
	NamedAttrs(DString name = DString()) : Name(name) {}
	NamedAttrs(const StrAttrs &at,DString name=DString()) : StrAttrs(at),Name(name) {}
};
// as we all should know by now, deriving a class and trying to override all its 
// methods in order to maintain a dictionary isn't all that good an idea.  
// in case it is not working, we have oopsRefreshDictionary() -- blecch!
// better would be LGraph support for multiple simultaneous lookup, as BGL has.
template<class ADTPolicy,class GData,class NData,class EData,class GIData=Nothing,class NIData=Nothing,class EIData=Nothing>
struct NamedGraph : LGraph<ADTPolicy,GData,NData,EData,GIData,NIData,EIData> {
    typedef LGraph<ADTPolicy,GData,NData,EData,GIData,NIData,EIData> Graph;
	typedef typename Graph::Node Node;
	typedef typename Graph::Edge Edge;
    typedef std::map<DString,Node*> NDict;
	typedef std::map<DString,Edge*> EDict;
	NDict ndict;
	EDict edict;
	NamedGraph(NamedGraph *parent = 0) : Graph(parent) {}
	NamedGraph(const NamedGraph &o) : Graph(o) {
		oopsRefreshDictionary();
	}
	NamedGraph(const Graph &g) : Graph(g) {
		oopsRefreshDictionary();
	}
	// LGraph overrides
    Node *create_node(DString name) { // non virtual
		return create_node(NData(name));
	}
	Node *create_node(const NData &nd) {
		Node *ret = Graph::create_node(nd);
		enter(nd,ret);
		return ret;
	}
	std::pair<Edge *,bool> create_edge(Node *tail,Node *head,DString name) {// non virtual
        EData ed(name);
		return create_edge(tail,head,ed);
	}
	std::pair<Edge *,bool> create_edge(Node *tail,Node *head,EData &ed) {
		std::pair<Edge *,bool> ret = Graph::create_edge(tail,head,ed);
		if(!ret.second)
			throw DGParallelEdgesNotSupported(ed);
		else
			enter(ed,ret.first);
		return ret;
	}
	std::pair<Node *,bool> insert_subnode(Node *n) {
		std::pair<Node *,bool> ret = Graph::insert_subnode(n);
		if(ret.second)
			enter(gd<Name>(n),ret.first);
		return ret;
	}
	std::pair<Edge *,bool> insert_subedge(Edge *e) {
		std::pair<Edge *,bool> ret = Graph::insert_subedge(e);
		if(ret.second)
			enter(gd<Name>(e),ret.first);
		return ret;
	}
	bool erase_node(Node *n) {
		forget(n);
        return Graph::erase_node(n);
	}
    bool erase_edge(Edge *e) {
		forget(e);
        return Graph::erase_edge(e);
    }
	// not in LGraph
	std::pair<Node *,bool> fetch_node(DString name,bool create) {
		if(Node *n = ndict[name])
			return make_pair(n,false);
		if(!create)
			return make_pair((Node*)0,false);
		return make_pair(create_node(name),true);
	}
	std::pair<Edge *,bool> fetch_edge(DString tail, DString head, DString name,bool create) {
		if(Edge *e = edict[name]) {
			if(gd<Name>(e->tail)!=tail || gd<Name>(e->head)!=head)
				throw DGEdgeNameUsed(name);
			return make_pair(e,false);
		}
		if(!create)
			return make_pair((Edge*)0,false);
		Node *t = fetch_node(tail,false).first,
			*h = fetch_node(head,false).first;
		if(!t)
			throw DGEdgeTailDoesNotExist(tail);
		if(!h)
			throw DGEdgeHeadDoesNotExist(head);
        return create_edge(t,h,name);
	}
	std::pair<Edge *,bool> fetch_edge(Node *tail,Node *head,DString name,bool create) {
		if(Edge *e = edict[name]) {
			if(e->tail!=tail || e->head!=head)
				throw DGEdgeNameUsed(name);
			return make_pair(e,false);
		}
		if(!create)
			return make_pair((Edge*)0,false);
        return create_edge(tail,head,name);
	}
	Edge *fetch_edge(DString name) {
		return edict[name];
	}
	void forget(Node *n) {
		ndict.erase(gd<Name>(n));
	}
	void forget(Edge *e) {
		edict.erase(gd<Name>(e));
	}
    void rename(Node *n,DString name) {
		forget(n);
        ndict[gd<Name>(n) = name] = n;
    }
    void rename(Edge *e,DString name) {
		forget(e);
        edict[gd<Name>(e) = name] = e;
    }
	void oopsRefreshDictionary() {
		ndict.clear();
		for(typename Graph::node_iter ni = Graph::nodes().begin(); ni!=Graph::nodes().end(); ++ni)
			ndict[gd<Name>(*ni)] = *ni;
	}
	void readSubgraph(NamedGraph *what) {
		std::map<DString,Node*> &pardict = static_cast<NamedGraph*>(Graph::parent)->ndict;
		for(typename Graph::node_iter ni = what->nodes().begin(); ni!=what->nodes().end(); ++ni) {
			Node *n = *ni,*found = pardict[gd<Name>(n)];
			if(!found)
				throw DGNodeDoesNotExist(gd<Name>(n));
			insert(found);
		}
		for(typename Graph::graphedge_iter ei = what->edges().begin(); ei!=what->edges().end(); ++ei) {
			Edge *e = *ei;
			Node *tail = pardict[gd<Name>(e->tail)],
				*head = pardict[gd<Name>(e->head)];
			Edge *found = Graph::parent->find_edge(tail,head);
			if(!found)
				throw DGEdgeDoesNotExist(gd<Name>(e->tail),gd<Name>(e->head));
			insert(found);
		}
	}
protected:
	void enter(const DString &name,Node *n) {
		if(name.empty())
			return;
		Node *&spot = ndict[name];
		if(spot)
			throw DGNodeNameUsed(name);
		spot = n;
	}
	void enter(const DString &name,Edge *e) {
		if(name.empty())
			return;
		Edge *&spot = edict[name];
		if(spot)
			throw DGEdgeNameUsed(name);
		spot = e;
	}
};

typedef NamedGraph<ADTisCDT,NamedAttrs,NamedAttrs,NamedAttrs> StrGraph;
typedef NamedGraph<ADTisCDT,NamedAttrs,NamedAttrs,NamedAttrs,StrAttrChanges,StrAttrChanges,StrAttrChanges> StrChGraph;

} // namespace Dynagraph

#endif // strattr_h
