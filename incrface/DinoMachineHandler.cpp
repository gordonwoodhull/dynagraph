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


#include "common/Dynagraph.h"
#include "DynaView.h"
#include "AbsGraphHandler.h"
#include "DinoMachineHandler.h"
#include "LabelToNameTranslator.h"
#include "MatchNames.h"
#include "common/breakList.h"
#include "DinoSearchLook.h"
#include "DinoPatternLook.h"

using namespace std;

DString DinoMachineHandler::incr_ev_ins_node(DString name, const StrAttrs &attrs, bool merge) {
    DString ret = AbsGraphHandler<DinoMachine>::incr_ev_ins_node(name,attrs,merge);
    modify_node(name,attrs);
    return ret;
}
DString DinoMachineHandler::incr_ev_ins_edge(DString name, DString tailname, DString headname, const StrAttrs &attrs) {
	DString ret = AbsGraphHandler<DinoMachine>::incr_ev_ins_edge(name,tailname,headname,attrs);
    modify_edge(name,attrs);
    // the NEID_map is shared in a 2-cycle
    DinoMachine::Edge *de = g->edict[name];
    if(DinoMachine::Edge *de2 = g->find_edge(de->head,de->tail)) {
        gd<DinoMachEdge>(de).mappings = gd<DinoMachEdge>(de2).mappings;
        gd<DinoMachEdge>(de).reversed = !gd<DinoMachEdge>(de2).reversed;
    }
    else {
        gd<DinoMachEdge>(de).mappings = new NEID_map;
        gd<DinoMachEdge>(de).reversed = false;
    }
    return ret;
}
bool DinoMachineHandler::incr_ev_mod_node(DString name,const StrAttrs &attrs) {
    bool ret = AbsGraphHandler<DinoMachine>::incr_ev_mod_node(name,attrs);
    modify_node(name,attrs);
    return ret;
}
bool DinoMachineHandler::incr_ev_mod_edge(DString name,const StrAttrs &attrs) {
    bool ret = AbsGraphHandler<DinoMachine>::incr_ev_mod_edge(name,attrs);
    modify_edge(name,attrs);
    return ret;
}
bool DinoMachineHandler::incr_ev_del_node(DString name) {
    return incr_ev_del_node(name);
}
bool DinoMachineHandler::incr_ev_del_edge(DString name) {
    DinoMachine::Edge *de = g->edict[name];
    if(!g->find_edge(de->head,de->tail))
        delete gd<DinoMachEdge>(de).mappings;

    return incr_ev_del_edge(name);
}
void DinoMachineHandler::modify_node(DString name,const StrAttrs &attrs) {
}
void DinoMachineHandler::modify_edge(DString name,const StrAttrs &attrs) {
    DinoMachine::Edge *de = g->edict[name];
    if(attrs.look("mapping").size() || attrs.look("l2n_edgelabels").size() || attrs.look("l2n_nodelabels").size()) {
        DinoInternalChanges *&handler = gd<DinoMachEdge>(de).handler;
        if(handler)
            delete handler;
        handler = 0;
        DString xlate = attrs.look("mapping");
        if(xlate.empty())
            xlate = gd<StrAttrs>(de)["mapping"];
        vector<DString> handlers;
        breakList(xlate,handlers);
        for(vector<DString>::iterator si = handlers.begin(); si!=handlers.end(); ++si)
            if(*si=="label2name")
                handler = new LabelToNameTranslator(handler,de,attrs.look("l2n_nodelabels")!="false",attrs.look("l2n_edgelabels")!="false");
            else if(*si=="matchnames")
                handler = new MatchNames(handler,de);
            else if(*si=="dinosearchIn")
                handler = new DinoSearchLookIn(handler,de);
            else if(*si=="dinopatternIn")
                handler = new DinoPatternLookIn(handler,de);
            else if(*si=="dinosearchOut")
                handler = new DinoSearchLookOut(handler,de);
            else if(*si=="dinopatternOut")
                handler = new DinoPatternLookOut(handler,de);
    }
}
