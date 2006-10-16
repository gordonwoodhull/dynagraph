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

//#define USE_OLD_GROSS_CONFIGURATOR

#include "incrface/incrparse.h"
#include "IncrCalledBack.h"
#include "common/useful.h"
#include "common/emitGraph.h"

#ifdef USE_OLD_GROSS_CONFIGURATOR
// this system of engine creation is deprecated in favor of the Configurator model
// keeping these files intact in case anyone is using createEngine (LMK!)
#include "common/GeneralLayout.h"
#include "incrface/createStringHandlers.h"
#include "dynadag/DynaDAGTranslatorTraits.h"
#else
#include "createConfiguration.h"
#endif

using namespace std;

namespace Dynagraph {

#ifdef USE_OLD_GROSS_CONFIGURATOR
using DynaDAG::DynaDAGLayout;
using FDP::FDPLayout;
extern bool g_useDotDefaults;
extern Transform *g_transform;
#endif 

extern StrAttrs g_defaultGraphAttrs;
IncrCalledBack g_incrPhone;


void IncrCalledBack::incr_cb_create_handler(Name name,StrAttrs &attrs) {
	attrs = g_defaultGraphAttrs+attrs;
#ifndef USE_OLD_GROSS_CONFIGURATOR
	createConfiguration(name,attrs);
	// hopefully you'll get a more helpful error if something failed but this one is here if the configurator chain itself failed
	if(!incr_get_handler(name))
		throw HandlerCreationFailed(); 
#else
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
    IncrLangEvents *handler = createHandlers(type,name,attrs);
	incr_set_handler(name,handler);
#endif
}
void IncrCalledBack::incr_cb_destroy_handler(IncrLangEvents *h) {
    delete h;
}
// echo all fulfils (prob from another server, not client)
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
