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


#include <stdio.h>
#include "Search.h"
#include "common/ag2str.h"

using namespace GSearch;
StrGraph *g_sourceGraph;

extern FILE *gs_yyin;
extern int gs_yyparse(); // in gsgram.c
int main(int argc, char *args[]) {
	// enable basic dynagraph report streams
	reports.enable(dgr::error,&cerr);
	reports.enable(dgr::cmdline);
	reports.enable(dgr::output);
	setvbuf(stderr,0,_IONBF,0);
	setvbuf(stdout,0,_IONBF,0);
	setvbuf(stdin,0,_IONBF,0);
	if(argc<2) {
		reports[dgr::error] << "please specify a graph to search!" << endl;
		return -1;
	}
	FILE *fSource = fopen(args[1],"r");
	if(!fSource) {
		reports[dgr::error] << "file " << args[1] << " not found!" << endl;
		return -1;
	}
	try {
	    if(!(g_sourceGraph = readStrGraph(fSource))) {
		    reports[dgr::error] << "error parsing " << args[1] << endl;
		    return -1;
	    }
	    if(argc>2) {
		    gs_yyin = fopen(args[2],"r");
		    if(!gs_yyin) {
			    reports[dgr::error] << "couldn't open script " << args[2] << endl;
			    return -1;
		    }
	    }
	    else
		    gs_yyin = stdin;
		while(!feof(gs_yyin)) // ?
			gs_yyparse();
	}
    catch(DGException xep) {
        reports[dgr::error] << "Dynagraph exception: " << xep.exceptype << endl;
    }
	catch(...) {
		reports[dgr::error] << "unhandled exception: closing..." << endl;
	}
	return 0;
}
// a one-time test
/*
	FILE *fPattern = 0,*fSearch = stdin,*fSource = stdin,*fStart = stdin;
	if(argc<2) {
		reports[dgr::error] << "graphsearch pattern-file [search-file] [source-file] [startset-file]" << endl;
		return -1;
	}
	fPattern = fopen(args[1],"r");
	if(argc>2)
		fSearch = fopen(args[2],"r");
	if(argc>3)
		fSource = fopen(args[3],"r");
	if(argc>4)
		fStart = fopen(args[4],"r");
	Patterns patterns;
	while(StrGraph *sPattern = readStrGraph(fPattern)) {
		patterns[gd<Name>(sPattern)].readStrGraph(*sPattern);
		delete sPattern;
	}
	StrGraph *sSearch = readStrGraph(fSearch),
		*source = readStrGraph(fSource);
	StrGraphsByName inputs;
	while(StrGraph *start0 = readStrGraph(fStart)) {
		StrGraph *start = new StrGraph(source);
		start->readSubgraph(start0);
		inputs.push_back(make_pair(gd<Name>(start0),start));
	}
	Search search(*source);
	search.readStrGraph(patterns,*sSearch);
	Search::Node *searchFinish = search.dict["finish"];
	if(!searchFinish) {
		reports[dgr::error] << "search must have a stage named \"finish\"" << endl;
		return -1;
	}
	search.Run(inputs);
	emitGraph(reports[dgr::output],&gd<SearchStage>(searchFinish).result);
*/
// pattern test only
/*
	Pattern::Node *startstate = pattern.dict["start"];
	if(!start) {
		reports[dgr::error] << "pattern must have a state named \"start\"" << endl;
		return;
	}
	queue<Match> Q;
	hash_set<FollowedPath> followed;
	StrGraph found(source);
	for(StrGraph::node_iter ni = start->nodes().begin(); ni !=start->nodes().end(); ++ni) {
		StrGraph::Node *nn = source->dict[gd<Name>(*ni)];
		if(!nn) {
			reports[dgr::error] << "start set node \"" << gd<Name>(*ni) << "\" not found" << endl;
			return;
		}
		found.insert(nn);
		Q.push(Match(startstate,nn));
	}
	runPattern(Q,followed,&found);
	emitGraph(reports[dgr::output],&found);

*/
