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
	const StrAttrs operator +(const StrAttrs &attrs) const {
		return StrAttrs(*this) += attrs;
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
inline bool SetNoMark(GO *go,DString name,DString val) {
	DString &cur = gd<StrAttrs>(go)[name];
	if(cur!=val) {
		cur = val;
		return true;
	}
	return false;
}
template<typename GO>
inline bool SetNoMark(GO *go,const StrAttrs &attrs) {
	bool result = false;
	for(StrAttrs::const_iterator ai = attrs.begin(); ai!=attrs.end(); ++ai)
		result |= SetNoMark(go,ai->first,ai->second);
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
template<class ADTPolicy,class GData,class NData,class EData,class GIData=Nothing,class NIData=Nothing,class EIData=Nothing>
struct NamedGraph : LGraph<ADTPolicy,GData,NData,EData,GIData,NIData,EIData> {
	typedef LGraph<ADTPolicy,GData,NData,EData,GIData,NIData,EIData> Graph;
	typedef typename Graph::Node Node;
	typedef typename Graph::Edge Edge;
	typedef std::map<DString,Node*> NDict;
	typedef std::map<DString,Edge*> EDict;
	NDict ndict;
	EDict edict;
	NamedGraph(NamedGraph *parent = 0) : Graph(parent) {
		dgassert(ndict.size()==this->nodes().size());
	}
	NamedGraph(const NamedGraph &o) : Graph(o.parent) {
		dgassert(ndict.size()==this->nodes().size());
		*this = o;
		dgassert(ndict.size()==this->nodes().size());
	}
	NamedGraph(const Graph &g) : Graph(g.parent) {
		dgassert(ndict.size()==this->nodes().size());
		*(Graph*)this = g;
		dgassert(ndict.size()==this->nodes().size());
	}
	Node *lookNode(Name name) {
	    typename NDict::iterator f = ndict.find(name);
	    return (f!=ndict.end())?f->second:(Node*)0;
	}
	Edge *lookEdge(Name name) {
		typename EDict::iterator f = edict.find(name);
		return (f!=edict.end())?f->second:(Edge*)0;
	}
	// LGraph overrides
	Node *create_node(DString name) { // non virtual
		dgassert(ndict.size()==this->nodes().size());
		Node *ret = create_node(NData(name));
		dgassert(ndict.size()==this->nodes().size());
		return ret;
	}
	Node *create_node(const NData &nd) {
		dgassert(ndict.size()==this->nodes().size());
		Node *ret = Graph::create_node(nd);
		enter(nd,ret);
		dgassert(ndict.size()==this->nodes().size());
		return ret;
	}
	std::pair<Edge *,bool> create_edge(Node *tail,Node *head,DString name) {// non virtual
		dgassert(ndict.size()==this->nodes().size());
        EData ed(name);
		std::pair<Edge*,bool> ret = create_edge(tail,head,ed);
		dgassert(ndict.size()==this->nodes().size());
		return ret;
	}
	std::pair<Edge *,bool> create_edge(Node *tail,Node *head,EData &ed) {
		dgassert(ndict.size()==this->nodes().size());
		std::pair<Edge *,bool> ret = Graph::create_edge(tail,head,ed);
		if(!ret.second)
			throw DGParallelEdgesNotSupported(ed);
		else
			enter(ed,ret.first);
		dgassert(ndict.size()==this->nodes().size());
		return ret;
	}
	std::pair<Node *,bool> insert_subnode(Node *n) {
		dgassert(ndict.size()==this->nodes().size());
		std::pair<Node *,bool> ret = Graph::insert_subnode(n);
		if(ret.second)
			enter(gd<Name>(n),ret.first);
		dgassert(ndict.size()==this->nodes().size());
		return ret;
	}
	std::pair<Edge *,bool> insert_subedge(Edge *e) {
		dgassert(ndict.size()==this->nodes().size());
		std::pair<Edge *,bool> ret = Graph::insert_subedge(e);
		if(ret.second)
			enter(gd<Name>(e),ret.first);
		dgassert(ndict.size()==this->nodes().size());
		return ret;
	}
	bool erase_node(Node *n) {
		dgassert(ndict.size()==this->nodes().size());
		Name name = gd<Name>(n);
        bool ret = Graph::erase_node(n);
		forgetNode(name);
		dgassert(ndict.size()==this->nodes().size());
		return ret;
	}
    bool erase_edge(Edge *e) {
		dgassert(ndict.size()==this->nodes().size());
		Name name = gd<Name>(e);
        bool ret = Graph::erase_edge(e);
		forgetEdge(name);
		dgassert(ndict.size()==this->nodes().size());
		return ret;
    }
	// not in LGraph
	std::pair<Node *,bool> fetch_node(DString name,bool create) {
		dgassert(ndict.size()==this->nodes().size());
		if(Node *n = lookNode(name))
			return std::make_pair(n,false);
		if(!create)
			return std::make_pair((Node*)0,false);
		std::pair<Node*,bool> ret = std::make_pair(create_node(name),true);
		dgassert(ndict.size()==this->nodes().size());
		return ret;
	}
	Node *fetch_node(DString name) {
		dgassert(ndict.size()==this->nodes().size());
		return fetch_node(name,false).first;
	}
	std::pair<Edge *,bool> fetch_edge(DString tail, DString head, DString name,bool create) {
		dgassert(ndict.size()==this->nodes().size());
		if(Edge *e = lookEdge(name)) {
			if(gd<Name>(e->tail)!=tail || gd<Name>(e->head)!=head)
				throw DGEdgeNameUsed(name);
			return std::make_pair(e,false);
		}
		if(!create)
			return std::make_pair((Edge*)0,false);
		Node *t = fetch_node(tail,false).first,
			*h = fetch_node(head,false).first;
		if(!t)
			throw DGEdgeTailDoesNotExist(tail);
		if(!h)
			throw DGEdgeHeadDoesNotExist(head);
        return create_edge(t,h,name);
	}
	std::pair<Edge *,bool> fetch_edge(Node *tail,Node *head,DString name,bool create) {
		dgassert(ndict.size()==this->nodes().size());
		if(Edge *e = lookEdge(name)) {
			if(e->tail!=tail || e->head!=head)
				throw DGEdgeNameUsed(name);
			return std::make_pair(e,false);
		}
		if(!create)
			return std::make_pair((Edge*)0,false);
        return create_edge(tail,head,name);
	}
	Edge *fetch_edge(DString name) {
		dgassert(ndict.size()==this->nodes().size());
		return lookEdge(name);
	}
	void forgetNode(Name name) {
		ndict.erase(name);
	}
	void forgetEdge(Name name) {
		edict.erase(name);
	}
	void forget(Node *n) {
		forgetNode(gd<Name>(n));
	}
	void forget(Edge *e) {
		forgetEdge(gd<Name>(e));
	}
    void rename(Node *n,DString name) {
		dgassert(ndict.size()==this->nodes().size());
		forget(n);
        ndict[gd<Name>(n) = name] = n;
		dgassert(ndict.size()==this->nodes().size());
    }
    void rename(Edge *e,DString name) {
		dgassert(ndict.size()==this->nodes().size());
		forget(e);
        edict[gd<Name>(e) = name] = e;
		dgassert(ndict.size()==this->nodes().size());
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
