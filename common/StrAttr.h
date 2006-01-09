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
#include "traversal.h"
#include <map>
#include "parsestr.h"
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
struct StrAttrs2 : StrAttrs,StrAttrChanges { // sorry, don't know what else to call it
	bool put(DString name,DString val) {
		StrAttrs &attrs = *this;
		StrAttrChanges &cha = *this;
		if(attrs[name]!=val) {
			attrs[name] = val;
			cha.insert(name);
            return true;
		}
        else return false;
	}
	StrAttrs getChanges() {
		StrAttrs ret;
		StrAttrs &sa = *this;
		StrAttrChanges &sac = *this;
		for(StrAttrChanges::iterator ci = sac.begin(); ci!=sac.end();++ci)
			ret[*ci] = sa[*ci];
		return ret;
	}
};
inline std::ostream &operator << (std::ostream &os,StrAttrs2 &sa2) {
	StrAttrChanges &cha = sa2;
	StrAttrs &att = sa2;
	os << "[";
	for(StrAttrChanges::const_iterator ci = cha.begin(); ci!=cha.end(); ++ci) {
	  if(ci!=cha.begin())
	    os << ",";
	  os << mquote(*ci) << "=" << mquote(att[*ci]);
	}
	os << "]";
	return os;
}
struct NamedAttrs : StrAttrs,Name,Hit {
	NamedAttrs(DString name = DString()) : Name(name) {}
	NamedAttrs(const StrAttrs &at,DString name=DString()) : StrAttrs(at),Name(name) {}
};
struct DuplicateNodeName : DGException {
  DString name;
  DuplicateNodeName(DString name) :
    DGException("names of StrGraph nodes must be unique",true),
    name(name)
  {}
};
struct DuplicateEdgeName : DGException {
  DString name;
  DuplicateEdgeName(DString name) :
    DGException("names of StrGraph edges must be unique",true),
    name(name)
  {}
};
struct ParallelEdgesUnsupported : DGException {
	DString name;
	ParallelEdgesUnsupported(DString name) :
		DGException("LGraph does not allow multiple edges between the same tail and head",true),
		name(name)
	{}
};
struct EndNodesDontMatch : DGException {
  DString name;
  EndNodesDontMatch(DString name) :
    DGException("get_node on this edge with new end nodes",true),
    name(name)
  {}
};
struct NodeNotFound : DGException {
  DString name;
  NodeNotFound(DString name) :
    DGException("StrGraph::readSubgraph encountered a node not in the parent",true),
    name(name)
  {}
};
struct EdgeNotFound : DGException {
  DString tail,head;
  EdgeNotFound(DString tail,DString head) :
    DGException("StrGraph::readSubgraph encountered an edge not in the parent"),
    tail(tail),head(head)
  {}
};

template<class GData,class NData,class EData>
struct NamedGraph : LGraph<ADTisCDT,GData,NData,EData> {
    typedef LGraph<ADTisCDT,GData,NData,EData> Graph;
	// as we all should know by now, deriving a class and adding a dictionary isn't all
	// that good an idea.  this class is no exception; to make this work, gotta call
	// oopsRefreshDictionary() before you need the ndict to work.  what's next?  LGraph events?
    typedef std::map<DString,typename Graph::Node*> NDict;
	typedef std::map<DString,typename Graph::Edge*> EDict;
	NDict ndict;
	EDict edict;
	NamedGraph(NamedGraph *parent = 0) : Graph(parent) {}
	NamedGraph(const NamedGraph &o) : Graph(o) {
		oopsRefreshDictionary();
	}
    typename Graph::Node *create_node(DString name) {
		return create_node(NData(name));
	}
	typename Graph::Node *create_node(const NData &nd) {
		typename Graph::Node *ret = Graph::create_node(nd);
		enter(nd,ret);
		return ret;
	}
	typename Graph::Node *get_node(DString name) {
		if(typename Graph::Node *n = ndict[name])
			return n;
		return create_node(name);
	}
	std::pair<typename Graph::Edge *,bool> create_edge(typename Graph::Node *tail,typename Graph::Node *head,DString name) {
        EData ed(name);
		return create_edge(tail,head,ed);
	}
	std::pair<typename Graph::Edge *,bool> create_edge(typename Graph::Node *tail,typename Graph::Node *head,EData &ed) {
		std::pair<typename Graph::Edge *,bool> ret = Graph::create_edge(tail,head,ed);
		if(!ret.second)
			throw ParallelEdgesUnsupported(ed);
		else
			enter(ed,ret.first);
		return ret;
	}
	typename Graph::Edge *get_edge(DString tail, DString head, DString name) {
		if(typename Graph::Edge *e = edict[name]) {
			if(gd<Name>(e->tail)!=tail || gd<Name>(e->head)!=head)
				throw EndNodesDontMatch(name);
			return e;
		}
		typename Graph::Node *t = get_node(tail),
			*h = get_node(head);
        return create_edge(t,h,name).first;
	}
    void rename(typename Graph::Node *n,DString name) {
        ndict[gd<Name>(n)] = 0;
        ndict[gd<Name>(n) = name] = n;
    }
    void rename(typename Graph::Edge *e,DString name) {
        edict[gd<Name>(e)] = 0;
        edict[gd<Name>(e) = name] = e;
    }
    void erase(typename Graph::Node *n) {
        ndict[gd<Name>(n)] = 0;
        Graph::erase(n);
    }
    void erase(typename Graph::Edge *e) {
        edict[gd<Name>(e)] = 0;
        Graph::erase(e);
    }
	void oopsRefreshDictionary() {
		ndict.clear();
		for(typename Graph::node_iter ni = Graph::nodes().begin(); ni!=Graph::nodes().end(); ++ni)
			ndict[gd<Name>(*ni)] = *ni;
	}
	void readSubgraph(NamedGraph *what) {
		std::map<DString,typename Graph::Node*> &pardict = static_cast<NamedGraph*>(Graph::parent)->ndict;
		for(typename Graph::node_iter ni = what->nodes().begin(); ni!=what->nodes().end(); ++ni) {
			typename Graph::Node *n = *ni,*found = pardict[gd<Name>(n)];
			if(!found)
				throw NodeNotFound(gd<Name>(n));
			insert(found);
		}
		for(typename Graph::graphedge_iter ei = what->edges().begin(); ei!=what->edges().end(); ++ei) {
			typename Graph::Edge *e = *ei;
			typename Graph::Node *tail = pardict[gd<Name>(e->tail)],
				*head = pardict[gd<Name>(e->head)];
			typename Graph::Edge *found = Graph::parent->find_edge(tail,head);
			if(!found)
				throw EdgeNotFound(gd<Name>(e->tail),gd<Name>(e->head));
			insert(found);
		}
	}
protected:
	void enter(const DString &name,typename Graph::Node *n) {
		if(name.empty())
			return;
		typename Graph::Node *&spot = ndict[name];
		if(spot)
			throw DuplicateNodeName(name);
		spot = n;
	}
	void enter(const DString &name,typename Graph::Edge *e) {
		if(name.empty())
			return;
		typename Graph::Edge *&spot = edict[name];
		if(spot)
			throw DuplicateEdgeName(name);
		spot = e;
	}
};

typedef NamedGraph<NamedAttrs,NamedAttrs,NamedAttrs> StrGraph;
#endif // strattr_h
