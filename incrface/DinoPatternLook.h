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


struct DinoPatternLookIn : DinoInternalChanges {
    DinoInternalChanges *m_chain;
	DinoMachine::Edge *m_dinoe;
	StrGraph *m_source;
	DynaView *m_dest;
    DinoPatternLookIn(DinoInternalChanges *chain,DinoMachine::Edge *de)
            : m_chain(chain),m_dinoe(de) {
        DinoMachine::Node *t = de->tail,
			*h = de->head;
		assert(gd<DinoMachNode>(t).handler->dinotype()=="abstract");
		assert(gd<DinoMachNode>(h).handler->dinotype()=="layout");
		m_source = static_cast<AbsGraphHandler<StrGraph>*>(gd<DinoMachNode>(t).handler)->g;
		m_dest = static_cast<DynaView*>(gd<DinoMachNode>(h).handler);
	}
    ~DinoPatternLookIn() {
    }

	void GraphChanged() {
        if(m_chain)
            m_chain->GraphChanged();
		DinoMachEdge &dme = gd<DinoMachEdge>(m_dinoe);
        IncrLangEvents *desthand = gd<DinoMachNode>(m_dinoe->head).handler;
        for(StrGraph::graphedge_iter ei = m_source->edges().begin(); ei!=m_source->edges().end(); ++ei) {
            DString match = gd<StrAttrs>(*ei)["match"];
            if(match.size()) {
                Name target = dme.tailmap()[NEID(true,gd<Name>(*ei))].begin()->name;
                StrAttrs attrs;
                attrs["label"] = match;
                desthand->incr_ev_mod_edge(target,attrs);
            }
        }
    }
};
struct DinoPatternLookOut : DinoInternalChanges {
    DinoInternalChanges *m_chain;
	DinoMachine::Edge *m_dinoe;
	DynaView *m_source;
	StrGraph *m_dest;
    DinoPatternLookOut(DinoInternalChanges *chain,DinoMachine::Edge *de)
            : m_chain(chain),m_dinoe(de) {
        DinoMachine::Node *t = de->tail,
			*h = de->head;
		assert(gd<DinoMachNode>(t).handler->dinotype()=="layout");
		assert(gd<DinoMachNode>(h).handler->dinotype()=="abstract");
		m_source = static_cast<DynaView*>(gd<DinoMachNode>(t).handler);
		m_dest = static_cast<AbsGraphHandler<StrGraph>*>(gd<DinoMachNode>(h).handler)->g;
	}
    ~DinoPatternLookOut() {
        if(m_chain)
            delete m_chain;
    }

	void GraphChanged() {
        if(m_chain)
            m_chain->GraphChanged();
		DinoMachEdge &dme = gd<DinoMachEdge>(m_dinoe);
        IncrLangEvents *desthand = gd<DinoMachNode>(m_dinoe->head).handler;
        for(Layout::graphedge_iter ei = m_source->layout.edges().begin(); ei!=m_source->layout.edges().end(); ++ei) {
            DString label = gd<StrAttrs>(*ei)["label"];
            if(label.size()) {
                Name target = dme.headmap()[NEID(true,gd<Name>(*ei))].begin()->name;
                StrAttrs attrs;
                attrs["match"] = label;
                desthand->incr_ev_mod_edge(target,attrs);
            }
        }
    }
};
