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


#include "common/LGraph-cdt.h"
#include "common/StrAttr.h"
#include <vector>
#include <queue>
#if defined(__GNUC__) && __GNUC__>=3
// hash_* are "extensions" as of gcc3
#include <ext/hash_set>
#define HASH_NAMESPACE __gnu_cxx
#else
#include <hash_set>
#if defined(_MSC_VER) && _MSC_VER>=1310
// just great
#define HASH_NAMESPACE stdext
#else
#define HASH_NAMESPACE std
#endif

#endif

namespace GSearch {

struct PatternState {
};
enum PatternDirection {matchDown=1,matchUp=2,matchBoth=3};
struct Test {
	DString attr,value; // what to match
	bool matches(StrGraph::Edge *e) {
		StrAttrs::iterator i = gd<StrAttrs>(e).find(attr);
		if(i==gd<StrAttrs>(e).end())
			return false;
		return i->second==value;
	}
};
struct Path {
	std::vector<Test> tests;
	int direction;
	Path() : direction(matchBoth) {}
	bool matches(StrGraph::Edge *e) {
		for(std::vector<Test>::iterator ci = tests.begin(); ci!=tests.end(); ++ci)
			if(!ci->matches(e))
				return false;
		return true;
	}
};
struct NamedState : PatternState,Name {};
struct NamedTransition : Path,Name {};
struct Pattern : LGraph<ADTisCDT,Nothing,NamedState,NamedTransition> {
	std::map<DString,Node*> dict;
	Pattern(const Pattern &copy) : Graph(copy) {
		for(node_iter ni = nodes().begin(); ni!=nodes().end(); ++ni)
			dict[gd<Name>(*ni)] = *ni;
	}
	Pattern() {}
	void readStrGraph(StrGraph &desc);
	Node *create_node(DString name) {
		Node *ret = Graph::create_node();
		gd<Name>(ret) = name;
		dict[name] = ret;
		return ret;
	}
};

struct BadArgument { DString attr,val; BadArgument(DString attr,DString val) : attr(attr),val(val) {} };

struct Match {
	Pattern::Node *pattern;
	StrGraph::Node *match;
	Match(Pattern::Node *pattern,StrGraph::Node *match) : pattern(pattern),match(match) {}
};
// no transition is allowed to follow the same StrGraph edge twice
// so use a hash_set to remember all edge,transition std::pairs (this requires SGI STL under MSVC++ 6.0)
struct FollowedPath {
	Pattern::Edge *transition;
	StrGraph::Edge *edge;
	FollowedPath(Pattern::Edge *transition,StrGraph::Edge *edge) : transition(transition),edge(edge) {}
};

// hash_* didn't make the standard!!! aieeee!!

#if defined(_MSC_VER) && _MSC_VER>=1300
// vc++.net stl uses one traits class rather than separate hash and eq
using GSearch::FollowedPath;
struct hash_fp {
	static const size_t bucket_size = 4,
		min_buckets = 8;
	hash_fp() {}
	size_t operator()(FollowedPath fp) const {
		return reinterpret_cast<size_t>(fp.transition)^reinterpret_cast<size_t>(fp.edge);
	}
	bool operator ()(FollowedPath fp1,FollowedPath fp2) const {
		return fp1.transition<fp2.transition || (fp1.transition==fp2.transition && fp1.edge<fp2.edge);
	}
};
typedef HASH_NAMESPACE::hash_set<FollowedPath,hash_fp> PathsFollowed;
#else
struct hashFollowedPath {
	size_t operator ()(FollowedPath fp) const {
		return reinterpret_cast<size_t>(fp.transition)^reinterpret_cast<size_t>(fp.edge);
	}
};
struct equal_toFollowedPath {
	bool operator ()(FollowedPath fp1,FollowedPath fp2) const {
		return fp1.transition==fp2.transition && fp1.edge==fp2.edge;
	}
};

typedef HASH_NAMESPACE::hash_set<GSearch::FollowedPath,hashFollowedPath,equal_toFollowedPath>
  PathsFollowed;
#endif

void runPattern(std::queue<GSearch::Match> &Q,PathsFollowed &followed,int limit,
		StrGraph *dest);
void matchPattern(Pattern::Node *start,StrGraph::Node *source,StrGraph *dest);

typedef std::map<DString,Pattern> Patterns;

} // namespace GSearch

