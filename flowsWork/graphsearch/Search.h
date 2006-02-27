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


#include "Pattern.h"

namespace GSearch {

enum InquiryType {
	UnionInquiry,
	IntersectionInquiry,
	PatternInquiry,
	PathInquiry
};
struct SearchStage {
	InquiryType type;
	StrGraph result;
	bool done;
	int limit; // approx. max number of nodes, default 0: no limit
	Pattern *pattern; // only if PatternInquiry
    bool goIn,goOut,firstOnly; // only if PathInquiry
	bool shortest;
	DString weightattr;
	SearchStage(StrGraph *parent) : pattern(0),result(parent),done(false),limit(0),
		goIn(true),goOut(true),firstOnly(false),shortest(false) {}
	SearchStage(const SearchStage &o) : type(o.type),pattern(o.pattern),result(o.result),done(false),limit(o.limit),
		goIn(o.goIn),goOut(o.goOut),firstOnly(o.firstOnly),shortest(o.shortest),weightattr(o.weightattr) {}
};
// edge names match pattern's start state name(s) or "a" or "b" for PathInquiry
// node names match runSearch input names
struct NamedStage : Name,SearchStage {
	NamedStage(const NamedStage &o) : Name(o),SearchStage(o) {}
	NamedStage(StrGraph *parent,DString name) : Name(name),SearchStage(parent) {}
};
typedef std::map<DString,StrGraph*> Inputs;
struct Search : LGraph<ADTisCDT,Nothing,NamedStage,Name> {
	StrGraph &source;
	std::map<DString,Node*> dict;
	Search(const Search &copy) : Graph(copy),source(copy.source) { // don't let them copy dict!
		for(node_iter ni = nodes().begin(); ni!=nodes().end(); ++ni)
			dict[gd<Name>(*ni)] = *ni;
	}
	Search(StrGraph &source);
	void readStrGraph(Patterns &patterns,StrGraph &desc);
	void Run(const Inputs &inputs);

	Node *create_node(DString name) {
		Node *n = Graph::create_node(NamedStage(&source,name));
		dict[name] = n;
		return n;
	}
	void reset() {
		for(node_iter ni = nodes().begin(); ni!=nodes().end(); ++ni) {
			Node *n = *ni;
			SearchStage &stage = gd<SearchStage>(n);
			stage.done = false;
			stage.result.clear();
		}
	}
};
// exceptions
struct StageNotFound : DGException {
  DString name;
  StageNotFound(DString name) : DGException("search stage not found (impossible!)"),
       name(name) {}
};
struct PatternNotThere : DGException {
  PatternNotThere() : DGException("search stage is supposed to execute a pattern, but the pattern wasn't specified") {}
};
struct PatternStateNotFound : DGException {
  DString name;
  PatternStateNotFound(DString name) : DGException("input state of pattern not found"),
       name(name) {}
};
struct UnknownAction : DGException {
  DString name; UnknownAction(DString name) : DGException("stage's action type not recognized"),
       name(name) {}
};
struct UndefinedPattern : DGException {
  DString name;
  UndefinedPattern(DString name) : DGException("pattern not defined"),
       name(name) {}
};

}
