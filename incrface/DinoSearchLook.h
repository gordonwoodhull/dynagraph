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

namespace Dynagraph {

template<typename Layout>
struct DinoSearchLookIn : DinoInternalChanges {
    DinoInternalChanges *m_chain;
	DinoMachine::Edge *m_dinoe;
	StrChGraph *m_source;
	IncrStrGraphHandler<Layout> *m_dest;
    DinoSearchLookIn(DinoInternalChanges *chain,DinoMachine::Edge *de)
            : m_chain(chain),m_dinoe(de) {
        DinoMachine::Node *t = de->tail,
			*h = de->head;
		assert(gd<DinoMachNode>(t).handler->dinotype()=="abstract");
		assert(gd<DinoMachNode>(h).handler->dinotype()=="layout");
		m_source = &static_cast<IncrStrGraphHandler<StrChGraph>*>(gd<DinoMachNode>(t).handler)->world_->whole_;
		m_dest = static_cast<IncrStrGraphHandler<Layout>*>(gd<DinoMachNode>(h).handler);
	}
    ~DinoSearchLookIn() {
    }

	void GraphChanged() {
        if(m_chain)
            m_chain->GraphChanged();
		DinoMachEdge &dme = gd<DinoMachEdge>(m_dinoe);
        IncrLangEvents *desthand = gd<DinoMachNode>(m_dinoe->head).handler;
        for(StrChGraph::graphedge_iter ei = m_source->edges().begin(); ei!=m_source->edges().end(); ++ei) {
            Name target = dme.tailmap()[NEID(true,gd<Name>(*ei))].begin()->name;
            StrAttrs attrs;
            DString input = gd<StrAttrs>(*ei)["input"];
            if(input.size())
                attrs["label"] = input;

            desthand->incr_ev_mod_edge(target,attrs);
        }
        for(StrChGraph::node_iter ni = m_source->nodes().begin(); ni!=m_source->nodes().end(); ++ni) {
            Name target = dme.tailmap()[NEID(false,gd<Name>(*ni))].begin()->name;
            DString action = gd<StrAttrs>(*ni)["action"];
            StrAttrs attrs;
            if(action=="pattern") {
                attrs["shape"] = "circle";
                attrs["color"] = "red";
            }
            else if(action=="path") {
                attrs["shape"] = "invtriangle";
                attrs["color"] = "darkgreen";
            }
            else if(action=="union") {
                attrs["shape"] = "diamond";
                attrs["color"] = "blue";
            }
            else if(action=="intersection") {
                attrs["shape"] = "pentagon";
                attrs["color"] = "orange";
            }
            else {
                attrs["shape"] = "box";
                attrs["color"] = "black";
            }
            desthand->incr_ev_mod_node(target,attrs);
        }
    }
};
template<typename Layout>
struct DinoSearchLookOut : DinoInternalChanges {
    DinoInternalChanges *m_chain;
	DinoMachine::Edge *m_dinoe;
	IncrStrGraphHandler<Layout> *m_source;
	StrChGraph *m_dest;
    DinoSearchLookOut(DinoInternalChanges *chain,DinoMachine::Edge *de)
            : m_chain(chain),m_dinoe(de) {
        DinoMachine::Node *t = de->tail,
			*h = de->head;
		assert(gd<DinoMachNode>(t).handler->dinotype()=="layout");
		assert(gd<DinoMachNode>(h).handler->dinotype()=="abstract");
		m_source = static_cast<IncrStrGraphHandler<Layout>*>(gd<DinoMachNode>(t).handler);
		m_dest = &static_cast<IncrStrGraphHandler<StrChGraph>*>(gd<DinoMachNode>(h).handler)->world_->whole_;
	}
    ~DinoSearchLookOut() {
        if(m_chain)
            delete m_chain;
    }

	void GraphChanged() {
        if(m_chain)
            m_chain->GraphChanged();
		DinoMachEdge &dme = gd<DinoMachEdge>(m_dinoe);
        IncrLangEvents *desthand = gd<DinoMachNode>(m_dinoe->head).handler;
        for(typename Layout::graphedge_iter ei = m_source->world_->whole_.edges().begin(); ei!=m_source->world_->whole_.edges().end(); ++ei) {
            DString label = gd<StrAttrs>(*ei)["label"];
            if(label.size()) {
                Name target = dme.headmap()[NEID(true,gd<Name>(*ei))].begin()->name;
                StrAttrs attrs;
                attrs["input"] = label;
                desthand->incr_ev_mod_edge(target,attrs);
            }
        }
    }
};

} // namespace Dynagraph
