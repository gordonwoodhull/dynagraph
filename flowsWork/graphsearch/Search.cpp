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


#include "Search.h"
#include "findAllPaths.h"
#include "findShortestPaths.h"

using namespace std;

namespace GSearch {

Search::Search(StrGraph &source) : source(source) {}

void Search::readStrGraph(Patterns &patterns,StrGraph &desc) {
	clear();
	map<StrGraph::Node*,Node*> recollect;
	for(StrGraph::node_iter ni = desc.nodes().begin(); ni!=desc.nodes().end(); ++ni) {
		Node *n = create_node(gd<Name>(*ni));
		recollect[*ni] = n;
		SearchStage &stage = gd<SearchStage>(n);
		DString limit = gd<StrAttrs>(*ni).look("limit","50");
		stage.limit = atoi(limit.c_str());
		DString action = gd<StrAttrs>(*ni).look("action","union");
		if(action=="union")
			stage.type = UnionInquiry;
		else if(action=="intersection")
			stage.type = IntersectionInquiry;
		else if(action=="pattern") {
			stage.type = PatternInquiry;
			DString pattern = gd<StrAttrs>(*ni).look("pattern","");
			Patterns::iterator pi = patterns.find(pattern);
			if(pi==patterns.end())
				throw UndefinedPattern(pattern);
			stage.pattern = &pi->second;
		}
        else if(action=="path") {
			stage.type = PathInquiry;
            DString ways = gd<StrAttrs>(*ni)["ways"];
            if(ways=="in")
                stage.goIn = true, stage.goOut = false;
            else if(ways=="both")
                stage.goIn = stage.goOut = true;
            else  // ways=="out" default
                stage.goIn = false, stage.goOut = true;
			stage.firstOnly = gd<StrAttrs>(*ni)["firstonly"]=="true";
			stage.shortest = gd<StrAttrs>(*ni)["shortest"]=="true";
			stage.weightattr = gd<StrAttrs>(*ni).look("weightattr","weight");
        }
		else
			throw UnknownAction(action);
	}
	for(StrGraph::graphedge_iter ei = desc.edges().begin(); ei!=desc.edges().end(); ++ei) {
		Edge *e = create_edge(recollect[(*ei)->tail],recollect[(*ei)->head]).first;
		DString input = gd<StrAttrs>(*ei).look("input","");
		gd<Name>(e) = input;
	}
}


void Search::Run(const Inputs &inputs) {
	for(Inputs::const_iterator ngi = inputs.begin(); ngi!=inputs.end(); ++ngi) {
		node_iter ni;
		for(ni = nodes().begin(); ni!=nodes().end(); ++ni)
			if(gd<Name>(*ni)==ngi->first) {
				StrGraph &dest = gd<SearchStage>(*ni).result;
				dest = *ngi->second;
				break;
			}
        /*
		if(ni==nodes().end())
			throw StageNotFound(ngi->first);
        */
	}
	while(1) {
		Node *stage = 0;
		// find a stage for which all prereq stages are done
		for(node_iter ni = nodes().begin(); ni!=nodes().end(); ++ni) {
			if(gd<SearchStage>(*ni).done)
				continue;
			inedge_iter ei;
			for(ei = (*ni)->ins().begin(); ei!=(*ni)->ins().end(); ++ei)
				if(!gd<SearchStage>((*ei)->tail).done)
					break;
			if(ei==(*ni)->ins().end()) {
				stage = *ni;
				break;
			}
		}
		if(!stage)
			break;
		SearchStage &earchage = gd<SearchStage>(stage);
		Search::inedge_iter ei = stage->ins().begin();
		if(ei!=stage->ins().end())
			switch(earchage.type) {
			case UnionInquiry:
				earchage.result = gd<SearchStage>((*ei)->tail).result;
				for(; ei!=stage->ins().end(); ++ei)
					earchage.result |= gd<SearchStage>((*ei)->tail).result;
				break;
			case IntersectionInquiry:
				earchage.result = gd<SearchStage>((*ei)->tail).result;
				for(; ei!=stage->ins().end(); ++ei)
					earchage.result &= gd<SearchStage>((*ei)->tail).result;
				break;
			case PatternInquiry: {
				if(!earchage.pattern)
					throw PatternNotThere();
				earchage.result.clear();
				queue<Match> Q;
				PathsFollowed followed;
				for(; ei!=stage->ins().end(); ++ei) {
					Name inputname = gd<Name>(*ei);
					Pattern::node_iter ni;
					for(ni = earchage.pattern->nodes().begin();
							ni!=earchage.pattern->nodes().end(); ++ni)
						if(gd<Name>(*ni)==inputname) {
							StrGraph &input = gd<SearchStage>((*ei)->tail).result;
							for(StrGraph::node_iter inpi = input.nodes().begin(); inpi !=input.nodes().end(); ++inpi) {
								earchage.result.insert(*inpi);
								Q.push(Match(*ni,source.find(*inpi)));
							}
							break;
						}
					if(ni==earchage.pattern->nodes().end())
						throw PatternStateNotFound(inputname);
				}
				runPattern(Q,followed,earchage.limit,&earchage.result);
				break;
			}
            case PathInquiry: {
				earchage.result.clear();
                StrGraph *a = 0, *b = 0;
				for(; ei!=stage->ins().end(); ++ei) {
                    DString inputname = gd<Name>(*ei);
                    if(inputname=="a")
                        a = &gd<SearchStage>((*ei)->tail).result;
                    else if(inputname=="b")
                        b = &gd<SearchStage>((*ei)->tail).result;
                }
                if(a&&b)
					if(earchage.shortest)
						findShortestPaths(source,*a,*b,earchage.weightattr,earchage.limit,earchage.result);
					else
						findAllPaths(source,*a,*b,earchage.goOut,earchage.goIn,earchage.firstOnly,earchage.limit,earchage.result);
            }
			}
		earchage.done = true;
	}
}

}
