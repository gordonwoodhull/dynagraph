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


#include <common/diff_strgraph.h>
struct AbGNodeUnknown : DGException2 {
    AbGNodeUnknown(const char *name) : DGException2("tried to modify unknown node",name) {}
};
struct AbGEdgeUnknown : DGException2 {
    AbGEdgeUnknown(const char *name) : DGException2("tried to modify unknown edge",name) {}
};
template<typename NGraph> // some graph supporting attributes of StrGraph
struct AbsGraphHandler : IncrLangEvents {
    NGraph *g;
    bool own;
    int locks;

    AbsGraphHandler(NGraph *g = 0) : locks(0) {
        this->g = 0;
        this->g = g?(own=false,g):(own=true,new NGraph);  // hnguh
    }
    virtual ~AbsGraphHandler() {
        if(own)
            delete g;
    }
    bool maybe_go();
    // IncrLangEvents
	DString dinotype() { return "abstract"; }
	bool incr_ev_open_graph(DString graph,const StrAttrs &attrs);
	bool incr_ev_close_graph();
	bool incr_ev_mod_graph(const StrAttrs &attrs);
	bool incr_ev_lock();
	bool incr_ev_unlock();
	DString incr_ev_ins_node(DString name, const StrAttrs &attrs, bool merge);
	DString incr_ev_ins_edge(DString name, DString tailname, DString headname, const StrAttrs &attrs);
	bool incr_ev_mod_node(DString name,const StrAttrs &attrs);
	bool incr_ev_mod_edge(DString name,const StrAttrs &attrs);
	bool incr_ev_del_node(DString name);
	bool incr_ev_del_edge(DString name);
	bool incr_ev_req_graph();
	bool incr_ev_req_node(DString name);
	bool incr_ev_req_edge(DString name);
    void incr_ev_load_strgraph(StrGraph *sg,bool merge, bool del);
};

// make changes immediately but only fire DinoInternalChanges when unlocked
template<typename NGraph>
bool AbsGraphHandler<NGraph>::maybe_go() {
    if(locks>0)
        return false;
    g_dinoMachine.changed(gd<Name>(g));
    return true;
}
template<typename NGraph>
bool AbsGraphHandler<NGraph>::incr_ev_open_graph(DString graph,const StrAttrs &attrs) {
    if(graph.empty())
        graph = randomName('g');
    gd<Name>(g) = graph;
    incr_set_handler(graph,this);
    gd<StrAttrs>(g) = attrs;
    maybe_go();
    return true;
}
template<typename NGraph>

bool AbsGraphHandler<NGraph>::incr_ev_close_graph() {
    return true;
}
template<typename NGraph>
bool AbsGraphHandler<NGraph>::incr_ev_mod_graph(const StrAttrs &attrs) {
    gd<StrAttrs>(g) += attrs;
    maybe_go();
    return true;
}
template<typename NGraph>
bool AbsGraphHandler<NGraph>::incr_ev_lock() {
    locks++;
    return true;
}
template<typename NGraph>
bool AbsGraphHandler<NGraph>::incr_ev_unlock() {
    --locks;
    maybe_go();
    return true;
}
template<typename NGraph>
DString AbsGraphHandler<NGraph>::incr_ev_ins_node(DString name, const StrAttrs &attrs, bool merge) {
    if(name.empty())
        name = randomName('n');
    typename NGraph::Node *n = g->get_node(name);
    gd<StrAttrs>(n) += attrs;
    maybe_go();
    return name;
}
template<typename NGraph>
DString AbsGraphHandler<NGraph>::incr_ev_ins_edge(DString name, DString tailname, DString headname, const StrAttrs &attrs) {
    if(name.empty())
        name = randomName('e');
    typename NGraph::Edge *e = g->get_edge(tailname,headname,name);
    gd<StrAttrs>(e) += attrs;
    maybe_go();
    return name;
}
template<typename NGraph>
bool AbsGraphHandler<NGraph>::incr_ev_mod_node(DString name,const StrAttrs &attrs) {
    typename NGraph::Node *n = g->ndict[name];
    if(!n)
        throw AbGNodeUnknown(name.c_str());
    gd<StrAttrs>(n) += attrs;
    maybe_go();
    return true;
}
template<typename NGraph>
bool AbsGraphHandler<NGraph>::incr_ev_mod_edge(DString name,const StrAttrs &attrs) {
    typename NGraph::Edge *e = g->edict[name];
    if(!e)
        throw AbGEdgeUnknown(name.c_str());
    gd<StrAttrs>(e) += attrs;
    maybe_go();
    return true;
}
template<typename NGraph>
bool AbsGraphHandler<NGraph>::incr_ev_del_node(DString name) {
    typename NGraph::Node *n = g->ndict[name];
    if(!n)
        throw AbGNodeUnknown(name.c_str());
    g->erase(n);
    maybe_go();
    return true;
}
template<typename NGraph>
bool AbsGraphHandler<NGraph>::incr_ev_del_edge(DString name) {
    typename NGraph::Edge *e = g->edict[name];
    if(!e)
        throw AbGEdgeUnknown(name.c_str());
    g->erase(e);
    maybe_go();
    return true;
}
// NO
template<typename NGraph>
bool AbsGraphHandler<NGraph>::incr_ev_req_graph() {
    return false;
}
template<typename NGraph>
bool AbsGraphHandler<NGraph>::incr_ev_req_node(DString name) {
    return false;
}
template<typename NGraph>
bool AbsGraphHandler<NGraph>::incr_ev_req_edge(DString name) {
    return false;
}
template<typename NGraph>
struct we_do_what_were_told {
    AbsGraphHandler<NGraph> *hand;
    we_do_what_were_told(AbsGraphHandler<NGraph> *hand) : hand(hand) {}
    void ins(StrGraph::Node *n) {
        hand->incr_ev_ins_node(gd<Name>(n),gd<StrAttrs>(n),true);
    }
    void ins(StrGraph::Edge *e) {
        hand->incr_ev_ins_edge(gd<Name>(e),gd<Name>(e->tail),gd<Name>(e->head),gd<StrAttrs>(e));
    }
    void mod(typename NGraph::Node *nn,StrGraph::Node *sn,StrAttrs *attrs) {
        hand->incr_ev_mod_node(gd<Name>(nn),*attrs);
    }
    void mod(typename NGraph::Edge *ne,StrGraph::Edge *se,StrAttrs *attrs) {
        hand->incr_ev_mod_edge(gd<Name>(ne),*attrs);
    }
    void del(typename NGraph::Node *n) {
        hand->incr_ev_del_node(gd<Name>(n));
    }
    void del(typename NGraph::Edge *e) {
        hand->incr_ev_del_edge(gd<Name>(e));
    }
};
template<typename NGraph>
void AbsGraphHandler<NGraph>::incr_ev_load_strgraph(StrGraph *sg,bool merge, bool del) {
    assert(merge && del);
    we_do_what_were_told<NGraph> react(this);
    incr_ev_lock();
    diff_strgraph(g,sg,react);
    incr_ev_unlock();
}
