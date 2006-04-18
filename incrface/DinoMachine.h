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


// the set of graphs that Dinograph is holding, and the relations between them

#include "IncrLangEvents.h"
#include "two_way_map.h"

namespace Dynagraph {

// node or edge ID
struct NEID {
    bool isEdge;
    DString name;
    NEID() : isEdge(false),name() {}
	NEID(bool isE,Name n) : isEdge(isE),name(n) {}
    bool operator <(NEID o) const {
        return isEdge!=o.isEdge ? isEdge<o.isEdge : name<o.name;
    }
    bool operator ==(NEID o) const {
        return isEdge==o.isEdge && name == o.name;
    }
    bool operator !=(NEID o) const {
        return !(*this==o);
    }
};
// to map between nodes, edges, and subgraphs
typedef two_way_map<NEID> NEID_map;

// fired by DinoMachine when the tail
struct DinoInternalChanges {
    virtual ~DinoInternalChanges() {}
    virtual void GraphChanged() = 0;
};

struct DinoMachNode : NamedAttrs {
    IncrLangEvents *handler;
	bool allowOneReopen,alreadyOpen;
    DinoMachNode(Name name = Name()) : NamedAttrs(name),handler(0),allowOneReopen(false),alreadyOpen(false) {}
    ~DinoMachNode() {
        if(handler&&g_incrCallback)
            g_incrCallback->incr_cb_destroy_handler(handler);
    }
};
struct DinoMachEdge : NamedAttrs {
    DinoInternalChanges *handler;
    // if there are edges a->b and b->a there's still just one NEID_map.
    // (DinoMachineHandler assures this.)  reversed means B is tail and A is head
    NEID_map *mappings;
    bool reversed;
    // generalize two_way_map interface to take reversed into account
    NEID_map::tmap &tailmap() {
        return !reversed?mappings->A:mappings->B;
    }
    NEID_map::tmap &headmap() {
        return !reversed?mappings->B:mappings->A;
    }
    void connect(NEID a,NEID b) {
        if(!reversed)
            mappings->connect(a,b);
        else
            mappings->connect(b,a);
    }
    void disconnect(NEID a,NEID b) {
        if(!reversed)
            mappings->disconnect(a,b);
        else
            mappings->disconnect(b,a);
    }
    void erase_tail(NEID t) {
        if(!reversed)
            mappings->erase_a(t);
        else
            mappings->erase_b(t);
    }
    void erase_head(NEID h) {
        if(!reversed)
            mappings->erase_b(h);
        else
            mappings->erase_a(h);
    }
    void rename_tail(NEID t,NEID t2) {
        if(!reversed)
            mappings->rename_a(t,t2);
        else
            mappings->rename_b(t,t2);
    }
    void rename_head(NEID h,NEID h2) {
        if(!reversed)
            mappings->rename_b(h,h2);
        else
            mappings->rename_a(h,h2);
    }
    DinoMachEdge(Name name = Name()) : NamedAttrs(name),handler(0),mappings(0) {}
    ~DinoMachEdge() {
        if(handler)
            delete handler;
    }
};
struct DinoMachine : NamedGraph<ADTisCDT,NamedAttrs,DinoMachNode,DinoMachEdge> {
	// eventually this will be a real data flow model
	// for now, it's a mess of cascading events
	// the only check is: it won't return to the starting node
	Node *m_start;
	DinoMachine() : m_start(0) {}
    void changed(DString nodename) {
        Node *n = ndict[nodename];
		if(!m_start)
			m_start = n;
		else if(m_start==n)
			return;
        assert(n);
        for(outedge_iter oi = n->outs().begin(); oi!=n->outs().end(); ++oi)
            if(DinoInternalChanges *handler = gd<DinoMachEdge>(*oi).handler)
                handler->GraphChanged();
		if(m_start==n)
			m_start = 0;
    }
};
extern DinoMachine g_dinoMachine;

} // namespace Dynagraph
