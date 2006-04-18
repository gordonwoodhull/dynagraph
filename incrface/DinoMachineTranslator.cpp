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

#pragma warning (disable : 4503)
#include "common/LayoutAttrs.h"
#include "DinoMachineTranslator.h"
#include "LabelToNameTranslator.h"
#include "MatchNames.h"
#include "common/breakList.h"
#include "DinoSearchLook.h"
#include "DinoPatternLook.h"
#include "common/GeneralLayout.h"

using namespace std;

namespace Dynagraph {

void DinoMachineTranslator::modify_edge(DinoMachine::Edge *dme) {
	StrAttrs &attrs = gd<StrAttrs>(dme);
    if(attrs.look("mapping").size() || attrs.look("l2n_edgelabels").size() || attrs.look("l2n_nodelabels").size()) {
        DinoInternalChanges *&handler = gd<DinoMachEdge>(dme).handler;
        if(handler)
            delete handler;
        handler = 0;
        DString xlate = attrs.look("mapping");
        if(xlate.empty())
            xlate = gd<StrAttrs>(dme)["mapping"];
        vector<DString> handlers;
        breakList(xlate,handlers);
        for(vector<DString>::iterator si = handlers.begin(); si!=handlers.end(); ++si)
            if(*si=="label2name")
                handler = new LabelToNameTranslator<GeneralLayout>(handler,dme,attrs.look("l2n_nodelabels")!="false",attrs.look("l2n_edgelabels")!="false");
            else if(*si=="matchnames")
                handler = new MatchNames<GeneralLayout>(handler,dme);
            else if(*si=="dinosearchIn")
                handler = new DinoSearchLookIn<GeneralLayout>(handler,dme);
            else if(*si=="dinopatternIn")
                handler = new DinoPatternLookIn<GeneralLayout>(handler,dme);
            else if(*si=="dinosearchOut")
                handler = new DinoSearchLookOut<GeneralLayout>(handler,dme);
            else if(*si=="dinopatternOut")
                handler = new DinoPatternLookOut<GeneralLayout>(handler,dme);
    }
}

} // namespace Dynagraph
