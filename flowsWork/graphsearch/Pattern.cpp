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
#include <stdio.h>
#include <sstream>
#include "common/parsestr.h"

using namespace std;

namespace GSearch {

istream &operator>>(istream &is,Test &test) {
	is >> ws >> test.attr >> ws >> must("=") >> ws >> test.value;
	return is;
}
istream &operator>>(istream &is,Path &pt) {
	pt.tests.clear();
	is >> ws;
	while(!is.eof()) {
		pt.tests.push_back(Test());
		is >> pt.tests.back();
		is >> ws;
		if(!is.eof())
			is >> must(",");
		is >> ws;
	}
	return is;
}
struct DirNames : map<DString,int> {
	DirNames() {
		map<DString,int> &mhp = *this;
		mhp["down"] = 1;
		mhp["up"] = 2;
		mhp["both"] = 3;
	}
} g_dirNames;
void Pattern::readStrGraph(StrGraph &desc) {
	clear();
	map<StrGraph::Node*,Pattern::Node*> recollect;
	for(StrGraph::node_iter ni = desc.nodes().begin(); ni!=desc.nodes().end(); ++ni) {
		Node *n = create_node(gd<Name>(*ni));
		recollect[*ni] = n;
	}
	for(StrGraph::graphedge_iter ei = desc.edges().begin(); ei!=desc.edges().end(); ++ei) {
		Edge *e = create_edge(recollect[(*ei)->tail],recollect[(*ei)->head]).first;
		DString match = gd<StrAttrs>(*ei).look("match","");
		istringstream stream(match);
		stream >> gd<Path>(e);
		DString dir = gd<StrAttrs>(*ei).look("dir","both");
		if(!(gd<Path>(e).direction = g_dirNames[dir]))
			throw BadArgument("dir",dir);
	}
}

void runPattern(queue<Match> &Q,PathsFollowed &followed,int limit,StrGraph *dest) {
	while(!Q.empty()) {
		Match match = Q.front();
		Q.pop();
		//printf("consider %x %x\n",match.pattern,match.match);
		for(Pattern::outedge_iter pei = match.pattern->outs().begin(); pei!=match.pattern->outs().end(); ++pei) {
			if(gd<Path>(*pei).direction&matchDown)
				for(StrGraph::outedge_iter sei = match.match->outs().begin(); sei!=match.match->outs().end(); ++sei) {
					//printf("look %x -> %x\n",(*sei)->tail,(*sei)->head);
					if(!followed.insert(FollowedPath(*pei,*sei)).second)
						continue;
					if(gd<Path>(*pei).matches(*sei)) {
						dest->insert(*sei).first;
						Q.push(Match((*pei)->head,(*sei)->head));
					}
					if(limit>0 && dest->nodes().size()>size_t(limit))
						return;
				}
			if(gd<Path>(*pei).direction&matchUp)
				for(StrGraph::inedge_iter sei = match.match->ins().begin(); sei!=match.match->ins().end(); ++sei) {
					//printf("look %x -> %x\n",(*sei)->tail,(*sei)->head);
					if(!followed.insert(FollowedPath(*pei,*sei)).second)
						continue;
					if(gd<Path>(*pei).matches(*sei)) {
						dest->insert(*sei).first;
						Q.push(Match((*pei)->head,(*sei)->tail));
					}
					if(limit>0 && dest->nodes().size()>size_t(limit))
						return;
				}
		}
	}
}
void matchPattern(Pattern::Node *start,StrGraph::Node *source,StrGraph *dest) {
  //StrGraph::Node *place =
  dest->insert(source).first;
	queue<Match> Q;
	PathsFollowed followed;
	Q.push(Match(start,source));
	runPattern(Q,followed,0,dest);
}

}
