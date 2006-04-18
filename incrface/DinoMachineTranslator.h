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

#include "DinoMachine.h"

namespace Dynagraph {

struct DinoMachineTranslator {
	void ModifyGraph(DinoMachine *dm) {}
	void InsertNode(DinoMachine::Node *dmn) {
		modify_node(dmn);
	}
	void InsertEdge(DinoMachine::Edge *dme) {
		modify_edge(dme);
		// the NEID_map is shared in a 2-cycle
		if(DinoMachine::Edge *dme2 = dme->g->find_edge(dme->head,dme->tail)) {
			gd<DinoMachEdge>(dme).mappings = gd<DinoMachEdge>(dme2).mappings;
			gd<DinoMachEdge>(dme).reversed = !gd<DinoMachEdge>(dme2).reversed;
		}
		else {
			gd<DinoMachEdge>(dme).mappings = new NEID_map;
			gd<DinoMachEdge>(dme).reversed = false;
		}
	}
	void ModifyNode(DinoMachine::Node *dmn) {
		modify_node(dmn);
	}
	void ModifyEdge(DinoMachine::Edge *dme) {
		modify_edge(dme);
	}
	void DeleteNode(DinoMachine::Node *dmn) {}
	void DeleteEdge(DinoMachine::Edge *dme) {
		if(!dme->g->find_edge(dme->head,dme->tail))
			delete gd<DinoMachEdge>(dme).mappings;
	}

	void modify_node(DinoMachine::Node *dmn) {}
    void modify_edge(DinoMachine::Edge *dme);
};

} // namespace Dynagraph
