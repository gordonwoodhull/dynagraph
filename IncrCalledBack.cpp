/**********************************************************
*            This software is part of Dynagraph           *
*                http://www.dynagraph.org/                *
*                                                         *
*            Copyright (c) 2006 Gordon Woodhull           *
*        Portions Copyright (c) 1994-2005 AT&T Corp.      *
*  Licensed under the Common Public License, Version 1.0  *
*                                                         *
*      Dynagraph originates in the Graphviz toolset       *
*                   http://graphviz.org                   *
*        Information and Software Systems Research        *
*              AT&T Research, Florham Park NJ             *
*                       Many thanks.                      *
**********************************************************/

#include "incrface/incrparse.h"
#include "dynadag/DynaDAGLayout.h"
#include "fdp/FDPLayout.h"
#include "common/GeneralLayout.h"
#include "incrface/createStringHandlers.h"
#include "dynadag/DynaDAGTranslatorTraits.h"
#include "common/emitGraph.h"
#include "IncrCalledBack.h"

using namespace std;

namespace Dynagraph {

using DynaDAG::DynaDAGLayout;
using FDP::FDPLayout;

IncrCalledBack g_incrPhone;
Transform *g_transform;
bool g_useDotDefaults;
StrAttrs g_defaultGraphAttrs;


template<typename Graph>
struct TextWatcherOutput : IncrViewWatcher<Graph> {
	// IncrViewWatcher
	void IncrOpen(ChangeQueue<Graph> &Q) {
		LOCK_REPORT(dgr::output);
		reports[dgr::output] << "open graph " << gd<Name>(Q.whole) << " " << gd<StrAttrs>(Q.whole) << endl;
		igd<StrAttrChanges>(Q.ModGraph()).clear();
	}
	void IncrClose(ChangeQueue<Graph> &Q) {
		LOCK_REPORT(dgr::output);
		reports[dgr::output] << "close graph " << gd<Name>(Q.whole) << endl;
	}
	void FulfilGraph(Graph *g) {
		LOCK_REPORT(dgr::output);
		reports[dgr::output] << "fulfil graph " << gd<Name>(g) << endl;
		emitGraph(reports[dgr::output],g);
	}
	void FulfilNode(typename Graph::Node *n) {
		LOCK_REPORT(dgr::output);
		reports[dgr::output] << "fulfil node " << gd<Name>(n->g) << " " << gd<Name>(n) << " " << gd<StrAttrs>(n) << endl;
	}
	void FulfilEdge(typename Graph::Edge *e) {
		LOCK_REPORT(dgr::output);
		reports[dgr::output] << "fulfil edge " << gd<Name>(e->g) << " " << gd<Name>(e) << " " << gd<StrAttrs>(e) << endl;
	}
};

template<typename Layout>
IncrLangEvents *createHandlers(DString gname,const StrAttrs &attrs) {
	if(attrs.look("superengines")) {
		ChangingGraph<GeneralLayout> *world = new ChangingGraph<GeneralLayout>;
		return createStringHandlers<GeneralLayout>(world,WorldGuts<Layout>(attrs.look("superengines"),attrs.look("engines")),
			new TextWatcherOutput<GeneralLayout>,0,new OutputIncrface<GeneralLayout>(world,dgr::output),
			gname,attrs,g_transform,g_useDotDefaults);
	}
	else {
		ChangingGraph<Layout> *world = new ChangingGraph<Layout>;
		return createStringHandlers<Layout>(world,SimpleGuts<Layout>(attrs.look("engines")),
			new TextWatcherOutput<Layout>,0,new OutputIncrface<Layout>(world,dgr::output),
			gname,attrs,g_transform,g_useDotDefaults);
	}
}
IncrLangEvents *IncrCalledBack::incr_cb_create_handler(Name name,StrAttrs &attrs) {
	attrs = g_defaultGraphAttrs+attrs;
	DString type = attrs.look("type"),
		&engines = attrs["engines"],
		&superengines=attrs["superengines"];
	if(!type) {
		if(!engines) {
			if(superengines.find("shapegen",0)!=DString::npos)
				engines = "dynadag";
			else
				engines = "shapegen,dynadag";
		}
		DString::size_type ddpos = engines.find("dynadag",0);
		if(ddpos!=DString::npos) {
			if(engines.find("nsranker",0)==DString::npos && superengines.find("nsranker",0)==DString::npos)
				if(superengines) {
					string s = superengines.c_str(); // ick
					s += ",nsranker";
					superengines = s.c_str();
				}
				else {
					string s = engines.c_str();
					s.insert(ddpos,"nsranker,");
					engines = s.c_str();
				}
			// BLEEEEE3EEEEEEEEEEE-AAAAAAARRRRRRGFGGGGGYGYGYYYYHHHHHHHHH   IIII HATE DEADLINES1!!!!!!!!1
			if(engines.find("clearextraranks",0)==DString::npos) {
				string s = engines.c_str();
				s.insert(engines.find("dynadag",0),"clearextraranks,");
				engines = s.c_str();
			}
			if(engines.find("edgesuppressor",0)==DString::npos) {
				string s = engines.c_str();
				s.insert(engines.find("dynadag",0),"edgesuppressor,");
				engines = s.c_str();
			}
		}		
		if(engines.find("dynadag",0)!=DString::npos)
			type = "dynadag";
		else if(engines.find("fdp",0)!=DString::npos)
			type = "fdp";
		else
			throw DGException2("could not deduce graph type from engine list",engines);
	}
	IncrLangEvents *ret;
	if(type=="dynadag") 
		ret = createHandlers<DynaDAGLayout>(name,attrs);
	else if(type=="fdp")
		ret = createHandlers<FDPLayout>(name,attrs);
    return ret;
}
void IncrCalledBack::incr_cb_destroy_handler(IncrLangEvents *h) {
    delete h;
}
// echo all fulfils (prob from another server not client!)
void IncrCalledBack::incr_cb_fulfil_graph(DString name,StrGraph *sg) {
	LOCK_REPORT(dgr::output);
    reports[dgr::output] << "fulfil graph " << name << endl;
    reports[dgr::output] << *sg;
}
void IncrCalledBack::incr_cb_fulfil_node(DString graphname,DString nodename,const StrAttrs &attrs) {
	LOCK_REPORT(dgr::output);
    reports[dgr::output] << "fulfil node " << graphname << ' ' << nodename << attrs << endl;
}
void IncrCalledBack::incr_cb_fulfil_edge(DString graphname,DString edgename,const StrAttrs &attrs) {
	LOCK_REPORT(dgr::output);
    reports[dgr::output] << "fulfil edge " << graphname << ' ' << edgename << attrs << endl;
}
void IncrCalledBack::incr_cb_message(const char *msg) {
	LOCK_REPORT(dgr::output);
    // pass through
    reports[dgr::output] << "message \"" << msg << '"' << endl;
}

} // namespace Dynagraph;
