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
	createConfiguration(name,attrs);
	// hopefully you'll get a more helpful error if something failed but this one is here if the configurator chain itself failed
	if(!incr_get_handler(name))
		throw HandlerCreationFailed(); 
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
